#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/time.h>

/* 철학자의 수와 반복 횟수 정의 */
#define NUM_MEN     5
#define NLOOPS      5

/* 젓가락의 왼쪽과 오른쪽을 정의하고 계산함 */
#define LEFT(i)     ((i+NUM_MEN-1)%NUM_MEN)
#define RIGHT(i)    ((i+1)%NUM_MEN)

/* 철학자의 상태를 나타냄.
THINKING은 실행하지 않는 상태, EATING은 실행 상태, HUNGRY는 대기 상태 */
enum  { THINKING, EATING, HUNGRY };

/* 철학자와 mutex를 semaphore로 선언 */
sem_t   Philosopher[NUM_MEN];
sem_t   Mutex;
int     State[NUM_MEN];

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

/* 철학자가 생각하는 행위 구현 (실행하지 않는 상태) */
void Thinking(int id)
{
    printf("Philosopher%d: Thinking.....\n", id);
    ThreadUsleep((rand()%200)*10000);
    printf("Philosopher%d: Want to eat.....\n", id);
}

/* 철학자가 먹는 행위 구현 (실행 상태) */
void Eating(int id)
{
    printf("Philosopher%d: Eating.....\n", id);
    ThreadUsleep((rand()%100)*10000);
}

/* 왼쪽 철학자와 오른쪽 철학자가 먹고있지 않고, 내가 배고픈 경우 EATING하도록 하는 함수 */
void Test(int id)
{
    if (State[id] == HUNGRY && State[LEFT(id)] != EATING
            && State[RIGHT(id)] != EATING)  {
        State[id] = EATING;

        if (sem_post(&Philosopher[id]) < 0)  {
            perror("sem_post");
            pthread_exit(NULL);
        }
    }
}

/* 젓가락을 요구하는 행위 구현 (state를 hungry로 두어 배고프다는걸 알리고,
test를 해서 젓가락을 잡는 것을 시도함. 만약 젓가락이 없다면 wait함 */
void PickUp(int id)
{
    if (sem_wait(&Mutex) < 0)  {
        perror("sem_wait");
        pthread_exit(NULL);
    }

    State[id] = HUNGRY;

    Test(id);

    if (sem_post(&Mutex) < 0)  {
        perror("sem_post");
        pthread_exit(NULL);
    }

    if (sem_wait(&Philosopher[id]) < 0)  {
        perror("sem_wait");
        pthread_exit(NULL);
    }
}

/* 다 먹고난 뒤 state를 변경함. 양 옆의 철학자가 젓가락을 이용할 수 있도록
내가 젓가락을 내려놓았다는것을 알려줌 */
void PutDown(int id)
{
    if (sem_wait(&Mutex) < 0)  {
        perror("sem_wait");
        pthread_exit(NULL);
    }

    State[id] = THINKING;

    Test(LEFT(id));
    Test(RIGHT(id));

    if (sem_post(&Mutex) < 0)  {
        perror("sem_post");
        pthread_exit(NULL);
    }
}

/* 철학자의 행동을 구현한 함수 */
void DiningPhilosopher(int *pId)
{
    int     i;
    int     id = *pId;

    for (i = 0 ; i < NLOOPS ; i++)  {

        Thinking(id);

        PickUp(id);

        Eating(id);

        PutDown(id);
    }

    printf("Philosopher%d: thinking & eating %d times.....\n", id, i);

    pthread_exit(NULL);
}

main()
{
    pthread_t   tid[NUM_MEN];
    int         i, id[NUM_MEN];

    srand(0x8888);

    /* 철학자 별 semaphore 초기화 */
    for (i = 0 ; i < NUM_MEN ; i++)  {
        if (sem_init(&Philosopher[i], 0, 0) < 0)  {
            perror("sem_init");
            exit(1);
        }
        id[i] = i;
    }
    /* mutex 초기화 */
    if (sem_init(&Mutex, 0, 1) < 0)  {
        perror("sem_init");
        exit(1);
    }

    /* 철학자 thread 생성 */
    for (i = 0 ; i < NUM_MEN ; i++)  {
        if (pthread_create(&tid[i], NULL, (void *)DiningPhilosopher,
                (void *)&id[i]) < 0)  {
            perror("pthread_create");
            exit(1);
        }
    }

    /* 철학자 thread 종료를 대기함 */
    for (i = 0 ; i < NUM_MEN ; i++)  {
        if (pthread_join(tid[i], NULL) < 0)  {
            perror("pthread_join");
            exit(1);
        }
    }

    /* 철학자별 semaphore 제거 */
    for (i = 0 ; i < NUM_MEN ; i++)  {
        if (sem_destroy(&Philosopher[i]) < 0)  {
            perror("sem_destroy");
        }
    }
}