#include <stdio.h>
#include <rtai.h>
#include <rtai_nam2num.h>
#include <rtai_shm.h>
#include <sys/msg.h>
#include <errno.h>
#define BUFF_SIZE 100
struct mymsgbuf {
  long mtype;
  int data[10];
};
struct mymsgbuf message;
struct mymsgbuf_01 {
long mtype;
int data[1];
};
struct mymsgbuf_01 message_receive;

typedef struct buffer
{
int a[BUFF_SIZE];
int readIndex;
int writeIndex;
int bufferSize;
}cbuff;
cbuff* mem_one; //mem1
int* set_point; //setpt
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
   int status,status_receive, i;
  int queue, queue_receive;
int cc=0;
  key_t msgkey, msgkey_receive;
mem_one = rtai_malloc(nam2num("shmem"), sizeof(struct buffer));
set_point = rtai_malloc(nam2num("set"), sizeof(int));

  // generate a key to obtain a queue id
  msgkey = ftok(".",'m'); // this queue is for sending data for display
 // msgkey_receive = ftok(".",'p'); // this queue is for receiving the setpoint from user

  // obtain a queue identifier
  queue = msgget(msgkey, IPC_CREAT | 0660); // sending buffer value 
  //queue_receive = msgget(msgkey_receive, IPC_CREAT |0660); // receiving the set status 
  message.mtype = 2;

  while (1)
  {
for (i=0;i<10;i++)
 {
 message.data[i] =cb_read();
 printf("%d",message.data[i]);
 }
//status2 = msgrcv(rqueue, &message, sizeof(message.data), 0, 0);
//if (rmessage.mtype==1)

//printf(" setpoint receiving is :%d\n", message.data[0]);
//message.data[0]=*set_point;

// sending data to client 
sleep(1);
status = msgsnd(queue, &message, sizeof(message.data),0);
status_receive = msgrcv(queue, &message_receive, sizeof(message_receive.data),1,IPC_NOWAIT);
//printf("%s\n", strerror(errno));
if (message_receive.mtype==1)
{
printf(" setpoint receiving is print:%d\n", message_receive.data[0]);
*set_point=message_receive.data[0];
}
//printf("mtype=%d\n",message_receive.mtype);
// writing value the  to shared set point 
//printf(" sending .... for %d time \n", cc);
//cc++;
}
rtai_free(nam2num("shmem"), sizeof(int));
rtai_free(nam2num("set"), sizeof(int));
return 0;
} 



