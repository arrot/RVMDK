#ifndef __BSP_ADC_H
#define	__BSP_ADC_H

#include "stm32f4xx.h"

// ADC GPIO 宏定义
#define BRK_POS_ADC_GPIO_PORT    GPIOA
#define BRK_POS_ADC_GPIO_PIN     GPIO_Pin_3
#define BRK_POS_ADC_GPIO_CLK     RCC_AHB1Periph_GPIOA

// ADC 序号宏定义
#define BRK_POS_ADC              ADC1
#define BRK_POS_ADC_CLK          RCC_APB2Periph_ADC1
#define BRK_POS_ADC_CHANNEL      ADC_Channel_3

// ADC DR寄存器宏定义，ADC转换后的数字值则存放在这里
#define BRK_POS_ADC_DR_ADDR    ((u32)ADC1+0x4c)

// ADC DMA 通道宏定义，这里我们使用DMA传输
#define BRK_POS_ADC_DMA_CLK      RCC_AHB1Periph_DMA2
#define BRK_POS_ADC_DMA_CHANNEL  DMA_Channel_0
#define BRK_POS_ADC_DMA_STREAM   DMA2_Stream0



void BRK_POS_Init(void);

#endif /* __BSP_ADC_H */



