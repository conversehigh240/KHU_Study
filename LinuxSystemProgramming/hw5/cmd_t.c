#include <stdio.h>
#include <pthread.h>

#define MAX_CMD 256

void DoCmd(char *cmd)
{
    printf("Doing %s", cmd);
    sleep(1);
    printf("Done\n");

    pthread_exit(NULL);
}

/* thread를 이용한 version */
main()
{
    char cmd[MAX_CMD];
    pthread_t tid;

    while(1) {
        printf("CMD> ");
        fgets(cmd, MAX_CMD, stdin);
        if (cmd[0] == 'q')
            break;

        /* DoCmd를 실행하도록 하는 thread를 생성함. */
        if (pthread_create(&tid, NULL, (void *)DoCmd, (void *)cmd) < 0) {
            perror("pthread_create");
            exit(1);
        }

#if 0
        pthread_join(tid, NULL);

#endif
    }
}