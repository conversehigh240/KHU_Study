/*
   컴파일 한 후 ./mysystem을 입력하여 프로그램을 실행함.
   프로그램이 실행되면 CMD> 라고 하는 프롬프트가 출력되고, 내가 실행하고자 하는 명령어를 입력한 후
   엔터 키를 누르면 됨.
   종료하고 싶으면 q를 입력하면 됨.

   (예시)
   $ make
   $ ./mysystem
   CMD> date
   Fri Nov 1 19:10:51 UTC 2024 
   CMD> q
   (프로그램 종료)

*/

#include <stdio.h>
#include <stdlib.h>

#define MAX_CMD 256

int mysystem(char *cmd)
{
    int status;
    char *argv[] = {"/bin/sh", "-c", cmd, NULL};

    /* execv를 이용하여 cmd를 실행하도록 함. 이 때, system 시스템 콜과 동일한 동작을 해야 하므로
       경로는 "/bin/sh"로 설정하고, argv로 파라미터를 넘겨줌.
       이 때, "/bin/sh"는 기본 쉘 경로이고, "-c"는 쉘에게 명령어를 실행하도록 지시하는 옵션,
       cmd는 실행할 명령어를 뜻함. */
    if (execv("/bin/sh", argv) < 0) {
        perror("execv");
        return -1; // 오류 시 -1 리턴
    }
    return 0;
}

main()
{
    pid_t pid;
    char cmd[MAX_CMD];

    while(1) {
        printf("CMD> ");
        /* 사용자가 원하는 cmd를 입력받음. 이 때, fgets를 이용하면 '\n'이 포함되므로
           이것을 제거하기 위해 strcspn을 사용함.
           cmd에서 '\n'을 찾고,  그것을 NULL로 대체해줌. */
        fgets(cmd, MAX_CMD, stdin);
        cmd[strcspn(cmd, "\n")] = 0;

        /* q를 입력받으면 프로그램 종료 */
        if (cmd[0] == 'q')
            break;

        /* fork를 이용하여 child process를 생성함 */
        if ((pid = fork()) < 0) {
            perror("fork");
            exit(1);
        }

        /* child process는 mysystem 함수를 실행한 수 exit */
        else if (pid == 0) {
            mysystem(cmd);
            exit(0);
        }

        /* parent process는 child process가 종료될 때까지 대기함.
           여기서 waitpid를 이용하여 특정 프로세스(child process)를 지정함. */
        else {
            waitpid(pid, NULL, 0);
        }
    }
}