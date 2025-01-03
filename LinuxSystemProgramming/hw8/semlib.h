#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

/* system V semaphore가 너무 복잡하기 때문에 posix semaphore같은 라이브러리를 만들어 사용 */
int     semInit(key_t key);
int     semInitValue(int semid, int value);
int     semWait(int semid);
int     semTryWait(int semid);
int     semPost(int semid);
int     semGetValue(int semid);
int     semDestroy(int semid);