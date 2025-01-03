/*
부모 자식 간 동기화를 이용해 공유된 메모리 영역을 업데이트 하는 프로그램임
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include "synclib.h"

#define NLOOPS  10
#define SIZE    sizeof(long)


int update(long *ptr)
{
    return ((*ptr)++);
}

main()
{
    int     fd, i, counter;
    pid_t   pid;
    caddr_t area;

    /* /dev/zero를 열어 shared memory를 초기화 함 */
    if ((fd = open("/dev/zero", O_RDWR)) < 0)  {
        perror("open");
        exit(1);
    }

    /* shared memory mapping : read-write 가능, 부모-자식 간 공유 */
    if ((area = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0))
        == (caddr_t) -1)  {
        perror("mmap");
        exit(1);
    }
    close(fd);

    /* 부모-자식 간 동기화 준비 */
    TELL_WAIT();

    /* 자식 프로세스 생성 */
    if ((pid = fork()) < 0)  {
        perror("fork");
        exit(1);
    }

    /* 부모 프로세스는 짝수 순서에만 shared memory 값을 업데이트하고 예상 값과 비교함.
    값이 예상과 다르다면 오류 출력 */
    else if (pid > 0)  {
        for (i = 0 ; i < NLOOPS ; i += 2)  {
            if ((counter = update((long *) area)) != i)  {
                fprintf(stderr, "Counter mismatch\n");
                exit(1);
            }
            printf("Parent: counter=%d\n", counter);
            TELL_CHILD();
            WAIT_CHILD();
        }
    }

    /* 자식 프로세스는 홀수 순서에서만 shared memory 값을 업데이트하고 예상 값과 비교함.
    값이 예상과 다르면 오류 출력하며 이 작업은 부모 프로세스의 작업이 완료되기를
    대기한 후 실행됨. */
    else  {
        for (i = 1 ; i < NLOOPS ; i += 2)  {
            WAIT_PARENT();
            if ((counter = update((long *) area)) != i)  {
                fprintf(stderr, "Counter mismatch\n");
                exit(1);
            }
            printf("Child : counter=%d\n", counter);
            TELL_PARENT();
        }
    }
}