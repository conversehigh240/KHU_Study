#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/time.h>

/* 철학자의 수와 반복하는 횟수를 5로 지정 */
#define NUM_MEN     5
#define NLOOPS      5

/* 철학자들이 사용하는 젓가락을 semaphore로 이용 */
sem_t   ChopStick[NUM_MEN];

/* 지정된 시간동안 대기하는 함수 */
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

/* Thinking은 실행하지 않는 상태 */
void Thinking(int id)
{
    printf("Philosopher%d: Thinking.....\n", id);
    ThreadUsleep((rand()%200)*10000);
    printf("Philosopher%d: Want to eat.....\n", id);
}

/* Eating은 실행 상태 */
void Eating(int id)
{
    printf("Philosopher%d: Eating.....\n", id);
    ThreadUsleep((rand()%100)*10000);
}

void DiningPhilosopher(int *pId)
{
    int     i;
    int     id = *pId;

    for (i = 0 ; i < NLOOPS ; i++)  {

        Thinking(id); // 실행하지 않고 대기함 (생각단계)

        /* 왼쪽 젓가락과 오른쪽 젓가락을 집음 */
        if (sem_wait(&ChopStick[id]) < 0)  {
            perror("sem_wait");
            pthread_exit(NULL);
        }
        if (sem_wait(&ChopStick[(id+1) % NUM_MEN]) < 0)  {
            perror("sem_wait");
            pthread_exit(NULL);
        }

        /* 실행 (식사 단계) */
        Eating(id);

        /* 실행이 끝났으니 젓가락을 내려놓음 (반환) */
        if (sem_post(&ChopStick[id]) < 0)  {
            perror("sem_post");
            pthread_exit(NULL);
        }
        if (sem_post(&ChopStick[(id+1) % NUM_MEN]) < 0)  {
            perror("sem_post");
            pthread_exit(NULL);
        }
    }

    printf("Philosopher%d: thinking & eating %d times.....\n", id, i);

    pthread_exit(NULL);
}

main()
{
    pthread_t   tid[NUM_MEN];
    int         i, id[NUM_MEN];

    srand(0x8888);

    /* 각 젓가락에 대해 semaphore를 초기화함 */
    for (i = 0 ; i < NUM_MEN ; i++)  {
        if (sem_init(&ChopStick[i], 0, 1) < 0)  {
            perror("sem_init");
            exit(1);
        }
        id[i] = i;
    }

    /* 철학자의 thread를 생성함 */
    for (i = 0 ; i < NUM_MEN ; i++)  {
        if (pthread_create(&tid[i], NULL, (void *)DiningPhilosopher,
                (void *)&id[i]) < 0)  {
            perror("pthread_create");
            exit(1);
        }
    }

    /* 철학자의 thread가 종료되기를 대기함 */
    for (i = 0 ; i < NUM_MEN ; i++)  {
        if (pthread_join(tid[i], NULL) < 0)  {
            perror("pthread_join");
            exit(1);
        }
    }

    /* 젓가락 semaphore를 제거함 */
    for (i = 0 ; i < NUM_MEN ; i++)  {
        if (sem_destroy(&ChopStick[i]) < 0)  {
            perror("sem_destroy");
        }
    }
}