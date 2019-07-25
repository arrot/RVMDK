/*******************************************************************************
  * @file    xxx.c
  * @author  ����
  * @version V1.0
  * @date    2019-06-04
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
	
#include "./AngularTransducer/AngularTransducer.h"
#include "./led/bsp_led.h"
__IO uint32_t IC2Value_1 = 0, IC2Value_3 = 0;
__IO uint32_t IC1Value_1 = 0, IC1Value_3 = 0;
__IO uint16_t DutyCycle_A1 = 0,DutyCycle_A2 = 0;//ռ�ձ�����10��
__IO int32_t Angle_set = 0,Angle = 0;//������100����

/**
  * @brief  ��ʼ������LED��IO
  * @param  ��
  * @retval ��
  */
void ANGULAR_TRANSDUCER_GPIO_Config(void) 
{		
		/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*����LED��ص�GPIO����ʱ��*/
		RCC_AHB1PeriphClockCmd ( ANGULAR_TRANSDUCER_GPIO_CLK, ENABLE); 

		/*ѡ��Ҫ���Ƶ�GPIO����*/															   
		GPIO_InitStructure.GPIO_Pin = ANGULAR_TRANSDUCER_PIN;	 

		/*��������ģʽΪ���ģʽ*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
    
    /*�������ŵ��������Ϊ�������*/
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    
    /*��������Ϊ����ģʽ��Ĭ��LED��*/
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

		/*������������Ϊ50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 

		/*���ÿ⺯����ʹ���������õ�GPIO_InitStructure��ʼ��GPIO*/
		GPIO_Init(ANGULAR_TRANSDUCER_GPIO_PORT, &GPIO_InitStructure);
		
		//ʹ�ܽǶȴ�������12V��Դ
		ANGULAR_TRANSDUCER_ENABLE;
}



/**
  * @brief  ����TIM�������PWMʱ�õ���I/O
  * @param  ��
  * @retval ��
  */
static void TIM1_3_GPIO_Config(void) 
{
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*����LED��ص�GPIO����ʱ��*/ 
	RCC_AHB1PeriphClockCmd (ADVANCE_ICPWM_A1_GPIO_CLK, ENABLE);
 	RCC_AHB1PeriphClockCmd (TIM3_ICPWM_A2_GPIO_CLK, ENABLE);

	/* ��ʱ���������� */
	GPIO_PinAFConfig(ADVANCE_ICPWM_A1_GPIO_PORT,ADVANCE_ICPWM_A1_PINSOURCE,ADVANCE_ICPWM_AF); 
	GPIO_PinAFConfig(TIM3_ICPWM_A2_GPIO_PORT,TIM3_ICPWM_A2_PINSOURCE,TIM3_ICPWM_AF); 
	
	/* ͨ�ö�ʱ��PWM������� */															   	
	GPIO_InitStructure.GPIO_Pin = ADVANCE_ICPWM_A1_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;    
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 	
	/* �߼����ƶ�ʱ��PWM���벶������ */
	GPIO_Init(ADVANCE_ICPWM_A1_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = TIM3_ICPWM_A2_PIN;
	/* �߼����ƶ�ʱ��PWM���벶������ */
	GPIO_Init(TIM3_ICPWM_A2_GPIO_PORT, &GPIO_InitStructure);
}

 /**
  * @brief  �߼����ƶ�ʱ�� TIMx,x[1,8]�ж����ȼ�����
  * @param  ��
  * @retval ��
  */
static void TIM1_3_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // �����ж���Ϊ0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);		
		// �����ж���Դ
    NVIC_InitStructure.NVIC_IRQChannel = ADVANCE_TIM_IRQn; 	
		// ������ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	  // ���������ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_TIM_IRQn;
	NVIC_Init(&NVIC_InitStructure);
	
}


