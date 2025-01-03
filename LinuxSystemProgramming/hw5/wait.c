#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

main()
{
    pid_t pid;
    int status;

    /* fork를 이용하여 child process 생성 */
    if ((pid = fork()) < 0) {
        perror("fork");
        exit(1);
    }
    /* child process는 문구 출력 후 2초간 대기 */
    else if (pid == 0) {
        printf("I'm a child\n");
        sleep(2);
    }
    else {
        /* 내가 만든 child process가 종료되길 기다림.
           여기서 파라미터에 포인터 변수를 주어야 하기 때문에 &status를 이용함.
           포인터 변수를 사용하는 이유는, 종료된 프로세스가 exit 하면서 status를 return하기 때문.*/
        wait(&status);
        printf("A child killed with %d status\n", status);
    }
}