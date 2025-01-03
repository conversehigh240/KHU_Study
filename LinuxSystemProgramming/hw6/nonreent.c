#include <stdio.h>
#include <signal.h>
#include <pwd.h>

void MyAlarmHandler(int signo)
{
    struct passwd *rootptr;

    signal(SIGALRM, MyAlarmHandler);
    alarm(1);

    printf("in signal handler\n");

    /* main에서 getpwnam을 사용하는데, 시그널 핸들러 함수 내에서도 또 getpwnam을 사용해서
       root에 대한 정보를 읽어오려고 함.
       getpwnam을 실행하는 와중에 또 getpwnam을 실행하게 된 것.
       비정상적인 호출로 프로그램은 종료됨.
       getpwnam은 non-reentrant function임. */
    if ((rootptr = getpwnam("root")) == NULL) {
        perror("getpwnam");
        exit(1);
    }
   
    return;
}

main()
{
    struct passwd *ptr;

    /* SIGALRM 시그널 발생 시 MyAlarmHandler 호출 */
    signal(SIGALRM, MyAlarmHandler);
    alarm(1);

    /* 루프를 돌면서 getpwnam으로 유저 네임 정보를 읽어옴 */
    for( ; ; ) {
        if ((ptr = getpwnam("lsp25")) == NULL) {
            perror("getpwnam");
            exit(1);
        }

        /* 이름이 lsp25와 다르면 프로그램 종료 */
        if (strcmp(ptr -> pw_name, "lsp25") != 0) {
            printf("return value corrupted!, pw_name = %s\n", ptr->pw_name);
            exit(0);
        }
    }
}