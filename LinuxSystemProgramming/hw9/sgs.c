#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include "sg.h"


int Sockfd;

/* 시그널 핸들러 함수 */
void CloseServer()
{
    close(Sockfd);
    printf("\nScatter/Gather TCP Server exit.....\n");

    exit(0);
}


main(int argc, char *argv[])
{
    int                 newSockfd, cliAddrLen, n;
    struct sockaddr_in  cliAddr, servAddr;
    MsgType             msg;
    HeaderType          hdr;
    struct iovec        iov[2];

    /* 시그널 핸들러 등록 */
    signal(SIGINT, CloseServer);

    /* TCP 소켓 생성 */
    if ((Sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)  {
        perror("socket");
        exit(1);
    }

    /* 서버 주소 구조체 초기화 */
    bzero((char *)&servAddr, sizeof(servAddr));
    servAddr.sin_family = PF_INET; // ipv4 사용
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY); // 모든 네트워크 인터페이스에 바인딩
    servAddr.sin_port = htons(SERV_TCP_PORT); // 정의된 TCP 포트로 바인딩

    /* 서버 주소로 소켓 바인딩 */
    if (bind(Sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)  {
        perror("bind");
        exit(1);
    }

    /* 클라이언트 연결 대기 시작 */
    listen(Sockfd, 5);

    printf("Scatter/Gather TCP Server started.....\n");

    cliAddrLen = sizeof(cliAddr);
    while (1)  {
        /* 클라이언트 연결 수락 */
        newSockfd = accept(Sockfd, (struct sockaddr *) &cliAddr, &cliAddrLen);
        if (newSockfd < 0)  {
            perror("accept");
            exit(1);
        }

        /* scatter-gatter 기법을 위한 iovec 설정 */
        iov[0].iov_base = (char *)&hdr; // 첫번째 데이터 블록(헤더)
        iov[0].iov_len = sizeof(hdr); // 첫번째 데이터 블록 크기
        iov[1].iov_base = (char *)&msg; // 두번째 데이터 블록(메세지)
        iov[1].iov_len = sizeof(msg); // 두번째 데이터 블록 크기

        /* 두 개의 데이터 블록을 한 번에 읽기 */
        if ((n = readv(newSockfd, iov, 2)) < 0)  {
            perror("read");
            exit(1);
        }
        printf("Received request: %s(%s).....", msg.data, hdr.info);

        /* 응답 메세지 준비 */
        strcpy(hdr.info, "REPLY"); // 헤더 정보 수정
        msg.type = MSG_REPLY; // 응답 메세지 타입 설정
        sprintf(msg.data, "This is a reply from %d.", getpid());

        /* 두 개의 데이터 블록을 한 번에 클라이언트에 쓰기 */
        if (writev(newSockfd, iov, 2) < 0)  {
            perror("write");
            exit(1);
        }
        printf("Replied.\n");

        /* 클라이언트 연결 종료 */
        close(newSockfd);
    }
}