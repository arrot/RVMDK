/**
  ******************************************************************************
  * @file    bsp_dac.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   adc1 应用bsp / DMA 模式
  ******************************************************************************
  * @attention
  *
  * 实验平台:秉火  STM32 F429 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 
  
#include "bsp_dac.h"

uint32_t DualSine12bit = 0;

DAC_InitTypeDef  SPEED_DAC_InitStructure;

/**
  * @brief  使能DAC的时钟，初始化GPIO
  * @param  无
  * @retval 无
  */
static void SPEED_DAC_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
//经测试SPEED_DAC_InitStructure必须为全局变量或静态变量
//	SPEED_DAC_InitTypeDef  SPEED_DAC_InitStructure;

  /* 使能GPIOA时钟 */
  RCC_AHB1PeriphClockCmd(SPEED_DAC_CH1_GPIO_CLK, ENABLE);	
	
	/* 使能DAC时钟 */	
  RCC_APB1PeriphClockCmd(SPEED_DAC_CLK, ENABLE);
  
  /* DAC的GPIO配置，模拟输入 */
  GPIO_InitStructure.GPIO_Pin =  SPEED_DAC_CH1_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(SPEED_DAC_CH1_GPIO_PORT, &GPIO_InitStructure);

		  /* 配置DAC 通道1 */
  SPEED_DAC_InitStructure.DAC_Trigger = SPEED_DAC_TRIGGER;						//使用TIM2作为触发源
  SPEED_DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;	//不使用波形发生器
  SPEED_DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;	//不使用DAC输出缓冲
  DAC_Init(SPEED_DAC_CH1_CHANNEL, &SPEED_DAC_InitStructure);

	/* 配置DAC 通道1、2 */
  DAC_Cmd(SPEED_DAC_CH1_CHANNEL, ENABLE);
	
	/* 使能 DAC的DMA请求 */
  DAC_DMACmd(SPEED_DAC_CH1_CHANNEL, ENABLE);

}


/**
  * @brief  配置TIM
  * @param  无
  * @retval 无
  */
static void SPEED_DAC_TIM_Config(void)
{
	
	TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
	
	/* 使能TIM2时钟，TIM2CLK 为180M */
  RCC_APB1PeriphClockCmd(SPEED_DAC_TIM_CLK, ENABLE);
	
  /* TIM2基本定时器配置 */
 // TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 
  TIM_TimeBaseStructure.TIM_Period = 19;       									//定时周期 20  
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0;       							//预分频，不分频 180M / (0+1) = 180M
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    						//时钟分频系数
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  	//向上计数模式
  TIM_TimeBaseInit(SPEED_DAC_TIM, &TIM_TimeBaseStructure);

  /* 配置TIM2触发源 */
  TIM_SelectOutputTrigger(SPEED_DAC_TIM, TIM_TRGOSource_Update);

	/* 使能TIM2 */
  TIM_Cmd(SPEED_DAC_TIM, ENABLE);

}

/**
  * @brief  配置DMA
  * @param  无
  * @retval 无
  */
static void SPEED_DAC_DMA_Config(void)
{	
	DMA_InitTypeDef  DMA_InitStructure;

	/* DAC1使用DMA1 通道7 数据流5时钟 */
	RCC_AHB1PeriphClockCmd(SPEED_DAC_DMA_CLK, ENABLE);
	
	/* 配置DMA2 */
  DMA_InitStructure.DMA_Channel = SPEED_DAC_CHANNEL;  
  DMA_InitStructure.DMA_PeripheralBaseAddr = SPEED_DAC_DHR12RD_Address;					//外设数据地址
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&DualSine12bit ;			//内存数据地址 DualSine12bit
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;									//数据传输方向内存至外设
  DMA_InitStructure.DMA_BufferSize = 1;																	//缓存大小为32字节	
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;				//外设数据地址固定	
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;									//内存数据地址不增加
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;	//外设数据以字为单位
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;					//内存数据以字为单位	
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;													//循环模式
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;											//高DMA通道优先级
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

  DMA_Init(SPEED_DAC_DMA_STREAM, &DMA_InitStructure);
  
  /* 使能 DMA_Stream */
  DMA_Cmd(SPEED_DAC_DMA_STREAM, ENABLE);
  

}




/**
  * @brief  DAC初始化函数
  * @param  无
  * @retval 无
  */
void SPEED_DAC_Mode_Init(void)
{
	SPEED_DAC_Config();
	SPEED_DAC_TIM_Config();
	SPEED_DAC_DMA_Config();
}









