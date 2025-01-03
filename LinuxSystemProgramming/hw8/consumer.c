#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "prodcons.h"

main()
{
    BoundedBufferType   *pBuf;
    int                 shmid, i, data;

    /* shared memory를 생성함. 기존에 생성된게 있다면 그것의 키를 불러옴 */
    if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0)  {
        perror("shmget");
        exit(1);
    }

    /* shared memory를 process memory address에 연결함 */
    if ((pBuf = (BoundedBufferType *)shmat(shmid, 0, 0)) == (void *) -1)  {
        perror("shmat");
        exit(1);
    }

    srand(0x9999);

    /* NLOOPS만큼 반복하며 데이터를 읽어옴 */
    for (i = 0 ; i < NLOOPS ; i++)  {
        /* 버퍼가 비어있을 경우 대기 */
        if (pBuf->counter == 0)  {
            printf("Consumer: Buffer empty. Waiting.....\n");
            while (pBuf->counter == 0)
                ;
        }

        /* 버퍼에서 데이터를 소비함 */
        printf("Consumer: Consuming an item.....\n");
        data = pBuf->buf[pBuf->out].data;
        pBuf->out = (pBuf->out + 1) % MAX_BUF;
        pBuf->counter--;

        usleep((rand()%100)*10000);
    }

    printf("Consumer: Consumed %d items.....\n", i);
    printf("Consumer: %d items in buffer.....\n", pBuf->counter);
}