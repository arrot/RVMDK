/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                             (c) Copyright 2013; Micrium, Inc.; Weston, FL
*
*                   All rights reserved.  Protected by international copyright laws.
*                   Knowledge of the source code may not be used to write a similar
*                   product.  This file may only be used in accordance with a license
*                   and should not be redistributed in any way.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                            EXAMPLE CODE
*
*                                       IAR Development Kits
*                                              on the
*
*                                    STM32F429II-SK KICKSTART KIT
*
* Filename      : app.c
* Version       : V1.00
* Programmer(s) : YS
*                 DC
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include <includes.h>
#include "Protocol.h"

/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                                 TCB
*********************************************************************************************************
*/

static  OS_TCB   AppTaskStartTCB;

static  OS_TCB   AppTaskLed1TCB;
static  OS_TCB   AppTaskLed2TCB;
static  OS_TCB   AppTaskLed3TCB;
static  OS_TCB   AppTaskCanTxTCB;//CAN总线发送任务
OS_TCB   AppTaskUsartTCB;//串口接收任务
OS_TCB   AppTaskUsartTxTCB;//串口发送任务
OS_TCB   AppTaskUsartTCB1;//串口接收任务
OS_TCB   AppTaskUsartTxTCB1;//串口发送任务
/*
*********************************************************************************************************
*                                                STACKS
*********************************************************************************************************
*/

static  CPU_STK  AppTaskStartStk[APP_TASK_START_STK_SIZE];

static  CPU_STK  AppTaskLed1Stk [ APP_TASK_LED1_STK_SIZE ];
static  CPU_STK  AppTaskLed2Stk [ APP_TASK_LED2_STK_SIZE ];
static  CPU_STK  AppTaskLed3Stk [ APP_TASK_LED3_STK_SIZE ];
static  CPU_STK  AppTaskCanTxStk [ APP_TASK_CANTX_STK_SIZE ];
static  CPU_STK  AppTaskUsartStk [ APP_TASK_USART_STK_SIZE ];
static  CPU_STK  AppTaskUsartTxStk [ APP_TASK_USART_TX_STK_SIZE ];
static  CPU_STK  AppTaskUsart1TxStk [ APP_TASK_USART1_TX_STK_SIZE ];
static  CPU_STK  AppTaskUsart1Stk [ APP_TASK_USART1_STK_SIZE ];
/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  AppTaskStart  (void *p_arg);

static  void  AppTaskLed1  ( void * p_arg );
static  void  AppTaskLed2  ( void * p_arg );
static  void  AppTaskLed3  ( void * p_arg );
static  void  AppTaskCanTx  ( void * p_arg );
static  void  AppTaskCan1Tx  ( void * p_arg );
static  void  AppTaskUsart  ( void * p_arg );
static  void  AppTaskUsartTx  ( void * p_arg );
static  void  AppTaskUsartTx1  ( void * p_arg );
static  void  AppTaskUsart11  ( void * p_arg );
/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary initialization.
*
* Arguments   : none
*
* Returns     : none
*********************************************************************************************************
*/

int  main (void)
{
    OS_ERR  err;//


    OSInit(&err); //初始化                                              /* Init uC/OS-III.                                      */
//新建一个主任务，操作系统独有，任务可理解为线程
    OSTaskCreate((OS_TCB     *)&AppTaskStartTCB,                /* Create the start task  任务名称                              */
                 (CPU_CHAR   *)"App Task Start",      
                 (OS_TASK_PTR ) AppTaskStart,          //任务事件
                 (void       *) 0,
                 (OS_PRIO     ) APP_TASK_START_PRIO,
                 (CPU_STK    *)&AppTaskStartStk[0],
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE / 10,
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE,
                 (OS_MSG_QTY  ) 5u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);

    OSStart(&err);                                              /* Start multitasking (i.e. give control to uC/OS-III). */
		
		
}


