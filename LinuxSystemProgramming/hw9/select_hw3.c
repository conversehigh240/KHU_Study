/*
select.c를 thread version으로 변경
컴파일 후 ./select_hw3 입력하여 서버 실행
*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include "select.h"
#include <pthread.h>

int TcpSockfd;
int UdpSockfd;
int UcoSockfd;
int UclSockfd;

/* 시그널 핸들러 함수 */
void CloseServer()
{
    close(TcpSockfd);
    close(UdpSockfd);
    close(UcoSockfd);
    close(UclSockfd);

    /* UNIX 소켓 파일 삭제 */
    if (remove(UNIX_STR_PATH) < 0)  {
        perror("remove");
    }
    if (remove(UNIX_DG_PATH) < 0)  {
        perror("remove");
    }

    printf("\nServer daemon exit.....\n");

    exit(0);
}

/* TCP 소켓 생성 및 설정 함수 */
void MakeTcpSocket()
{
    struct sockaddr_in  servAddr;

    /* TCP 소켓 생성 */
    if ((TcpSockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)  {
        perror("socket");
        exit(1);
    }

    /* 서버 주소 초기화 */
    bzero((char *)&servAddr, sizeof(servAddr));
    servAddr.sin_family = PF_INET; // ipv4 프로토콜 사용
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY); // 모든 네트워크 인터페이스에 바인딩
    servAddr.sin_port = htons(SERV_TCP_PORT); // 지정된 TCP 포트로 바인딩

    /* 서버 주소로 소켓 바인딩 */
    if (bind(TcpSockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)  {
        perror("bind");
        exit(1);
    }

    /* 클라이언트 연결 대기 시작 */
    listen(TcpSockfd, 5);
}

/* UDP 소켓 생성 및 설정 함수 */
void MakeUdpSocket()
{
    struct sockaddr_in  servAddr;

    /* UDP 소켓 생성 */
    if ((UdpSockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)  {
        perror("socket");
        exit(1);
    }

    /* 서버 주소 초기화 */
    bzero((char *)&servAddr, sizeof(servAddr));
    servAddr.sin_family = PF_INET; // ipv4 프로토콜 사용
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY); // 모든 네트워크 인터페이스에 바인딩
    servAddr.sin_port = htons(SERV_UDP_PORT); // 지정된 UDP 포트로 바인딩

    /* 서버 주소로 소켓 바인딩 */
    if (bind(UdpSockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)  {
        perror("bind");
        exit(1);
    }

}

/* UNIX 도메인 스트림 소켓 생성 및 설정 함수 */
void MakeUcoSocket()
{
    struct sockaddr_un  servAddr;
    int                 servAddrLen;

    /* UNIX 도메인 스트림 소켓 생성 */
    if ((UcoSockfd = socket(PF_UNIX, SOCK_STREAM, 0)) < 0)  {
        perror("socket");
        exit(1);
    }

    /* 서버 주소 초기화 */
    bzero((char *)&servAddr, sizeof(servAddr));
    servAddr.sun_family = PF_UNIX; // UNIX 도메인 프로토콜 사용
    strcpy(servAddr.sun_path, UNIX_STR_PATH); // UNIX 도메인 소켓 경로 설정
    servAddrLen = strlen(servAddr.sun_path) + sizeof(servAddr.sun_family);

    /* 서버 주소로 소켓 바인딩 */
    if (bind(UcoSockfd, (struct sockaddr *) &servAddr, servAddrLen) < 0)  {
        perror("bind");
        exit(1);
    }

    /* 클라이언트 연결 대기 시작 */
    listen(UcoSockfd, 5);

}

/* UNIX 도메인 데이터그램 소켓 생성 및 설정 함수 */
void MakeUclSocket()
{
    struct sockaddr_un  servAddr;
    int                 servAddrLen;

    /* UNIX 도메인 데이터그램 소켓 생성 */
    if ((UclSockfd = socket(PF_UNIX, SOCK_DGRAM, 0)) < 0)  {
        perror("socket");
        exit(1);
    }

    /* 서버 주소 초기화 */
    bzero((char *)&servAddr, sizeof(servAddr));
    servAddr.sun_family = PF_UNIX; // UNIX 도메인 프로토콜 사용
    strcpy(servAddr.sun_path, UNIX_DG_PATH); // UNIX 도메인 데이터그램 소켓 경로 설정
    servAddrLen = strlen(servAddr.sun_path) + sizeof(servAddr.sun_family);

    /* 서버 주소로 소켓 바인딩 */
    if (bind(UclSockfd, (struct sockaddr *) &servAddr, servAddrLen) < 0)  {
        perror("bind");
        exit(1);
    }

}

/* TCP 요청 처리 함수 */
void ProcessTcpRequest()
{
    int                 newSockfd, cliAddrLen, n;
    struct sockaddr_in  cliAddr;
    MsgType             msg;

    cliAddrLen = sizeof(cliAddr);
    /* 클라이언트 연결 수락 */
    newSockfd = accept(TcpSockfd, (struct sockaddr *) &cliAddr, &cliAddrLen);
    if (newSockfd < 0)  {
        perror("accept");
        exit(1);
    }

    /* 클라이언트로부터 메세지 읽기 */
    if ((n = read(newSockfd, (char *)&msg, sizeof(msg))) < 0)  {
        perror("read");
        exit(1);
    }
    printf("Received TCP request: %s.....", msg.data);

    /* 응답 메세지 작성 및 전송 */
    msg.type = MSG_REPLY;
    sprintf(msg.data, "This is a reply from %d.", getpid());
    if (write(newSockfd, (char *)&msg, sizeof(msg)) < 0)  {
        perror("write");
        exit(1);
    }
    printf("Replied.\n");

    close(newSockfd);
}

/* UDP 요청 처리 함수 */
void ProcessUdpRequest()
{
    int                 cliAddrLen, n;
    struct sockaddr_in  cliAddr;
    MsgType             msg;

    cliAddrLen = sizeof(cliAddr);
    /* 클라이언트로부터 메세지 수신 */
    if ((n = recvfrom(UdpSockfd, (char *)&msg, sizeof(msg),
                0, (struct sockaddr *)&cliAddr, &cliAddrLen)) < 0)  {
        perror("recvfrom");
        exit(1);
    }
    printf("Received UDP request: %s.....", msg.data);

    /* 응답 메세지 작성 및 전송 */
    msg.type = MSG_REPLY;
    sprintf(msg.data, "This is a reply from %d.", getpid());
    if (sendto(UdpSockfd, (char *)&msg, sizeof(msg),
                0, (struct sockaddr *)&cliAddr, cliAddrLen) < 0)  {
        perror("sendto");
        exit(1);
    }
    printf("Replied.\n");
}

/* UNIX 도메인 스트림 요청 처리 함수 */
void ProcessUcoRequest()
{
    int                 newSockfd, cliAddrLen, n;
    struct sockaddr_un  cliAddr;
    MsgType             msg;

    cliAddrLen = sizeof(cliAddr);
    /* 클라이언트 연결 수락 */
    newSockfd = accept(UcoSockfd, (struct sockaddr *) &cliAddr, &cliAddrLen);
    if (newSockfd < 0)  {
        perror("accept");
        exit(1);
    }

    /* 클라이언트로부터 메세지 읽기 */
    if ((n = read(newSockfd, (char *)&msg, sizeof(msg))) < 0)  {
        perror("read");
        exit(1);
    }
    printf("Received UNIX-domain CO request: %s.....", msg.data);

    /* 응답 메세지 작성 및 전송 */
    msg.type = MSG_REPLY;
    sprintf(msg.data, "This is a reply from %d.", getpid());
    if (write(newSockfd, (char *)&msg, sizeof(msg)) < 0)  {
        perror("write");
        exit(1);
    }
    printf("Replied.\n");

    /* 클라이언트 연결 종료 */
    close(newSockfd);
}

/* UNIX 도메인 데이터그램 요청 처리 함수 */
void ProcessUclRequest()
{
    int                 cliAddrLen, n;
    struct sockaddr_un  cliAddr;
    MsgType             msg;

    cliAddrLen = sizeof(cliAddr);
    /* 클라이언트로부터 메시지 수신 */
    if ((n = recvfrom(UclSockfd, (char *)&msg, sizeof(msg),
                0, (struct sockaddr *)&cliAddr, &cliAddrLen)) < 0)  {
        perror("recvfrom");
        exit(1);
    }
    printf("Received UNIX-domain CL request: %s.....", msg.data);

    /* 응답 메세지 작성 및 전송 */
    msg.type = MSG_REPLY;
    sprintf(msg.data, "This is a reply from %d.", getpid());
    if (sendto(UclSockfd, (char *)&msg, sizeof(msg),
                0, (struct sockaddr *)&cliAddr, cliAddrLen) < 0)  {
        perror("sendto");
        exit(1);
    }
    printf("Replied.\n");
}

main(int argc, char *argv[])
{
    fd_set  fdvar;
    int     count;
    pthread_t tcpThread, udpThread, ucoThread, uclThread;

    /* 시그널 핸들러 등록 */
    signal(SIGINT, CloseServer);

    /* 소켓 생성 함수 호출 */
    MakeTcpSocket();
    MakeUdpSocket();
    MakeUcoSocket();
    MakeUclSocket();

    printf("Server daemon started.....\n");

    if (pthread_create(&tcpThread, NULL, (void *)ProcessTcpRequest, NULL) < 0)  {
        perror("pthread_create");
        exit(1);
    }

    if (pthread_create(&udpThread, NULL, (void *)ProcessUdpRequest, NULL) < 0)  {
        perror("pthread_create");
        exit(1);
    }

    if (pthread_create(&ucoThread, NULL, (void *)ProcessUcoRequest, NULL) < 0)  {
        perror("pthread_create");
        exit(1);
    }

    if (pthread_create(&uclThread, NULL, (void *)ProcessUclRequest, NULL) < 0)  {
        perror("pthread_create");
        exit(1);
    }

    while (1) {
        sleep(1);
    }

}