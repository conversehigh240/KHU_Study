#include <stdio.h>
#include <signal.h>

void SigAlarmHandler(int signo)
{
    /* nothing to do, just return to wake up the pause */
    return;
}

unsigned int mysleep(unsigned int nsecs)
{

    /* SIGALRM 시그널이 발생할 때 SigAlarmHandler를 호출하도록 등록함. */
    if (signal(SIGALRM, SigAlarmHandler) == SIG_ERR) {
        return nsecs;
    }

    /* nsecs 후에 SIGALRM 시그널 발생 */
    alarm(nsecs);

    pause();

    /* alarm(0)은 이전에 한 alarm부터 지금까지 시간이 얼마나 지났는가를 return함.
       정상적으로 nsecs 후에 시그널이 갔다면 return value가 0이 되지만,
       nsecs가 지나기 전에 시그널이 간다면 그 시간의 차이만큼 return됨.
       이 return value를 체크해서 error handling을 함. */
    return alarm(0);
}

main()
{
    printf("Wait for 5 seconds...\n");

    mysleep(5);
}