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
  
#include "./tim/bsp_general_tim.h"

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
		RCC_AHB1PeriphClockCmd ( DIR_A_GPIO_CLK|DIR_B_GPIO_CLK|DIR_C_GPIO_CLK|DIR_D_GPIO_CLK, ENABLE); 

		/*ѡ��Ҫ���Ƶ�GPIO����*/															   
		GPIO_InitStructure.GPIO_Pin = DIR_A_PIN;	 

		/*��������ģʽΪ���ģʽ*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
    
    /*�������ŵ��������Ϊ�������*/
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    
    /*��������Ϊ����ģʽ��Ĭ��������ת*/
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

		/*������������Ϊ50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 

		/*���ÿ⺯����ʹ���������õ�GPIO_InitStructure��ʼ��GPIO*/
		GPIO_Init(DIR_A_GPIO_PORT, &GPIO_InitStructure);	
    
    /*ѡ��Ҫ���Ƶ�GPIO����*/															   
		GPIO_InitStructure.GPIO_Pin = DIR_B_PIN;	
    GPIO_Init(DIR_B_GPIO_PORT, &GPIO_InitStructure);	
    
    /*ѡ��Ҫ���Ƶ�GPIO����*/															   
		GPIO_InitStructure.GPIO_Pin = DIR_C_PIN;	
    GPIO_Init(DIR_C_GPIO_PORT, &GPIO_InitStructure);	
		
		/*ѡ��Ҫ���Ƶ�GPIO����*/															   
		GPIO_InitStructure.GPIO_Pin = DIR_D_PIN;	
    GPIO_Init(DIR_D_GPIO_PORT, &GPIO_InitStructure);	
		
}


/**
  * @brief  ����TIM�������PWMʱ�õ���I/O
  * @param  ��
  * @retval ��
  */
