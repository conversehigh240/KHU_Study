#include <stdio.h>
#include "semlib.h"

/* semaphore 생성 및 초기화 */
int semInit(key_t key)
{
    int     semid;

    /* semaphore를 생성함. 혹은 기존 semaphore를 불러옴 */
    if ((semid = semget(key, 1, 0600 | IPC_CREAT)) < 0)  {
        perror("semget");
        return -1;
    }

    return semid;
}

/* semaphore 초기값을 설정함 */
int semInitValue(int semid, int value)
{
    union semun {
            int     val;
    } semun;

    semun.val = value;
    if (semctl(semid, 0, SETVAL, semun) < 0)  {
        perror("semctl");
        return -1;
    }

    return semid;
}

/* semaphore를 -1을 하고, 만약 0이면 wait함 */
int semWait(int semid)
{
    struct sembuf   semcmd;

    semcmd.sem_num = 0;
    semcmd.sem_op = -1;
    semcmd.sem_flg = SEM_UNDO;
    if (semop(semid, &semcmd, 1) < 0)  {
        perror("semop");
        return -1;
    }

    return 0;
}

/* semaphore를 -1을 하고, 만약 0이면 wait하지 않고 false로 return함. */
int semTryWait(int semid)
{
    struct sembuf   semcmd;

    semcmd.sem_num = 0;
    semcmd.sem_op = -1;
    semcmd.sem_flg = IPC_NOWAIT | SEM_UNDO;
    if (semop(semid, &semcmd, 1) < 0)  {
        perror("semop");
        return -1;
    }

    return 0;
}

/* semaphore +1 함 */
int semPost(int semid)
{
    struct sembuf   semcmd;

    semcmd.sem_num = 0;
    semcmd.sem_op = 1;
    semcmd.sem_flg = SEM_UNDO;
    if (semop(semid, &semcmd, 1) < 0)  {
        perror("semop");
        return -1;
    }

    return 0;
}

/* semaphore 값을 가져옴 */
int semGetValue(int semid)
{
    union semun {
            int     val;
    } dummy;

    return semctl(semid, 0, GETVAL, dummy);
}

/* semaphore를 제거함 */
int semDestroy(int semid)
{
    union semun {
            int     val;
    } dummy;

    if (semctl(semid, 0, IPC_RMID, dummy) < 0)  {
        perror("semctl");
        return -1;
    }
    close(semid);

    return 0;
}