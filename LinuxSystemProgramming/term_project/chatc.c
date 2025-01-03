#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "chat.h"

/* 메세지 버퍼 최대 크기 정의 */
#define MAX_BUF     256

int     Sockfd;

/* 채팅 클라이언트 기능을 처리하는 함수 */
void ChatClient(void)
{
    char    buf[MAX_BUF];
    int     count, n;
    fd_set  fdset;

    /* 사용자 id 입력 및 서버로 전송 */
    printf("Enter ID: ");
    fflush(stdout);
    fgets(buf, MAX_BUF, stdin);
    *strchr(buf, '\n') = '\0';
    if (send(Sockfd, buf, strlen(buf)+1, 0) < 0)  { // 사용자 id 전송
        perror("send");
        exit(1);
    }
    printf("Press ^C to exit\n");

    /* 채팅 클라이언트의 메인 루프 */
    while (1)  {
        /* file descriptor 집합 초기화 및 설정 */
        FD_ZERO(&fdset);
        FD_SET(Sockfd, &fdset); // 서버 소켓 추가
        FD_SET(STDIN_FILENO, &fdset); // STDIN 추가

        /* select를 사용하여 소켓과 표준 입력에 데이터가 있는지 확인 */
        if ((count = select(10, &fdset, (fd_set *)NULL, (fd_set *)NULL,
            (struct timeval *)NULL)) < 0)  {
            perror("select");
            exit(1);
        }
        /* 데이터가 있는 file descriptor에 대한 처리 */
        while (count--)  {
            /* 서버로부터 데이터 수신 */
            if (FD_ISSET(Sockfd, &fdset))  {
                if ((n = recv(Sockfd, buf, MAX_BUF, 0)) < 0)  {
                    perror("recv");
                    exit(1);
                }
                if (n == 0)  { // 서버가 종료된 경우
                    fprintf(stderr, "Server terminated.....\n");
                    close(Sockfd);
                    exit(1);
                }
                printf("%s", buf);
            }
            else if (FD_ISSET(STDIN_FILENO, &fdset))  { // 사용자 입력 처리
                fgets(buf, MAX_BUF, stdin); // 입력받은 데이터 읽기
                if ((n = send(Sockfd, buf, strlen(buf)+1, 0)) < 0)  { // 서버로 전송
                    perror("send");
                    exit(1);
                }
            }
        }
    }
}

/* 클라이언트 정료 시 리소스 정리 및 종료 메세지 출력 */
void CloseClient(int signo)
{
    close(Sockfd);
    printf("\nChat client terminated.....\n");

    exit(0);
}


main(int argc, char *argv[])
{
    struct sockaddr_in  servAddr;
    struct hostent      *hp;

    if (argc != 2)  {
        fprintf(stderr, "Usage: %s ServerIPaddress\n", argv[0]);
        exit(1);
    }

    /* 소켓 생성 */
    if ((Sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)  {
        perror("socket");
        exit(1);
    }

    /* 서버 주소 초기화 */
    bzero((char *)&servAddr, sizeof(servAddr));
    servAddr.sin_family = PF_INET; // ipv4
    servAddr.sin_port = htons(SERV_TCP_PORT); // 지정된 포트 사용

    /* 서버 ip 주소 설정 */
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

    /* 서버에 연결 요청 */
    if (connect(Sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)  {
        perror("connect");
        exit(1);
    }

    /* 시그널 핸들러 등록 */
    signal(SIGINT, CloseClient);

    ChatClient();
}