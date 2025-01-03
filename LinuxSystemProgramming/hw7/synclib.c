#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

/* 부모와 자식 간 통신에 사용될 두 개의 파이프 file descriptor 배열.
Pfd1 : 부모 -> 자식
Pfd2 : 자식 -> 부모 */
static int  Pfd1[2], Pfd2[2];

void
TELL_WAIT(void)
{
    /* 두 개의 파이프를 생성함 */
    if (pipe(Pfd1) < 0 || pipe(Pfd2) < 0)  {
        perror("pipe");
        exit(1);
    }
}

void
TELL_PARENT(void)
{
    /*Pfd2[1]에 "c" 문자를 써서 자식이 부모에게 신호를 보냄 */
    if (write(Pfd2[1], "c", 1) != 1)  {
        perror("write");
        exit(1);
    }
}

void
WAIT_PARENT(void)
{
    char    c;

    /* Pfd1[0]에서 데이터를 읽어 부모의 신호를 대기함 */
    if (read(Pfd1[0], &c, 1) != 1)  {
        perror("read");
        exit(1);
    }
    if (c != 'p')  {
        fprintf(stderr, "WAIT_PARENT: incorrect data");
        exit(1);
    }
}

void
TELL_CHILD(void)
{
    /* Pfd1[1]에 "p" 문자를 써서 부모가 자식에게 신호를 보냄 */
    if (write(Pfd1[1], "p", 1) != 1)  {
        perror("write");
        exit(1);
    }
}

void
WAIT_CHILD(void)
{
    char    c;

    /* Pfd2[0]에서 데이터를 읽어 자식의 신호를 대기함 */
    if (read(Pfd2[0], &c, 1) != 1)  {
        perror("read");
        exit(1);
    }
    if (c != 'c')  {
        fprintf(stderr, "WAIT_CHILD: incorrect data");
        exit(1);
    }
}