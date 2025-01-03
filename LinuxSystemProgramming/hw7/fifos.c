#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include "fifo.h"

/* 시그널 핸들러 함수 */
void
SigIntHandler(int signo)
{
    /* 서버 FIFO 파일 삭제 */
    if (remove(SERV_FIFO) < 0)  {
        perror("remove");
        exit(1);
    }

    exit(0);
}


main()
{
    int         fd, cfd, n;
    MsgType     msg;

    if (signal(SIGINT, SigIntHandler) == SIG_ERR)  {
        perror("signal");
        exit(1);
    }

    /* 서버 FIFO 파일 생성 */
    if (mkfifo(SERV_FIFO, 0600) < 0)  {
        if (errno != EEXIST)  {
            perror("mkfifo");
            exit(1);
        }
    }

    /* 서버 FIFO 파일을 open함 (read/write 모드) */
    if ((fd = open(SERV_FIFO, O_RDWR)) < 0)  {
        perror("open");
        exit(1);
    }

    /* 클라이언트 요청을 지속적으로 처리함 */
    while (1)  {
        /* 서버 FIFO에서 메세지를 읽음 */
        if ((n = read(fd, (char *)&msg, sizeof(msg))) < 0)  {
            if (errno == EINTR)  {
                continue;
            }
            else  {
                perror("read");
                exit(1);
            }
        }

        /* 요청 메세지를 출력함 */
        printf("Received request: %s.....", msg.data);

        /* 클라이언트 FIFO를 write only 모드로 open함 */
        if ((cfd = open(msg.returnFifo, O_WRONLY)) < 0)  {
            perror("open");
            exit(1);
        }

        /* 응답 메세지를 생성하여 클라이언트 FIFO에 write함 */
        sprintf(msg.data, "This is a reply from %d.", getpid());
        write(cfd, (char *)&msg, sizeof(msg));
        close(cfd);
        printf("Replied.\n");
    }
}