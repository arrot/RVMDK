/**
  ******************************************************************************
  * @file    bsp_rs232_usart.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   ʹ�ô���2���ض���c��printf������usart�˿ڣ��жϽ���ģʽ
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:����  STM32 F429 ������  
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 
  
#include "./usart/bsp_rs232_usart.h"
#include <includes.h>

 /**
  * @brief  ����Ƕ�������жϿ�����NVIC
  * @param  ��
  * @retval ��
  */
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  /* �����ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = RS232_USART_IRQ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	
	NVIC_InitStructure.NVIC_IRQChannel = RS232_USART1_IRQ;
	NVIC_Init(&NVIC_InitStructure);
  
}


 /**
  * @brief  RS232_USART GPIO ����,����ģʽ���á�115200 8-N-1 ���жϽ���ģʽ
  * @param  ��
  * @retval ��
  */
void Debug_USART_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
	//USART�Ǵ���2 USART1�Ǵ���1���¼Ӵ�����Ҫ����д��ʼ�����ݣ�����ȫ��
  RCC_AHB1PeriphClockCmd( RS232_USART_RX_GPIO_CLK|RS232_USART_TX_GPIO_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd( RS232_USART1_RX_GPIO_CLK|RS232_USART1_TX_GPIO_CLK, ENABLE);//2.1�����
	//��1���� RS232_USART1_RX_GPIO_CLK��//����1�����Ŷ����//����2�����Ŷ�����Ӧ����
  
	/* ʹ�� UART ʱ�� */
  RCC_APB1PeriphClockCmd(RS232_USART_CLK, ENABLE);
	
	RCC_APB2PeriphClockCmd(RS232_USART1_CLK, ENABLE);//2.2�����
  /* ���� PXx �� USARTx_Tx*/
  GPIO_PinAFConfig(RS232_USART_RX_GPIO_PORT,RS232_USART_RX_SOURCE, RS232_USART_RX_AF);

  /*  ���� PXx �� USARTx__Rx*/
  GPIO_PinAFConfig(RS232_USART_TX_GPIO_PORT,RS232_USART_TX_SOURCE,RS232_USART_TX_AF);
  /* ���� PXx �� USARTx_Tx*/
  GPIO_PinAFConfig(RS232_USART1_RX_GPIO_PORT,RS232_USART1_RX_SOURCE, RS232_USART1_RX_AF);//2.3�����

  /*  ���� PXx �� USARTx__Rx*/
  GPIO_PinAFConfig(RS232_USART1_TX_GPIO_PORT,RS232_USART1_TX_SOURCE,RS232_USART1_TX_AF);//2.4�����

  /* ����Tx����Ϊ���ù���  */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

  GPIO_InitStructure.GPIO_Pin = RS232_USART_TX_PIN  ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(RS232_USART_TX_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = RS232_USART1_TX_PIN  ;//2.5��������
	GPIO_Init(RS232_USART1_TX_GPIO_PORT, &GPIO_InitStructure);

  /* ����Rx����Ϊ���ù��� */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = RS232_USART_RX_PIN;
  GPIO_Init(RS232_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = RS232_USART1_RX_PIN;//2.6��������
	GPIO_Init(RS232_USART1_RX_GPIO_PORT, &GPIO_InitStructure);
	
  /* ���ô���RS232_USART2 ģʽ */
  USART_InitStructure.USART_BaudRate = RS232_USART_BAUDRATE;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No ;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(RS232_USART, &USART_InitStructure); 
	
	 /* ���ô���RS232_USART1 ģʽ */

  USART_Init(RS232_USART1, &USART_InitStructure); //2.7�����
	
	
	NVIC_Configuration();//��ģʽ��Ч��2.7�����������һ��
	/*���ô��ڽ����ж�*/
	USART_ITConfig(RS232_USART, USART_IT_RXNE, ENABLE);
	/*���ô��ڽ����ж�*/
	USART_ITConfig(RS232_USART1, USART_IT_RXNE, ENABLE);//2.8�����
  USART_Cmd(RS232_USART, ENABLE);
	USART_Cmd(RS232_USART1, ENABLE);//2.9�����
	
}



/*****************  ����һ���ַ� **********************/
static void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch )
{
	/* ����һ���ֽ����ݵ�USART1 */
	USART_SendData(pUSARTx,ch);
		
	/* �ȴ�������� */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}
/*****************  ָ�����ȵķ����ַ��� **********************/
void Usart_SendStr_length( USART_TypeDef * pUSARTx, uint8_t *str,uint32_t strlen )
{
	unsigned int k=0;
    do 
    {
        Usart_SendByte( pUSARTx, *(str + k) );
        k++;
    } while(k < strlen);
}

/*****************  �����ַ��� **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, uint8_t *str)
{
	unsigned int k=0;
    do 
    {
        Usart_SendByte( pUSARTx, *(str + k) );
        k++;
    } while(*(str + k)!='\0');
}




///�ض���c�⺯��printf������RS232_USART���ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ�����RS232_USART */
		USART_SendData(RS232_USART, (uint8_t) ch);
		
		/* �ȴ�������� */
		while (USART_GetFlagStatus(RS232_USART, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///�ض���c�⺯��scanf������RS232_USART����д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{
		/* �ȴ������������� */
		while (USART_GetFlagStatus(RS232_USART, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(RS232_USART);
}


extern OS_TCB	 AppTaskUsartTCB;
extern OS_TCB	 AppTaskUsartTCB1;
CPU_INT08U data[20] = {0};
void RS232_USART_IRQHandler(void)
{
	OS_ERR   err;
	static uint8_t num = 0;
	OSIntEnter();
	if(USART_GetITStatus( RS232_USART, USART_IT_RXNE ) != RESET)
	{		
	  		
		data[num] = USART_ReceiveData( RS232_USART );
		num++;
		if(num == 1)//֡ͷ���տ�ʼ
		{
			if(data[0]!=0x55 )//Ѱ��֡ͷ
			{
				num = 0;//���¿�ʼ����֡ͷ
			}
		}
		if(num == 2)//֡ͷ�������
		{
			if(data[1]!=0x66)//Ѱ��֡ͷ
			{
				num = 0;//���¿�ʼ����֡ͷ
			}
		}
		
		
		if(num >= 14)
		{
			num = 0;
			/* ����������Ϣ������ AppTaskUsart */
			OSTaskQPost ((OS_TCB      *)&AppTaskUsartTCB,      //Ŀ������Ŀ��ƿ�
									 (void        *)data,             //��Ϣ���ݵ��׵�ַ
									 (OS_MSG_SIZE  )14,                     //��Ϣ����
									 (OS_OPT       )OS_OPT_POST_FIFO,      //������������Ϣ���е���ڶ�
									 (OS_ERR      *)&err);                 //���ش�������
		}
		
	} 
	OSIntExit();
}
//���ڵ�4��������1���жϳ���
void RS232_USART111_IRQHandler(void)
{
	OS_ERR   err;
	static uint8_t num = 0;
	OSIntEnter();
	if(USART_GetITStatus( RS232_USART1, USART_IT_RXNE ) != RESET)
	{		
	  		
		data[num] = USART_ReceiveData( RS232_USART1 );
		num++;
		if(num == 1)//֡ͷ���տ�ʼ
		{
			if(data[0]!=0x55 )//Ѱ��֡ͷ
			{
				num = 0;//���¿�ʼ����֡ͷ
			}
		}
		if(num == 2)//֡ͷ�������
		{
			if(data[1]!=0x66)//Ѱ��֡ͷ
			{
				num = 0;//���¿�ʼ����֡ͷ
			}
		}
		
		
		if(num >= 10)
		{
			num = 0;
			/* ����������Ϣ������ AppTaskUsart */
			OSTaskQPost ((OS_TCB      *)&AppTaskUsartTCB1,      //Ŀ������Ŀ��ƿ�
									 (void        *)data,             //��Ϣ���ݵ��׵�ַ
									 (OS_MSG_SIZE  )10,                     //��Ϣ����
									 (OS_OPT       )OS_OPT_POST_FIFO,      //������������Ϣ���е���ڶ�
									 (OS_ERR      *)&err);                 //���ش�������
		}
	} 
	OSIntExit();
}
/*********************************************END OF FILE**********************/
