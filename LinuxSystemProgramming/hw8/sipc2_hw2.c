/*
sipc2에서 busy waiting을 없애고 semaphore를 이용하여 동기화 시킨 버전.
컴파일 후 ./sipc2_hw2를 실행한 후
sipc1_hw2의 답장을 받으면 semaphore와 shared memory를 삭제함
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "sipc.h"
#include "semlib.h"

main()
{
    char *ptr;
    int shmid;
    int emptySemid, fullSemid, mutexSemid;

    /* shared memory 생성 */
    if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0)  {
        perror("shmget");
        exit(1);
    }
   
    if ((ptr = shmat(shmid, 0, 0)) == (void *) -1)  {
        perror("shmat");
        exit(1);
    }

    /* semaphore 생성 */
    if ((emptySemid = semInit(EMPTY_SEM_KEY)) < 0)  {
        fprintf(stderr, "semInit failure\n");
        exit(1);
    }
    if ((fullSemid = semInit(FULL_SEM_KEY)) < 0)  {
        fprintf(stderr, "semInit failure\n");
        exit(1);
    }
    if ((mutexSemid = semInit(MUTEX_SEM_KEY)) < 0)  {
        fprintf(stderr, "semInit failure\n");
        exit(1);
    }

    /* wait (empty) */
    if (semWait(emptySemid) < 0)  {
        fprintf(stderr, "semWait failure\n");
        exit(1);
    }
    /* wait(mutex) */
    if (semWait(mutexSemid) < 0)  {
        fprintf(stderr, "semWait failure\n");
        exit(1);
    }

    sprintf(ptr, "This is a request from %d.", getpid());
    printf("Sent a request.....\n");

    /* signal(mutex) */
    if (semPost(mutexSemid) < 0)  {
        fprintf(stderr, "semPost failure\n");
        exit(1);
    }
    /* signal(full) */
    if (semPost(fullSemid) < 0)  {
        fprintf(stderr, "semPost failure\n");
        exit(1);
    }

    sleep(2);

    if (semWait(fullSemid) < 0)  {
        fprintf(stderr, "semWait failure\n");
        exit(1);
        }
    if (semWait(mutexSemid) < 0)  {
        fprintf(stderr, "semWait failure\n");
        exit(1);
    }

    printf("Received reply: %s\n", ptr);

    /* semaphore 삭제 */
    if (semDestroy(emptySemid) < 0)  {
        fprintf(stderr, "semDestroy failure\n");
    }
    if (semDestroy(fullSemid) < 0)  {
        fprintf(stderr, "semDestroy failure\n");
    }
    if (semDestroy(mutexSemid) < 0)  {
        fprintf(stderr, "semDestroy failure\n");
    }
    /* shared memory 삭제 */
    if (shmctl(shmid, IPC_RMID, 0) < 0)  {
        perror("shmctl");
        exit(1);
    }
}