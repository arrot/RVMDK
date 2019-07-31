#ifndef __GENERAL_TIM_H
#define	__GENERAL_TIM_H

#include "stm32f4xx.h"
/*
制动电机 A号
转向电机 B号
*/
//控制电机方向的引脚定义
/*******************************************************/
//转向电机方向控制引脚的宏定义
#define DIR1_TURN_PIN                  GPIO_Pin_2                 
#define DIR1_TURN_GPIO_PORT            GPIOD                     
#define DIR1_TURN_GPIO_CLK             RCC_AHB1Periph_GPIOD

#define DIR2_TURN_PIN                  GPIO_Pin_12                 
#define DIR2_TURN_GPIO_PORT            GPIOC                     
#define DIR2_TURN_GPIO_CLK             RCC_AHB1Periph_GPIOC

//制动电机方向控制引脚的宏定义
#define DIR1_BRK_PIN                  GPIO_Pin_8               
#define DIR1_BRK_GPIO_PORT            GPIOC                    
#define DIR1_BRK_GPIO_CLK             RCC_AHB1Periph_GPIOC

#define DIR2_BRK_PIN                  GPIO_Pin_9               
#define DIR2_BRK_GPIO_PORT            GPIOC                    
#define DIR2_BRK_GPIO_CLK             RCC_AHB1Periph_GPIOC
/************************************************************/
/* 直接操作寄存器的方法控制IO */
#define	digitalHi(p,i)			  {p->BSRRL=i;}			  //设置为高电平		
#define digitalLo(p,i)			  {p->BSRRH=i;}				//输出低电平
#define digitalToggle(p,i)		{p->ODR ^=i;}			  //输出反转状态

#define DIR_P                  1
#define DIR_N                  0

/* 定义制动电机方向的IO宏 */
#define DIR_BRK_TOGGLE		digitalToggle(DIR1_BRK_GPIO_PORT,DIR1_BRK_PIN);digitalToggle(DIR2_BRK_GPIO_PORT,DIR2_BRK_PIN);
#define DIR_BRK_P		      digitalHi    (DIR1_BRK_GPIO_PORT,DIR1_BRK_PIN);digitalLo    (DIR2_BRK_GPIO_PORT,DIR2_BRK_PIN)
#define DIR_BRK_N			    digitalLo    (DIR1_BRK_GPIO_PORT,DIR1_BRK_PIN);digitalHi    (DIR2_BRK_GPIO_PORT,DIR2_BRK_PIN)
/* 定义转向电机方向的IO宏 */
#define DIR_TURN_TOGGLE		digitalToggle(DIR1_TURN_GPIO_PORT,DIR1_TURN_PIN);digitalToggle(DIR2_TURN_GPIO_PORT,DIR2_TURN_PIN)
#define DIR_TURN_P		    digitalHi    (DIR1_TURN_GPIO_PORT,DIR1_TURN_PIN);digitalLo    (DIR2_TURN_GPIO_PORT,DIR2_TURN_PIN)
#define DIR_TURN_N			  digitalLo    (DIR1_TURN_GPIO_PORT,DIR1_TURN_PIN);digitalHi    (DIR2_TURN_GPIO_PORT,DIR2_TURN_PIN)

/******************  控制制动电机 **********************/
#define BRK_OCPWM_PIN             GPIO_Pin_1              
#define BRK_OCPWM_GPIO_PORT       GPIOA                      
#define BRK_OCPWM_GPIO_CLK        RCC_AHB1Periph_GPIOA
#define BRK_OCPWM_PINSOURCE				GPIO_PinSource1
#define BRK_OCPWM_AF							GPIO_AF_TIM2
/******************  控制转向电机 **********************/
#define TURN_OCPWM_PIN             GPIO_Pin_2              
#define TURN_OCPWM_GPIO_PORT       GPIOA                      
#define TURN_OCPWM_GPIO_CLK        RCC_AHB1Periph_GPIOA
#define TURN_OCPWM_PINSOURCE			 GPIO_PinSource2
#define TURN_OCPWM_AF							 GPIO_AF_TIM2

/******************  PWM引脚设置完毕 ********************/
#define PWM_TIM           		    TIM2
#define PWM_TIM_CLK       		    RCC_APB1Periph_TIM2

#define PWM_TIM_IRQn					    TIM2_IRQn
#define PWM_TIM_IRQHandler        TIM2_IRQHandler

#define CYCLE                     500
#define TURN_THRESHOLD_POS        5   //转向位置精度
#define BRK_THRESHOLD_POS         5   //制动位置精度


void Motor_Configuration(void);
uint16_t TurnMotorSpeedSet(int8_t Speed);
uint16_t BRK_MotorSpeedSet(int8_t Speed);
uint8_t BRKMotorPositionControl(int8_t Position,int32_t PositionSet);
uint8_t TurnMotorPositionControl(int32_t Position,int32_t PositionSet);
#endif /* __GENERAL_TIM_H */

