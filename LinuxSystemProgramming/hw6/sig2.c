#include <stdio.h>
#include <signal.h>

void SigUsrHandler(int signo)
{
    if (signo == SIGUSR1) {
        printf("Received a SIGUSR1 signal\n");
    }
    else if (signo == SIGUSR2) {
        printf("Received a SIGUSR2 signal\n");
    }
    else {
        printf("Received unknown signal\n");
        printf("Terminate this process\n");
        exit(0);
    }
}

main()
{
    /* signal 시스템콜을 이용해서 SIGUSR1 시그널이 들어왔을 때 SigUsrHandler 핸들러 함수가
       동작하도록 등록함.*/
    if (signal(SIGUSR1, SigUsrHandler) == SIG_ERR) {
        perror("signal");
        exit(1);
    }

    /* SIGUSR2 시그널이 들어왔을 때 SigUsrHandler 핸들러 함수가 동작하도록 등록함. */
    if (signal(SIGUSR2, SigUsrHandler) == SIG_ERR) {
        perror("signal");
        exit(1);
    }

    for ( ; ; )
        pause();
}