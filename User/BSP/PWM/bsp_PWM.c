/*******************************************************************************
  * @file    xxx.c
  * @author  ����
  * @version V1.0
  * @date    2019-04-15
  * @brief   xxx����
  ******************************************************************************
  * @attention
  * δ��������ɣ��������������κ���;
  * ��Ȩ���У�����ؾ���
  * All rights reserved	
  * ��˾���廪���˳��Ŷ�
  * ����ƽ̨:STM32F429
  * ���䣺snowman4219@163.com
  *
  ******************************************************************************
  */
  
#include "bsp_PWM.h"

/**
  * @brief  ��ʼ�����Ƶ�������IO
  * @param  ��
  * @retval ��
  */
static void DIR_GPIO_Config(void) 
{		
		/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*�������������ص�GPIO����ʱ��*/
		RCC_AHB1PeriphClockCmd ( DIR_BRK_GPIO_CLK|DIR_TURN_GPIO_CLK, ENABLE); 

		/*ѡ��Ҫ���Ƶ�GPIO����*/															   
		GPIO_InitStructure.GPIO_Pin = DIR_BRK_PIN;	 

		/*��������ģʽΪ���ģʽ*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
    
    /*�������ŵ��������Ϊ�������*/
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    
    /*��������Ϊ����ģʽ��Ĭ��������ת*/
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

		/*������������Ϊ50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 

		/*���ÿ⺯����ʹ���������õ�GPIO_InitStructure��ʼ��GPIO*/
		GPIO_Init(DIR_BRK_GPIO_PORT, &GPIO_InitStructure);	
    
    /*ѡ��Ҫ���Ƶ�GPIO����*/															   
		GPIO_InitStructure.GPIO_Pin = DIR_TURN_PIN;	
    GPIO_Init(DIR_TURN_GPIO_PORT, &GPIO_InitStructure);		
}


/**
  * @brief  ����TIM�������PWMʱ�õ���I/O
  * @param  ��
  * @retval ��
  */
static void PWM_GPIO_Config(void) 
{
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*������ص�GPIO����ʱ��*/
	RCC_AHB1PeriphClockCmd (BRK_OCPWM_GPIO_CLK | TURN_OCPWM_GPIO_CLK, ENABLE); 
  
	/* ��ʱ��ͨ�����Ÿ��� 1 */
	GPIO_PinAFConfig(BRK_OCPWM_GPIO_PORT,BRK_OCPWM_PINSOURCE,BRK_OCPWM_AF); 
  /* ��ʱ��ͨ�����Ÿ��� 2 */
	GPIO_PinAFConfig(TURN_OCPWM_GPIO_PORT,TURN_OCPWM_PINSOURCE,TURN_OCPWM_AF); 
 
	/* ��ʱ��ͨ������ 1 ���� */															   
	GPIO_InitStructure.GPIO_Pin = BRK_OCPWM_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;    
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
	GPIO_Init(BRK_OCPWM_GPIO_PORT, &GPIO_InitStructure);
	
	/* ��ʱ��ͨ������ 2 ���� */	
	GPIO_InitStructure.GPIO_Pin = TURN_OCPWM_PIN;
	GPIO_Init(TURN_OCPWM_GPIO_PORT, &GPIO_InitStructure);
}


/*
 * ע�⣺TIM_TimeBaseInitTypeDef�ṹ��������5����Ա��TIM6��TIM7�ļĴ�������ֻ��
 * TIM_Prescaler��TIM_Period������ʹ��TIM6��TIM7��ʱ��ֻ���ʼ����������Ա���ɣ�
 * ����������Ա��ͨ�ö�ʱ���͸߼���ʱ������.
 *-----------------------------------------------------------------------------
 * TIM_Prescaler         ����
 * TIM_CounterMode			 TIMx,x[6,7]û�У��������У�������ʱ����
 * TIM_Period            ����
 * TIM_ClockDivision     TIMx,x[6,7]û�У���������(������ʱ��)
 * TIM_RepetitionCounter TIMx,x[1,8]����(�߼���ʱ��)
 *-----------------------------------------------------------------------------
 */