static void TIM1_3_PWMINPUT_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	// ����TIMx_CLK,x[1,8] 
  RCC_APB2PeriphClockCmd(ADVANCE_TIM_CLK, ENABLE); 
	RCC_APB1PeriphClockCmd(TIM3_TIM_CLK, ENABLE);
  TIM_TimeBaseStructure.TIM_Period =0xFFFF-1; 	
	// �߼����ƶ�ʱ��ʱ��ԴTIMxCLK = HCLK=180MHz 
	// �趨��ʱ��Ƶ��Ϊ=TIMxCLK/(TIM_Prescaler+1)=1MHz
  TIM_TimeBaseStructure.TIM_Prescaler = 180-1;	
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  // ������ʽ
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;	
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	// ��ʼ����ʱ��TIMx, x[1,8]
	TIM_TimeBaseInit(ADVANCE_TIM, &TIM_TimeBaseStructure);	
	TIM_TimeBaseStructure.TIM_Prescaler = 90-1;
	// ��ʼ����ʱ��TIMx, x[1,8]
	TIM_TimeBaseInit(TIM3_TIM, &TIM_TimeBaseStructure);
	/* �߼���ʱ��IC1���������ش��� TI1FP1 */
  TIM_ICInitStructure.TIM_Channel = ADVANCE_IC1PWM_CHANNEL;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;
  TIM_PWMIConfig(ADVANCE_TIM, &TIM_ICInitStructure);
	/* �߼���ʱ��IC2�����½��ش��� TI1FP2 */	
	TIM_ICInitStructure.TIM_Channel = ADVANCE_IC2PWM_CHANNEL;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_IndirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;
  TIM_PWMIConfig(ADVANCE_TIM, &TIM_ICInitStructure);
	/* ͨ�ö�ʱ��IC1���������ش��� TI1FP1 */
  TIM_ICInitStructure.TIM_Channel = TIM3_IC1PWM_CHANNEL;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;
  TIM_PWMIConfig(TIM3_TIM, &TIM_ICInitStructure);
	/* ͨ�ö�ʱ��IC2�����½��ش��� TI1FP2 */	
	TIM_ICInitStructure.TIM_Channel = TIM3_IC2PWM_CHANNEL;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_IndirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;
  TIM_PWMIConfig(TIM3_TIM, &TIM_ICInitStructure);
	/* Select the TIM1 Input Trigger: TI1FP1 */ 
	TIM_SelectInputTrigger(ADVANCE_TIM, TIM_TS_TI1FP1);
	/* Select the slave Mode: Reset Mode */ 
	TIM_SelectSlaveMode(ADVANCE_TIM, TIM_SlaveMode_Reset);
	/* Enable the Master/Slave Mode */ 
	TIM_SelectMasterSlaveMode(ADVANCE_TIM, TIM_MasterSlaveMode_Enable);
	/* Select the TIM3 Input Trigger: TI1FP1 */ 
	TIM_SelectInputTrigger(TIM3_TIM, TIM_TS_TI1FP1); 
	/* Select the slave Mode: Reset Mode */ 
	TIM_SelectSlaveMode(TIM3_TIM, TIM_SlaveMode_Reset);
	/* Enable the Master/Slave Mode */ 
	TIM_SelectMasterSlaveMode(TIM3_TIM, TIM_MasterSlaveMode_Enable);
  /* ʹ�ܸ߼����ƶ�ʱ�� */
  TIM_Cmd(ADVANCE_TIM, ENABLE);
	/* ʹ��ͨ�ÿ��ƶ�ʱ�� */
  TIM_Cmd(TIM3_TIM, ENABLE);
  /* ʹ�ܸ߼���ʱ������/�Ƚ�2�ж����� */
  TIM_ITConfig(ADVANCE_TIM, TIM_IT_CC1, ENABLE);
	/* ʹ��ͨ�ö�ʱ������/�Ƚ�2�ж����� */
  TIM_ITConfig(TIM3_TIM, TIM_IT_CC1, ENABLE);
}


/**
  * @brief  ��ʼ���߼����ƶ�ʱ����ʱ��1ms����һ���ж�
  * @param  ��
  * @retval ��
  */
void TIM1_3_Configuration(void)
{
	TIM1_3_GPIO_Config();
	TIM1_3_NVIC_Configuration();	
	TIM1_3_PWMINPUT_Config();
}
/**
  * @brief  This function handles TIM interrupt request.
  * @param  None
  * @retval None
  */
void  ADVANCE_TIM_IRQHandler (void)
{
	if(TIM_GetITStatus(ADVANCE_TIM,TIM_IT_CC1)!= RESET)
	{
		IC1Value_3 = TIM_GetCapture1(ADVANCE_TIM);//Ƶ��
		IC1Value_1 = TIM_GetCapture2(ADVANCE_TIM);//ռ�ձ�
		if(IC1Value_3 != 0)
		{
			DutyCycle_A1 = (IC1Value_1 * 1000) / IC1Value_3;
			
		}
		/* �����ʱ������/�Ƚ�1�ж� */
		TIM_ClearITPendingBit(ADVANCE_TIM, TIM_IT_CC1);
	}
}
int yy1=0,yy2=0,yy3=0;
void  TIM3_TIM_IRQHandler (void)
{
	if(TIM_GetITStatus(TIM3_TIM,TIM_IT_CC1)!= RESET)
	{
		IC2Value_3 = TIM_GetCapture1(TIM3_TIM);//Ƶ��
		IC2Value_1 = TIM_GetCapture2(TIM3_TIM);//ռ�ձ�
		
		if((IC2Value_1<1750)&&(IC2Value_1>250))
		{
			yy2++;
		}

		
		if((IC2Value_3 != 0)&&(IC2Value_3<2200)&&(IC2Value_1<1750)&&(IC2Value_1>250))
		{
			DutyCycle_A2 = (float)(IC2Value_1 * 1000) / IC2Value_3;
			yy1++;
		}
		
		/* �����ʱ������/�Ƚ�1�ж� */
		TIM_ClearITPendingBit(TIM3_TIM, TIM_IT_CC1);
	}
	
}

int32_t AngleTransducer(uint16_t Duty)
{
	static int32_t Angle_final = 0,Angle_Last = 0;//����100��
	int32_t Delt_Angle = 0,Angle_Now = 0;
	
	Angle_Now = (int32_t)((Duty - 125)*24);
	Delt_Angle = Angle_Now-Angle_Last;
	if((Angle_Now<2000)&&(Angle_Last>16000))
	{
		Delt_Angle = Angle_Now+18000-Angle_Last;
	}
	if((Angle_Now>16000)&&(Angle_Last<2000))
	{
		Delt_Angle = Angle_Now-(Angle_Last+18000);
	}
	if(Delt_Angle>5000)
		Delt_Angle = 0;
	Angle_final += Delt_Angle;
	Angle_Last = Angle_Now;
	return Angle_final;
}


/*********************************************END OF FILE**********************/













