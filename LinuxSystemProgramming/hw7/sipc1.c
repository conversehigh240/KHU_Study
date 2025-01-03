#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "shm.h"

main()
{
    int     shmid;
    char    *ptr, *pData;
    int     *pInt;
    
    /* shared memory를 생성/접근함 */
    if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0)  {
        perror("shmget");
        exit(1);
    }   
    
    /* shared memory를 프로세스 주소 공간에 첨부함.
    ptr로 할당받은 공간의 첫 4byte를 flag로 이용함 */
    if ((ptr = shmat(shmid, 0, 0)) == (void *) -1)  {
        perror("shmat");
        exit(1);
    }   
    
    pInt = (int *)ptr;
    
    /* busy waiting */
    while ((*pInt) == 0)
        ; 
        
    pData = ptr + sizeof(int);
    printf("Received request: %s.....", pData);
    sprintf(pData, "This is a reply from %d.", getpid());
    *pInt = 0;
    printf("Replied.\n");
    
    sleep(1);
    
    /* shared memory 삭제 */
    if (shmctl(shmid, IPC_RMID, 0) < 0)  {
        perror("shmctl");
        exit(1);
    }   
} 