/*
*********************************************************************************************************
*                                          STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Arguments   : p_arg   is the argument passed to 'AppTaskStart()' by 'OSTaskCreate()'.
*
* Returns     : none
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/
OS_MEM MyPartition;
CPU_INT08U MyPartitionStorage[12][12];
static  void  AppTaskStart (void *p_arg)
{
    CPU_INT32U  cpu_clk_freq;
    CPU_INT32U  cnts;
    OS_ERR      err;


   (void)p_arg;

    BSP_Init();                                                 /* Initialize BSP functions                             */
    CPU_Init();

    cpu_clk_freq = BSP_CPU_ClkFreq();                           /* Determine SysTick reference freq.                    */
    cnts = cpu_clk_freq / (CPU_INT32U)OSCfg_TickRate_Hz;        /* Determine nbr SysTick increments                     */
    OS_CPU_SysTickInit(cnts);                                   /* Init uC/OS periodic time src (SysTick).              */

    Mem_Init();                                                 /* Initialize Memory Management Module                  */

#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&err);                               /* Compute CPU capacity with no task running            */
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();
#endif
//这句话是整理碎片用
    OSMemCreate((OS_MEM *)&MyPartition,
								(CPU_CHAR *)"My Partition",
								(void *) &MyPartitionStorage[0][0],
								(OS_MEM_QTY)12,
								(OS_MEM_SIZE)100,
								(OS_ERR*)&err);
					//打开很多任务线程，三个灯，两个串口接收，两个发送，两个can发送			
    OSTaskCreate((OS_TCB     *)&AppTaskLed1TCB,                /* Create the Led1 task                                */
                 (CPU_CHAR   *)"App Task Led1",
                 (OS_TASK_PTR ) AppTaskLed1,
                 (void       *) 0,
                 (OS_PRIO     ) APP_TASK_LED1_PRIO,
                 (CPU_STK    *)&AppTaskLed1Stk[0],
                 (CPU_STK_SIZE) APP_TASK_LED1_STK_SIZE / 10,
                 (CPU_STK_SIZE) APP_TASK_LED1_STK_SIZE,
                 (OS_MSG_QTY  ) 5u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
								 
//    OSTaskCreate((OS_TCB     *)&AppTaskLed2TCB,                /* Create the Led2 task                                */
//                 (CPU_CHAR   *)"App Task Led2",
//                 (OS_TASK_PTR ) AppTaskLed2,
//                 (void       *) 0,
//                 (OS_PRIO     ) APP_TASK_LED2_PRIO,
//                 (CPU_STK    *)&AppTaskLed2Stk[0],
//                 (CPU_STK_SIZE) APP_TASK_LED2_STK_SIZE / 10,
//                 (CPU_STK_SIZE) APP_TASK_LED2_STK_SIZE,
//                 (OS_MSG_QTY  ) 5u,
//                 (OS_TICK     ) 0u,
//                 (void       *) 0,
//                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
//                 (OS_ERR     *)&err);

//    OSTaskCreate((OS_TCB     *)&AppTaskLed3TCB,                /* Create the Led3 task                                */
//                 (CPU_CHAR   *)"App Task Led3",
//                 (OS_TASK_PTR ) AppTaskLed3,
//                 (void       *) 0,
//                 (OS_PRIO     ) APP_TASK_LED3_PRIO,
//                 (CPU_STK    *)&AppTaskLed3Stk[0],
//                 (CPU_STK_SIZE) APP_TASK_LED3_STK_SIZE / 10,
//                 (CPU_STK_SIZE) APP_TASK_LED3_STK_SIZE,
//                 (OS_MSG_QTY  ) 5u,
//                 (OS_TICK     ) 0u,
//                 (void       *) 0,
//                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
//                 (OS_ERR     *)&err);
//								 
//    OSTaskCreate((OS_TCB     *)&AppTaskCanTxTCB,                /* Create the Led3 task                                */
//                 (CPU_CHAR   *)"App Task CanTx",
//                 (OS_TASK_PTR ) AppTaskCanTx,
//                 (void       *) 0,
//                 (OS_PRIO     ) APP_TASK_CANTX_PRIO,
//                 (CPU_STK    *)&AppTaskCanTxStk[0],
//                 (CPU_STK_SIZE) APP_TASK_CANTX_STK_SIZE / 10,
//                 (CPU_STK_SIZE) APP_TASK_CANTX_STK_SIZE,
//                 (OS_MSG_QTY  ) 5u,
//                 (OS_TICK     ) 0u,
//                 (void       *) 0,
//                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
//                 (OS_ERR     *)&err);		
//    OSTaskCreate((OS_TCB     *)&AppTaskUsartTCB,                /* Create the Led3 task                                */
//                 (CPU_CHAR   *)"App Task Usart",
//                 (OS_TASK_PTR ) AppTaskUsart,
//                 (void       *) 0,
//                 (OS_PRIO     ) APP_TASK_USART_PRIO,
//                 (CPU_STK    *)&AppTaskUsartStk[0],
//                 (CPU_STK_SIZE) APP_TASK_USART_STK_SIZE / 10,
//                 (CPU_STK_SIZE) APP_TASK_USART_STK_SIZE,
//                 (OS_MSG_QTY  ) 5u,
//                 (OS_TICK     ) 0u,
//                 (void       *) 0,
//                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
//                 (OS_ERR     *)&err);	
//OSTaskCreate((OS_TCB     *)&AppTaskUsartTxTCB,                /* Create the Led3 task                                */
//                 (CPU_CHAR   *)"App Task Usart Tx",
//                 (OS_TASK_PTR ) AppTaskUsartTx,
//                 (void       *) 0,
//                 (OS_PRIO     ) APP_TASK_USART_PRIO,
//                 (CPU_STK    *)&AppTaskUsartTxStk[0],
//                 (CPU_STK_SIZE) APP_TASK_USART_TX_STK_SIZE / 10,
//                 (CPU_STK_SIZE) APP_TASK_USART_TX_STK_SIZE,
//                 (OS_MSG_QTY  ) 5u,
//                 (OS_TICK     ) 0u,
//                 (void       *) 0,
//                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
//                 (OS_ERR     *)&err);	
//串口1接收线程，串口的接收是靠中断，见bsp_rs232_usart.c中的RS232_USART111_IRQHandler，发布消息激活此任务
								 //串口第3步，增加串口接收线程，需要第4步，增加串口中断函数来激活线程
//OSTaskCreate((OS_TCB     *)&AppTaskUsartTCB1,                /* Create the Led3 task                                */
//                 (CPU_CHAR   *)"App Task Usart",
//                 (OS_TASK_PTR ) AppTaskUsart11,
//                 (void       *) 0,
//                 (OS_PRIO     ) APP_TASK_USART_PRIO,
//                 (CPU_STK    *)&AppTaskUsart1Stk[0],
//                 (CPU_STK_SIZE) APP_TASK_USART_STK_SIZE / 10,
//                 (CPU_STK_SIZE) APP_TASK_USART_STK_SIZE,
//                 (OS_MSG_QTY  ) 5u,
//                 (OS_TICK     ) 0u,
//                 (void       *) 0,
//                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
//                 (OS_ERR     *)&err);									 
		
		OSTaskCreate((OS_TCB     *)&AppTaskUsartTxTCB1,      //这要改          /* Create the Led3 task                                */
                 (CPU_CHAR   *)"App Task Usart Tx",
                 (OS_TASK_PTR ) AppTaskUsartTx1,    //这要改
                 (void       *) 0,
                 (OS_PRIO     ) APP_TASK_USART_PRIO,
                 (CPU_STK    *)&AppTaskUsart1TxStk[0],    //这要改
                 (CPU_STK_SIZE) APP_TASK_USART_TX_STK_SIZE / 10,
                 (CPU_STK_SIZE) APP_TASK_USART_TX_STK_SIZE,
                 (OS_MSG_QTY  ) 5u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);	
								 
								 OSTaskDel ( & AppTaskStartTCB, & err );
}


