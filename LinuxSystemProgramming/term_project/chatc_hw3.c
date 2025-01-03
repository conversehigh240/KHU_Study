/*
chatc.c를 select 시스템 콜이 아닌, multi-thread로 처리하도록 변경.
*/
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <netdb.h>
#include "chat.h"

#define MAX_BUF 256

int Sockfd;

/* 서버로부터 메시지를 수신하는 쓰레드 함수 */
void ReceiveMessages(void *arg) {
    char buf[MAX_BUF];
    int n;

    while (1) {
        /* 서버로부터 메시지 수신*/
        n = recv(Sockfd, buf, MAX_BUF, 0);
        if (n < 0) {
            perror("recv");
            exit(1);
        }
        if (n == 0) {
            /* 서버와의 연결 종료 */
            printf("Server terminated.....\n");
            close(Sockfd);
            pthread_exit(NULL);
        }
        printf("%s", buf);
        fflush(stdout);
    }
}

/* 서버에 메시지를 보내는 쓰레드 함수*/
void SendMessages(void *arg) {
    char buf[MAX_BUF];
    int n;

    while (1) {
        fgets(buf, MAX_BUF, stdin);
        /* 서버로 메세지를 전송함 */
        n = send(Sockfd, buf, strlen(buf) + 1, 0);
        if (n < 0) {
            perror("send");
            pthread_exit(NULL);
        }
    }
}

/* 시그널 핸들러 함수 */
void CloseClient(int signo) {
    close(Sockfd);
    printf("\nChat client terminated.....\n");
    exit(0);
}

main(int argc, char *argv[]) {
    struct sockaddr_in servAddr;
    struct hostent *hp;
    pthread_t recvThread, sendThread;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s ServerIPaddress\n", argv[0]);
        exit(1);
    }

    /* 서버와의 연결을 위한 소켓 생성*/
    if ((Sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    /* 서버 주소 초기화 */
    bzero((char *)&servAddr, sizeof(servAddr));
    servAddr.sin_family = PF_INET;
    servAddr.sin_port = htons(SERV_TCP_PORT);

    /* 서버 IP 주소 설정*/
    if (isdigit(argv[1][0])) {
        servAddr.sin_addr.s_addr = inet_addr(argv[1]);
    } else {
        if ((hp = gethostbyname(argv[1])) == NULL) {
            fprintf(stderr, "Unknown host: %s\n", argv[1]);
            exit(1);
        }
        memcpy(&servAddr.sin_addr, hp->h_addr, hp->h_length);
    }

    /* 서버에 연결 요청*/
    if (connect(Sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) {
        perror("connect");
        exit(1);
    }

    /* 시그널 핸들러 등록 */
    signal(SIGINT, CloseClient);

    /* 사용자 ID 입력 및 서버로 전송*/
    char buf[MAX_BUF];
    printf("Enter ID: ");
    fflush(stdout);
    fgets(buf, MAX_BUF, stdin);
    *strchr(buf, '\n') = '\0';
    if (send(Sockfd, buf, strlen(buf) + 1, 0) < 0) {
        perror("send");
        exit(1);
    }

    printf("Press ^C to exit\n");

    /* 메시지 수신 및 송신을 위한 thread 생성*/
    if (pthread_create(&recvThread, NULL,(void *)ReceiveMessages, NULL) != 0) {
        perror("pthread_create (recvThread)");
        exit(1);
    }
    if (pthread_create(&sendThread, NULL, (void *)SendMessages, NULL) != 0) {
        perror("pthread_create (sendThread)");
        exit(1);
    }

    pthread_join(recvThread, NULL);
    pthread_join(sendThread, NULL);

}