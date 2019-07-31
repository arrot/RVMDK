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
//全局变量
double Angle = 0;
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

static  OS_TCB   AppTaskTurnMotorTCB;//转向电机的控制
static  OS_TCB   AppTaskBRK_MotorTCB;//制动电机的控制
static  OS_TCB   AppTaskAngularTransducerTCB;

OS_TCB	 AppTaskUsartTCB;
OS_TCB   AppTaskUsartTxTCB;
/*
*********************************************************************************************************
*                                                STACKS
*********************************************************************************************************
*/

static  CPU_STK  AppTaskStartStk[APP_TASK_START_STK_SIZE];
static  CPU_STK  AppTaskTurnMotorStk [ APP_TASK_TURN_MOTOR_STK_SIZE ];
static  CPU_STK  AppTaskBRK_MotorStk [ APP_TASK_BRK_MOTOR_STK_SIZE ];
static  CPU_STK  AppTaskAngularTransducerStk [ APP_TASK_ANGULAR_TRANSDUCER_STK_SIZE ];


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  AppTaskStart  (void *p_arg);

static  void  AppTaskTurnMotor  ( void * p_arg );
static  void  AppTaskBRK_Motor  ( void * p_arg );
static  void  AppTaskAngularTransducer  ( void * p_arg );


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
	//创建我的分区
    OSMemCreate((OS_MEM *)&MyPartition,
								(CPU_CHAR *)"My Partition",
								(void *) &MyPartitionStorage[0][0],
								(OS_MEM_QTY)12,
								(OS_MEM_SIZE)100,
								(OS_ERR*)&err);
								
    OSTaskCreate((OS_TCB     *)&AppTaskTurnMotorTCB,                /* Create the Led1 task                                */
                 (CPU_CHAR   *)"App Task TurnMotor",
                 (OS_TASK_PTR ) AppTaskTurnMotor,
                 (void       *) 0,
                 (OS_PRIO     ) APP_TASK_TURN_MOTOR_PRIO,
                 (CPU_STK    *)&AppTaskTurnMotorStk[0],
                 (CPU_STK_SIZE) APP_TASK_TURN_MOTOR_STK_SIZE / 10,
                 (CPU_STK_SIZE) APP_TASK_TURN_MOTOR_STK_SIZE,
                 (OS_MSG_QTY  ) 5u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
								 
    OSTaskCreate((OS_TCB     *)&AppTaskBRK_MotorTCB,                /* Create the Led2 task                                */
                 (CPU_CHAR   *)"App Task BRK_Motor",
                 (OS_TASK_PTR ) AppTaskBRK_Motor,
                 (void       *) 0,
                 (OS_PRIO     ) APP_TASK_BRK_MOTOR_PRIO,
                 (CPU_STK    *)&AppTaskBRK_MotorStk[0],
                 (CPU_STK_SIZE) APP_TASK_BRK_MOTOR_STK_SIZE / 10,
                 (CPU_STK_SIZE) APP_TASK_BRK_MOTOR_STK_SIZE,
                 (OS_MSG_QTY  ) 5u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);

    
		OSTaskCreate((OS_TCB     *)&AppTaskAngularTransducerTCB,                /* Create the Led3 task                                */
                 (CPU_CHAR   *)"App Task AngularTransducer",
                 (OS_TASK_PTR ) AppTaskAngularTransducer,
                 (void       *) 0,
                 (OS_PRIO     ) APP_TASK_ANGULAR_TRANSDUCER_PRIO,
                 (CPU_STK    *)&AppTaskAngularTransducerStk[0],
                 (CPU_STK_SIZE) APP_TASK_ANGULAR_TRANSDUCER_STK_SIZE / 10,
                 (CPU_STK_SIZE) APP_TASK_ANGULAR_TRANSDUCER_STK_SIZE,
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
uint8_t speed = 0;
uint8_t Mode = MANUAL_CONTROL;
int32_t PositionSet = 0;
static  void  AppTaskTurnMotor ( void * p_arg )
{
    OS_ERR      err;


   (void)p_arg;
    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.       */
			//macLED1_TOGGLE ();
			TurnMotorPositionControl((int32_t)Angle,PositionSet);
			OSTimeDly ( 10, OS_OPT_TIME_DLY, & err );
    }
		
		
}


/*
*********************************************************************************************************
*                                          LED2 TASK
*********************************************************************************************************
*/
int32_t BRK_MotorPositionSet = 0;
static  void  AppTaskBRK_Motor ( void * p_arg )
{
    OS_ERR      err;


   (void)p_arg;


    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.       */
			BRKMotorPositionControl(ADC_ConvertedValue,BRK_MotorPositionSet);
			OSTimeDly ( 5000, OS_OPT_TIME_DLY, & err );
    }
		
		
}


/*
*********************************************************************************************************
*                                          AngularTransducer TASK
*********************************************************************************************************
*/

static  void  AppTaskAngularTransducer ( void * p_arg )
{
    OS_ERR      err;
		uint8_t i;

   (void)p_arg;


    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.       */
			if(AngleTransducer(Angle_1,Angle_2,&Angle))
			{
				i++;
				if(i>200)//连续读取错误
				{
					printf("AngularTransducer ERROR :NUM 1");
				}
			}
			else
			{
				i = 0;
			}
				
			
			OSTimeDly ( 10, OS_OPT_TIME_DLY, & err );
    }	
}






