/*******************************************************************************
  * @file    xxx.c
  * @author  ����
  * @version V1.0
  * @date    2019-04-15
  * @brief   xxx����
  ******************************************************************************
  * @attention
  * δ��������ɣ��������������κ���;
  * ��Ȩ���У�����ؾ���
  * All rights reserved	
  * ��˾���廪���˳��Ŷ�
  * ����ƽ̨:STM32F429
  * ���䣺snowman4219@163.com
  *
  ******************************************************************************
  */
#include <includes.h>
#include "./can/bsp_can.h"
#include "Protocol.h"


/*
 * ��������CAN_GPIO_Config
 * ����  ��CAN��GPIO ����
 * ����  ����
 * ���  : ��
 * ����  ���ڲ�����
 */
static void CAN_GPIO_Config(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;   	

  /* ʹ��GPIOʱ��*/
  RCC_AHB1PeriphClockCmd(CAN_TX_GPIO_CLK|CAN_RX_GPIO_CLK, ENABLE);
	
	  /* ����Դ*/
  GPIO_PinAFConfig(CAN_RX_GPIO_PORT, CAN_RX_SOURCE, CAN_AF_PORT);
  GPIO_PinAFConfig(CAN_RX_GPIO_PORT, CAN_TX_SOURCE, CAN_AF_PORT); 

	  /* ���� CAN TX ���� */
  GPIO_InitStructure.GPIO_Pin = CAN_TX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(CAN_TX_GPIO_PORT, &GPIO_InitStructure);
	
	  /* ���� CAN RX ���� */
  GPIO_InitStructure.GPIO_Pin = CAN_RX_PIN ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_Init(CAN_RX_GPIO_PORT, &GPIO_InitStructure);
}

/*
 * ��������CAN_NVIC_Config
 * ����  ��CAN��NVIC ����,��1���ȼ��飬0��0���ȼ�
 * ����  ����
 * ���  : ��
 * ����  ���ڲ�����
 */
