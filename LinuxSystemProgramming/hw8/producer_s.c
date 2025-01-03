#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "semlib.h"
#include "prodcons.h"

main()
{
    BoundedBufferType   *pBuf;
    int                 shmid, i, data;
    int                 emptySemid, fullSemid, mutexSemid;

    /* shared memory를 생성함. 기존에 존재한다면 그 키값을 가져옴 */
    if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0)  {
        perror("shmget");
        exit(1);
    }

    /* shared memory를 프로세스와 연결함 */
    if ((pBuf = (BoundedBufferType *)shmat(shmid, 0, 0)) == (void *) -1)  {
        perror("shmat");
        exit(1);
    }

    /* empty semaphore 초기화 */
    if ((emptySemid = semInit(EMPTY_SEM_KEY)) < 0)  {
        fprintf(stderr, "semInit failure\n");
        exit(1);
    }

    /* full semaphore 초기화 */
    if ((fullSemid = semInit(FULL_SEM_KEY)) < 0)  {
        fprintf(stderr, "semInit failure\n");
        exit(1);
    }

    /* mutex semaphore 초기화 */
    if ((mutexSemid = semInit(MUTEX_SEM_KEY)) < 0)  {
        fprintf(stderr, "semInit failure\n");
        exit(1);
    }

    srand(0x8888);
    /* NLOOPS 횟수만큼 데이터를 만듦 */
    for (i = 0 ; i < NLOOPS ; i++)  {
        /* empty semaphore 감소(빈 공간 확보를 확인함) */
        if (semWait(emptySemid) < 0)  {
            fprintf(stderr, "semWait failure\n");
            exit(1);
        }

        /* mutex semaphore 감소 (버퍼의 접근을 잠금) */
        if (semWait(mutexSemid) < 0)  {
            fprintf(stderr, "semWait failure\n");
            exit(1);
        }

        /* 버퍼에 데이터를 생성하여 저장 */
        printf("Producer: Producing an item.....\n");
        data = (rand()%100)*10000;
        pBuf->buf[pBuf->in].data = data;
        pBuf->in = (pBuf->in + 1) % MAX_BUF;
        pBuf->counter++;

        /* mutex semaphore 증가 (버퍼의 접근을 해제함) */
        if (semPost(mutexSemid) < 0)  {
            fprintf(stderr, "semPost failure\n");
            exit(1);
        }

        /* full semaphore 증가 (채워진 공간 증가) */
        if (semPost(fullSemid) < 0)  {
            fprintf(stderr, "semPost failure\n");
            exit(1);
        }
        /* 데이터 생성 후 대기함 */
        usleep(data);
    }

    printf("Producer: Produced %d items.....\n", i);

    sleep(2);
    printf("Producer: %d items in buffer.....\n", pBuf->counter);

    /* empty semaphore 제거 */
    if (semDestroy(emptySemid) < 0)  {
        fprintf(stderr, "semDestroy failure\n");
    }

    /* full semaphore 제거 */
    if (semDestroy(fullSemid) < 0)  {
        fprintf(stderr, "semDestroy failure\n");
    }

    /* mutex semaphore 제거 */
    if (semDestroy(mutexSemid) < 0)  {
        fprintf(stderr, "semDestroy failure\n");
    }

    /* shared memory 해제 */
    if (shmctl(shmid, IPC_RMID, 0) < 0)  {
        perror("shmctl");
        exit(1);
    }
}