#include <stdio.h>
  
#define MAX_CMD 256

void DoCmd(char *cmd)
{
    printf("Doing %s", cmd);
    sleep(1);
    printf("Done\n");
}

/* loop를 이용한 iteration version.
   단점은 만약 한 명령을 실행하는데 1시간이 걸린다면 그 시간 동안 다른 어떤 것도 실행할 수 없음. */
main()
{
    char cmd[MAX_CMD];

    while (1) {
        printf("CMD> ");
        fgets(cmd, MAX_CMD, stdin);
        if (cmd[0] == 'q')
            break;
        DoCmd(cmd);
    }
}