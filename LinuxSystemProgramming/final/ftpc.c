#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "tcp.h"

int Sockfd;

struct packet {
    int count;
    char buf[100];
};

int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <server_address> <id>\n", argv[0]);
        exit(1);
    }

    const char *server_address = argv[1];
    const char *id = argv[2];

    struct sockaddr_in servAddr;
    MsgType msg;
    struct hostent *hp;

    // 소켓 생성
    Sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (Sockfd < 0) {
        perror("socket");
        exit(1);
    }

    // 서버 주소 설정
    bzero((char *)&servAddr, sizeof(servAddr));
    servAddr.sin_family = PF_INET;
    servAddr.sin_port = htons(SERV_TCP_PORT);

    if (isdigit(argv[1][0]))  {
        servAddr.sin_addr.s_addr = inet_addr(argv[1]);
    }
    else  {
        if ((hp = gethostbyname(argv[1])) == NULL)  {
            fprintf(stderr, "Unknown host: %s\n", argv[1]);
            exit(1);
        }
        memcpy(&servAddr.sin_addr, hp->h_addr, hp->h_length);
    }

    // 서버에 연결
    if (connect(Sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) {
        perror("connect");
        close(Sockfd);
        exit(1);
    }

    // ID 전송
    msg.type = MSG_REQUEST;
    strncpy(msg.data, id, sizeof(msg.data) - 1);
    msg.data[sizeof(msg.data) - 1] = '\0';

    if (write(Sockfd, &msg, sizeof(msg)) < 0) {
        perror("write");
        close(Sockfd);
        exit(1);
    }

    printf("Sent ID: %s to server.\n", id);

    // 서버로부터 응답 받기
    if (read(Sockfd, &msg, sizeof(msg)) < 0) {
        perror("read");
        close(Sockfd);
        exit(1);
    }

    printf("Server reply: %s\n", msg.data);

    close(Sockfd);
    return 0;
}