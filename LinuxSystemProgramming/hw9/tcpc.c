#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "tcp.h"


main(int argc, char *argv[])
{
    int                 sockfd, n;
    struct sockaddr_in  servAddr;
    MsgType             msg;

    /* 소켓 생성 */
    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)  {
        perror("socket");
        exit(1);
    }

    /* 서버 주소 구조체 초기화 */
    bzero((char *)&servAddr, sizeof(servAddr));
    servAddr.sin_family = PF_INET; // ipv4 사용
    servAddr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR); // 서버 ip 주소 설정
    servAddr.sin_port = htons(SERV_TCP_PORT); // 서버 포트 번호 설정

    /* 서버에 연결 요청 */
    if (connect(sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)  {
        perror("connect");
        exit(1);
    }

    /* 서버로 보낼 메세지 작성 */
    msg.type = MSG_REQUEST;
    sprintf(msg.data, "This is a request from %d.", getpid());

    /* 서버에 메세지 전송 */
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