/*
*********************************************************************************************************
*                                          LED1 TASK
*********************************************************************************************************
*/
__IO uint16_t A1_now=0,A1_last=0;//占空比扩大10倍
int16_t A1_Delt = 0;
int32_t Angle_final = 0;//用来表示方向盘的角度,扩大100倍，单位为°
int iii = 0;
static  void  AppTaskLed1 ( void * p_arg )
{
    OS_ERR      err;


   (void)p_arg;


    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.       */
			//macLED1_TOGGLE ();
			//key设置PA0为读按键
			if(1 == GPIO_ReadInputDataBit(GPIOA,  GPIO_Pin_0))
			{
				//macLED3_TOGGLE ();
				if(iii % 100 == 0)
				TurnMotorSpeedSet(100);
			}
			else
			{
				TurnMotorSpeedSet(0);
			}
			iii++;
			A1_now = 	DutyCycle_A2;	
			OSTimeDly ( 10, OS_OPT_TIME_DLY, & err );
    }
		
		
}


/*
*********************************************************************************************************
*                                          LED2 TASK
*********************************************************************************************************
*/
int8_t speed;
static  void  AppTaskLed2 ( void * p_arg )
{
    OS_ERR      err;


   (void)p_arg;


    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.       */
			//macLED2_TOGGLE ();
			//key A0是按键，读按键状态
//			if(1 == GPIO_ReadInputDataBit(GPIOA,  GPIO_Pin_0))
//			{
//				macLED3_TOGGLE ();
//			}
//			//改变电机的转速；PE3接DIR PA1接PWM
//			BrakeSpeedSet(speed);
			OSTimeDly ( 1, OS_OPT_TIME_DLY, & err );
    }
		
		
}


