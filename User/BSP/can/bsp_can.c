/*******************************************************************************
  * @file    xxx.c
  * @author  郭栋
  * @version V1.0
  * @date    2019-04-15
  * @brief   xxx程序
  ******************************************************************************
  * @attention
  * 未经作者许可，不得用于其它任何用途
  * 版权所有，盗版必究。
  * All rights reserved	
  * 公司：清华无人车团队
  * 开发平台:STM32F429
  * 邮箱：snowman4219@163.com
  *
  ******************************************************************************
  */
#include <includes.h>
#include "./can/bsp_can.h"
#include "Protocol.h"


/*
 * 函数名：CAN_GPIO_Config
 * 描述  ：CAN的GPIO 配置
 * 输入  ：无
 * 输出  : 无
 * 调用  ：内部调用
 */
static void CAN_GPIO_Config(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;   	

  /* 使能GPIO时钟*/
  RCC_AHB1PeriphClockCmd(CAN_TX_GPIO_CLK|CAN_RX_GPIO_CLK, ENABLE);
	
	  /* 引脚源*/
  GPIO_PinAFConfig(CAN_RX_GPIO_PORT, CAN_RX_SOURCE, CAN_AF_PORT);
  GPIO_PinAFConfig(CAN_RX_GPIO_PORT, CAN_TX_SOURCE, CAN_AF_PORT); 

	  /* 配置 CAN TX 引脚 */
  GPIO_InitStructure.GPIO_Pin = CAN_TX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(CAN_TX_GPIO_PORT, &GPIO_InitStructure);
	
	  /* 配置 CAN RX 引脚 */
  GPIO_InitStructure.GPIO_Pin = CAN_RX_PIN ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_Init(CAN_RX_GPIO_PORT, &GPIO_InitStructure);
}

/*
 * 函数名：CAN_NVIC_Config
 * 描述  ：CAN的NVIC 配置,第1优先级组，0，0优先级
 * 输入  ：无
 * 输出  : 无
 * 调用  ：内部调用
 */
