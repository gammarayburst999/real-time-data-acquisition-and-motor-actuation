#include <linux/module.h>
#include <linux/init.h>

#include <rtai.h>
#include <rtai_sched.h>
#include <rtai_shm.h>
#include <rtai_nam2num.h>
#include <linux/comedi.h>
#include <linux/comedilib.h>

#define ARG 0
#define STACK_SIZE 1024
#define PRIORITY RT_SCHED_HIGHEST_PRIORITY
#define USE_FPU 1
#define NOW rt_get_time()
#define PERIOD nano2count(1e6)
#define BUFF_SIZE 100
#define READ_SUBDEVICE 0
#define WRITE_SUBDEVICE 1
#define READ_CHANNEL 0
#define WRITE_CHANNEL 0
#define RANGE 0
#define AREF AREF_GROUND

/* Store data needed for the thread */
RT_TASK thread_data;

/* Data needed by comedi */
comedi_t *comedi_dev;
int counter =0;
    int avg_data =0;
    int avg_data_sum =0;
   // int theta_01=189; // value at which the a/d read 0 
    int theta_real= 0;// value on the dial of the board
    int constant_01 =0;
   int constant_02 =0;
    int constant_03 =0;
   int constant_04 =0;
int calibrate =0;
    int angle_01=0;
    int value_01 =2087;// value at which it is 360 degree on dial
   int angle_02=90;
    int value_02 =1020 ;
 int angle_03=172; // beginning of  the dead zone
    int value_03 =0;
 int angle_04=197; // end of the dead zone
    int value_04 =4095;
 int angle_05=270;
    int value_05 =3174 ;
 int angle_06 =360;
//int value_06=360;
    int value_06 =2087;
typedef struct buffer
{
int a[BUFF_SIZE];
int readIndex;
int writeIndex;
int bufferSize;
}
cbuff;
cbuff* cbuffer;
int count_01 =0;
int count_02 =0;
int count_03 =0;
int count_04 =0;
int flag_01=0;
int flag_02=0;
int flag_03 =0;
int counter_04=0;
int error_value =0;
//int* set_point; ///////////////
int constant_07 = 0;
int Digital_value_write =0;
int set_point_02 =0;
int set_point_01 =100; // use when you want to remove set_point and hard code manually


