#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include "prodcons.h"

BoundedBufferType   Buf;

/* 일정 시간동안 thread를 정지시킴 */
void ThreadUsleep(int usecs)
{
    pthread_cond_t      cond;
    pthread_mutex_t     mutex;
    struct timespec     ts;
    struct timeval      tv;

    /* condition variable을 만들어줌 */
    if (pthread_cond_init(&cond, NULL) < 0)  {
        perror("pthread_cond_init");
        pthread_exit(NULL);
    }

    /* mutex를 만들어줌 */
    if (pthread_mutex_init(&mutex, NULL) < 0)  {
        perror("pthread_mutex_init");
        pthread_exit(NULL);
    }

    /* 현재 시간과 지정된 대기 시간을 기반으로 대기 종료 시간을 설정함. */
    gettimeofday(&tv, NULL);
    ts.tv_sec = tv.tv_sec + usecs/1000000;
    ts.tv_nsec = (tv.tv_usec + (usecs%1000000)) * 1000;
    if (ts.tv_nsec >= 1000000000)  {
        ts.tv_nsec -= 1000000000;
        ts.tv_sec++;
    }

    /* mutex를 잠그고 대기 */
    if (pthread_mutex_lock(&mutex) < 0)  {
        perror("pthread_mutex_lock");
        pthread_exit(NULL);
    }
    if (pthread_cond_timedwait(&cond, &mutex, &ts) < 0)  {
        perror("pthread_cond_timedwait");
        pthread_exit(NULL);
    }

    /* condition variable과 mutex를 해제시킴 */
    if (pthread_cond_destroy(&cond) < 0)  {
        perror("pthread_cond_destroy");
        pthread_exit(NULL);
    }
    if (pthread_mutex_destroy(&mutex) < 0)  {
        perror("pthread_mutex_destroy");
        pthread_exit(NULL);
    }
}

/* thread 함수 (producer) */
void Producer(void *dummy)
{
    int     i, data;

    printf("Producer: Start.....\n");

    /* NLOOPS만큼 반복하며 데이터를 생성함 */
    for (i = 0 ; i < NLOOPS ; i++)  {
        /* 버퍼가 꽉 찬 경우 대기 */
        if (Buf.counter == MAX_BUF)  {
            printf("Producer: Buffer full. Waiting.....\n");
            while (Buf.counter == MAX_BUF)
                ;
        }

        /* 데이터를 생성해서 버퍼에 추가함 */
        printf("Producer: Producing an item.....\n");
        data = (rand()%100)*10000;
        Buf.buf[Buf.in].data = data;
        Buf.in = (Buf.in + 1) % MAX_BUF;
        Buf.counter++;

        /* 생성된 데이터만큼 대기함 */
        ThreadUsleep(data);
    }

    printf("Producer: Produced %d items.....\n", i);
    printf("Producer: %d items in buffer.....\n", Buf.counter);

    pthread_exit(NULL);
}

/* thread 함수(consumer) */
void Consumer(void *dummy)
{
    int     i, data;

    printf("Consumer: Start.....\n");

    /* NLOOPS만큼 반복하며 데이터를 읽음 */
    for (i = 0 ; i < NLOOPS ; i++)  {
        /* 버퍼가 비어있는 경우 대기 */
        if (Buf.counter == 0)  {
            printf("Consumer: Buffer empty. Waiting.....\n");
            while (Buf.counter == 0)
                ;
        }

        /* 버퍼에서 데이터를 읽고 제거함 */
        printf("Consumer: Consuming an item.....\n");
        data = Buf.buf[Buf.out].data;
        Buf.out = (Buf.out + 1) % MAX_BUF;
        Buf.counter--;

        /* 랜덤 시간 만큼 대기함 */
        ThreadUsleep((rand()%100)*10000);
    }

    printf("Consumer: Consumed %d items.....\n", i);
    printf("Consumer: %d items in buffer.....\n", Buf.counter);

    pthread_exit(NULL);
}

main()
{
    pthread_t   tid1, tid2;

    srand(0x8888); // 랜덤 시드 설정

    /* producer thread 생성 */
    if (pthread_create(&tid1, NULL, (void *)Producer, (void *)NULL) < 0)  {
        perror("pthread_create");
        exit(1);
    }

    /* consumer thread 생성 */
    if (pthread_create(&tid2, NULL, (void *)Consumer, (void *)NULL) < 0)  {
        perror("pthread_create");
        exit(1);
    }

    /* thread가 종료될 때까지 대기 */
    if (pthread_join(tid1, NULL) < 0)  {
        perror("pthread_join");
        exit(1);
    }
    if (pthread_join(tid2, NULL) < 0)  {
        perror("pthread_join");
        exit(1);
    }

    printf("Main    : %d items in buffer.....\n", Buf.counter);
}