/*
*********************************************************************************************************
*                                          LED3 TASK
*********************************************************************************************************
*/

static  void  AppTaskLed3 ( void * p_arg )
{
    OS_ERR      err;


   (void)p_arg;


    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.       */
			//macLED3_TOGGLE ();
			
			GPIO_SetBits(GPIOA, GPIO_Pin_4);
			OSTimeDly ( 50, OS_OPT_TIME_DLY, & err );
			GPIO_ResetBits(GPIOA, GPIO_Pin_4);
			OSTimeDly ( 50, OS_OPT_TIME_DLY, & err );
    }
		
		
}
CanTxMsg TxMessage;			     //发送缓冲区
static  void  AppTaskCanTx  ( void * p_arg )
{
    OS_ERR      err;
		uint8_t num;

   (void)p_arg;


    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.       */
//			macLED3_TOGGLE ();
//			macLED1_TOGGLE ();
			//第2步，设置PA4为高电平和低电平
//			GPIO_SetBits(GPIOA, GPIO_Pin_4);
//			GPIO_ResetBits(GPIOA, GPIO_Pin_4);
			//key或第2步，设置PA4为读高1低0
			//num = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4);
			/*设置要发送的报文*/
			
			CAN_SetMsg(&TxMessage);
			
			/*把报文存储到发送邮箱，发送*/
			CAN_Transmit(CANx, &TxMessage);
			
			OSTimeDly ( 50, OS_OPT_TIME_DLY, & err );
    }
		
		
}

