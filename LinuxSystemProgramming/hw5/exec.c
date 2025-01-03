#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

main()
{
    pid_t pid;
    char *argv[] = { "mycp", "exec.c", "exec2.c", NULL };

    //execl
    if ((pid = fork()) < 0) {
        perror("fork");
        exit(1);
    }

    /* child process는 /home/lsp25/hw3/mycp를 실행하는데, main의 파라미터로 mycp, exec.c, exec2.c가
       넘어가게 됨.
       여기서, execl을 사용했으므로 pathname 뒤에 list를 파라미터로 이용함. */
    else if (pid == 0) {
        if (execl("/home/lsp25/hw3/mycp", "mycp", "exec.c", "exec1.c", NULL) < 0) {
            perror("execl");
            exit(1);
        }
    }

    //execv
    if ((pid = fork()) < 0) {
        perror("fork");
        exit(1);
    }

    /* child process는 /home/lsp25/hw3/mycp를 실행하는데, 파라미터를 vector, 즉 1차원 array를
       이용하여 넘겨주도록 함.
       위에서 선언한 array를 파라미터로 주었기 때문에 mycp에서 main의 파라미터로 mycp, exec.c, 
       exec2.c가 넘겨지게 됨. */
    else if (pid == 0) {
        //mycp exec.c exec2.c
        if (execv("/home/lsp25/hw3/mycp", argv) < 0) {
            perror("execv");
            exit(1);
        }
    }

    if (waitpid(pid, NULL, 0) < 0) {
        perror("waitpid");
        exit(1);
    }

    //execlp
    if ((pid = fork()) < 0) {
        perror("fork");
        exit(1);
    }

    /* execlp는 path라는 환경변수를 이용하여 사용함.
       즉, pathname이 아닌 실행 파일 이름을 주고, 이 실행 파일을 PATH에서 찾아서 실행하도록 함.
       여기서는  env라는 실행 파일을 PATH에서 찾아서 실행하도록 하는것. */
    else if (pid == 0) {
        // execute 'env' from PATH
        if (execlp("env", "env", NULL) < 0) {
            perror("execlp");
            exit(1);
        }
    }

    if (waitpid(pid, NULL, 0) < 0) {
        perror("waitpid");
        exit(1);
    }
}