/*
컴파일 후 ./mipc2 실행 시 아래와 같은 결과가 출력됨.
Parent: counter=0
Child : counter=1
Parent: counter=2
Child : counter=3
Parent: counter=4
Child : counter=5
Parent: counter=6
Child : counter=7
Parent: counter=8
Child : counter=9

해당 프로그램은 open, mmap 대신 shmget, shmat을 포함한 shared memory system call을
사용한 방식으로 수정됨.
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "synclib.h"

#define NLOOPS 10
#define SIZE sizeof(long)

int update(long *ptr)
{
    return ((*ptr)++);
}

main()
{
    int shmid, i, counter;
    pid_t pid;
    long *area;

    /* shared memory 할당 */
    if ((shmid = shmget(IPC_PRIVATE, SIZE, SHM_R | SHM_W | IPC_CREAT)) < 0) {
        perror("shmget");
        exit(1);
    }

    /* shared memory 연결 */
    if ((area = (long *)shmat(shmid, 0, 0)) == (long *) -1) {
        perror("shmat");
        exit(1);
    }

    /* 초기화 */
    *area = 0;

    TELL_WAIT();

    if ((pid = fork()) < 0) {
        perror("fork");
        exit(1);
    }
    else if (pid > 0) {
        for (i = 0; i < NLOOPS; i += 2) {
            if ((counter = update(area)) != i) {
                fprintf(stderr, "Counter mismatch\n");
                exit(1);
            }
            printf("Parent: counter=%d\n", counter);
            TELL_CHILD();
            WAIT_CHILD();
        }
    }
    else {
        for (i = 1; i < NLOOPS; i += 2) {
            WAIT_PARENT();
            if ((counter = update(area)) != i) {
                fprintf(stderr, "Counter mismatch\n");
                exit(1);
            }
            printf("Child : counter=%d\n", counter);
            TELL_PARENT();
        }
    }

    /* shared memory 제거 */
    if (shmctl(shmid, IPC_RMID, 0) < 0) {
        perror("shmctl");
        exit(1);
    }
}