#ifndef __GENERAL_TIM_H
#define	__GENERAL_TIM_H

#include "stm32f4xx.h"

//���Ƶ����������Ŷ���
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
/* ֱ�Ӳ����Ĵ����ķ�������IO */
#define	digitalHi(p,i)			  {p->BSRRL=i;}			  //����Ϊ�ߵ�ƽ		
#define digitalLo(p,i)			  {p->BSRRH=i;}				//����͵�ƽ
#define digitalToggle(p,i)		{p->ODR ^=i;}			  //�����ת״̬

//��ȡ�������ŵĵ�ƽ
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
#define            BRAKE_TIME_STEP                 1//״̬���ʱ��



#define            SPEED_MIN                10
#define            SPEED_STEP                5
#define            SPEED_MAX                 100
#define            TIME_STEP                 1//״̬���ʱ��

/* ������Ƶ��A�����IO�� */
#define DIR_A_TOGGLE		digitalToggle(DIR_A_GPIO_PORT,DIR_A_PIN)
#define DIR_A_P		    digitalHi(DIR_A_GPIO_PORT,DIR_A_PIN)
#define DIR_A_N			  digitalLo(DIR_A_GPIO_PORT,DIR_A_PIN)
/* ������Ƶ��B�����IO�� */
#define DIR_B_TOGGLE		digitalToggle(DIR_B_GPIO_PORT,DIR_B_PIN)
#define DIR_B_P		    digitalHi(DIR_B_GPIO_PORT,DIR_B_PIN)
#define DIR_B_N			  digitalLo(DIR_B_GPIO_PORT,DIR_B_PIN)
/* ������Ƶ��C�����IO�� */
#define DIR_C_TOGGLE		digitalToggle(DIR_C_GPIO_PORT,DIR_C_PIN)
#define DIR_C_P		    digitalHi(DIR_C_GPIO_PORT,DIR_C_PIN)
#define DIR_C_N			  digitalLo(DIR_C_GPIO_PORT,DIR_C_PIN)
/* ������Ƶ��A�����IO�� */
#define DIR_D_TOGGLE		digitalToggle(DIR_D_GPIO_PORT,DIR_D_PIN)
#define DIR_D_P		    digitalHi(DIR_D_GPIO_PORT,DIR_D_PIN)
#define DIR_D_N			  digitalLo(DIR_D_GPIO_PORT,DIR_D_PIN)
/******************  PWM�������� ********************/

/******************  ���Ƶ��1 **********************/
#define GENERAL_OCPWM_1_PIN             GPIO_Pin_15              
#define GENERAL_OCPWM_1_GPIO_PORT       GPIOA                      
#define GENERAL_OCPWM_1_GPIO_CLK        RCC_AHB1Periph_GPIOA
#define GENERAL_OCPWM_1_PINSOURCE				GPIO_PinSource15
#define GENERAL_OCPWM_1_AF							GPIO_AF_TIM2
/******************  ���Ƶ��2 **********************/
#define GENERAL_OCPWM_2_PIN             GPIO_Pin_1              
#define GENERAL_OCPWM_2_GPIO_PORT       GPIOA                      
#define GENERAL_OCPWM_2_GPIO_CLK        RCC_AHB1Periph_GPIOA
#define GENERAL_OCPWM_2_PINSOURCE				GPIO_PinSource1
#define GENERAL_OCPWM_2_AF							GPIO_AF_TIM2
/******************  ���Ƶ��3 **********************/
#define GENERAL_OCPWM_3_PIN             GPIO_Pin_10              
#define GENERAL_OCPWM_3_GPIO_PORT       GPIOB                      
#define GENERAL_OCPWM_3_GPIO_CLK        RCC_AHB1Periph_GPIOB
#define GENERAL_OCPWM_3_PINSOURCE				GPIO_PinSource10
#define GENERAL_OCPWM_3_AF							GPIO_AF_TIM2
/******************  ���Ƶ��4 **********************/
#define GENERAL_OCPWM_4_PIN             GPIO_Pin_11              
#define GENERAL_OCPWM_4_GPIO_PORT       GPIOB                     
#define GENERAL_OCPWM_4_GPIO_CLK        RCC_AHB1Periph_GPIOB
#define GENERAL_OCPWM_4_PINSOURCE				GPIO_PinSource11
#define GENERAL_OCPWM_4_AF							GPIO_AF_TIM2
/******************  PWM����������� ********************/
#define GENERAL_TIM           		    TIM2
#define GENERAL_TIM_CLK       		    RCC_APB1Periph_TIM2

#define GENERAL_TIM_IRQn					    TIM2_IRQn
#define GENERAL_TIM_IRQHandler        TIM2_IRQHandler


void TIMx_Configuration(void);
void BrakeSpeedSet(int8_t Speed);
void TurnMotorSpeedSet(int8_t Speed);
#endif /* __GENERAL_TIM_H */

