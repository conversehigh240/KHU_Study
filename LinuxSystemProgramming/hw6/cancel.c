#include <stdio.h>
#include <signal.h>
#include <pthread.h>

pthread_t ThreadId[2];

void SigIntHandler(int signo)
{
    int i;
   
    printf("Received a SIGINT signal by thread %d\n", pthread_self());
    printf("Terminated other threads: tid=%d, %d\n", ThreadId[0], ThreadId[1]);

    for (i = 0; i < 2; i++) {
   
        /* 각 thread에 취소 요청을 함. 즉, thread를 kill 시키는 것 */
        if (pthread_cancel(ThreadId[i])) {
            perror("pthread_cancel");
            exit(1);
        }
    }

    /* 각 thread가 종료될 때 까지 기다림 */
    for (i = 0; i < 2; i++) {
        if (pthread_join(ThreadId[i], NULL)) {
            perror("pthread_join");
            exit(1);
        }
    }
   
    printf("Threads terminated: tid=%d, %d\n", ThreadId[0], ThreadId[1]);

    exit(0);
}

void Thread1(void *dummy)
{
    printf("Thread %d created.....\n", pthread_self());

    /* 다른 누군가가 내 thread를 cancel시킬 수 있도록 설정함 */
    if (pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL)) {
        perror("pthread_setcancelstate");
        pthread_exit(NULL);
    }

    /* cancel되는 타이밍을 asynchronous하게 되도록 함. 즉, 바로 cancel될 수 있도록 함 */
    if (pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL)) {
        perror("pthread_setcanceltype");
        pthread_exit(NULL);
    }

    while (1)
        ;
}

void Thread2(void *dummy)
{
    printf("Thread %d created.....\n", pthread_self());

    if (pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL)) {
        perror("pthread_setcancelstate");
        pthread_exit(NULL);
    }

    if (pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL)) {
        perror("pthread_setcanceltype");
        pthread_exit(NULL);
    }

    while(1)
        ;
}

main()
{
    if (pthread_create(&ThreadId[0], NULL, (void *)Thread1, NULL) < 0) {
        perror("pthread_create");
        exit(1);
    }

    if (pthread_create(&ThreadId[1], NULL, (void *)Thread2, NULL) < 0) {
        perror("pthread_create");
        exit(1);
    }

    /* SIGINT 발생 시 SigIntHandler 함수를 호출함 */
    signal(SIGINT, SigIntHandler);

    printf("Press ^C to quit\n");

    for( ; ; )
        pause();

}