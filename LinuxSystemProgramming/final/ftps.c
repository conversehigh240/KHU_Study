#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <signal.h>
#include "tcp.h"

int Sockfd;

struct packet{
    int count;
    char buf[100];
};

void CloseServer()
{
    close(Sockfd);
    printf("\nTCP Server exit.....\n");
    exit(0);
}

// 클라이언트의 ID와 접속 시간을 파일에 저장하는 함수
void log_client_info(const char *id, time_t time)
{
    FILE *file = fopen(id, "wb");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    fprintf(file, "%ld\n", time);
    fclose(file);
}

// 클라이언트와 통신하는 함수
void handle_client(int newSockfd)
{
    MsgType msg;
    struct timeval tv;
    time_t now;

    // 클라이언트 ID 받기
    if (read(newSockfd, &msg, sizeof(msg)) < 0) {
        perror("read");
        close(newSockfd);
        return;
    }

    printf("Received request: %s\n", msg.data);

    // 현재 시간 얻기
    gettimeofday(&tv, NULL);
    now = tv.tv_sec;

    // 접속 정보 기록
    log_client_info(msg.data, now);

    // 클라이언트에 응답
    msg.type = MSG_REPLY;
    sprintf(msg.data, "This is a reply from server at %ld.", now);
    if (write(newSockfd, &msg, sizeof(msg)) < 0) {
        perror("write");
        close(newSockfd);
        return;
    }

    printf("Replied to client.\n");

    usleep(10000);
    close(newSockfd);
}

int main()
{
    int newSockfd, cliAddrLen, n;
    struct sockaddr_in cliAddr, servAddr;

    signal(SIGINT, CloseServer);

    // 소켓 생성
    Sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (Sockfd < 0) {
        perror("socket");
        exit(1);
    }

    // 서버 주소 설정
    bzero((char *)&servAddr, sizeof(servAddr));
    servAddr.sin_family = PF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(SERV_TCP_PORT);

    // 소켓 바인딩
    if (bind(Sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0) {
        perror("bind");
        exit(1);
    }

    // 연결 대기
    listen(Sockfd, 5);

    printf("TCP Server started.....\n");

    cliAddrLen = sizeof(cliAddr);
    while (1) {
        // 클라이언트 연결 수락
        newSockfd = accept(Sockfd, (struct sockaddr *) &cliAddr, &cliAddrLen);
        if (newSockfd < 0) {
            perror("accept");
            exit(1);
        }

        // 자식 프로세스에서 클라이언트 처리
        if (fork() == 0) {
            close(Sockfd); // 자식 프로세스에서 서버 소켓 닫기
            handle_client(newSockfd);
            exit(0);
        }

        close(newSockfd); // 부모는 클라이언트 소켓을 닫고 계속 대기
    }

    close(Sockfd);
    return 0;
}