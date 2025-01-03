#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include "prodcons.h"

BoundedBufferType   Buf;
pthread_cond_t      NotFull;
pthread_cond_t      NotEmpty;
pthread_mutex_t     Mutex;

/* 특정 시간 대기하는 함수 */
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

/* producer thread 함수 */
void Producer(void *dummy)
{
    int     i, data;

    printf("Producer: Start.....\n");

    for (i = 0 ; i < NLOOPS ; i++)  {
        /* mutex를 잠금 */
        if (pthread_mutex_lock(&Mutex) < 0)  {
            perror("pthread_mutex_lock");
            pthread_exit(NULL);
        }

        /* 버퍼가 가득 찼다면 NotFull 조건 변수에서 대기함 */
        while (Buf.counter == MAX_BUF)  {
            if (pthread_cond_wait(&NotFull, &Mutex) < 0)  {
                perror("pthread_cond_wait");
                pthread_exit(NULL);
            }
        }

        /* 데이터를 버퍼에 추가함 */
        printf("Producer: Producing an item.....\n");
        data = (rand()%100)*10000;
        Buf.buf[Buf.in].data = data;
        Buf.in = (Buf.in + 1) % MAX_BUF;
        Buf.counter++;

        /* NotEmpty 조건 변수 알림 */
        if (pthread_cond_signal(&NotEmpty) < 0)  {
            perror("pthread_cond_signal");
            pthread_exit(NULL);
        }
        /* mutex 해제 */
        if (pthread_mutex_unlock(&Mutex) < 0)  {
            perror("pthread_mutex_unlock");
            pthread_exit(NULL);
        }

        ThreadUsleep(data);
    }

    printf("Producer: Produced %d items.....\n", i);
    printf("Producer: %d items in buffer.....\n", Buf.counter);

    pthread_exit(NULL);
}

/* consumer thread 함수 */
void Consumer(void *dummy)
{
    int     i, data;

    printf("Consumer: Start.....\n");

    for (i = 0 ; i < NLOOPS ; i++)  {
        /* mutex 잠금 */
        if (pthread_mutex_lock(&Mutex) < 0)  {
            perror("pthread_mutex_lock");
            pthread_exit(NULL);
        }

        /* 버퍼가 비어있다면 NotEmpty 조건 변수에서 대기함 */
        while (Buf.counter == 0)  {
            if (pthread_cond_wait(&NotEmpty, &Mutex) < 0)  {
                perror("pthread_cond_wait");
                pthread_exit(NULL);
            }
        }

        /* 데이터를 버퍼에서 소비함 */
        printf("Consumer: Consuming an item.....\n");
        data = Buf.buf[Buf.out].data;
        Buf.out = (Buf.out + 1) % MAX_BUF;
        Buf.counter--;

        /* NotFull 조건 변수에게 알림을 줌 */
        if (pthread_cond_signal(&NotFull) < 0)  {
            perror("pthread_cond_signal");
            pthread_exit(NULL);
        }
        /* mutex 해제 */
        if (pthread_mutex_unlock(&Mutex) < 0)  {
            perror("pthread_mutex_unlock");
            pthread_exit(NULL);
        }

        ThreadUsleep((rand()%100)*10000);
    }

    printf("Consumer: Consumed %d items.....\n", i);
    printf("Consumer: %d items in buffer.....\n", Buf.counter);

    pthread_exit(NULL);
}

main()
{
    pthread_t   tid1, tid2;

    srand(0x8888);

    /* 조건 변수(NotFull, NotEmpty)와 mutex를 초기화함 */
    if (pthread_cond_init(&NotFull, NULL) < 0)  {
        perror("pthread_cond_init");
        pthread_exit(NULL);
    }
    if (pthread_cond_init(&NotEmpty, NULL) < 0)  {
        perror("pthread_cond_init");
        pthread_exit(NULL);
    }
    if (pthread_mutex_init(&Mutex, NULL) < 0)  {
        perror("pthread_mutex_init");
        pthread_exit(NULL);
    }

    /* producer와 consumer thread를 생성함 */
    if (pthread_create(&tid1, NULL, (void *)Producer, (void *)NULL) < 0)  {
        perror("pthread_create");
        exit(1);
    }

    if (pthread_create(&tid2, NULL, (void *)Consumer, (void *)NULL) < 0)  {
        perror("pthread_create");
        exit(1);
    }

    /* thread 종료 대기 */
    if (pthread_join(tid1, NULL) < 0)  {
        perror("pthread_join");
        exit(1);
    }
    if (pthread_join(tid2, NULL) < 0)  {
        perror("pthread_join");
        exit(1);
    }

    printf("Main    : %d items in buffer.....\n", Buf.counter);

    /* 조건 변수 및 mutex 제거 */
    if (pthread_cond_destroy(&NotFull) < 0)  {
        perror("pthread_cond_destroy");
    }
    if (pthread_cond_destroy(&NotEmpty) < 0)  {
        perror("pthread_cond_destroy");
    }
    if (pthread_mutex_destroy(&Mutex) < 0)  {
        perror("pthread_mutex_destroy");
    }
}