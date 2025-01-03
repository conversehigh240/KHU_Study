#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "msg.h"

#define MSG_MODE    (0600)


main()
{
    int     msqid, n;
    MsgType msg;

    /* 메세지 큐를 연결함 */
    if ((msqid = msgget(MSG_KEY, MSG_MODE)) < 0)  {
        perror("msgget");
        exit(1);
    }

    msg.type = MSG_REQUEST;
    sprintf(msg.data, "This is a request from %d.", getpid());
   
    /* 메세지 큐에 요청 메세지를 전송함 */
    if (msgsnd(msqid, (char *)&msg, sizeof(msg), 0) < 0)  {
        perror("msgsnd");
        exit(1);
    }
    printf("Sent a request.....");

    /* 메세지 큐에서 응답 메세지를 수신함 */
    if ((n = msgrcv(msqid, (char *)&msg, sizeof(msg), MSG_REPLY, 0)) < 0)  {
        perror("msgrcv");
        exit(1);
    }
    printf("Received reply: %s\n", msg.data);
}