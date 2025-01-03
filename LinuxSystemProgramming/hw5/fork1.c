#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

main()
{
    int pid;

    /* fork 함수 호출을 통해 새로운 프로세스를 생성함. */
    if ((pid = fork()) < 0) {
        perror("fork");
        exit(1);
    }

    /* pid가 0이라면 자식 프로세스임 */
    else if (pid == 0) {
        /* child */
        printf("I am %d. My parent is %d.\n", getpid(), getppid());
    }

    /* pid가 0이 아니라면 부모 프로세스임.
       여기서, 정확히는 pid에 자식의 프로세스 id가 저장됨. */
    else {
        /* parent */
        printf("I am %d. My child is %d.\n", getpid(), pid);
    }
}