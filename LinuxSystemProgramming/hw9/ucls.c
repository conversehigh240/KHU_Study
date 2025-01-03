#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <signal.h>
#include "unix.h"


int Sockfd;

/* 시그널 핸들러 함수 */
void CloseServer()
{
    close(Sockfd);
    if (remove(UNIX_DG_PATH) < 0)  {
        perror("remove");
    }

    printf("\nUNIX-domain Connection-Less Server exit.....\n");

    exit(0);
}


main(int argc, char *argv[])
{
    int                 servAddrLen, cliAddrLen, n;
    struct sockaddr_un  cliAddr, servAddr;
    MsgType             msg;

    /* 시그널 핸들러 등록 */
    signal(SIGINT, CloseServer);

    /* 소켓 생성 */
    if ((Sockfd = socket(PF_UNIX, SOCK_DGRAM, 0)) < 0)  {
        perror("socket");
        exit(1);
    }

    /* 서버 주소 구조체 초기화 */
    bzero((char *)&servAddr, sizeof(servAddr));
    servAddr.sun_family = PF_UNIX; // UNIX 도메인 소켓 설정
    strcpy(servAddr.sun_path, UNIX_DG_PATH); // 서버 소켓 파일 경로 설정
    servAddrLen = strlen(servAddr.sun_path) + sizeof(servAddr.sun_family); // 전체 주소 길이 계산

    /* 서버 소켓에 주소 바인딩 */
    if (bind(Sockfd, (struct sockaddr *) &servAddr, servAddrLen) < 0)  {
        perror("bind");
        exit(1);
    }

    printf("UNIX-domain Connection-Less Server started.....\n");

    cliAddrLen = sizeof(cliAddr);
    while (1)  {
        /* 클라이언트로부터 데이터 수신 */
        if ((n = recvfrom(Sockfd, (char *)&msg, sizeof(msg),
                    0, (struct sockaddr *)&cliAddr, &cliAddrLen)) < 0)  {
            perror("recvfrom");
            exit(1);
        }
        printf("Received request: %s.....", msg.data);

        /* 클라이언트에 응답 메세지 작성 */
        msg.type = MSG_REPLY;
        sprintf(msg.data, "This is a reply from %d.", getpid());
        /* 클라이언트로 응답 메세지 전송 */
        if (sendto(Sockfd, (char *)&msg, sizeof(msg),
                    0, (struct sockaddr *)&cliAddr, cliAddrLen) < 0)  {
            perror("sendto");
            exit(1);
        }
        printf("Replied.\n");
    }
}