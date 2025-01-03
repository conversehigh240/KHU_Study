#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "fifo.h"


main()
{
    char        fname[MAX_FIFO_NAME];
    int         fd, sfd, n;
    MsgType     msg;

    /* 클라이언트 전용 FIFO 생성함 */
    sprintf(fname, ".fifo%d", getpid());
    if (mkfifo(fname, 0600) < 0)  {
        perror("mkfifo");
        exit(1);
    }

    /* 클라이언트 전용 FIFO 파일을 read/write 모드로 open함 */
    if ((fd = open(fname, O_RDWR)) < 0)  {
        perror("open");
        exit(1);
    }

    /* 서버 FIFO 파일을 read/write 모드로 open함 */
    if ((sfd = open(SERV_FIFO, O_RDWR)) < 0)  {
        perror("open");
        exit(1);
    }

    /* 서버 FIFO에 메세지를 전송함 */
    strcpy(msg.returnFifo, fname);
    sprintf(msg.data, "This is a request from %d.", getpid());
    write(sfd, (char *)&msg, sizeof(msg));
    printf("Sent a request.....");

    /* 클라이언트 전용 FIFO에서 서버의 응답을 읽음 */
    if ((n = read(fd, (char *)&msg, sizeof(msg))) < 0)  {
        perror("read");
        exit(1);
    }

    /* 서버 응답 메세지 출력 */
    printf("Received reply: %s\n", msg.data);

    close(fd);
    close(sfd);

    /* 클라이언트 전용 FIFO 파일을 삭제함 */
    if (remove(fname) < 0)  {
        perror("remove");
        exit(1);
    }
}