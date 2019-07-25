#ifndef __GENERAL_TIM_H
#define	__GENERAL_TIM_H

#include "stm32f4xx.h"

//控制电机方向的引脚定义
/*******************************************************/
//DIR_A
#define DIR_A_PIN                  GPIO_Pin_2                 
#define DIR_A_GPIO_PORT            GPIOE                     
#define DIR_A_GPIO_CLK             RCC_AHB1Periph_GPIOE

//DIR_B
#define DIR_B_PIN                  GPIO_Pin_3                
#define DIR_B_GPIO_PORT            GPIOE                     
#define DIR_B_GPIO_CLK             RCC_AHB1Periph_GPIOE

//DIR_C
#define DIR_C_PIN                  GPIO_Pin_4                 
#define DIR_C_GPIO_PORT            GPIOE                      
#define DIR_C_GPIO_CLK             RCC_AHB1Periph_GPIOE

//DIR_D
#define DIR_D_PIN                  GPIO_Pin_5                 
#define DIR_D_GPIO_PORT            GPIOE                       
#define DIR_D_GPIO_CLK             RCC_AHB1Periph_GPIOE
/************************************************************/
/* 直接操作寄存器的方法控制IO */
#define	digitalHi(p,i)			  {p->BSRRL=i;}			  //设置为高电平		
#define digitalLo(p,i)			  {p->BSRRH=i;}				//输出低电平
#define digitalToggle(p,i)		{p->ODR ^=i;}			  //输出反转状态

//读取方向引脚的电平
#define DIR_A                  GPIO_ReadInputDataBit(DIR_A_GPIO_PORT,DIR_A_PIN)
#define DIR_B                  GPIO_ReadInputDataBit(DIR_B_GPIO_PORT,DIR_B_PIN)
#define DIR_C                  GPIO_ReadInputDataBit(DIR_C_GPIO_PORT,DIR_C_PIN)
#define DIR_D                  GPIO_ReadInputDataBit(DIR_D_GPIO_PORT,DIR_D_PIN)

#define DIR_P                  1
#define DIR_N                  0

#define            BRAKE_SPEED_MIN                10
#define            BRAKE_SPEED_STEP                5
#define            BRAKE_SPEED_UP                  50
#define            BRAKE_SPEED_MAX                 100
#define            BRAKE_TIME_STEP                 1//状态检查时间



#define            SPEED_MIN                10
#define            SPEED_STEP                5
#define            SPEED_MAX                 100
#define            TIME_STEP                 1//状态检查时间

/* 定义控制电机A方向的IO宏 */
#define DIR_A_TOGGLE		digitalToggle(DIR_A_GPIO_PORT,DIR_A_PIN)
#define DIR_A_P		    digitalHi(DIR_A_GPIO_PORT,DIR_A_PIN)
#define DIR_A_N			  digitalLo(DIR_A_GPIO_PORT,DIR_A_PIN)
/* 定义控制电机B方向的IO宏 */
#define DIR_B_TOGGLE		digitalToggle(DIR_B_GPIO_PORT,DIR_B_PIN)
#define DIR_B_P		    digitalHi(DIR_B_GPIO_PORT,DIR_B_PIN)
#define DIR_B_N			  digitalLo(DIR_B_GPIO_PORT,DIR_B_PIN)
/* 定义控制电机C方向的IO宏 */
#define DIR_C_TOGGLE		digitalToggle(DIR_C_GPIO_PORT,DIR_C_PIN)
#define DIR_C_P		    digitalHi(DIR_C_GPIO_PORT,DIR_C_PIN)
#define DIR_C_N			  digitalLo(DIR_C_GPIO_PORT,DIR_C_PIN)
/* 定义控制电机A方向的IO宏 */
#define DIR_D_TOGGLE		digitalToggle(DIR_D_GPIO_PORT,DIR_D_PIN)
#define DIR_D_P		    digitalHi(DIR_D_GPIO_PORT,DIR_D_PIN)
#define DIR_D_N			  digitalLo(DIR_D_GPIO_PORT,DIR_D_PIN)
/******************  PWM引脚设置 ********************/

/******************  控制电机1 **********************/
#define GENERAL_OCPWM_1_PIN             GPIO_Pin_15              
#define GENERAL_OCPWM_1_GPIO_PORT       GPIOA                      
#define GENERAL_OCPWM_1_GPIO_CLK        RCC_AHB1Periph_GPIOA
#define GENERAL_OCPWM_1_PINSOURCE				GPIO_PinSource15
#define GENERAL_OCPWM_1_AF							GPIO_AF_TIM2
/******************  控制电机2 **********************/
#define GENERAL_OCPWM_2_PIN             GPIO_Pin_1              
#define GENERAL_OCPWM_2_GPIO_PORT       GPIOA                      
#define GENERAL_OCPWM_2_GPIO_CLK        RCC_AHB1Periph_GPIOA
#define GENERAL_OCPWM_2_PINSOURCE				GPIO_PinSource1
#define GENERAL_OCPWM_2_AF							GPIO_AF_TIM2
/******************  控制电机3 **********************/
#define GENERAL_OCPWM_3_PIN             GPIO_Pin_10              
#define GENERAL_OCPWM_3_GPIO_PORT       GPIOB                      
#define GENERAL_OCPWM_3_GPIO_CLK        RCC_AHB1Periph_GPIOB
#define GENERAL_OCPWM_3_PINSOURCE				GPIO_PinSource10
#define GENERAL_OCPWM_3_AF							GPIO_AF_TIM2
/******************  控制电机4 **********************/
#define GENERAL_OCPWM_4_PIN             GPIO_Pin_11              
#define GENERAL_OCPWM_4_GPIO_PORT       GPIOB                     
#define GENERAL_OCPWM_4_GPIO_CLK        RCC_AHB1Periph_GPIOB
#define GENERAL_OCPWM_4_PINSOURCE				GPIO_PinSource11
#define GENERAL_OCPWM_4_AF							GPIO_AF_TIM2
/******************  PWM引脚设置完毕 ********************/
#define GENERAL_TIM           		    TIM2
#define GENERAL_TIM_CLK       		    RCC_APB1Periph_TIM2

#define GENERAL_TIM_IRQn					    TIM2_IRQn
#define GENERAL_TIM_IRQHandler        TIM2_IRQHandler


void TIMx_Configuration(void);
void BrakeSpeedSet(int8_t Speed);
void TurnMotorSpeedSet(int8_t Speed);
#endif /* __GENERAL_TIM_H */

