/*
sipc1, sipc2를 multi-threaded version으로 변경하고, 전역변수를 이용해서 메세지를 주고 받을 때
mutex와 condition variable을 이용한 버전.
컴파일 후 ./sipc_hw3_1을 실행하면 sipc2가 먼저 메세지를 보내고, sipc1에서 답장함.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include "prodcons.h"

int count = 0;
char data[1024]; // 메세지 전역변수
pthread_cond_t      Request;
pthread_cond_t      Reply;
pthread_mutex_t     Mutex;

void
ThreadUsleep(int usecs)
{
    pthread_cond_t      cond;
    pthread_mutex_t     mutex;
    struct timespec     ts;
    struct timeval      tv;


    if (pthread_cond_init(&cond, NULL) < 0)  {
        perror("pthread_cond_init");
        pthread_exit(NULL);
    }
    if (pthread_mutex_init(&mutex, NULL) < 0)  {
        perror("pthread_mutex_init");
        pthread_exit(NULL);
    }

    gettimeofday(&tv, NULL);
    ts.tv_sec = tv.tv_sec + usecs/1000000;
    ts.tv_nsec = (tv.tv_usec + (usecs%1000000)) * 1000;
    if (ts.tv_nsec >= 1000000000)  {
        ts.tv_nsec -= 1000000000;
        ts.tv_sec++;
    }

    if (pthread_mutex_lock(&mutex) < 0)  {
        perror("pthread_mutex_lock");
        pthread_exit(NULL);
    }
    if (pthread_cond_timedwait(&cond, &mutex, &ts) < 0)  {
        perror("pthread_cond_timedwait");
        pthread_exit(NULL);
    }

    if (pthread_cond_destroy(&cond) < 0)  {
        perror("pthread_cond_destroy");
        pthread_exit(NULL);
    }
    if (pthread_mutex_destroy(&mutex) < 0)  {
        perror("pthread_mutex_destroy");
        pthread_exit(NULL);
    }
}

void sipc1(void *dummy)
{
    /* 일단 뮤텍스 잠가서 다른 스레드가 접근 못하게함 */
    if (pthread_mutex_lock(&Mutex) < 0)  {
        perror("pthread_mutex_lock");
        pthread_exit(NULL);
    }

    /* sipc2의 request를 기다림 */
    if (pthread_cond_wait(&Request, &Mutex) < 0)  {
        perror("pthread_cond_wait");
        pthread_exit(NULL);
    }

    /* sipc2의 메세지를 받고, 답장을 보냄 */
    printf("Received request: %s.....\n", data);
    sprintf(data, "This is a reply from %ld.", pthread_self());
    printf("Replied.\n");

    /* sipc2에게 응답을 보냈다고 신호를 보내줌 */
    if (pthread_cond_signal(&Reply) < 0)  {
        perror("pthread_cond_signal");
        pthread_exit(NULL);
    }
    /* 뮤텍스 해제해서 다른 스레드 접근 허용해줌 */
    if (pthread_mutex_unlock(&Mutex) < 0)  {
        perror("pthread_mutex_unlock");
        pthread_exit(NULL);
    }

    pthread_exit(NULL);
}

{
    /* 뮤텍스 잠가서 다른 스레드 접근 못하게함 */
    if (pthread_mutex_lock(&Mutex) < 0)  {
        perror("pthread_mutex_lock");
        pthread_exit(NULL);
    }   
    
    /* sipc1에게 메세지를 보냄 */
    sprintf(data, "This is a reply from %ld.", pthread_self());
    printf("Sent a request.....\n");
    count++;
    
    /* sipc1의 request에 signal을 보냄 */
    if (pthread_cond_signal(&Request) < 0)  {
        perror("pthread_mutex_lock"); 
        pthread_exit(NULL);
    }   
    /* 뮤텍스 해제해서 다른 스레드 접근 허용 */
    if (pthread_mutex_unlock(&Mutex) < 0)  {
        perror("pthread_cond_wait"); 
        pthread_exit(NULL);
    }   
     
    /* sipc1의 응답 대기 */
    if (pthread_mutex_lock(&Mutex) < 0)  {
        perror("pthread_mutex_lock");
        pthread_exit(NULL);
    }   
    
    if (pthread_cond_wait(&Reply, &Mutex) < 0)  {
        perror("pthread_cond_wait");
        pthread_exit(NULL);
    }   
    printf("Received request: %s.....\n", data);
    
    pthread_exit(NULL);
}   

main()
{
    pthread_t   tid1, tid2;

    srand(0x8888);

    if (pthread_cond_init(&Request, NULL) < 0)  {
        perror("pthread_cond_init");
        pthread_exit(NULL);
    }
    if (pthread_cond_init(&Reply, NULL) < 0)  {
        perror("pthread_cond_init");
        pthread_exit(NULL);
    }
    if (pthread_mutex_init(&Mutex, NULL) < 0)  {
        perror("pthread_mutex_init");
        pthread_exit(NULL);
    }

    if (pthread_create(&tid1, NULL, (void *)sipc1, (void *)NULL) < 0)  {
        perror("pthread_create");
        exit(1);
    }

    if (pthread_create(&tid2, NULL, (void *)sipc2, (void *)NULL) < 0)  {
        perror("pthread_create");
        exit(1);
    }

    if (pthread_join(tid1, NULL) < 0)  {
        perror("pthread_join");
        exit(1);
    }
    if (pthread_join(tid2, NULL) < 0)  {
        perror("pthread_join");
        exit(1);
    }

    if (pthread_cond_destroy(&Request) < 0)  {
        perror("pthread_cond_destroy");
    }
    if (pthread_cond_destroy(&Reply) < 0)  {
        perror("pthread_cond_destroy");
    }
    if (pthread_mutex_destroy(&Mutex) < 0)  {
        perror("pthread_mutex_destroy");
    }
}