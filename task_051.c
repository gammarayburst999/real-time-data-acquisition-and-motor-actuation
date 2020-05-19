#include <stdio.h>
#include <rtai.h>
#include <rtai_nam2num.h>
#include <rtai_shm.h>
#include <sys/msg.h>
#define BUFF_SIZE 100
struct mymsgbuf {
  long mtype;
  int data[10];
};
struct mymsgbuf message;
typedef struct buffer
{
int a[BUFF_SIZE];
int readIndex;
int writeIndex;
int bufferSize;
}cbuff;
cbuff* mem_one; //mem1
//int* set_point; //setpt
int readValue;
 int cb_read()
 {
 readValue=mem_one->a[mem_one->readIndex];
 if (mem_one->bufferSize>0)
{
mem_one->bufferSize = mem_one->bufferSize-1;
//printf("buffer size= %d\n", cbuffer->bufferSize);
mem_one->readIndex = mem_one->readIndex +1;
printf("degree value=%d\n",readValue );
//printf("write Index size= %d\n", cbuffer->writeIndex);
 if (mem_one->readIndex ==BUFF_SIZE)
 {
 mem_one->readIndex = 0;
 }
}
 return readValue;
 }
int main(void)
{
  
  // from msgsend
   int status,status2, i;
  int queue, rqueue;
int cc=0;
  key_t msgkey;
mem_one = rtai_malloc(nam2num("shmem"), sizeof(struct buffer));
//set_point = rtai_malloc(nam2num("set"), sizeof(int));
//printf("Enter desired value: \n");
//scanf("%d",set_point);
  // generate a key to obtain a queue id
  msgkey = ftok(".",'m');

  // obtain a queue identifier
  queue = msgget(msgkey, IPC_CREAT | 0660);
  //rqueue = msgget(msgkey, IPC_CREAT |0660);
  // set up message
  // set the message type to 1 - can be any positive number
  message.mtype = 1;
  // code over here 
printf("buffer size= intially so here it all start %d\n");
//printf("test struct index=%d", mem->writeIndex);
//cbuffer=*mem;
  while (1)
  {
//status2 = msgrcv(rqueue, &message, sizeof(message.data), 0, 0);
//printf(" setpoint receiving is :%d\n", message.data[0]);f
for (i=0;i<10;i++)
{
message.data[i] =cb_read();
printf("%d",message.data[i]);
}
sleep(1);
//message.data[0]=*set_point;
printf(" sending .... for %d time \n", cc);
cc++;
status = msgsnd(queue, &message, sizeof(message.data),0);
}
rtai_free(nam2num("shmem"), sizeof(int));
//rtai_free(nam2num("set"), sizeof(int));
return 0;
} 



