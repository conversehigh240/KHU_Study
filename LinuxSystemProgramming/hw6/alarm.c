#include <stdio.h>
#include <signal.h>

static unsigned int AlarmSecs;

void SigAlarmHandler(int signo)
{
    /* SIGALRM 시그널 발생 시 SigAlarmHandler 호출 */
    if (signal(SIGALRM, SigAlarmHandler) == SIG_ERR)
    {
        perror("signal");
        exit(1);
    }

    /* AlarmSecs 후에 SIGALRM 시그널 발생 */
    alarm(AlarmSecs);

    /* Do something */

    printf(".");

    /* 버퍼를 강제로 비워 즉시 화면에 출력할 수 있도록 함 */
    fflush(stdout);

    return;
}

int SetPeriodicAlarm(unsigned int nsecs)
{
    /* SIGALRM 시그널 발생 시 SigAlarmHandler 호출 */
    if (signal(SIGALRM, SigAlarmHandler) == SIG_ERR) {
        return -1;
    }

    /* alarm 주기를 nsecs로 설정 */
    AlarmSecs = nsecs;
   
    /* nsecs 후에 SIGALRM 시그널 발생 */
    alarm(nsecs);

    return 0;
}

main()
{
    printf("Doing something every one seconds\n");

    /* SetPeriodicAlarm 함수를 이용하여 1초마다 주기적인 알람을 설정함 */
    SetPeriodicAlarm(1);

    for ( ; ; )
        pause();
}