/* The code that is run */
void thread_code(long arg)
{

  while (1)
  {
    int read_value, write_value = 0; /* What value should write_value be? */
    comedi_data_read(comedi_dev, READ_SUBDEVICE, READ_CHANNEL, RANGE, AREF, &read_value);
    avg_data_sum= avg_data_sum+read_value;
      //  printk("counter: %d, avg_data_sum = %d\n",counter,  avg_data_sum) ;
     counter++;
     if(counter==100)
     {
     avg_data =avg_data_sum/100;
     printk("average = %d\n", avg_data);

if(( flag_01 ==1 || flag_02 ==1))
{
if( avg_data >2071 && avg_data <2075)
{
theta_real= 180;
 printk("dead zone %d\n", theta_real);
flag_03 =1;
}
}   
if(flag_03 !=1)
{
if (avg_data>=(value_02+calibrate) && avg_data<=(value_01+calibrate))// will read between angle 0 to 90 value_01 =2090; value_02 =1020; 
     {
constant_01 =  (angle_02-angle_01)*(value_01-avg_data);
      theta_real = constant_01/(value_01-value_02);
         printk("average in degree= %d\n", theta_real);
     }
}

// second half between 90 to 172
 if(avg_data>=(value_03+calibrate) && avg_data<(value_02+calibrate))  // will read between angle 90  to 172// value_03 =0
     {
constant_02 =  (angle_03-angle_02)*(value_02-avg_data);
      theta_real = angle_02+constant_02/(value_02-value_03);
         printk("average in degree= %d\n", theta_real);
count_02 = 3;
if(theta_real == 172)
{
flag_01 =1;
flag_02 =0;
flag_03=0;
}
     }


// second half between 188 to 270
if(avg_data>=(value_05+calibrate) && avg_data<=(value_04+calibrate))  // will read between angle 188  to 270// value_04 =4095 value_05= 3174 angle_05=270 angle_04=188
     {
constant_03 =  (angle_05-angle_04)*(value_04-avg_data);
      theta_real = angle_04+(constant_03/(value_04-value_05));
         printk("average in degree= %d\n", theta_real);
count_03 =2;
if(theta_real == 197)
{
flag_02 =1;
flag_01 =0;
flag_03=0;
}

     }
// fourth half between 270 to 360
 if(avg_data>(value_06+calibrate) && avg_data<(value_05+calibrate))  // will read between angle 270  to 360// value_04 =4095 value_05= 3174 angle_05=270 angle_04=188
     {
constant_04 =  (angle_06-angle_05)*(value_05-avg_data);
      theta_real = angle_05+(constant_04/(value_05-value_06));
         printk("average in degree= %d\n", theta_real);
count_04 = 4;

     }
//if ( theta_real == set_point)
if ( theta_real == set_point_01)
{
set_point_02 = avg_data;
}
 //counter_04++;
//return theta_real;
//////////////////// write ////////////////////////////////
//cbuffer->a[cbuffer->writeIndex] = counter_04;
cbuffer->a[cbuffer->writeIndex] = theta_real;
cbuffer->bufferSize = cbuffer->bufferSize +1;

printk("buffer size= %d\n", cbuffer->bufferSize);
printk(" The current write Index is =%d\n",cbuffer->writeIndex);
printk(" The current written value is =%d\n",cbuffer->a[cbuffer->writeIndex]);

cbuffer->writeIndex = cbuffer->writeIndex +1;
if (cbuffer->writeIndex == BUFF_SIZE)
{
cbuffer->writeIndex=0;
}
if (cbuffer->bufferSize == BUFF_SIZE)
{
cbuffer->bufferSize=0;
}
printk("old buffer size= %d\n", cbuffer->bufferSize);

constant_07 = 4096/360;
printk("theta reaaaaaaaaaaallllllll= %d\n", theta_real);
printk("set point realllllll reaaaaaaaaaaallllllll= %d\n", set_point_01);
error_value = set_point_01 - theta_real;
 //error_value = set_point - theta_real;
if( error_value > 0)
{
Digital_value_write = 1950 - (error_value*0.9*constant_07);
//printk(" constant value =%d\n",constant_07);
printk(" error value =%d\n",error_value);
printk(" Digital_value_write =%d\n",Digital_value_write);
write_value = Digital_value_write;
comedi_data_write(comedi_dev, WRITE_SUBDEVICE, WRITE_CHANNEL, RANGE, AREF, write_value);
}
if( error_value < 0)
{
Digital_value_write = 2100 - (error_value*0.9*constant_07);
printk(" constant value =%d\n",constant_07);
printk(" error value =%d\n",error_value);
printk(" Digital_value_write =%d\n",Digital_value_write);
write_value = Digital_value_write;
comedi_data_write(comedi_dev, WRITE_SUBDEVICE, WRITE_CHANNEL, RANGE, AREF, write_value);
}

avg_data =0;
     avg_data_sum =0;
     counter=0;
     theta_real =0;
 //*mem =;
////////////////////////////////////////////
/////////////////// read ///////////////
/*int readValue =cbuffer.a[cbuffer.readIndex];
if (cbuffer.bufferSize>0)
{
cbuffer.bufferSize =cbuffer.bufferSize-1;
cbuffer.readIndex = cbuffer.readIndex +1;
if (cbuffer.readIndex ==BUFF_SIZE)
{
cbuffer.readIndex = 0;
}
}
*/
///////////////////////////////////////////
     }
    rt_task_wait_period();
  }
}
/* Called when "insmod" is used */
static int __init template_init(void)
{
cbuffer = rtai_kmalloc(nam2num("shmem"), sizeof(struct buffer));
//set_point = rtai_kmalloc(nam2num("set"), sizeof(int));
//printk("set point by user %d\n",*set_point);
  /* Start the RT timer, NB only needs to be done once */
  rt_set_periodic_mode();
  start_rt_timer(PERIOD);

  comedi_dev = comedi_open("/dev/comedi0");

  /* Initialise the data associated with a thread and make it periodic */
  rt_task_init(&thread_data, thread_code, ARG, STACK_SIZE, PRIORITY, USE_FPU, NULL);
  rt_task_make_periodic(&thread_data, NOW, PERIOD);

  /* Return success */
  return 0;
}

/* Called when "rmmod" is used */
static void __exit template_exit(void)
{
  rt_task_delete(&thread_data);
  comedi_close(comedi_dev);
 rtai_kfree(nam2num("shmem"));
}

module_init(template_init);
module_exit(template_exit);
