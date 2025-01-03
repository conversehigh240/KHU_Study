#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

void SignalHandler(int signo)
{
    int status;
    pid_t pid = wait(&status);

    if (signo == SIGUSR1) {
        exit(0);
    }
    else if (signo == SIGCHLD) {
        if (pid > 0) {
            printf("Child killed");
        }
    }
    else {
        exit(0);
    }
   
}

main()
{
    int count;
    pid_t pid;

    signal(SIGCHLD, SignalHandler);

    count = 0;

    if ((pid = fork()) < 0)  {
        perror("fork");
        exit(1);
    }
    else if (pid == 0)  {
        while(1) {
            count++;
        }
    }
    else  {
        sleep(1);
        while(1) {
            signal(SIGUSR1, SignalHandler);
        }
    }
}