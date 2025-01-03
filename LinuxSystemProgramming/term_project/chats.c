#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include "chat.h"

/* 디버깅 용도의 매크로 정의 */
#define DEBUG

/* 최대 클라이언트 수, 사용자 id, 메세지 버퍼 크기 정의 */
#define MAX_CLIENT  5
#define MAX_ID      32
#define MAX_BUF     256

/* 클라이언트 정보 구조체 정의 */
typedef struct  {
    int         sockfd;
    int         inUse;
    pthread_t   tid;
    char        uid[MAX_ID];
}
    ClientType;

int             Sockfd;
pthread_mutex_t Mutex;

ClientType      Client[MAX_CLIENT];

/* 사용 가능한 클라이언트 id를 반환하는 함수 */
int GetID()
{
    int i;

    for (i = 0 ; i < MAX_CLIENT ; i++)  {
        if (! Client[i].inUse)  { //사용중이지 않은 클라이언트를 찾음
            Client[i].inUse = 1; // 해당 클라이언트를 사용중으로 설정
            return i;
        }
    }
}

/* 다른 클라이언트들에게 메세지를 전송하는 함수 */
void SendToOtherClients(int id, char *buf)
{
    int     i;
    char    msg[MAX_BUF+MAX_ID];

    /* 보낼 메세지를 포맷팅 */
    sprintf(msg, "%s> %s", Client[id].uid, buf);
/* 디버깅 모드일 때 메세지 출력 */
#ifdef  DEBUG
    printf("%s", msg);
    fflush(stdout);
#endif

    /* 모든 클라이언트들에게 메세지를 전송 */
    pthread_mutex_lock(&Mutex);
    for (i = 0 ; i < MAX_CLIENT ; i++)  {
        if (Client[i].inUse && (i != id))  { // 현재 클라이언트를 제외한 클라이언트에게 메세지를 보냄
            if (send(Client[i].sockfd, msg, strlen(msg)+1, 0) < 0)  {
                perror("send");
                exit(1);
            }
        }
    }
    pthread_mutex_unlock(&Mutex);
}
   
/* 클라이언트와 통신을 처리하는 함수 (thread에서 실행) */
void ProcessClient(int id)
{
    char    buf[MAX_BUF];
    int     n;

    /* thread 취소 설정 */
    if (pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL))  {
        perror("pthread_setcancelstate");
        exit(1);
    }
    if (pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL))  {
        perror("pthread_setcanceltype");
        exit(1);
    }

    /* 클라이언트로부터 사용자 id 수신 */
    if ((n = recv(Client[id].sockfd, Client[id].uid, MAX_ID, 0)) < 0)  {
        perror("recv");
        exit(1);
    }
    printf("Client %d log-in(ID: %s).....\n", id, Client[id].uid);

    while (1)  {
        /* 클라이언트로부터 메세지 수신 */
        if ((n = recv(Client[id].sockfd, buf, MAX_BUF, 0)) < 0)  {
            perror("recv");
            exit(1);
        }
        /* 클라이언트 연결 종료 처리 */
        if (n == 0)  {
            printf("Client %d log-out(ID: %s).....\n", id, Client[id].uid);

            pthread_mutex_lock(&Mutex);
            close(Client[id].sockfd); // 소켓 닫기
            Client[id].inUse = 0; // 클라이언트 상태 초기화
            pthread_mutex_unlock(&Mutex);

            strcpy(buf, "log-out.....\n"); // 로그아웃 메세지 생성
            SendToOtherClients(id, buf); // 다른 클라이언트에 알림

            pthread_exit(NULL); // 스레드 종료
        }

        /* 다른 클라이언트에게 메세지 전송 */
        SendToOtherClients(id, buf);
    }
}

/* 서버 종료 시 리소스를 정리하는 함수 */
void CloseServer(int signo)
{
    int     i;

    /* 서버 소켓 닫기 */
    close(Sockfd);

    /* 각 클라이언트 스레드 정리 */
    for (i = 0 ; i < MAX_CLIENT ; i++)  {
        if (Client[i].inUse)  {
            if (pthread_cancel(Client[i].tid))  {
                perror("pthread_cancel");
                exit(1);
            }
            if (pthread_join(Client[i].tid, NULL))  {
                perror("pthread_join");
                exit(1);
            }
            close(Client[i].sockfd);
        }
    }
    /* 뮤텍스 해제 */
    if (pthread_mutex_destroy(&Mutex) < 0)  {
        perror("pthread_mutex_destroy");
        exit(1);
    }

    printf("\nChat server terminated.....\n");

    exit(0);
}

main(int argc, char *argv[])
{
    int                 newSockfd, cliAddrLen, id, one = 1;
    struct sockaddr_in  cliAddr, servAddr;

    /* 시그널 핸들러 등록 */
    signal(SIGINT, CloseServer);
    /* 뮤텍스 초기화 */
    if (pthread_mutex_init(&Mutex, NULL) < 0)  {
        perror("pthread_mutex_init");
        exit(1);
    }

    /* 서버 소켓 생성 */
    if ((Sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)  {
        perror("socket");
        exit(1);
    }

    /* 소켓 옵션 설정 */
    if (setsockopt(Sockfd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one)) < 0)  {
        perror("setsockopt");
        exit(1);
    }

    /* 서버 주소 설정 */
    bzero((char *)&servAddr, sizeof(servAddr));
    servAddr.sin_family = PF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY); // 모든 ip로부터 수신 허용
    servAddr.sin_port = htons(SERV_TCP_PORT); // 지정된 포트 사용

    /* 소켓 바인딩 */
    if (bind(Sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)  {
        perror("bind");
        exit(1);
    }

    /* 클라이언트 연결 대기 */
    listen(Sockfd, 5);

    printf("Chat server started.....\n");

    cliAddrLen = sizeof(cliAddr);
    while (1)  {
        /* 클라이언트 연결 수락 */
        newSockfd = accept(Sockfd, (struct sockaddr *) &cliAddr, &cliAddrLen);
        if (newSockfd < 0)  {
            perror("accept");
            exit(1);
        }

        /* 사용 가능한 클라이언트 id 확보 */
        id = GetID();
        Client[id].sockfd = newSockfd;

        /* 새로운 thread 생성하여 클라이언트 처리 */
        if (pthread_create(&Client[id].tid, NULL, (void *)ProcessClient, (void *)id) < 0)  {
            perror("pthread_create");
            exit(1);
        }
    }
}