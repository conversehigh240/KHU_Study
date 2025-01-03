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
    /* UNIX 도메인 소켓 파일 제거 */
    if (remove(UNIX_STR_PATH) < 0)  {
        perror("remove");
    }

    printf("\nUNIX-domain Connection-Oriented Server exit.....\n");

    exit(0);
}


main(int argc, char *argv[])
{
    int                 newSockfd, servAddrLen, cliAddrLen, n;
    struct sockaddr_un  cliAddr, servAddr;
    MsgType             msg;

    /* 시그널 핸들러 등록 */
    signal(SIGINT, CloseServer);

    /* 소켓 생성 */
    if ((Sockfd = socket(PF_UNIX, SOCK_STREAM, 0)) < 0)  {
        perror("socket");
        exit(1);
    }

    /* 서버 주소 구조체 초기화 */
    bzero((char *)&servAddr, sizeof(servAddr));
    servAddr.sun_family = PF_UNIX; //UNIX 도메인 사용
    strcpy(servAddr.sun_path, UNIX_STR_PATH); // 소켓 파일 경로 설정
    servAddrLen = strlen(servAddr.sun_path) + sizeof(servAddr.sun_family); // 전체 주소 길이 계산

    /* 소켓에 주소 바인딩 */
    if (bind(Sockfd, (struct sockaddr *) &servAddr, servAddrLen) < 0)  {
        perror("bind");
        exit(1);
    }

    /* 클라이언트 연결 요청 대기 */
    listen(Sockfd, 5);

    printf("UNIX-domain Connection-Oriented Server started.....\n");

    cliAddrLen = sizeof(cliAddr);
    while (1)  {
        /* 클라이언트 연결 수락 */
        newSockfd = accept(Sockfd, (struct sockaddr *) &cliAddr, &cliAddrLen);
        if (newSockfd < 0)  {
            perror("accept");
            exit(1);
        }

        /* 클라이언트로부터 메세지 읽기 */
        if ((n = read(newSockfd, (char *)&msg, sizeof(msg))) < 0)  {
            perror("read");
            exit(1);
        }
        printf("Received request: %s.....", msg.data);

        /* 응답 메세지 작성 */
        msg.type = MSG_REPLY;
        sprintf(msg.data, "This is a reply from %d.", getpid());

        /* 클라이언트로 응답 전송 */
        if (write(newSockfd, (char *)&msg, sizeof(msg)) < 0)  {
            perror("write");
            exit(1);
        }
        printf("Replied.\n");

        /* 소켓 종료 */
        close(newSockfd);
    }
}