uint32_t Date_R = 0;
CPU_INT08U GloDataUToC[12];//串口数据传给can的全局变量
static  void  AppTaskUsart  ( void * p_arg )
{
    OS_ERR      err;
	
	OS_MSG_SIZE    msg_size;
	CPU_SR_ALLOC();
		enum DownMsgType MsgType = ERR;
		
	uint8_t * pMsg;
		printf("Usart2 Initialize success!!!");
   (void)p_arg;


    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.       */
			//macLED3_TOGGLE ();
			/* 阻塞任务，等待任务消息 */
			pMsg = OSTaskQPend ((OS_TICK        )0,                    //无期限等待
											  (OS_OPT         )OS_OPT_PEND_BLOCKING, //没有消息就阻塞任务
											  (OS_MSG_SIZE   *)&msg_size,            //返回消息长度
											  (CPU_TS        *)0,                    //返回消息被发布的时间戳
											  (OS_ERR        *)&err); 
			//将串口接收的14字节中去掉字头2字节，剩下给全局变量
			OS_CRITICAL_ENTER();//锁定其它任务，必须做完我才能做别人
			for(int i = 0; i < 12; i++)
			{
			   GloDataUToC[i] = *(pMsg + 2 + i);
			}
			OS_CRITICAL_EXIT();
			
			OSTaskQPost ((OS_TCB      *)&AppTaskUsartTxTCB1,      //目标任务的控制块
									 (void        *)pMsg,             //消息内容的首地址
									 (OS_MSG_SIZE  )10,                     //消息长度
									 (OS_OPT       )OS_OPT_POST_FIFO,      //发布到任务消息队列的入口端
									 (OS_ERR      *)&err);                 //返回错误类型
									 
			OS_CRITICAL_ENTER();//锁定其它任务，必须做完我才能做别人
			MsgType = Msg_Process(msg_size,(const uint8_t * )pMsg, &Date_R);
		switch (MsgType)
		{
			case PosSetMsg:
			{
				//更新位置信息
				//BrakeSystemPosition_set = Date;
				/* 发送任务信号量到任务 AppTaskBrakeSystemControl */
				break;
			}
			case PulseMsg:
			{
				
				break;
			}
			case ERR:
			{
				
				break;
			}
			default:
			{
				
				break;
			}
		}
			OS_CRITICAL_EXIT(); 
			//OSTimeDly ( 10000, OS_OPT_TIME_DLY, & err );
    }
		
		
}	            
static  void  AppTaskUsart11  ( void * p_arg )
{
    OS_ERR      err;
	OS_MSG_SIZE    msg_size;
		enum DownMsgType MsgType = ERR;
		
	uint8_t * pMsg;
		//printf("Usart Initialize success!!!");
   (void)p_arg;


    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.       */
			//macLED3_TOGGLE ();
			/* 阻塞任务，等待任务消息 */
			//由中断触发的任务
			pMsg = OSTaskQPend ((OS_TICK        )0,                    //无期限等待
											  (OS_OPT         )OS_OPT_PEND_BLOCKING, //没有消息就阻塞任务
											  (OS_MSG_SIZE   *)&msg_size,            //返回消息长度
											  (CPU_TS        *)0,                    //返回消息被发布的时间戳
											  (OS_ERR        *)&err); 
			CPU_INT08U a[3];
			CPU_INT08S b[3];
			CPU_INT16U c[3];
			CPU_INT32U d[3];
			double v;
			a[0] = *pMsg;
			a[1] = *(pMsg + 1);
			a[2] = 0x15;
//			OS_CRITICAL_ENTER();
//			MsgType = Msg_Process(msg_size,(const uint8_t * )pMsg, &Date_R);
//		switch (MsgType)
//		{
//			case PosSetMsg:
//			{
//				//更新位置信息
//				//BrakeSystemPosition_set = Date;
//				/* 发送任务信号量到任务 AppTaskBrakeSystemControl */
//				break;
//			}
//			case PulseMsg:
//			{
//				
//				break;
//			}
//			case ERR:
//			{
//				
//				break;
//			}
//			default:
//			{
//				
//				break;
//			}
//		}
//			OS_CRITICAL_EXIT(); 
//			//OSTimeDly ( 10000, OS_OPT_TIME_DLY, & err );
    }
		
		
}	
static  void  AppTaskUsartTx  ( void * p_arg )
{
	 OS_ERR      err;
	 OS_MSG_SIZE size;
	 CPU_TS ts;
	 CPU_INT08U *DataRx;
   (void)p_arg;
	

	
    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.       */
			//macLED3_TOGGLE ();
			DataRx = OSTaskQPend(0,OS_OPT_PEND_BLOCKING,&size,&ts,&err);//被任务唤醒
			//一、十六进制发送
			//1定义指针DataRx的发送方法
		  DataRx = (CPU_INT08U*)OSMemGet(&MyPartition,&err);//申请内存
			*(DataRx+0) = 0xff;//地址高8位
		    *(DataRx+1) = 0xdd;//地址低8位
			Usart_SendStr_length( RS232_USART, DataRx,2 );
			//2定义数组的发送方法
			CPU_INT08U a[3];
			a[0] = 0x55;
			a[1] = 0x35;
			a[2] = 0x15;
			Usart_SendStr_length( RS232_USART, a,3 );//需写出数组长度
			
			//二、ascall码的发送方法
			//1发送字符串
	uint8_t *str;
	str = "hgfhgfh";
			Usart_SendString( RS232_USART, str);
//2发送数组,最后一个数组为0且不输出
			CPU_INT08U aa[3];
			aa[0] = 0x56;
			aa[1] = 0x36;
			aa[2] = 0x00;
			Usart_SendString( RS232_USART, aa);
			
			OSTimeDly ( 3000, OS_OPT_TIME_DLY, & err );
    }
}

