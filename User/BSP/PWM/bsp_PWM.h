#ifndef __GENERAL_TIM_H
#define	__GENERAL_TIM_H

#include "stm32f4xx.h"
/*
1�ŵ����Ӧ�ƶ����
2�ŵ����Ӧת����
*/
//���Ƶ����������Ŷ���
/*******************************************************/
//ת��������������ŵĺ궨��
#define DIR_TURN_PIN                  GPIO_Pin_2                 
#define DIR_TURN_GPIO_PORT            GPIOD                     
#define DIR_TURN_GPIO_CLK             RCC_AHB1Periph_GPIOD

//�ƶ��������������ŵĺ궨��
#define DIR_BRK_PIN                  GPIO_Pin_8               
#define DIR_BRK_GPIO_PORT            GPIOC                    
#define DIR_BRK_GPIO_CLK             RCC_AHB1Periph_GPIOC

/************************************************************/
/* ֱ�Ӳ����Ĵ����ķ�������IO */
#define	digitalHi(p,i)			  {p->BSRRL=i;}			  //����Ϊ�ߵ�ƽ		
#define digitalLo(p,i)			  {p->BSRRH=i;}				//����͵�ƽ
#define digitalToggle(p,i)		{p->ODR ^=i;}			  //�����ת״̬

#define DIR_P                  1
#define DIR_N                  0

/* �����ƶ���������IO�� */
#define DIR_BRK_TOGGLE		digitalToggle(DIR_BRK_GPIO_PORT,DIR_BRK_PIN)
#define DIR_BRK_P		      digitalHi    (DIR_BRK_GPIO_PORT,DIR_BRK_PIN)
#define DIR_BRK_N			    digitalLo    (DIR_BRK_GPIO_PORT,DIR_BRK_PIN)
/* ����ת���������IO�� */
#define DIR_TURN_TOGGLE		digitalToggle(DIR_TURN_GPIO_PORT,DIR_TURN_PIN)
#define DIR_TURN_P		    digitalHi    (DIR_TURN_GPIO_PORT,DIR_TURN_PIN)
#define DIR_TURN_N			  digitalLo    (DIR_TURN_GPIO_PORT,DIR_TURN_PIN)

/******************  �����ƶ���� **********************/
#define BRK_OCPWM_PIN             GPIO_Pin_1              
#define BRK_OCPWM_GPIO_PORT       GPIOA                      
#define BRK_OCPWM_GPIO_CLK        RCC_AHB1Periph_GPIOA
#define BRK_OCPWM_PINSOURCE				GPIO_PinSource1
#define BRK_OCPWM_AF							GPIO_AF_TIM2
/******************  ����ת���� **********************/
#define TURN_OCPWM_PIN             GPIO_Pin_2              
#define TURN_OCPWM_GPIO_PORT       GPIOA                      
#define TURN_OCPWM_GPIO_CLK        RCC_AHB1Periph_GPIOA
#define TURN_OCPWM_PINSOURCE			 GPIO_PinSource2
#define TURN_OCPWM_AF							 GPIO_AF_TIM2

/******************  PWM����������� ********************/
#define PWM_TIM           		    TIM2
#define PWM_TIM_CLK       		    RCC_APB1Periph_TIM2

#define PWM_TIM_IRQn					    TIM2_IRQn
#define PWM_TIM_IRQHandler        TIM2_IRQHandler


void Motor_Configuration(void);
uint16_t TurnMotorSpeedSet(int8_t Speed);
uint16_t BRK_MotorSpeedSet(int8_t Speed);
#endif /* __GENERAL_TIM_H */

