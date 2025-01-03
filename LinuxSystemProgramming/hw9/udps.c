#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include "udp.h"


int Sockfd;

/* 시그널 핸들러 함수 */
void CloseServer()
{
    close(Sockfd);
    printf("\nUDP Server exit.....\n");

    exit(0);
}


main(int argc, char *argv[])
{
    int                 cliAddrLen, n;
    struct sockaddr_in  cliAddr, servAddr;
    MsgType             msg;

    /* 시그널 핸들러 설정 */
    signal(SIGINT, CloseServer);

    /* 소켓 생성 */
    if ((Sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)  {
        perror("socket");
        exit(1);
    }

    /* 서버 주소 구조체 초기화 */
    bzero((char *)&servAddr, sizeof(servAddr));
    servAddr.sin_family = PF_INET; //ipv4 사용
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY); // 모든 ip 주소에서 접속 허용
    servAddr.sin_port = htons(SERV_UDP_PORT); // 서버의 포트 번호 설정

    /* 소켓에 주소 할당 */
    if (bind(Sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)  {
        perror("bind");
        exit(1);
    }

    printf("UDP Server started.....\n");

    cliAddrLen = sizeof(cliAddr);
    while (1)  {
        /* 클라이언트로부터 메세지 수신 */
        if ((n = recvfrom(Sockfd, (char *)&msg, sizeof(msg),
                    0, (struct sockaddr *)&cliAddr, &cliAddrLen)) < 0)  {
            perror("recvfrom");
            exit(1);
        }
        printf("Received request: %s.....", msg.data);

        /* 응답 메세지 작성 */
        msg.type = MSG_REPLY;
        sprintf(msg.data, "This is a reply from %d.", getpid());
        /* 클라이언트에 응답 메세지 전송 */
        if (sendto(Sockfd, (char *)&msg, sizeof(msg),
                    0, (struct sockaddr *)&cliAddr, cliAddrLen) < 0)  {
            perror("sendto");
            exit(1);
        }
        printf("Replied.\n");
    }
}