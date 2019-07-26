#ifndef __SPEED_DAC_H
#define	__SPEED_DAC_H


#include "stm32f4xx.h"

#define SPEED_DAC_DHR12RD_Address     (uint32_t)(DAC_BASE+0x20)   //DAC通道数据输出寄存器地址  0x40007420//

#define SPEED_DAC_CLK                     RCC_APB1Periph_DAC
#define SPEED_DAC_TIM                     TIM4
#define SPEED_DAC_TIM_CLK                 RCC_APB1Periph_TIM4
#define SPEED_DAC_TRIGGER                 DAC_Trigger_T4_TRGO

#define SPEED_DAC_DMA_CLK                 RCC_AHB1Periph_DMA1
#define SPEED_DAC_CHANNEL                 DMA_Channel_7
#define SPEED_DAC_DMA_STREAM              DMA1_Stream5

#define SPEED_DAC_CH1_GPIO_CLK            RCC_AHB1Periph_GPIOA
#define SPEED_DAC_CH1_GPIO_PORT           GPIOA
#define SPEED_DAC_CH1_GPIO_PIN            GPIO_Pin_4
#define SPEED_DAC_CH1_CHANNEL             DAC_Channel_1

void SPEED_DAC_Mode_Init(void);


#endif /* __SPEED_DAC_H */

