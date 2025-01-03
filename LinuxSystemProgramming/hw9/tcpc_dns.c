#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "tcp.h"


main(int argc, char *argv[])
{
    int                 sockfd, n;
    struct sockaddr_in  servAddr;
    MsgType             msg;
    struct hostent      *hp;

    if (argc != 2)  {
        fprintf(stderr, "Usage: %s IPaddress\n", argv[0]);
        exit(1);
    }

    /* 소켓 생성 */
    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)  {
        perror("socket");
        exit(1);
    }

    /* 서버 주소 구조체 초기화 및 설정 */
    bzero((char *)&servAddr, sizeof(servAddr));
    servAddr.sin_family = PF_INET; // ipv4 프로토콜 설정
    servAddr.sin_port = htons(SERV_TCP_PORT); // 서버 포트 번호 설정(네트워크 바이트 순서로 변환)

    /* 입력한 주소가 ip인지 호스트 이름인지 확인 */
    if (isdigit(argv[1][0]))  {
        servAddr.sin_addr.s_addr = inet_addr(argv[1]);
    }
    else  {
        /* 호스트 이름이면 gethostbyname 사용해서 이름 -> ip 주소 변환 과정 거침 */
        if ((hp = gethostbyname(argv[1])) == NULL)  {
            fprintf(stderr, "Unknown host: %s\n", argv[1]);
            exit(1);
        }
        /* 변환된 주소를 서버 주소 구조체에 복사함 */
        memcpy(&servAddr.sin_addr, hp->h_addr, hp->h_length);
    }

    /* 서버에 연결 요청 */
    if (connect(sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)  {
        perror("connect");
        exit(1);
    }

    /* 메세지 준비 */
    msg.type = MSG_REQUEST;
    sprintf(msg.data, "This is a request from %d.", getpid());
    /* 서버에 메세지 전송 */
    if (write(sockfd, (char *)&msg, sizeof(msg)) < 0)  {
        perror("write");
        exit(1);
    }
    printf("Sent a request.....");

    /* 서버로부터 응답 수신 */
    if ((n = read(sockfd, (char *)&msg, sizeof(msg))) < 0)  {
        perror("read");
        exit(1);
    }
    printf("Received reply: %s\n", msg.data);

    /* 소켓 닫음 */
    close(sockfd);
}