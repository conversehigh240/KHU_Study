/*
   make를 이용하여 컴파일 한 뒤
   $ ./child
   라고 입력하여 실행함.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

void SigChildHandler(int signo)
{
    int status;
    pid_t pid = wait(&status);
    if (pid > 0)
    {
        printf("A child killed with %d status\n", status);
    }
}

main()
{
    pid_t pid;

    /* SIGCHLD 시그널 발생 시 SigChildHandler 함수 호출 */
    signal(SIGCHLD, SigChildHandler);

    /* 프로세스를 fork하여 child process를 만듦 */
    if ((pid = fork()) < 0) {
        perror("fork");
        exit(1);
    }

    /* child process가 실행하는 부분 */
    else if (pid == 0) {
        printf("I'm a child\n");
        sleep(2);
    }

    /* parent process는 child process가 끝나길 기다리는데, wait이 아닌 시그널을 이용함.
       pause를 통해 시그널이 들어올 때 까지 기다리고, SIGCHLD는 child process가 끝났음을
       parent process에게 알려주는 시그널이므로 pause를 통해 시그널이 감지되면 child process가
       끝났다는 의미임.
       이 때, signal이 아닌 wait을 사용한다면 parent process가 child process의 종료를 기다리며,
       child process가 종료된 후에야 parent process의 작업을 실행할 수 있기 때문에 synchronous하다고
       볼 수 있음.
       하지만 signal을 사용하면 parent process가 child process의 종료를 직접 기다리지 않고,
       신호를 수신하기 전까지 다른 작업을 수행할 수 있기 때문에 asynchronous한 것임. */
    else {
        while (1) {
            pause();
        }
    }
}