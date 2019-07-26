/**
  ******************************************************************************
  * @file    bsp_dac.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   adc1 Ӧ��bsp / DMA ģʽ
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:����  STM32 F429 ������  
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 
  
#include "bsp_dac.h"

uint32_t DualSine12bit = 0;

DAC_InitTypeDef  SPEED_DAC_InitStructure;

/**
  * @brief  ʹ��DAC��ʱ�ӣ���ʼ��GPIO
  * @param  ��
  * @retval ��
  */
static void SPEED_DAC_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
//������SPEED_DAC_InitStructure����Ϊȫ�ֱ�����̬����
//	SPEED_DAC_InitTypeDef  SPEED_DAC_InitStructure;

  /* ʹ��GPIOAʱ�� */
  RCC_AHB1PeriphClockCmd(SPEED_DAC_CH1_GPIO_CLK, ENABLE);	
	
	/* ʹ��DACʱ�� */	
  RCC_APB1PeriphClockCmd(SPEED_DAC_CLK, ENABLE);
  
  /* DAC��GPIO���ã�ģ������ */
  GPIO_InitStructure.GPIO_Pin =  SPEED_DAC_CH1_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(SPEED_DAC_CH1_GPIO_PORT, &GPIO_InitStructure);

		  /* ����DAC ͨ��1 */
  SPEED_DAC_InitStructure.DAC_Trigger = SPEED_DAC_TRIGGER;						//ʹ��TIM2��Ϊ����Դ
  SPEED_DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;	//��ʹ�ò��η�����
  SPEED_DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;	//��ʹ��DAC�������
  DAC_Init(SPEED_DAC_CH1_CHANNEL, &SPEED_DAC_InitStructure);

	/* ����DAC ͨ��1��2 */
  DAC_Cmd(SPEED_DAC_CH1_CHANNEL, ENABLE);
	
	/* ʹ�� DAC��DMA���� */
  DAC_DMACmd(SPEED_DAC_CH1_CHANNEL, ENABLE);

}


/**
  * @brief  ����TIM
  * @param  ��
  * @retval ��
  */
static void SPEED_DAC_TIM_Config(void)
{
	
	TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
	
	/* ʹ��TIM2ʱ�ӣ�TIM2CLK Ϊ180M */
  RCC_APB1PeriphClockCmd(SPEED_DAC_TIM_CLK, ENABLE);
	
  /* TIM2������ʱ������ */
 // TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 
  TIM_TimeBaseStructure.TIM_Period = 19;       									//��ʱ���� 20  
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0;       							//Ԥ��Ƶ������Ƶ 180M / (0+1) = 180M
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    						//ʱ�ӷ�Ƶϵ��
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  	//���ϼ���ģʽ
  TIM_TimeBaseInit(SPEED_DAC_TIM, &TIM_TimeBaseStructure);

  /* ����TIM2����Դ */
  TIM_SelectOutputTrigger(SPEED_DAC_TIM, TIM_TRGOSource_Update);

	/* ʹ��TIM2 */
  TIM_Cmd(SPEED_DAC_TIM, ENABLE);

}

/**
  * @brief  ����DMA
  * @param  ��
  * @retval ��
  */
static void SPEED_DAC_DMA_Config(void)
{	
	DMA_InitTypeDef  DMA_InitStructure;

	/* DAC1ʹ��DMA1 ͨ��7 ������5ʱ�� */
	RCC_AHB1PeriphClockCmd(SPEED_DAC_DMA_CLK, ENABLE);
	
	/* ����DMA2 */
  DMA_InitStructure.DMA_Channel = SPEED_DAC_CHANNEL;  
  DMA_InitStructure.DMA_PeripheralBaseAddr = SPEED_DAC_DHR12RD_Address;					//�������ݵ�ַ
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&DualSine12bit ;			//�ڴ����ݵ�ַ DualSine12bit
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;									//���ݴ��䷽���ڴ�������
  DMA_InitStructure.DMA_BufferSize = 1;																	//�����СΪ32�ֽ�	
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;				//�������ݵ�ַ�̶�	
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;									//�ڴ����ݵ�ַ������
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;	//������������Ϊ��λ
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;					//�ڴ���������Ϊ��λ	
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;													//ѭ��ģʽ
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;											//��DMAͨ�����ȼ�
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

  DMA_Init(SPEED_DAC_DMA_STREAM, &DMA_InitStructure);
  
  /* ʹ�� DMA_Stream */
  DMA_Cmd(SPEED_DAC_DMA_STREAM, ENABLE);
  

}




/**
  * @brief  DAC��ʼ������
  * @param  ��
  * @retval ��
  */
void SPEED_DAC_Mode_Init(void)
{
	SPEED_DAC_Config();
	SPEED_DAC_TIM_Config();
	SPEED_DAC_DMA_Config();
}









