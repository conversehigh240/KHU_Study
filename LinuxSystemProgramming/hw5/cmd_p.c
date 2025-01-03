#include <stdio.h>
  
#define MAX_CMD 256

void DoCmd(char *cmd)
{
    printf("Doing %s", cmd);
    sleep(1);
    printf("Done\n");
}

/* process의 fork를 이용한 version. */
main()
{
    char cmd[MAX_CMD];
    int pid;

    while (1) {
        printf("CMD> ");
        fgets(cmd, MAX_CMD, stdin);
        if (cmd[0] == 'q')
            break;

        /* 사용자 입력을 받은 후 fork를 통해 child process를 생성함. */
        if ((pid = fork()) < 0) {
            perror("fork");
            exit(1);
        }

        /* child process는 DoCmd를 실행한 후 exit */
        else if (pid == 0) {
            DoCmd(cmd);
            exit(0);
        }

/* 이 부분은 if 0 이므로 compile 되지 않는 부분에 해당함.
   그러니 child process가 실행되는 동안 parent process는 다시 루프의 처음으로 돌아가게 됨.
   만약 if 1 이였다면 아래의 내용도 같이 compile되고, parent process는 child process가 exit하기를
   기다려야 하므로 iteration version과 동일하게 동작하게 됨. (process가 하나인 것과 동일) */
#if 0
        else {
            wait(NULL);
        }

#endif
    }
}