#include <stdio.h>
#include <signal.h>

void SigIntHandler(int signo)
{
    printf("Received a SIGINT signal\n");
    printf("Terminate this process\n");

    exit(0);
}

main()
{
    /* signal이라는 시스템콜을 이용해서 시그널 핸들러를 등록함
       첫번재 파라미터는 시그널 번호, 두번째 파라미터는 시그널 핸들러 함수
       여기서 SIGINT는 interrupt의 약자로, ctrl + c 의 동작을 뜻함. */
    signal(SIGINT, SigIntHandler);

    printf("Press ^C to quit\n");

    for ( ; ; )
        pause();
}