/*
sipc1을 busy waiting을 없애고 semaphore를 통해 동기화 시킨 버전.
컴파일 후 ./sipc1_hw2를 실행한 뒤, sipc2_hw2의 메세지를 기다림
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "sipc.h"
#include "semlib.h"

int main() {
    int     shmid;
    char    *ptr;
    int     emptySemid, fullSemid, mutexSemid;

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

    /* semaphore 초기화 */
    if (semInitValue(emptySemid, 1) < 0)  {
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

    /* wait (full) */
    if (semWait(fullSemid) < 0)  {
        fprintf(stderr, "semWait failure\n");
        exit(1);
    }
    /* wait (mutex) */
    if (semWait(mutexSemid) < 0)  {
        fprintf(stderr, "semWait failure\n");
        exit(1);
    }

    /* sipc2로부터 메세지를 전달받음 */
    printf("Received request: %s.....\n", ptr);
    sprintf(ptr, "This is a reply from %d.", getpid());
    printf("Replied.\n");

    /* signal(mutex) */
    if (semPost(mutexSemid) < 0)  {
        fprintf(stderr, "semPost failure\n");
        exit(1);
    }
    /* signal(empty) */
    if (semPost(emptySemid) < 0)  {
        fprintf(stderr, "semPost failure\n");
        exit(1);
    }

}