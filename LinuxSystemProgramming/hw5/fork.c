#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int Var = 6;
char Buf[] = "a write to stdout\n";

main()
{
    int var;
    pid_t pid;

    var = 88;

    /* write 함수로 Buf 내용을 출력함. 즉, printf와 동일한 기능을 함. */
    write(STDOUT_FILENO, Buf, sizeof(Buf) - 1);
    printf("Before fork\n");

    /* fork를 이용하여 새로운 자식 프로세스를 생성함 */
    if ((pid = fork()) < 0) {
        perror("fork");
        exit(1);
    }

    /* 자식 프로세스에서의 Var과 var을 각각 1씩 증가시킴 */
    else if (pid == 0) {
        /* child */
        Var++;
        var++;
    }

    /* 부모 프로세스에서는 자식 프로세스가 먼저 실행되도록 2초간 대기함 */
    else {
        /* parent */
        sleep(2);
    }

    printf("pid = %d, Var = %d, var = %d\n", getpid(), Var, var);
}