/**
  ******************************************************************************
  * @file    bsp_rs232_usart.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   使用串口2，重定向c库printf函数到usart端口，中断接收模式
  ******************************************************************************
  * @attention
  *
  * 实验平台:秉火  STM32 F429 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 
  
#include "./usart/bsp_rs232_usart.h"
#include <includes.h>

 /**
  * @brief  配置嵌套向量中断控制器NVIC
  * @param  无
  * @retval 无
  */
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  /* 配置中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = RS232_USART_IRQ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	
	NVIC_InitStructure.NVIC_IRQChannel = RS232_USART1_IRQ;
	NVIC_Init(&NVIC_InitStructure);
  
}


 /**
  * @brief  RS232_USART GPIO 配置,工作模式配置。115200 8-N-1 ，中断接收模式
  * @param  无
  * @retval 无
  */
void Debug_USART_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
	//USART是串口2 USART1是串口1，新加串口需要照着写初始化内容，以下全是
  RCC_AHB1PeriphClockCmd( RS232_USART_RX_GPIO_CLK|RS232_USART_TX_GPIO_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd( RS232_USART1_RX_GPIO_CLK|RS232_USART1_TX_GPIO_CLK, ENABLE);//2.1改这句
	//第1步将 RS232_USART1_RX_GPIO_CLK等//串口1的引脚定义和//串口2的引脚定义相应复制
  
	/* 使能 UART 时钟 */
  RCC_APB1PeriphClockCmd(RS232_USART_CLK, ENABLE);
	
	RCC_APB2PeriphClockCmd(RS232_USART1_CLK, ENABLE);//2.2改这句
  /* 连接 PXx 到 USARTx_Tx*/
  GPIO_PinAFConfig(RS232_USART_RX_GPIO_PORT,RS232_USART_RX_SOURCE, RS232_USART_RX_AF);

  /*  连接 PXx 到 USARTx__Rx*/
  GPIO_PinAFConfig(RS232_USART_TX_GPIO_PORT,RS232_USART_TX_SOURCE,RS232_USART_TX_AF);
  /* 连接 PXx 到 USARTx_Tx*/
  GPIO_PinAFConfig(RS232_USART1_RX_GPIO_PORT,RS232_USART1_RX_SOURCE, RS232_USART1_RX_AF);//2.3改这句

  /*  连接 PXx 到 USARTx__Rx*/
  GPIO_PinAFConfig(RS232_USART1_TX_GPIO_PORT,RS232_USART1_TX_SOURCE,RS232_USART1_TX_AF);//2.4改这句

  /* 配置Tx引脚为复用功能  */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

  GPIO_InitStructure.GPIO_Pin = RS232_USART_TX_PIN  ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(RS232_USART_TX_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = RS232_USART1_TX_PIN  ;//2.5改这两句
	GPIO_Init(RS232_USART1_TX_GPIO_PORT, &GPIO_InitStructure);

  /* 配置Rx引脚为复用功能 */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = RS232_USART_RX_PIN;
  GPIO_Init(RS232_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = RS232_USART1_RX_PIN;//2.6改这两句
	GPIO_Init(RS232_USART1_RX_GPIO_PORT, &GPIO_InitStructure);
	
  /* 配置串口RS232_USART2 模式 */
  USART_InitStructure.USART_BaudRate = RS232_USART_BAUDRATE;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No ;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(RS232_USART, &USART_InitStructure); 
	
	 /* 配置串口RS232_USART1 模式 */

  USART_Init(RS232_USART1, &USART_InitStructure); //2.7改这句
	
	
	NVIC_Configuration();//让模式生效，2.7改完后须有这一句
	/*配置串口接收中断*/
	USART_ITConfig(RS232_USART, USART_IT_RXNE, ENABLE);
	/*配置串口接收中断*/
	USART_ITConfig(RS232_USART1, USART_IT_RXNE, ENABLE);//2.8改这句
  USART_Cmd(RS232_USART, ENABLE);
	USART_Cmd(RS232_USART1, ENABLE);//2.9改这句
	
}



/*****************  发送一个字符 **********************/
static void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch )
{
	/* 发送一个字节数据到USART1 */
	USART_SendData(pUSARTx,ch);
		
	/* 等待发送完毕 */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}
/*****************  指定长度的发送字符串 **********************/
void Usart_SendStr_length( USART_TypeDef * pUSARTx, uint8_t *str,uint32_t strlen )
{
	unsigned int k=0;
    do 
    {
        Usart_SendByte( pUSARTx, *(str + k) );
        k++;
    } while(k < strlen);
}

/*****************  发送字符串 **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, uint8_t *str)
{
	unsigned int k=0;
    do 
    {
        Usart_SendByte( pUSARTx, *(str + k) );
        k++;
    } while(*(str + k)!='\0');
}




///重定向c库函数printf到串口RS232_USART，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到串口RS232_USART */
		USART_SendData(RS232_USART, (uint8_t) ch);
		
		/* 等待发送完毕 */
		while (USART_GetFlagStatus(RS232_USART, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///重定向c库函数scanf到串口RS232_USART，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
		/* 等待串口输入数据 */
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
		if(num == 1)//帧头接收开始
		{
			if(data[0]!=0x55 )//寻找帧头
			{
				num = 0;//重新开始接收帧头
			}
		}
		if(num == 2)//帧头接收完毕
		{
			if(data[1]!=0x66)//寻找帧头
			{
				num = 0;//重新开始接收帧头
			}
		}
		
		
		if(num >= 14)
		{
			num = 0;
			/* 发布任务消息到任务 AppTaskUsart */
			OSTaskQPost ((OS_TCB      *)&AppTaskUsartTCB,      //目标任务的控制块
									 (void        *)data,             //消息内容的首地址
									 (OS_MSG_SIZE  )14,                     //消息长度
									 (OS_OPT       )OS_OPT_POST_FIFO,      //发布到任务消息队列的入口端
									 (OS_ERR      *)&err);                 //返回错误类型
		}
		
	} 
	OSIntExit();
}
//串口第4步，串口1的中断程序
void RS232_USART111_IRQHandler(void)
{
	OS_ERR   err;
	static uint8_t num = 0;
	OSIntEnter();
	if(USART_GetITStatus( RS232_USART1, USART_IT_RXNE ) != RESET)
	{		
	  		
		data[num] = USART_ReceiveData( RS232_USART1 );
		num++;
		if(num == 1)//帧头接收开始
		{
			if(data[0]!=0x55 )//寻找帧头
			{
				num = 0;//重新开始接收帧头
			}
		}
		if(num == 2)//帧头接收完毕
		{
			if(data[1]!=0x66)//寻找帧头
			{
				num = 0;//重新开始接收帧头
			}
		}
		
		
		if(num >= 10)
		{
			num = 0;
			/* 发布任务消息到任务 AppTaskUsart */
			OSTaskQPost ((OS_TCB      *)&AppTaskUsartTCB1,      //目标任务的控制块
									 (void        *)data,             //消息内容的首地址
									 (OS_MSG_SIZE  )10,                     //消息长度
									 (OS_OPT       )OS_OPT_POST_FIFO,      //发布到任务消息队列的入口端
									 (OS_ERR      *)&err);                 //返回错误类型
		}
	} 
	OSIntExit();
}
/*********************************************END OF FILE**********************/