static void CAN_NVIC_Config(void)
{
   	NVIC_InitTypeDef NVIC_InitStructure;
		/* Configure one bit for preemption priority */
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	 	/*�ж�����*/
		NVIC_InitStructure.NVIC_IRQChannel = CAN_RX_IRQ;	   //CAN RX�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			   
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
 * ��������CAN_Mode_Config
 * ����  ��CAN��ģʽ ����
 * ����  ����
 * ���  : ��
 * ����  ���ڲ�����
 */
static void CAN_Mode_Config(void)
{
	CAN_InitTypeDef        CAN_InitStructure;
	/************************CANͨ�Ų�������**********************************/
	/* Enable CAN clock */
  RCC_APB1PeriphClockCmd(CAN_CLK, ENABLE);

	/*CAN�Ĵ�����ʼ��*/
	CAN_DeInit(CAN1);
	CAN_StructInit(&CAN_InitStructure);

	/*CAN��Ԫ��ʼ��*/
	CAN_InitStructure.CAN_TTCM=DISABLE;			   //MCR-TTCM  �ر�ʱ�䴥��ͨ��ģʽʹ��
	CAN_InitStructure.CAN_ABOM=ENABLE;			   //MCR-ABOM  ʹ���Զ����߹��� 
	CAN_InitStructure.CAN_AWUM=ENABLE;			   //MCR-AWUM  ʹ���Զ�����ģʽ
	CAN_InitStructure.CAN_NART=DISABLE;			   //MCR-NART  ��ֹ�����Զ��ش�	  
	CAN_InitStructure.CAN_RFLM=DISABLE;			   //MCR-RFLM  ����FIFO ������ ���ʱ�±��ĻḲ��ԭ�б���  
	CAN_InitStructure.CAN_TXFP=DISABLE;			   //MCR-TXFP  ����FIFO���ȼ� ȡ���ڱ��ı�ʾ�� 
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal ;  //��������ģʽCAN_Mode_LoopBack
	CAN_InitStructure.CAN_SJW=CAN_SJW_2tq;		   //BTR-SJW ����ͬ����Ծ��� 2��ʱ�䵥Ԫ
	 
	/* ss=1 bs1=5 bs2=3 λʱ����Ϊ(1+5+3) �����ʼ�Ϊʱ������tq*(1+3+5)  */
	CAN_InitStructure.CAN_BS1=CAN_BS1_5tq;		   //BTR-TS1 ʱ���1 ռ����5��ʱ�䵥Ԫ
	CAN_InitStructure.CAN_BS2=CAN_BS2_3tq;		   //BTR-TS1 ʱ���2 ռ����3��ʱ�䵥Ԫ	
	
	/* CAN Baudrate = 1 MBps (1MBps��Ϊstm32��CAN�������) (CAN ʱ��Ƶ��Ϊ APB 1 = 45 MHz) */     
	CAN_InitStructure.CAN_Prescaler =10;		  //BTR-BRP �����ʷ�Ƶ��  ������ʱ�䵥Ԫ��ʱ�䳤�� 45/(1+5+3)/10=500k bps
	CAN_Init(CANx, &CAN_InitStructure);
}


/*
 * ��������CAN_Filter_Config
 * ����  ��CAN��ɸѡ�� ����
 * ����  ����
 * ���  : ��
 * ����  ���ڲ�����
 */
static void CAN_Filter_Config(void)
{
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;

	/*CANɸѡ����ʼ��*/
	CAN_FilterInitStructure.CAN_FilterNumber=0;						//ɸѡ����0
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;	//����������ģʽ
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_16bit;	//ɸѡ��λ��Ϊ����32λ��
	/* ʹ��ɸѡ�������ձ�־�����ݽ��бȶ�ɸѡ����չID�������µľ����������ǵĻ��������FIFO0�� */

//	CAN_FilterInitStructure.CAN_FilterIdHigh= ((((u32)0x0000)|CAN_ID_STD|CAN_RTR_DATA)&0xFFFF0000)>>16;		//Ҫɸѡ��ID��λ 
//	CAN_FilterInitStructure.CAN_FilterIdLow= (((u32)0x0469)|CAN_ID_STD|CAN_RTR_DATA)&0xFFFF; //Ҫɸѡ��ID��λ 
//	
	CAN_FilterInitStructure.CAN_FilterIdHigh= 0;		//Ҫɸѡ��ID��λ 
	CAN_FilterInitStructure.CAN_FilterIdLow= 0; //Ҫɸѡ��ID��λ 

	CAN_FilterInitStructure.CAN_FilterMaskIdHigh= 0x0000;			//ɸѡ����16λ��ʹ��
	CAN_FilterInitStructure.CAN_FilterMaskIdLow= 0x0;			//ɸѡ����16λÿλ����ƥ��
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0 ;				//ɸѡ����������FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;			//ʹ��ɸѡ��
	CAN_FilterInit(&CAN_FilterInitStructure);
	/*CANͨ���ж�ʹ��*/
	CAN_ITConfig(CANx, CAN_IT_FMP0, ENABLE);
}


/*
 * ��������CAN_Config
 * ����  ����������CAN�Ĺ���
 * ����  ����
 * ���  : ��
 * ����  ���ⲿ����
 */
void CAN_Config(void)
{
  CAN_GPIO_Config();
  CAN_NVIC_Config();
  CAN_Mode_Config();
  CAN_Filter_Config();   
}


/**
  * @brief  ��ʼ�� Rx Message���ݽṹ��
  * @param  RxMessage: ָ��Ҫ��ʼ�������ݽṹ��
  * @retval None
  */
void Init_RxMes(CanRxMsg *RxMessage)
{
  uint8_t ubCounter = 0;

	/*�ѽ��սṹ������*/
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
 * ��������CAN_SetMsg
 * ����  ��CANͨ�ű�����������,����һ����������Ϊ0-7�����ݰ�
 * ����  ����
 * ���  : ��
 * ����  ���ⲿ����
 */	
	uint8_t Date[8] = {0};
	uint16_t angle = 45;
	uint8_t control_mode = 0x20,duizhong = 0x00;
extern uint32_t Date_R;
	extern CPU_INT08U GloDataUToC[12];
void CAN_SetMsg(CanTxMsg *TxMessage)
{	  
	uint8_t i = 0;
	

//  Date[0] = (Date_R >> 0) & 0xff; //�ֶ�����-0x10 ���Զ�����-0x20
//	Date[1] = (Date_R >> 8) & 0xff;
//	Date[2] = (Date_R >> 16) & 0xff;
//	Date[3] = (Date_R >> 24) & 0xff;
//	Date[4] = (Date_R >> 0) & 0xff;
//	Date[5] = (Date_R >> 8) & 0xff;
//  Date[6] = (Date_R >> 16) & 0xff;
//	Date[7] = (Date_R >> 24) & 0xff;	
	

  //TxMessage->StdId=0x469;						 
  //TxMessage->ExtId=0x1314;					 //ʹ�õ���չID
  TxMessage->IDE=CAN_ID_STD;					 //��չģʽ
  TxMessage->RTR=CAN_RTR_DATA;				 //���͵�������
  TxMessage->DLC=8;							 //���ݳ���Ϊ8�ֽ�
	
	/*����Ҫ���͵�����0-7*/
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
	/*�������ж�������*/
	CAN_Receive(CANx, CAN_FIFO0, &RxMessage);

	/* �Ƚ�ID�Ƿ�Ϊ0x1314 */ 
	//if((RxMessage.ExtId==0x1314) && (RxMessage.IDE==CAN_ID_EXT) && (RxMessage.DLC==8) )
		if((RxMessage.DLC==8) )
	{
		//RxMessage.StdId==0x1314
		DataRx = (CPU_INT08U*)OSMemGet(&MyPartition,&err);//�����ڴ�
//		CPU_INT32S *test1;
//		
//		test1 = (CPU_INT32S*)OSMemGet(&MyPartition,&err);
		*(DataRx+0) = (RxMessage.ExtId>>24) & 0xff;//��ַ��8λ
		*(DataRx+1) = (RxMessage.ExtId>>16) & 0xff;//��ַ�θ�8λ
		*(DataRx+2) = (RxMessage.ExtId>>8) & 0xff;//��ַ�δθ�8λ
		*(DataRx+3) = (RxMessage.ExtId>>0) & 0xff;//��ַ��8λ
		for(i=0;i<8;i++)
		{
			*(DataRx+4+i) =  RxMessage.Data[i];
		}
		//����������ڴ浥Ԫ
		//OSMemPut (&MyPartition,DataRx,&err);
		
//		OS_TaskQPost(&AppTaskUsartTxTCB,(void*)DataRx,(OS_MSG_SIZE)12,OS_OPT_POST_FIFO,0,&err);
		
		OSTaskQPost(&AppTaskUsartTxTCB1,(void*)DataRx,(OS_MSG_SIZE)12,OS_OPT_POST_FIFO,&err);
		//OS_TaskSemPost(&AppTaskUsartTxTCB,OS_OPT_NONE,(CPU_TS)0,&err);
	}
	else
	{
	 					   //����ʧ��
	}
}

/**************************END OF FILE************************************/