/**
  * @brief  PWM��ʱ������.
  * @param  None
  * @retval None
  * @notice None
  */
static void PWMOUTPUT_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef        TIM_OCInitStructure;
	
	// ����TIMx_CLK,x[2,3,4,5,12,13,14] 
  RCC_APB1PeriphClockCmd(PWM_TIM_CLK, ENABLE); 

  /* �ۼ� TIM_Period�������һ�����»����ж�*/		
  //����ʱ����0������8999����Ϊ9000�Σ�Ϊһ����ʱ����
  TIM_TimeBaseStructure.TIM_Period = 10000;       
	
	// �߼����ƶ�ʱ��ʱ��ԴTIMxCLK = HCLK/2=90MHz 
	// �趨��ʱ��Ƶ��Ϊ=TIMxCLK/(TIM_Prescaler+1)=100KHz
  TIM_TimeBaseStructure.TIM_Prescaler = 9-1;	
  // ����ʱ�ӷ�Ƶ
  TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
  // ������ʽ
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	
	// ��ʼ����ʱ��TIMx, x[2,3,4,5,12,13,14] 
	TIM_TimeBaseInit(PWM_TIM, &TIM_TimeBaseStructure);
	
	/*PWMģʽ����*/
	/* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //����ΪPWMģʽ1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  	  //����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ
 
  TIM_OC2Init(PWM_TIM, &TIM_OCInitStructure);	 //ʹ��ͨ��2
	TIM_OC3Init(PWM_TIM, &TIM_OCInitStructure);	 //ʹ��ͨ��3
	
		/*ʹ��ͨ��2����*/
	TIM_OC2PreloadConfig(PWM_TIM, TIM_OCPreload_Enable);
	/*ʹ��ͨ��3����*/
	TIM_OC3PreloadConfig(PWM_TIM, TIM_OCPreload_Enable);

	// ʹ�ܶ�ʱ��
	TIM_Cmd(PWM_TIM, ENABLE);	
}

/**
  * @brief  ��ʼ���߼����ƶ�ʱ����ʱ��1ms����һ���ж�
  * @param  ��
  * @retval ��
  */
void Motor_Configuration(void)
{
	DIR_GPIO_Config();
	
	PWM_GPIO_Config();
  
  PWMOUTPUT_Config();
}
/**
  * @brief  �ƶ����ת������
	* @param  Speed: -95 -- +95  �ٶ��ɸ�����������
  * @retval ���������õ�ֵ
  */
uint16_t BRK_MotorSpeedSet(int8_t Speed)
{
	uint16_t Duty = 0;
	//�������ĺϷ���
	if(Speed > 99)
		Speed = 99;
	if(Speed < -99)
		Speed = -99;
	
	if(Speed < 0)//�����ѹΪ��ֵ
	{
		Speed = -Speed;
		DIR_BRK_N;
	}
	else//�����ѹΪ��ֵ
	{
		DIR_BRK_P;
	}
	Duty = (uint16_t)(Speed*100);
	if(Duty>9900)
	{
		Duty = 9900;
	}
	TIM_SetCompare2(PWM_TIM, Duty);
	return Duty;
}
/**
  * @brief  �ƶ����ת������
	* @param  Speed: -95 -- +95  �ٶ��ɸ�����������
  * @retval ���������õ�ֵ
  */
uint16_t TurnMotorSpeedSet(int8_t Speed)
{
	uint16_t Duty = 0;
	//�������ĺϷ���
	if(Speed > 99)
		Speed = 99;
	if(Speed < -99)
		Speed = -99;
	
	if(Speed < 0)
	{
		Speed = -Speed;
		DIR_TURN_N;
	}
	else
	{
		DIR_TURN_P;
	}
	Duty = (uint16_t)(Speed*100);
	if(Duty>9900)
	{
		Duty = 9900;
	}
	TIM_SetCompare3(PWM_TIM, Duty);
	return Duty;
}

/*********************************************END OF FILE**********************/
