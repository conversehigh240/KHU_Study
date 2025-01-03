#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include "unix.h"


main(int argc, char *argv[])
{
    int                 sockfd, n, servAddrLen;
    struct sockaddr_un  servAddr;
    MsgType             msg;

    /* 소켓 생성 */
    if ((sockfd = socket(PF_UNIX, SOCK_STREAM, 0)) < 0)  {
        perror("socket");
        exit(1);
    }

    /* 서버 주소 구조체 초기화 */
    bzero((char *)&servAddr, sizeof(servAddr));
    servAddr.sun_family = PF_UNIX; // UNIX 도메인 소켓 사용
    strcpy(servAddr.sun_path, UNIX_STR_PATH); // 서버 소켓의 파일 경로 설정
    servAddrLen = strlen(servAddr.sun_path) + sizeof(servAddr.sun_family); // 전체 주소 길이 계산

    /* 서버에 연결 요청 */
    if (connect(sockfd, (struct sockaddr *) &servAddr, servAddrLen) < 0)  {
        perror("connect");
        exit(1);
    }

    /* 서버에 보낼 메세지 작성 */
    msg.type = MSG_REQUEST;
    sprintf(msg.data, "This is a request from %d.", getpid());

    /* 서버로 메세지 전송 */
    if (write(sockfd, (char *)&msg, sizeof(msg)) < 0)  {
        perror("write");
        exit(1);
    }
    printf("Sent a request.....");

    /* 서버로부터 응답 메세지 수신 */
    if ((n = read(sockfd, (char *)&msg, sizeof(msg))) < 0)  {
        perror("read");
        exit(1);
    }
    printf("Received reply: %s\n", msg.data);

    /* 소켓 종료 */
    close(sockfd);
}