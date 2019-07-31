#ifndef __GENERAL_TIM_H
#define	__GENERAL_TIM_H

#include "stm32f4xx.h"
/*
�ƶ���� A��
ת���� B��
*/
//���Ƶ����������Ŷ���
/*******************************************************/
//ת��������������ŵĺ궨��
#define DIR1_TURN_PIN                  GPIO_Pin_2                 
#define DIR1_TURN_GPIO_PORT            GPIOD                     
#define DIR1_TURN_GPIO_CLK             RCC_AHB1Periph_GPIOD

#define DIR2_TURN_PIN                  GPIO_Pin_12                 
#define DIR2_TURN_GPIO_PORT            GPIOC                     
#define DIR2_TURN_GPIO_CLK             RCC_AHB1Periph_GPIOC

//�ƶ��������������ŵĺ궨��
#define DIR1_BRK_PIN                  GPIO_Pin_8               
#define DIR1_BRK_GPIO_PORT            GPIOC                    
#define DIR1_BRK_GPIO_CLK             RCC_AHB1Periph_GPIOC

#define DIR2_BRK_PIN                  GPIO_Pin_9               
#define DIR2_BRK_GPIO_PORT            GPIOC                    
#define DIR2_BRK_GPIO_CLK             RCC_AHB1Periph_GPIOC
/************************************************************/
/* ֱ�Ӳ����Ĵ����ķ�������IO */
#define	digitalHi(p,i)			  {p->BSRRL=i;}			  //����Ϊ�ߵ�ƽ		
#define digitalLo(p,i)			  {p->BSRRH=i;}				//����͵�ƽ
#define digitalToggle(p,i)		{p->ODR ^=i;}			  //�����ת״̬

#define DIR_P                  1
#define DIR_N                  0

/* �����ƶ���������IO�� */
#define DIR_BRK_TOGGLE		digitalToggle(DIR1_BRK_GPIO_PORT,DIR1_BRK_PIN);digitalToggle(DIR2_BRK_GPIO_PORT,DIR2_BRK_PIN);
#define DIR_BRK_P		      digitalHi    (DIR1_BRK_GPIO_PORT,DIR1_BRK_PIN);digitalLo    (DIR2_BRK_GPIO_PORT,DIR2_BRK_PIN)
#define DIR_BRK_N			    digitalLo    (DIR1_BRK_GPIO_PORT,DIR1_BRK_PIN);digitalHi    (DIR2_BRK_GPIO_PORT,DIR2_BRK_PIN)
/* ����ת���������IO�� */
#define DIR_TURN_TOGGLE		digitalToggle(DIR1_TURN_GPIO_PORT,DIR1_TURN_PIN);digitalToggle(DIR2_TURN_GPIO_PORT,DIR2_TURN_PIN)
#define DIR_TURN_P		    digitalHi    (DIR1_TURN_GPIO_PORT,DIR1_TURN_PIN);digitalLo    (DIR2_TURN_GPIO_PORT,DIR2_TURN_PIN)
#define DIR_TURN_N			  digitalLo    (DIR1_TURN_GPIO_PORT,DIR1_TURN_PIN);digitalHi    (DIR2_TURN_GPIO_PORT,DIR2_TURN_PIN)

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

#define CYCLE                     500
#define TURN_THRESHOLD_POS        5   //ת��λ�þ���
#define BRK_THRESHOLD_POS         5   //�ƶ�λ�þ���


void Motor_Configuration(void);
uint16_t TurnMotorSpeedSet(int8_t Speed);
uint16_t BRK_MotorSpeedSet(int8_t Speed);
uint8_t BRKMotorPositionControl(int8_t Position,int32_t PositionSet);
uint8_t TurnMotorPositionControl(int32_t Position,int32_t PositionSet);
#endif /* __GENERAL_TIM_H */

