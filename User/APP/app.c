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
static  void  AppTaskUsart  ( void * p_arg );
static  void  AppTaskUsartTx  ( void * p_arg );
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
    OS_ERR  err;


    OSInit(&err);                                               /* Init uC/OS-III.                                      */

    OSTaskCreate((OS_TCB     *)&AppTaskStartTCB,                /* Create the start task                                */
                 (CPU_CHAR   *)"App Task Start",
                 (OS_TASK_PTR ) AppTaskStart,
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
CPU_INT08U MyPartitionStorage[10][12];
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

    OSMemCreate((OS_MEM *)&MyPartition,
								(CPU_CHAR *)"My Partition",
								(void *) &MyPartitionStorage[0][0],
								(OS_MEM_QTY)12,
								(OS_MEM_SIZE)100,
								(OS_ERR*)&err);
								
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
								 
    OSTaskCreate((OS_TCB     *)&AppTaskLed2TCB,                /* Create the Led2 task                                */
                 (CPU_CHAR   *)"App Task Led2",
                 (OS_TASK_PTR ) AppTaskLed2,
                 (void       *) 0,
                 (OS_PRIO     ) APP_TASK_LED2_PRIO,
                 (CPU_STK    *)&AppTaskLed2Stk[0],
                 (CPU_STK_SIZE) APP_TASK_LED2_STK_SIZE / 10,
                 (CPU_STK_SIZE) APP_TASK_LED2_STK_SIZE,
                 (OS_MSG_QTY  ) 5u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);

    OSTaskCreate((OS_TCB     *)&AppTaskLed3TCB,                /* Create the Led3 task                                */
                 (CPU_CHAR   *)"App Task Led3",
                 (OS_TASK_PTR ) AppTaskLed3,
                 (void       *) 0,
                 (OS_PRIO     ) APP_TASK_LED3_PRIO,
                 (CPU_STK    *)&AppTaskLed3Stk[0],
                 (CPU_STK_SIZE) APP_TASK_LED3_STK_SIZE / 10,
                 (CPU_STK_SIZE) APP_TASK_LED3_STK_SIZE,
                 (OS_MSG_QTY  ) 5u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
								 
    OSTaskCreate((OS_TCB     *)&AppTaskCanTxTCB,                /* Create the Led3 task                                */
                 (CPU_CHAR   *)"App Task CanTx",
                 (OS_TASK_PTR ) AppTaskCanTx,
                 (void       *) 0,
                 (OS_PRIO     ) APP_TASK_CANTX_PRIO,
                 (CPU_STK    *)&AppTaskCanTxStk[0],
                 (CPU_STK_SIZE) APP_TASK_CANTX_STK_SIZE / 10,
                 (CPU_STK_SIZE) APP_TASK_CANTX_STK_SIZE,
                 (OS_MSG_QTY  ) 5u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);		
    OSTaskCreate((OS_TCB     *)&AppTaskUsartTCB,                /* Create the Led3 task                                */
                 (CPU_CHAR   *)"App Task Usart",
                 (OS_TASK_PTR ) AppTaskUsart,
                 (void       *) 0,
                 (OS_PRIO     ) APP_TASK_USART_PRIO,
                 (CPU_STK    *)&AppTaskUsartStk[0],
                 (CPU_STK_SIZE) APP_TASK_USART_STK_SIZE / 10,
                 (CPU_STK_SIZE) APP_TASK_USART_STK_SIZE,
                 (OS_MSG_QTY  ) 5u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);	
OSTaskCreate((OS_TCB     *)&AppTaskUsartTxTCB,                /* Create the Led3 task                                */
                 (CPU_CHAR   *)"App Task Usart Tx",
                 (OS_TASK_PTR ) AppTaskUsartTx,
                 (void       *) 0,
                 (OS_PRIO     ) APP_TASK_USART_PRIO,
                 (CPU_STK    *)&AppTaskUsartTxStk[0],
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

static  void  AppTaskLed1 ( void * p_arg )
{
    OS_ERR      err;


   (void)p_arg;


    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.       */
			//macLED1_TOGGLE ();
			OSTimeDly ( 1000, OS_OPT_TIME_DLY, & err );
    }
		
		
}


/*
*********************************************************************************************************
*                                          LED2 TASK
*********************************************************************************************************
*/

static  void  AppTaskLed2 ( void * p_arg )
{
    OS_ERR      err;


   (void)p_arg;


    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.       */
			//macLED2_TOGGLE ();
			OSTimeDly ( 5000, OS_OPT_TIME_DLY, & err );
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
			OSTimeDly ( 10000, OS_OPT_TIME_DLY, & err );
    }
		
		
}
CanTxMsg TxMessage;			     //发送缓冲区
static  void  AppTaskCanTx  ( void * p_arg )
{
    OS_ERR      err;


   (void)p_arg;


    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.       */
			macLED3_TOGGLE ();
			/*设置要发送的报文*/
			CAN_SetMsg(&TxMessage);
			/*把报文存储到发送邮箱，发送*/
			CAN_Transmit(CANx, &TxMessage);
			
			OSTimeDly ( 50, OS_OPT_TIME_DLY, & err );
    }
		
		
}
uint32_t Date_R = 0;
static  void  AppTaskUsart  ( void * p_arg )
{
    OS_ERR      err;
	OS_MSG_SIZE    msg_size;
	CPU_SR_ALLOC();
		enum DownMsgType MsgType = ERR;
		
	uint8_t * pMsg;
		printf("Usart Initialize success!!!");
   (void)p_arg;


    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.       */
			//macLED3_TOGGLE ();
			/* 阻塞任务，等待任务消息 */
			pMsg = OSTaskQPend ((OS_TICK        )0,                    //无期限等待
											  (OS_OPT         )OS_OPT_PEND_BLOCKING, //没有消息就阻塞任务
											  (OS_MSG_SIZE   *)&msg_size,            //返回消息长度
											  (CPU_TS        *)0,                    //返回消息被发布的时间戳
											  (OS_ERR        *)&err); 
			OS_CRITICAL_ENTER();
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
static  void  AppTaskUsartTx  ( void * p_arg )
{
	 OS_ERR      err;
	 OS_MSG_SIZE size;
	 CPU_TS ts;
	 CPU_INT08U *DataRx;
   (void)p_arg;
	

    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.       */
			//macLED3_TOGGLE ();
			DataRx = OSTaskQPend(0,OS_OPT_PEND_BLOCKING,&size,&ts,&err);
			Usart_SendStr_length( RS232_USART, DataRx,size );
    }
}



