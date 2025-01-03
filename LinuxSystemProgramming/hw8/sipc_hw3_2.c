/*
sipc1, sipc2를 multi-thread version으로 변경하고, 전역변수를 이용하여 메세지를 주고받을 때
semaphore를 사용한 버전.
컴파일 한 뒤 ./sipc_hw3_2를 실행하면 sipc2가 메세지를 보내고, sipc1에서 그 메세지를 받고 답장을 보냄.

<mutex+condition variable과 semaphore 방법의 비교 >
mutex+condition variable은 특정 조건을 기다릴 때에 적합함. 그러므로 복잡한 동기화 문제에 유용함.
semaphore는 간단한 동기화 문제에 더 직관적이고 효율적으로 동작함.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/time.h>
#include "sipc.h"

char data[1024]; //메세지 전역변수
sem_t Request, Reply, Mutex;

void ThreadUsleep(int usecs)
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
    /* wait (request) */
    if (sem_wait(&Request) < 0)  {
        perror("sem_wait");
        pthread_exit(NULL);
    }
    /* wait (mutex) */
    if (sem_wait(&Mutex) < 0)  {
        perror("sem_wait");
        pthread_exit(NULL);
    }
    /* sipc2의 메세지를 받고, 답장을 보냄 */
    printf("Received request: %s.....\n", data);
    sprintf(data, "This is a reply from %ld.", pthread_self());
    printf("Replied.\n");

    /* signal (mutex) */
    if (sem_post(&Mutex) < 0)  {
        perror("sem_post");
        pthread_exit(NULL);
    }
    /* signal (reply) */
    if (sem_post(&Reply) < 0)  {
        perror("sem_post");
        pthread_exit(NULL);
    }

    pthread_exit(NULL);
}

void sipc2(void *dummy)
{
    /* wait (mutex) */
    if (sem_wait(&Mutex) < 0)  {
        perror("sem_wait");
        pthread_exit(NULL);
    }

    /* sipc1에게 메세지를 보냄 */
    sprintf(data, "This is a reply from %ld.", pthread_self());
    printf("Sent a request.....\n");

    /* signal (mutex) */
    if (sem_post(&Mutex) < 0) {
        perror("sem_post");
        pthread_exit(NULL);
    }
    /* signal(request) */
    if (sem_post(&Request) < 0) {
        perror("sem_post");
        pthread_exit(NULL);
    }

    /* wait(reply) */
    if (sem_wait(&Reply) < 0) {
        perror("sem_wait");
        pthread_exit(NULL);
    }
    /* wait(mutex) */
    if (sem_wait(&Mutex) < 0) {
        perror("sem_wait");
        pthread_exit(NULL);
    }
    printf("Received request: %s.....\n", data);

    pthread_exit(NULL);
}

main()
{
    pthread_t   tid1, tid2;

    srand(0x9999);

    if (sem_init(&Request, 0, 0) < 0)  {
        perror("sem_init");
        exit(1);
    }
    if (sem_init(&Reply, 0, 0) < 0)  {
        perror("sem_init");
        exit(1);
    }
    if (sem_init(&Mutex, 0, 1) < 0)  {
        perror("sem_init");
        exit(1);
    }

    /* thread 생성 */
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


    /* semaphore 삭제 */
    if (sem_destroy(&Request) < 0)  {
        perror("sem_destroy");
    }
    if (sem_destroy(&Reply) < 0)  {
        perror("sem_destroy");
    }
    if (sem_destroy(&Mutex) < 0)  {
        perror("sem_destroy");
    }
}