static void CAN_NVIC_Config(void)
{
   	NVIC_InitTypeDef NVIC_InitStructure;
		/* Configure one bit for preemption priority */
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	 	/*中断设置*/
		NVIC_InitStructure.NVIC_IRQChannel = CAN_RX_IRQ;	   //CAN RX中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			   
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
 * 函数名：CAN_Mode_Config
 * 描述  ：CAN的模式 配置
 * 输入  ：无
 * 输出  : 无
 * 调用  ：内部调用
 */
static void CAN_Mode_Config(void)
{
	CAN_InitTypeDef        CAN_InitStructure;
	/************************CAN通信参数设置**********************************/
	/* Enable CAN clock */
  RCC_APB1PeriphClockCmd(CAN_CLK, ENABLE);

	/*CAN寄存器初始化*/
	CAN_DeInit(CAN1);
	CAN_StructInit(&CAN_InitStructure);

	/*CAN单元初始化*/
	CAN_InitStructure.CAN_TTCM=DISABLE;			   //MCR-TTCM  关闭时间触发通信模式使能
	CAN_InitStructure.CAN_ABOM=ENABLE;			   //MCR-ABOM  使能自动离线管理 
	CAN_InitStructure.CAN_AWUM=ENABLE;			   //MCR-AWUM  使用自动唤醒模式
	CAN_InitStructure.CAN_NART=DISABLE;			   //MCR-NART  禁止报文自动重传	  
	CAN_InitStructure.CAN_RFLM=DISABLE;			   //MCR-RFLM  接收FIFO 不锁定 溢出时新报文会覆盖原有报文  
	CAN_InitStructure.CAN_TXFP=DISABLE;			   //MCR-TXFP  发送FIFO优先级 取决于报文标示符 
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal ;  //正常工作模式CAN_Mode_LoopBack
	CAN_InitStructure.CAN_SJW=CAN_SJW_2tq;		   //BTR-SJW 重新同步跳跃宽度 2个时间单元
	 
	/* ss=1 bs1=5 bs2=3 位时间宽度为(1+5+3) 波特率即为时钟周期tq*(1+3+5)  */
	CAN_InitStructure.CAN_BS1=CAN_BS1_5tq;		   //BTR-TS1 时间段1 占用了5个时间单元
	CAN_InitStructure.CAN_BS2=CAN_BS2_3tq;		   //BTR-TS1 时间段2 占用了3个时间单元	
	
	/* CAN Baudrate = 1 MBps (1MBps已为stm32的CAN最高速率) (CAN 时钟频率为 APB 1 = 45 MHz) */     
	CAN_InitStructure.CAN_Prescaler =10;		  //BTR-BRP 波特率分频器  定义了时间单元的时间长度 45/(1+5+3)/10=500k bps
	CAN_Init(CANx, &CAN_InitStructure);
}


/*
 * 函数名：CAN_Filter_Config
 * 描述  ：CAN的筛选器 配置
 * 输入  ：无
 * 输出  : 无
 * 调用  ：内部调用
 */
static void CAN_Filter_Config(void)
{
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;

	/*CAN筛选器初始化*/
	CAN_FilterInitStructure.CAN_FilterNumber=0;						//筛选器组0
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;	//工作在掩码模式
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_16bit;	//筛选器位宽为单个32位。
	/* 使能筛选器，按照标志的内容进行比对筛选，扩展ID不是如下的就抛弃掉，是的话，会存入FIFO0。 */

//	CAN_FilterInitStructure.CAN_FilterIdHigh= ((((u32)0x0000)|CAN_ID_STD|CAN_RTR_DATA)&0xFFFF0000)>>16;		//要筛选的ID高位 
//	CAN_FilterInitStructure.CAN_FilterIdLow= (((u32)0x0469)|CAN_ID_STD|CAN_RTR_DATA)&0xFFFF; //要筛选的ID低位 
//	
	CAN_FilterInitStructure.CAN_FilterIdHigh= 0;		//要筛选的ID高位 
	CAN_FilterInitStructure.CAN_FilterIdLow= 0; //要筛选的ID低位 

	CAN_FilterInitStructure.CAN_FilterMaskIdHigh= 0x0000;			//筛选器高16位不使用
	CAN_FilterInitStructure.CAN_FilterMaskIdLow= 0x0;			//筛选器低16位每位必须匹配
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0 ;				//筛选器被关联到FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;			//使能筛选器
	CAN_FilterInit(&CAN_FilterInitStructure);
	/*CAN通信中断使能*/
	CAN_ITConfig(CANx, CAN_IT_FMP0, ENABLE);
}


/*
 * 函数名：CAN_Config
 * 描述  ：完整配置CAN的功能
 * 输入  ：无
 * 输出  : 无
 * 调用  ：外部调用
 */
void CAN_Config(void)
{
  CAN_GPIO_Config();
  CAN_NVIC_Config();
  CAN_Mode_Config();
  CAN_Filter_Config();   
}


/**
  * @brief  初始化 Rx Message数据结构体
  * @param  RxMessage: 指向要初始化的数据结构体
  * @retval None
  */
void Init_RxMes(CanRxMsg *RxMessage)
{
  uint8_t ubCounter = 0;

	/*把接收结构体清零*/
  RxMessage->StdId = 0x00;
  RxMessage->ExtId = 0x00;
  RxMessage->IDE = CAN_ID_STD;
  RxMessage->DLC = 0;
  RxMessage->FMI = 0;
  for (ubCounter = 0; ubCounter < 8; ubCounter++)
  {
    RxMessage->Data[ubCounter] = 0x00;
  }
}


/*
 * 函数名：CAN_SetMsg
 * 描述  ：CAN通信报文内容设置,设置一个数据内容为0-7的数据包
 * 输入  ：无
 * 输出  : 无
 * 调用  ：外部调用
 */	
	uint8_t Date[8] = {0};
	uint16_t angle = 45;
	uint8_t control_mode = 0x20,duizhong = 0x00;
extern uint32_t Date_R;
	extern CPU_INT08U GloDataUToC[12];
void CAN_SetMsg(CanTxMsg *TxMessage)
{	  
	uint8_t i = 0;
	

//  Date[0] = (Date_R >> 0) & 0xff; //手动控制-0x10 ，自动控制-0x20
//	Date[1] = (Date_R >> 8) & 0xff;
//	Date[2] = (Date_R >> 16) & 0xff;
//	Date[3] = (Date_R >> 24) & 0xff;
//	Date[4] = (Date_R >> 0) & 0xff;
//	Date[5] = (Date_R >> 8) & 0xff;
//  Date[6] = (Date_R >> 16) & 0xff;
//	Date[7] = (Date_R >> 24) & 0xff;	
	

  //TxMessage->StdId=0x469;						 
  //TxMessage->ExtId=0x1314;					 //使用的扩展ID
  TxMessage->IDE=CAN_ID_STD;					 //扩展模式
  TxMessage->RTR=CAN_RTR_DATA;				 //发送的是数据
  TxMessage->DLC=8;							 //数据长度为8字节
	
	/*设置要发送的数据0-7*/
//	for (i = 0; i < 8; i++)
//  {
//    TxMessage->Data[i] = Date[i];
//  }
CPU_SR_ALLOC();
			OS_CRITICAL_ENTER();
			TxMessage->StdId=GloDataUToC[0] << 24 + GloDataUToC[1] << 16 + GloDataUToC[2] << 8 + GloDataUToC[0];
for (i = 0; i < 8; i++)
  {
    TxMessage->Data[i] = GloDataUToC[i + 4];
  }
	OS_CRITICAL_EXIT();
//OS_CRITICAL_EXIT();

}

CanRxMsg RxMessage;
extern OS_MEM MyPartition;
extern OS_TCB AppTaskUsartTxTCB;
extern OS_TCB AppTaskUsartTxTCB1;
CPU_INT08U *DataRx;
OS_ERR err;
void CAN_RX_IRQHandler(void)
{
	uint8_t i;
	/*从邮箱中读出报文*/
	CAN_Receive(CANx, CAN_FIFO0, &RxMessage);

	/* 比较ID是否为0x1314 */ 
	//if((RxMessage.ExtId==0x1314) && (RxMessage.IDE==CAN_ID_EXT) && (RxMessage.DLC==8) )
		if((RxMessage.DLC==8) )
	{
		//RxMessage.StdId==0x1314
		DataRx = (CPU_INT08U*)OSMemGet(&MyPartition,&err);//申请内存
//		CPU_INT32S *test1;
//		
//		test1 = (CPU_INT32S*)OSMemGet(&MyPartition,&err);
		*(DataRx+0) = (RxMessage.ExtId>>24) & 0xff;//地址高8位
		*(DataRx+1) = (RxMessage.ExtId>>16) & 0xff;//地址次高8位
		*(DataRx+2) = (RxMessage.ExtId>>8) & 0xff;//地址次次高8位
		*(DataRx+3) = (RxMessage.ExtId>>0) & 0xff;//地址低8位
		for(i=0;i<8;i++)
		{
			*(DataRx+4+i) =  RxMessage.Data[i];
		}
		//回收申请的内存单元
		//OSMemPut (&MyPartition,DataRx,&err);
		
//		OS_TaskQPost(&AppTaskUsartTxTCB,(void*)DataRx,(OS_MSG_SIZE)12,OS_OPT_POST_FIFO,0,&err);
		
		OSTaskQPost(&AppTaskUsartTxTCB1,(void*)DataRx,(OS_MSG_SIZE)12,OS_OPT_POST_FIFO,&err);
		//OS_TaskSemPost(&AppTaskUsartTxTCB,OS_OPT_NONE,(CPU_TS)0,&err);
	}
	else
	{
	 					   //接收失败
	}
}

/**************************END OF FILE************************************/











