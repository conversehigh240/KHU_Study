#define MAX_FIFO_NAME   32
  
#define SERV_FIFO   "./.fifo"

typedef struct  {
    char    returnFifo[MAX_FIFO_NAME];
    char    data[128];
}
    MsgType;