static  void  AppTaskUsartTx1  ( void * p_arg )
{
	 OS_ERR      err;
	 OS_MSG_SIZE size;
	 CPU_TS ts;
	 CPU_INT08U *DataRx;
   (void)p_arg;
	
	uint8_t * pMsg1;
		OS_MSG_SIZE    msg_size;
    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.       */
			//macLED3_TOGGLE ();
			/* 阻塞任务，等待任务消息 */
			//pMsg1= OSTaskQPend(0,OS_OPT_PEND_BLOCKING,&size,&ts,&err);//被任务唤醒
			CPU_INT08U n[3];
			n[0] = A1_now >> 8;
			n[1] = A1_now;
			n[2] = iii;
//			for(int i = 0; i < 12; i++)
//			{
//			n[i + 2] = *(pMsg1 + i);
//			}
			Usart_SendStr_length( RS232_USART1, n,3 );
			OSTimeDly ( 50, OS_OPT_TIME_DLY, & err );
      //Usart_SendStr_length( RS232_USART1, (pMsg1 + 2),12 );
}

//    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.       */
//			//macLED3_TOGGLE ();
//			//DataRx = OSTaskQPend(0,OS_OPT_PEND_BLOCKING,&size,&ts,&err);//被任务唤醒
//			//一、十六进制发送
//			//1定义指针DataRx的发送方法
////			*(DataRx+0) = 0xff;//地址高8位
////		*(DataRx+1) = 0xdd;//地址低8位
////			Usart_SendStr_length( RS232_USART, DataRx,2 );
//			//2定义数组的发送方法
//			CPU_INT08U a[3];
//			a[0] = 0x55;
//			a[1] = 0x35;
//			a[2] = 0x15;
//			Usart_SendStr_length( RS232_USART1, a,3 );//需写出数组长度
//			
//			//二、ascall码的发送方法
//			//1发送字符串
//	uint8_t *str;
//	str = "h";
//			Usart_SendString( RS232_USART1, str);
////2发送数组,最后一个数组为0且不输出
//			CPU_INT08U aa[3];
//			aa[0] = 0x56;
//			aa[1] = 0x36;
//			aa[2] = 0x00;
//			Usart_SendString( RS232_USART1, aa);
//			
//			OSTimeDly ( 3000, OS_OPT_TIME_DLY, & err );
//    }
}