static void TIMx_GPIO_Config(void) 
{
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*������ص�GPIO����ʱ��*/
	RCC_AHB1PeriphClockCmd (GENERAL_OCPWM_1_GPIO_CLK | GENERAL_OCPWM_2_GPIO_CLK | GENERAL_OCPWM_3_GPIO_CLK | GENERAL_OCPWM_4_GPIO_CLK, ENABLE); 
  
	/* ��ʱ��ͨ�����Ÿ��� 1 */
	GPIO_PinAFConfig(GENERAL_OCPWM_1_GPIO_PORT,GENERAL_OCPWM_1_PINSOURCE,GENERAL_OCPWM_1_AF); 
  /* ��ʱ��ͨ�����Ÿ��� 2 */
	GPIO_PinAFConfig(GENERAL_OCPWM_2_GPIO_PORT,GENERAL_OCPWM_2_PINSOURCE,GENERAL_OCPWM_2_AF); 
  /* ��ʱ��ͨ�����Ÿ��� 3 */
	GPIO_PinAFConfig(GENERAL_OCPWM_3_GPIO_PORT,GENERAL_OCPWM_3_PINSOURCE,GENERAL_OCPWM_3_AF); 
  /* ��ʱ��ͨ�����Ÿ��� 4 */
	GPIO_PinAFConfig(GENERAL_OCPWM_4_GPIO_PORT,GENERAL_OCPWM_4_PINSOURCE,GENERAL_OCPWM_4_AF); 
  
	/* ��ʱ��ͨ������ 1 ���� */															   
	GPIO_InitStructure.GPIO_Pin = GENERAL_OCPWM_1_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;    
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
	GPIO_Init(GENERAL_OCPWM_1_GPIO_PORT, &GPIO_InitStructure);
	
	/* ��ʱ��ͨ������ 2 ���� */	
	GPIO_InitStructure.GPIO_Pin = GENERAL_OCPWM_2_PIN;
	GPIO_Init(GENERAL_OCPWM_2_GPIO_PORT, &GPIO_InitStructure);
	
	/* ��ʱ��ͨ������ 3 ���� */	
	GPIO_InitStructure.GPIO_Pin = GENERAL_OCPWM_3_PIN;
	GPIO_Init(GENERAL_OCPWM_3_GPIO_PORT, &GPIO_InitStructure);
	
	/* ��ʱ��ͨ������ 4 ���� */	
	GPIO_InitStructure.GPIO_Pin = GENERAL_OCPWM_4_PIN;
	GPIO_Init(GENERAL_OCPWM_4_GPIO_PORT, &GPIO_InitStructure);
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
static void TIM_PWMOUTPUT_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef        TIM_OCInitStructure;
	
	// ����TIMx_CLK,x[2,3,4,5,12,13,14] 
  RCC_APB1PeriphClockCmd(GENERAL_TIM_CLK, ENABLE); 

  /* �ۼ� TIM_Period�������һ�����»����ж�*/		
  //����ʱ����0������8999����Ϊ9000�Σ�Ϊһ����ʱ����
  TIM_TimeBaseStructure.TIM_Period = 9000-1;       
	
	// �߼����ƶ�ʱ��ʱ��ԴTIMxCLK = HCLK/2=90MHz 
	// �趨��ʱ��Ƶ��Ϊ=TIMxCLK/(TIM_Prescaler+1)=100KHz
  TIM_TimeBaseStructure.TIM_Prescaler = 9-1;	
  // ����ʱ�ӷ�Ƶ
  TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
  // ������ʽ
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	
	// ��ʼ����ʱ��TIMx, x[2,3,4,5,12,13,14] 
	TIM_TimeBaseInit(GENERAL_TIM, &TIM_TimeBaseStructure);
	
	/*PWMģʽ����*/
	/* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //����ΪPWMģʽ1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  	  //����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ
  
	
	TIM_OC1Init(GENERAL_TIM, &TIM_OCInitStructure);	 //ʹ��ͨ��1
  TIM_OC2Init(GENERAL_TIM, &TIM_OCInitStructure);	 //ʹ��ͨ��2
	TIM_OC3Init(GENERAL_TIM, &TIM_OCInitStructure);	 //ʹ��ͨ��3
	TIM_OC4Init(GENERAL_TIM, &TIM_OCInitStructure);	 //ʹ��ͨ��4
	
	/*ʹ��ͨ��1����*/
	TIM_OC1PreloadConfig(GENERAL_TIM, TIM_OCPreload_Enable);
	/*ʹ��ͨ��2����*/
	TIM_OC2PreloadConfig(GENERAL_TIM, TIM_OCPreload_Enable);
	/*ʹ��ͨ��3����*/
	TIM_OC3PreloadConfig(GENERAL_TIM, TIM_OCPreload_Enable);
	/*ʹ��ͨ��4����*/
	TIM_OC4PreloadConfig(GENERAL_TIM, TIM_OCPreload_Enable);
	
	
	//TIM_SetCompare3(GENERAL_TIM, 0);
	// ʹ�ܶ�ʱ��
	TIM_Cmd(GENERAL_TIM, ENABLE);	
}

/**
  * @brief  ��ʼ���߼����ƶ�ʱ����ʱ��1ms����һ���ж�
  * @param  ��
  * @retval ��
  */
void TIMx_Configuration(void)
{
	DIR_GPIO_Config();
	
	TIMx_GPIO_Config();
  
  TIM_PWMOUTPUT_Config();
}
/**
  * @brief  �ƶ����ת������
	* @param  Speed: -100 -- +100  �ٶ��ɸ�VCC��ѹ�������VCC���
  * @retval ��
  */


void BrakeSpeedSet(int8_t Speed)
{
	uint16_t my_Speed = 0;
	if(Speed < 0)//�����ѹΪ��ֵ
	{
		Speed = -Speed;
		DIR_B_P;
	}
	else//�����ѹΪ��ֵ
	{
		DIR_B_N;
	}
	if(Speed > 100)
		Speed = 100;
	if(Speed < -100)
		Speed = -100;
	my_Speed = (uint16_t)(Speed*90);
	//TIM_SetCompare2(GENERAL_TIM, 9001 - my_Speed);
	TIM_SetCompare2(GENERAL_TIM, my_Speed);
}
void TurnMotorSpeedSet(int8_t Speed)
{
	uint16_t my_Speed = 0;
	if(Speed < 0)//�����ѹΪ��ֵ
	{
		Speed = -Speed;
		DIR_C_P;
	}
	else//�����ѹΪ��ֵ
	{
		DIR_C_N;
	}
	my_Speed = (uint16_t)(Speed*90);
	if(my_Speed > 8999)
		my_Speed = 8999;
	if(my_Speed < 0)
		my_Speed = 0;
	
	
	
	
	TIM_SetCompare3(GENERAL_TIM, my_Speed);
}

/*********************************************END OF FILE**********************/
