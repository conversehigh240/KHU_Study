#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

/* 문자열을 한 글자씩 출력하는 함수 */
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
   
    /* fork를 이용하여 child process 생성 */
    if ((pid = fork()) < 0) {
        perror("fork");
        exit(1);
    }

    /* 함수 내에서 루프를 999999만큼 돌기 때문에 자신에게 할당된 시간을 모두 사용하게 됨.
       child process에서 맨 앞 글자인 'o'를 출력한 후에 자신의 시간을 다 사용하므로
       다른 process를 실행해야 함.
       즉, shared resource를 여러 process가 사용하려고 하기 때문에 race condition이 발생함.
       이렇게 되면, 출력은 child process와 parent process가 번갈아가며 출력하게 되므로
       oouuttppuutt 이런 형태로 나오게 된다. */
    else if (pid == 0) {
        CharAtaTime("output from child\n");
    }
    else {
        CharAtaTime("output from parent\n");
    }
}