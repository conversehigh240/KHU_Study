#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "msg.h"

#define MSG_MODE    (0600 | IPC_CREAT)


main()
{
    int     msqid, n;
    MsgType msg;

    /* 메세지 큐를 생성함. 기존 메세지 큐가 존재한다면 그것과 연결함 */
    if ((msqid = msgget(MSG_KEY, MSG_MODE)) < 0)  {
        perror("msgget");
        exit(1);
    }

    /* 메세지 큐에서 메세지를 수신함 */
    if ((n = msgrcv(msqid, (char *)&msg, sizeof(msg), 0, 0)) < 0)  {
        perror("msgrcv");
        exit(1);
    }

    /* 수신한 메세지 내용을 출력함 */
    printf("Received request: %s.....", msg.data);

    /* 응답 메세지를 작성함 */
    msg.type = MSG_REPLY;
    sprintf(msg.data, "This is a reply from %d.", getpid());

    /* 메세지 큐에 응답 메세지를 전송함 */
    if (msgsnd(msqid, (char *)&msg, sizeof(msg), 0) < 0)  {
        perror("msgsnd");
        exit(1);
    }
    printf("Replied.\n");

    /* 메세지 큐 삭제 전 1초의 지연을 줌 (안정성 확보를 위함) */
    sleep(1);

    /* 메세지 큐를 삭제함 */
    if (msgctl(msqid, IPC_RMID, 0) < 0)  {
        perror("msgctl");
        exit(1);
    }
}