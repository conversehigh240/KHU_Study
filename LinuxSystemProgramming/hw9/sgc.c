#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "sg.h"


main(int argc, char *argv[])
{
    int                 sockfd, n;
    struct sockaddr_in  servAddr;
    MsgType             msg;
    HeaderType          hdr;
    struct iovec        iov[2];

    /* TCP 소켓 생성 */
    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)  {
        perror("socket");
        exit(1);
    }

    /* 서버 주소 구조체 초기화 */
    bzero((char *)&servAddr, sizeof(servAddr));
    servAddr.sin_family = PF_INET; // ipv4 사용
    servAddr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR); // 서버의 ip 주소 설정
    servAddr.sin_port = htons(SERV_TCP_PORT); // 서버의 포트 번호 설정

    /* 서버에 연결 */
    if (connect(sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)  {
        perror("connect");
        exit(1);
    }

    /* 요청 헤더 및 메세지 설정 */
    strcpy(hdr.info, "REQST"); // 헤더 정보 설정
    msg.type = MSG_REQUEST; // 메세지 타입 설정
    sprintf(msg.data, "This is a request from %d.", getpid());

    /* scatter-gather 기법을 위한 iovec  설정 */
    iov[0].iov_base = (char *)&hdr; // 첫번째 데이터 블록(헤더)
    iov[0].iov_len = sizeof(hdr); // 첫번째 데이터 블록 크기
    iov[1].iov_base = (char *)&msg; // 두번째 데이터 블록(메세지)
    iov[1].iov_len = sizeof(msg); // 두번째 데이터 블록 크기

    /* 두 개의 데이터 블록을 한 번에 서버에 쓰기 (요청 전송) */
    if (writev(sockfd, iov, 2) < 0)  {
        perror("write");
        exit(1);
    }
    printf("Sent a request.....");

    /* 서버로부터의 응답을 읽음 */
    if ((n = readv(sockfd, iov, 2)) < 0)  {
        perror("read");
        exit(1);
    }
    printf("Received reply: %s(%s)\n", msg.data, hdr.info);

    /* 소켓 종료 */
    close(sockfd);
}