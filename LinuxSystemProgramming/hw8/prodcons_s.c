#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include "semlib2.h"
#include "prodcons.h"

BoundedBufferType   Buf;
sem_t               EmptySem, FullSem, MutexSem;

/* 특정 시간동안 대기하는 함수 */
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
        /* 버퍼에 빈 공간이 생길 때까지 대기 */
        if (sem_wait(&EmptySem) < 0)  {
            perror("sem_wait");
            pthread_exit(NULL);
        }
        /* mutex를 사용하여 버퍼의 접근을 보호함 */
        if (sem_wait(&MutexSem) < 0)  {
            perror("sem_wait");
            pthread_exit(NULL);
        }

        /* 데이터 생성 및 버퍼에 추가함 */
        printf("Producer: Producing an item.....\n");
        data = (rand()%100)*10000;
        Buf.buf[Buf.in].data = data;
        Buf.in = (Buf.in + 1) % MAX_BUF;
        Buf.counter++;

        /* mutex를 해제하고 full semaphore 값을 증가시킴 */
        if (sem_post(&MutexSem) < 0)  {
            perror("sem_post");
            pthread_exit(NULL);
        }
        if (sem_post(&FullSem) < 0)  {
            perror("sem_post");
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
        /* 버퍼에 데이터가 생길 때 까지 대기 */
        if (sem_wait(&FullSem) < 0)  {
            perror("sem_wait");
            pthread_exit(NULL);
        }
        /* mutex를 사용하여 버퍼의 접근을 보호함 */
        if (sem_wait(&MutexSem) < 0)  {
            perror("sem_wait");
            pthread_exit(NULL);
        }

        /* 데이터를 소비하고 버퍼에서 제거함 */
        printf("Consumer: Consuming an item.....\n");
        data = Buf.buf[Buf.out].data;
        Buf.out = (Buf.out + 1) % MAX_BUF;
        Buf.counter--;

        /* mutex를 해제하고 empty semaphore 값을 증가시킴 */
        if (sem_post(&MutexSem) < 0)  {
            perror("sem_post");
            pthread_exit(NULL);
        }
        if (sem_post(&EmptySem) < 0)  {
            perror("sem_post");
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

    /* empty semaphore, full semaphore, mutex semaphore 초기화 */
    if (sem_init(&EmptySem, 0, MAX_BUF) < 0)  {
        perror("sem_init");
        exit(1);
    }
    if (sem_init(&FullSem, 0, 0) < 0)  {
        perror("sem_init");
        exit(1);
    }
    if (sem_init(&MutexSem, 0, 1) < 0)  {
        perror("sem_init");
        exit(1);
    }

    /* producer와 consumer thread 생성 */
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

    /* semaphore 제거 */
    if (sem_destroy(&EmptySem) < 0)  {
        perror("sem_destroy");
    }
    if (sem_destroy(&FullSem) < 0)  {
        perror("sem_destroy");
    }
    if (sem_destroy(&MutexSem) < 0)  {
        perror("sem_destroy");
    }
}