/*
tcps.c를 connection이 맺어진 후 process를 생성하여 전담 처리하도록 변경한 것.
*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include "tcp.h"


int Sockfd;

/* 시그널 처리 함수 */
void CloseServer()
{
    close(Sockfd);
    printf("\nTCP Server exit.....\n");

    exit(0);
}


main(int argc, char *argv[])
{
    int                 newSockfd, cliAddrLen, n;
    struct sockaddr_in  cliAddr, servAddr;
    MsgType             msg;
    pid_t               pid;

    /* 시그널 핸들러 생성 */
    signal(SIGINT, CloseServer);

    /* 소켓 생성 */
    if ((Sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)  {
        perror("socket");
        exit(1);
    }

    /* 서버 주소 구조체 초기화 */
    bzero((char *)&servAddr, sizeof(servAddr));
    servAddr.sin_family = PF_INET; // ipv4 사용
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY); // 모든 ip 주소에서 접속 허용
    servAddr.sin_port = htons(SERV_TCP_PORT); // 서버 포트 번호 설정

    /* 소켓에 주소 할당 */
    if (bind(Sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)  {
        perror("bind");
        exit(1);
    }

    /* 클라이언트 요청 대기 */
    listen(Sockfd, 5);

    printf("TCP Server started.....\n");

    cliAddrLen = sizeof(cliAddr);
    while (1)  {
        /* 클라이언트 요청 수락 */
        newSockfd = accept(Sockfd, (struct sockaddr *) &cliAddr, &cliAddrLen);
        if (newSockfd < 0)  {
            perror("accept");
            exit(1);
        }

        /* connection이 맺어진 후 process를 생성하여 전담 처리 */
        if ((pid = fork()) < 0) {
            perror("fork");
            exit(1);
        }
        else if (pid == 0) {
            /* 클라이언트로부터 메세지 수신 */
            if ((n = read(newSockfd, (char *)&msg, sizeof(msg))) < 0)  {
                perror("read");
                exit(1);
            }
            printf("Received request: %s.....", msg.data);

            /* 클라이언트에 응답 메세지 작성 및 전송 */
            msg.type = MSG_REPLY;
            sprintf(msg.data, "This is a reply from %d.", getpid());
            if (write(newSockfd, (char *)&msg, sizeof(msg)) < 0)  {
                perror("write");
                exit(1);
            }
            printf("Replied.\n");

            /* 잠시 대기 후 클라이언트 소켓 연결 종료 */
            usleep(10000);
            close(newSockfd);
            exit(0);
        }
    }
}