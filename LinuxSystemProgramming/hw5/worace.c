#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

/* 문자열을 한 글자씩 출력해주는 함수 */
void CharAtaTime(char *str)
{
    char *ptr;
    int c, i;

    setbuf(stdout, NULL);
    for (ptr = str; c = *ptr++; ) {
        for (i = 0; i < 999999; i++)
            ;
        putc(c, stdout);
    }
}

main()
{
    pid_t pid;

    if ((pid = fork()) < 0) {
        perror("fork");
        exit(1);
    }
    else if (pid == 0) {
        CharAtaTime("output from child\n");
    }
    else {
        /* race.c 에서와 다른 점은, wait을 걸어주어 race condition을 방지하는 것이다.
           wait은 parent process가 자신이 만든 child process의 종료를 기다리는 것이므로
           child process가 완전히 종료된 후에 parent process가 실행되는 것을 알 수 있다. */
        wait(NULL);
        CharAtaTime("output from parent\n");
    }
}