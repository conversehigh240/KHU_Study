#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "prodcons.h"

main()
{
    BoundedBufferType   *pBuf; // shared memory에서 사용할 버퍼 포인터
    int                 shmid, i, data; // shared memory의 id와 데이터 변수

    /* shared memory를 할당함 */
    if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0)  {
        perror("shmget");
        exit(1);
    }

    /* shared memory를 virtual address space에 mapping함 */
    if ((pBuf = (BoundedBufferType *)shmat(shmid, 0, 0)) == (void *) -1)  {
        perror("shmat");
        exit(1);
    }

    srand(0x8888);
    /* NLOOPS만큼 돌면서 데이터를 생성하고 shared memory에 추가함 */
    for (i = 0 ; i < NLOOPS ; i++)  {
        /* 버퍼가 꽉 찬 경우 대기 */
        if (pBuf->counter == MAX_BUF)  {
            printf("Producer: Buffer full. Waiting.....\n");
            while (pBuf->counter == MAX_BUF)
                ;
        }

        /* 데이터를 생성하여 버퍼에 추가함 */
        printf("Producer: Producing an item.....\n");
        data = (rand()%100)*10000;
        pBuf->buf[pBuf->in].data = data;
        pBuf->in = (pBuf->in + 1) % MAX_BUF;
        pBuf->counter++;

        /* 데이터만큼 대기함 */
        usleep(data);
    }

    printf("Producer: Produced %d items.....\n", i);

    /* consumer가 버퍼를 읽어들일 시간을 줌 */
    sleep(2);
    printf("Producer: %d items in buffer.....\n", pBuf->counter);

    /* shared memory를 해제함 */
    if (shmctl(shmid, IPC_RMID, 0) < 0)  {
        perror("shmctl");
        exit(1);
    }
}