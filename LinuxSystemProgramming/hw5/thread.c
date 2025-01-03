#include <stdio.h>
#include <pthread.h>

void PrintMsg(char *msg)
{
    printf("%s", msg);
    pthread_exit(NULL);
}

main()
{
    pthread_t tid1, tid2;
    char *msg1 = "Hello, ";
    char *msg2 = "World!\n";

    /* Thread ID : tid1, Thread function: PrintMsg, Thread argument: msg1 */
    /* pthread_create를 이용하여 thread를 생성함. 프로세스에서 fork에 해당하는 기능.
    첫 번째 파라미터는 thread id를 포인터로 넣어주고, 두 번째 파라미터는 attribute인데
    보통 default로 만들라는 의미로 NULL을 사용함. 보통 option이나 priority에 변화를 주고 싶을 때
    attribute에 해당하는 파라미터에 값을 주어 이용.
    세 번째 파라미터는 thread 함수이고 네번째 파라미터는 thread 함수의 파라미터임.
    즉, PrintMsg 함수를 실행하는 thread를 만드는 과정임. */
    if (pthread_create(&tid1, NULL, (void *)PrintMsg, (void *)msg1) < 0) {
        perror("pthread_create");
        exit(1);
    }

    if (pthread_create(&tid2, NULL, (void *)PrintMsg, (void *)msg2) < 0) {
        perror("pthread_create");
        exit(1);
    }

    printf("Threads created: tid = %d, %d\n", tid1, tid2);

    /* pthread_join을 통해 thread가 종료되길 기다림. 프로세스에서 wait에 해당하는 기능 */
    if (pthread_join(tid1, NULL) < 0) {
        perror("pthread_join");
        exit(1);
    }

    if (pthread_join(tid2, NULL) < 0) {
        perror("pthread_join");
        exit(1);
    }

    printf("Threads terminated: tid=%d, %d\n", tid1, tid2);
}