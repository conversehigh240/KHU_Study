/*
컴파일 후 ./pipe를 입력하면 아래와 같은 결과가 나옴.
Parent: Send data to child
Child : Wait for parent to send data
Child : Received data from parent: Hello, World!
Child : Send data to parent
Parent : Received data from child: Hello from child!

기존의 부모 -> 자식 메세지 전달과 더불어 자식 -> 부모 메세지 전달도 가능한 것을 확인.
*/

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_BUF 128

main()
{
    int     n, fd1[2], fd2[2];
    pid_t   pid;
    char    buf[MAX_BUF];

    /* 파이프를 생성함.
    fd[0]은 read, fd[1]은 write
    fd1 : 부모 -> 자식, fd2 : 자식 -> 부모 */
    if (pipe(fd1) < 0 || pipe(fd2) < 0)  {
        perror("pipe");
        exit(1);
    }

    /* 자식 프로세스 생성 */
    if ((pid = fork()) < 0)  {
        perror("fork");
        exit(1);
    }
    /* 자식 프로세스는 파이프에 데이터가 써지는 이벤트를 기다림.
    부모 프로세스가 data를 pipe에 write하면 자식 프로세스는 해당 데이터를 읽어옴.
    그 후 표준 출력을 이용하여 데이터를 출력함 */
    else if (pid == 0)  {
        close(fd1[1]); // 부모 -> 자식 쓰기 close
        close(fd2[0]); // 자식 -> 부모 읽기 close
        printf("Child : Wait for parent to send data\n");
        if ((n = read(fd1[0], buf, MAX_BUF)) < 0)  {
            perror("read");
            exit(1);
        }
        printf("Child : Received data from parent: ");
        fflush(stdout);
        write(STDOUT_FILENO, buf, n);

        /* 부모에게 데이터 보내기 */
        strcpy(buf, "Hello from child!\n");
        printf("Child : Send data to parent\n");
        write(fd2[1], buf, strlen(buf) + 1);

        close(fd1[0]);
        close(fd2[1]);
    }
    else  {
        close(fd1[0]);
        close(fd2[1]);

        strcpy(buf, "Hello, World!\n");
        printf("Parent: Send data to child\n");
        write(fd1[1], buf, strlen(buf)+1);

        if ((n = read(fd2[0], buf, MAX_BUF)) < 0) {
            perror("read");
            exit(1);
        }

        /* 자식이 보낸 데이터 읽기 */
        printf("Parent : Received data from child: ");
        fflush(stdout);
        write(STDOUT_FILENO, buf, n);

        close(fd1[1]);
        close(fd2[0]);
    }

    exit(0);
}