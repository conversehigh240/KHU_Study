#include <stdio.h>
#include "synclib.h"

#define NLOOPS  5


main()
{
    int     i;
    pid_t   pid;

    /* 부모 프로세스와 자식 프로세스 간 동기화를 위한 초기화 */
    TELL_WAIT();

    /* 자식 프로세스 생성 */
    if ((pid = fork()) < 0)  {
        perror("fork");
        exit(1);
    }

    /* 부모 프로세스는 자식 프로세스에게 신호를 보낸 후
    자식 프로세스가 신호를 보낼 때 까지 대기함 */
    else if (pid > 0)  {
        for (i = 0 ; i < NLOOPS ; i++)  {
            TELL_CHILD();
            printf("Parent: Tell to child\n");
            printf("Parent: Wait for child to tell\n");
            WAIT_CHILD();
        }
    }

    /* 자식 프로세스는 부모 프로세스의 신호를 기다린 후
    부모 프로세스에게 신호를 보냄 */
    else  {
        for (i = 0 ; i < NLOOPS ; i++)  {
            printf("Child: Wait for parent to tell\n");
            WAIT_PARENT();
            TELL_PARENT();
            printf("Child: Tell to parent\n");
        }
    }
}