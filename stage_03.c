#include <stdio.h>
#define BUFF_SIZE 100
#include <rtai.h>
#include <rtai_nam2num.h>
#include <rtai_shm.h>
typedef struct buffer
{
int a[BUFF_SIZE];
int readIndex;
int writeIndex;
int bufferSize;
}
cbuff;
cbuff *cbuffer;
int counter =0;
int readValue;
int main(void)
{
  cbuffer = rtai_malloc(nam2num("shmem"), sizeof( struct buffer));
  // code over here 
printf("buffer size= intially so here it all start %d\n", cbuffer->bufferSize);
//printf("test struct index=%d", mem->writeIndex);
//cbuffer=*mem;
  while (1)
  {
//counter++;
 //a[cbuffer->readIndex];
//if(cbuffer->bufferSize >= 0)
//{
//counter++;
//}
if (cbuffer->bufferSize == 50 )
{ 
printf("buffer size= hereeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee %d\n", cbuffer->writeIndex);
while (cbuffer->bufferSize>0)
{
cbuffer->bufferSize = cbuffer->bufferSize-1;
readValue=cbuffer->a[cbuffer->readIndex];
printf("degree value=%d\n",readValue );
//printf("buffer size= %d\n", cbuffer->bufferSize);
cbuffer->readIndex = cbuffer->readIndex +1;
//printf("write Index size= %d\n", cbuffer->writeIndex);
 if (cbuffer->readIndex ==BUFF_SIZE)
 {
 cbuffer->readIndex = 0;
 }
}
}

}
}


