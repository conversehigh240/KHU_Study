#include <stdio.h>
#include <pthread.h>
#include "semlib2.h"

/* semaphore 초기화 함수 */
int sem_init(sem_t *sem, int pshared, int value)
{
    if (pshared)  {
        fprintf(stderr, "Function not supported\n");
        return -1;
    }

    /* mutex 초기화 */
    if (pthread_mutex_init(&sem->mutex, NULL) < 0)
        return -1;

    /* conditional variable 초기화 */
    if (pthread_cond_init(&sem->cond, NULL) < 0)
        return -1;

    /* 초기 semaphore 값 설정 */
    sem->sval = value;

    return 0;
}

/* semaphore 대기 함수(감소 연산) */
int sem_wait(sem_t *sem)
{
    /* mutex 잠금 */
    if (pthread_mutex_lock(&sem->mutex) < 0)
        return -1;

    /* semaphore 값이 0일 경우 대기 */
    if (sem->sval == 0)  {
        while (sem->sval == 0)  {
            if (pthread_cond_wait(&sem->cond, &sem->mutex) < 0)  {
                if (pthread_mutex_unlock(&sem->mutex) < 0)
                    return -1;
                return -1;
            }
        }
        sem->sval--;
    }
    else  {
        sem->sval--;
    }

    /* mutex 해제 */
    if (pthread_mutex_unlock(&sem->mutex) < 0)
        return -1;

    return 0;
}

/* semaphore 대기 함수(만약 0이면 false 리턴) */
int sem_trywait(sem_t *sem)
{
    /* mutex 잠금 */
    if (pthread_mutex_lock(&sem->mutex) < 0)
        return -1;

    /* semaphore 값이 0이면 false 반환 */
    if (sem->sval == 0)  {
        if (pthread_mutex_unlock(&sem->mutex) < 0)
            return -1;
        return -1;
    }
    else  {
        sem->sval--;
    }

    /* mutex 해제 */
    if (pthread_mutex_unlock(&sem->mutex) < 0)
        return -1;

    return 0;
}

/* semaphore post 함수 (증가 연산) */
int sem_post(sem_t *sem)
{
    /* mutex 잠금 */
    if (pthread_mutex_lock(&sem->mutex) < 0)
        return -1;

    /* semaphore 값이 0인 경우 conditional variable에 signal을 줌 */
    if (sem->sval == 0)  {
        if (pthread_cond_signal(&sem->cond) < 0)  {
            if (pthread_mutex_unlock(&sem->mutex) < 0)
                return -1;
            return -1;
        }
    }

    /* semaphore 값 증가 */
    sem->sval++;

    /* mutex 해제 */
    if (pthread_mutex_unlock(&sem->mutex) < 0)
        return -1;

    return 0;
}

/* semaphore 값 조회 */
int sem_getvalue(sem_t *sem, int *sval)
{
    *sval = sem->sval;

    return 0;
}

/* semaphore 제거 */
int sem_destroy(sem_t *sem)
{
    if (pthread_mutex_destroy(&sem->mutex) < 0)
        return -1;

    if (pthread_cond_destroy(&sem->cond) < 0)
        return -1;

    return 0;
}