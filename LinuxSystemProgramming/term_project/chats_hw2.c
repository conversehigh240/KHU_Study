/*
chats.c를 thread를 생성하지 않고, select 시스템 콜을 이용하여 multiplexing을 처리하는 방식.
기존 chats.c에서는 thread를 생성해서 클라이언트 소켓을 개별적으로 처리했지만,
수정된 코드에서는 select 시스템 콜을 이용하기 위해 file descriptor set을 이용함.
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include "chat.h"

#define DEBUG

#define MAX_CLIENT 5
#define MAX_ID 32
#define MAX_BUF 256

/* 클라이언트 정보를 저장하는 구조체 */
typedef struct {
    int sockfd;
    int inUse;
    char uid[MAX_ID];
} ClientType;

int Sockfd;
ClientType Client[MAX_CLIENT];

/* 사용 가능한 클라이언트 id를 반환하는 함수 */
int GetID()
{
    int i;

    for (i = 0 ; i < MAX_CLIENT ; i++)  {
        if (! Client[i].inUse)  {
            Client[i].inUse = 1;
            return i;
        }
    }
}

/* 모든 클라이언트에게 메세지를 전달하는 함수 */
void SendToOtherClients(int id, char *buf) {
    int i;
    char msg[MAX_BUF + MAX_ID];

    /* 보낼 메세지 포맷팅 */
    sprintf(msg, "%s> %s", Client[id].uid, buf);
#ifdef DEBUG
    printf("%s", msg);
    fflush(stdout);
#endif

    for (i = 0; i < MAX_CLIENT; i++) {
        if (Client[i].inUse && (i != id)) {
            if (send(Client[i].sockfd, msg, strlen(msg) + 1, 0) < 0) {
                perror("send");
                exit(1);
            }
        }
    }
}

/* 서버 종료 시 리소스를 정리하는 함수 */
void CloseServer(int signo) {
    int i;

    /* 서버 소켓 닫기 */
    close(Sockfd);

    for (i = 0; i < MAX_CLIENT; i++) {
        if (Client[i].inUse) {
            close(Client[i].sockfd);
        }
    }

    printf("\nChat server terminated.....\n");
    exit(0);
}

int main(int argc, char *argv[]) {
    int newSockfd, cliAddrLen, id, maxFd, i;
    struct sockaddr_in cliAddr, servAddr;
    fd_set readFds, allFds;
    char buf[MAX_BUF];

    /* 시그널 핸들러 등록 */
    signal(SIGINT, CloseServer);

    /* 서버 소켓 생성 */
    if ((Sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    int one = 1;
    if (setsockopt(Sockfd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one)) < 0) {
        perror("setsockopt");
        exit(1);
    }

    /* 서버 주소 구조체 초기화 */
    bzero((char *)&servAddr, sizeof(servAddr));
    servAddr.sin_family = PF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(SERV_TCP_PORT);

    /* 서버 주소와 소켓 바인딩 */
    if (bind(Sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) {
        perror("bind");
        exit(1);
    }

    /* 클라이언트 접속 대기 */
    listen(Sockfd, 5);
    printf("Chat server started.....\n");

    /* 파일 디스크립터 집합 초기화.
    기존 chats.c에서는 thread를 생성하여 클라이언트 소켓을 개별적으로 처리했지만,
    여기서는 select() 시스템 콜 호출을 사용하여 처리하기 위해 파일 디스크립터가 필요함. */
    FD_ZERO(&allFds);
    FD_SET(Sockfd, &allFds);
    maxFd = Sockfd;

    for (i = 0; i < MAX_CLIENT; i++) {
        Client[i].inUse = 0;
    }

    cliAddrLen = sizeof(cliAddr);

    while (1) {
        readFds = allFds;

        /* select를 이용하여 다중 입출력 처리 */
        if (select(maxFd + 1, &readFds, NULL, NULL, NULL) < 0) {
            perror("select");
            exit(1);
        }

        /* 서버 소켓에 새로운 클라이언트 접속 */
        if (FD_ISSET(Sockfd, &readFds)) {
            newSockfd = accept(Sockfd, (struct sockaddr *)&cliAddr, &cliAddrLen);
            if (newSockfd < 0) {
                perror("accept");
                exit(1);
            }

            /* 빈 슬롯을 찾아 클라이언트 정보 저장 */
            for (id = 0; id < MAX_CLIENT; id++) {
                if (!Client[id].inUse) {
                    Client[id].inUse = 1;
                    Client[id].sockfd = newSockfd;
                    FD_SET(newSockfd, &allFds);
                    if (newSockfd > maxFd) {
                        maxFd = newSockfd;
                    }

                    if (recv(newSockfd, Client[id].uid, MAX_ID, 0) < 0) {
                        perror("recv");
                        exit(1);
                    }
                    printf("Client %d log-in(ID: %s).....\n", id, Client[id].uid);
                    break;
                }
            }

            if (id == MAX_CLIENT) {
                fprintf(stderr, "Too many clients\n");
                close(newSockfd);
            }
        }

        /* 각 클라이언트의 소켓에서 데이터 처리 */
        for (id = 0; id < MAX_CLIENT; id++) {
            if (Client[id].inUse && FD_ISSET(Client[id].sockfd, &readFds)) {
                int n = recv(Client[id].sockfd, buf, MAX_BUF, 0);
                if (n < 0) {
                    perror("recv");
                    exit(1);
                }
                if (n == 0) {
                    printf("Client %d log-out(ID: %s).....\n", id, Client[id].uid);
                    close(Client[id].sockfd);
                    FD_CLR(Client[id].sockfd, &allFds);
                    Client[id].inUse = 0;
                } else {
                    SendToOtherClients(id, buf);
                }
            }
        }
    }
}