#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include "unix.h"


main(int argc, char *argv[])
{
    int                 sockfd, n, servAddrLen, myAddrLen, peerAddrLen;
    struct sockaddr_un  servAddr, myAddr, peerAddr;
    MsgType             msg;

    /* 소켓 생성 */
    if ((sockfd = socket(PF_UNIX, SOCK_DGRAM, 0)) < 0)  {
        perror("socket");
        exit(1);
    }

    /* 클라이언트 소켓 주소 구조체 설정 */
    bzero((char *)&myAddr, sizeof(myAddr));
    myAddr.sun_family = PF_UNIX; //UNIX 도메인 소켓 설정
    sprintf(myAddr.sun_path, ".unix-%d", getpid()); // 클라이언트 고유 소켓 경로 설정(프로세스 id 사용)
    myAddrLen = strlen(myAddr.sun_path) + sizeof(myAddr.sun_family); // 주소 길이 계산

    /* 클라이언트 소켓에 바인딩 */
    if (bind(sockfd, (struct sockaddr *)&myAddr, myAddrLen) < 0)  {
        perror("bind");
        exit(1);
    }

    /* 서버 소켓 주소 구조체 설정 */
    bzero((char *)&servAddr, sizeof(servAddr));
    servAddr.sun_family = PF_UNIX; // UNIX 도메인 소켓 설정
    strcpy(servAddr.sun_path, UNIX_DG_PATH); // 서버 소켓 파일 경로 설정
    servAddrLen = strlen(servAddr.sun_path) + sizeof(servAddr.sun_family); // 주소 길이 계산

    msg.type = MSG_REQUEST;
    sprintf(msg.data, "This is a request from %d.", getpid());
    /* 서버로 메세지 전송 */
    if (sendto(sockfd, (char *)&msg, sizeof(msg),
            0, (struct sockaddr *)&servAddr, servAddrLen) < 0)  {
        perror("sendto");
        exit(1);
    }
    printf("Sent a request.....");

    peerAddrLen = sizeof(peerAddr);
    /* 서버로부터 응답 수신 */
    if ((n = recvfrom(sockfd, (char *)&msg, sizeof(msg),
                0, (struct sockaddr *)&peerAddr, &peerAddrLen)) < 0)  {
        perror("recvfrom");
        exit(1);
    }
    printf("Received reply: %s\n", msg.data);

    /* 소켓 종료 및 클라이언트 소켓 파일 삭제 */
    close(sockfd);
    if (remove(myAddr.sun_path) < 0)  {
        perror("remove");
        exit(1);
    }
}