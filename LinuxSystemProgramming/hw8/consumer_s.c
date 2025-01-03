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

    /* shared memory를 생성함. 기존에 존재하면 그 키값을 가져옴 */
    if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0)  {
        perror("shmget");
        exit(1);
    }

    /* shared memory를 process와 연결함 */
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

    /* empty, full, mutex semaphore 초기값 설정 */
    if (semInitValue(emptySemid, MAX_BUF) < 0)  {
        fprintf(stderr, "semInitValue failure\n");
        exit(1);
    }
    if (semInitValue(fullSemid, 0) < 0)  {
        fprintf(stderr, "semInitValue failure\n");
        exit(1);
    }
    if (semInitValue(mutexSemid, 1) < 0)  {
        fprintf(stderr, "semInitValue failure\n");
        exit(1);
    }

    srand(0x9999);

    /* NLOOPS 횟수만큼 데이터를 소비함 */
    for (i = 0 ; i < NLOOPS ; i++)  {
        /* full semaphore 감소 (버퍼에 데이터가 존재하는지 확인) */
        if (semWait(fullSemid) < 0)  {
            fprintf(stderr, "semWait failure\n");
            exit(1);
        }

        /* mutex semaphore 감소 (버퍼의 접근을 잠금) */
        if (semWait(mutexSemid) < 0)  {
            fprintf(stderr, "semWait failure\n");
            exit(1);
        }

        /* 버퍼에서 데이터를 읽어들임 */
        printf("Consumer: Consuming an item.....\n");
        data = pBuf->buf[pBuf->out].data;
        pBuf->out = (pBuf->out + 1) % MAX_BUF;
        pBuf->counter--;

        /* mutex semaphore 증가 (버퍼 접근 해제) */
        if (semPost(mutexSemid) < 0)  {
            fprintf(stderr, "semPost failure\n");
            exit(1);
        }
        /* empty semaphore 증가 (빈 공간 증가) */
        if (semPost(emptySemid) < 0)  {
            fprintf(stderr, "semPost failure\n");
            exit(1);
        }
        /* 데이터 소비 후 대기 */
        usleep((rand()%100)*10000);
    }

    printf("Consumer: Consumed %d items.....\n", i);
    printf("Consumer: %d items in buffer.....\n", pBuf->counter);
}