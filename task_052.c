#include <stdio.h>
#include <rtai.h>
#include <rtai_nam2num.h>
#include <rtai_shm.h>
#include <sys/msg.h>
#define BUFF_SIZE 100
struct mymsgbuf {
long mtype;
int rdata[10];
};
struct mymsgbuf rmessage;
int main(void)
{
int status, i;
int rqueue;
key_t msgkey;
msgkey = ftok(".",'m');
rqueue = msgget(msgkey, IPC_CREAT | 0660);

status = msgrcv(rqueue, &rmessage, sizeof(rmessage.rdata),1,0);
if (rmessage.mtype==1)
{
for(i=0;i<10;i++)
{
printf("sequence [%d]: %d \n",i,rmessage.rdata[i]);
}
sleep(1);
//printf("hello \n");
}
else printf("wrong message type \n");
return 0;
}


