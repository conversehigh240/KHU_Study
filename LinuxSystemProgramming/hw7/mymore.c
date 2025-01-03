#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main()
{
    int fd[2];
    pid_t child;

    /* 파이프 생성 */
    if(pipe(fd) == -1)  {
        perror("pipe");
        exit(1);
    }

    /* 자식 프로세스 생성 */
    child = fork();
    if(child == (pid_t)(-1))  {
        perror("fork");
        exit(1);
    }
   
    /* 자식 프로세스는 STDOUT을 닫고, 파이프의 읽기 끝을 닫음.
    즉, 자식 프로세스는 쓰기만 함 */
    else if(child == (pid_t)0) {
        close(1);
        close(fd[0]);

        /* 파이프의 쓰기 끝 (fd[1])을 표준 출력으로 복제함 */
        if(dup(fd[1]) == -1)  {
            perror("dup");
            exit(1);
        }

        /* ls -l 명령을 실행함. 결과는 파이프 쓰기 끝(표준 출력)으로 출력됨 */
        if((execlp("ls", "ls", "-l", NULL)) == -1)  {
            perror("execlp");
            exit(1);
        }
    }

    /* 부모 프로세스는 STDIN(표준 입력)을 닫고 파이프의 쓰기 끝을 닫음.
    즉, 부모 프로세스는 읽기만 함 */
    else {
        close(0);
        close(fd[1]);

        /*파이프의 읽기 끝(fd[0])을 표준 입력으로 복제함 */
        if(dup2(fd[0], 0) == -1)  {
            perror("dup");
            exit(1);
        }

        /* more 명령을 실행하며 파이프의 내용을 표준 입력으로 받아 출력함 */
        if((execlp("more", "more", NULL)) == -1)  {
            perror("execlp");
            exit(1);
        }
    }

    return 0;
}