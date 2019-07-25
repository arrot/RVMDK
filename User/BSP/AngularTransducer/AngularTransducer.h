#ifndef __ANGULAR_TRANSDUCER__
#define __ANGULAR_TRANSDUCER__

#include "stm32f4xx.h"
//引脚定义
/*******************************************************/
//12V电源使能引脚
#define ANGULAR_TRANSDUCER_PIN                  GPIO_Pin_7                 
#define ANGULAR_TRANSDUCER_GPIO_PORT            GPIOG                     
#define ANGULAR_TRANSDUCER_GPIO_CLK             RCC_AHB1Periph_GPIOG
/* 直接操作寄存器的方法控制IO */
#define	digitalHi(p,i)			{p->BSRRL=i;}			  //设置为高电平		
#define digitalLo(p,i)			{p->BSRRH=i;}				//输出低电平
#define digitalToggle(p,i)		{p->ODR ^=i;}			//输出反转状态


/* 定义控制IO的宏 */
#define ANGULAR_TRANSDUCER_TOGGLE		digitalToggle(ANGULAR_TRANSDUCER_GPIO_PORT,ANGULAR_TRANSDUCER_PIN)
#define ANGULAR_TRANSDUCER_DISABLE  digitalLo(ANGULAR_TRANSDUCER_GPIO_PORT,ANGULAR_TRANSDUCER_PIN)
#define ANGULAR_TRANSDUCER_ENABLE		digitalHi(ANGULAR_TRANSDUCER_GPIO_PORT,ANGULAR_TRANSDUCER_PIN)



//函数声明
void ANGULAR_TRANSDUCER_GPIO_Config(void);


/* 高级控制定时器PWM输入捕获 */
/* PWM输入捕获引脚 */
#define ADVANCE_ICPWM_A1_PIN             GPIO_Pin_9            
#define ADVANCE_ICPWM_A1_GPIO_PORT       GPIOE                      
#define ADVANCE_ICPWM_A1_GPIO_CLK        RCC_AHB1Periph_GPIOE
#define ADVANCE_ICPWM_A1_PINSOURCE			GPIO_PinSource9

#define TIM3_ICPWM_A2_PIN             GPIO_Pin_6            
#define TIM3_ICPWM_A2_GPIO_PORT       GPIOC                      
#define TIM3_ICPWM_A2_GPIO_CLK        RCC_AHB1Periph_GPIOC
#define TIM3_ICPWM_A2_PINSOURCE				GPIO_PinSource6


#define ADVANCE_ICPWM_AF							GPIO_AF_TIM1
#define ADVANCE_IC1PWM_CHANNEL        TIM_Channel_1
#define ADVANCE_IC2PWM_CHANNEL        TIM_Channel_2
#define ADVANCE_IC3PWM_CHANNEL        TIM_Channel_3
#define ADVANCE_IC4PWM_CHANNEL        TIM_Channel_4

#define TIM3_ICPWM_AF							GPIO_AF_TIM3
#define TIM3_IC1PWM_CHANNEL        TIM_Channel_1
#define TIM3_IC2PWM_CHANNEL        TIM_Channel_2
#define TIM3_IC3PWM_CHANNEL        TIM_Channel_3
#define TIM3_IC4PWM_CHANNEL        TIM_Channel_4

/* 高级控制定时器 */
#define ADVANCE_TIM           		    TIM1
#define ADVANCE_TIM_CLK       		    RCC_APB2Periph_TIM1

/* 通用控制定时器 */
#define TIM3_TIM           		    TIM3
#define TIM3_TIM_CLK       		    RCC_APB1Periph_TIM3

/* 高级定时器 捕获/比较中断 */
#define ADVANCE_TIM_IRQn					    TIM1_CC_IRQn
#define ADVANCE_TIM_IRQHandler        TIM1_CC_IRQHandler
/* 通用定时器 捕获/比较中断 */
#define TIM3_TIM_IRQn					    TIM3_IRQn
#define TIM3_TIM_IRQHandler        TIM3_IRQHandler


void TIM1_3_Configuration(void);
int32_t AngleTransducer(uint16_t Duty);
extern __IO uint16_t DutyCycle_A1,DutyCycle_A2;
extern __IO uint32_t IC2Value_1, IC2Value_3;
extern __IO int32_t Angle_set,Angle;
#endif //__ANGULAR_TRANSDUCER__
