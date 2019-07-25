/*******************************************************************************
  * @file    xxx.c
  * @author  郭栋
  * @version V1.0
  * @date    2019-06-04
  * @brief   xxx程序
  ******************************************************************************
  * @attention
  * 未经作者许可，不得用于其它任何用途
  * 版权所有，盗版必究。
  * All rights reserved	
  * 公司：清华无人车团队
  * 开发平台:STM32F429
  * 邮箱：snowman4219@163.com
  *
  ******************************************************************************
  */
	
#include "./AngularTransducer/AngularTransducer.h"
#include "./led/bsp_led.h"
__IO uint32_t IC2Value_1 = 0, IC2Value_3 = 0;
__IO uint32_t IC1Value_1 = 0, IC1Value_3 = 0;
__IO uint16_t DutyCycle_A1 = 0,DutyCycle_A2 = 0;//占空比扩大10倍
__IO int32_t Angle_set = 0,Angle = 0;//扩大了100倍；

/**
  * @brief  初始化控制LED的IO
  * @param  无
  * @retval 无
  */
void ANGULAR_TRANSDUCER_GPIO_Config(void) 
{		
		/*定义一个GPIO_InitTypeDef类型的结构体*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*开启LED相关的GPIO外设时钟*/
		RCC_AHB1PeriphClockCmd ( ANGULAR_TRANSDUCER_GPIO_CLK, ENABLE); 

		/*选择要控制的GPIO引脚*/															   
		GPIO_InitStructure.GPIO_Pin = ANGULAR_TRANSDUCER_PIN;	 

		/*设置引脚模式为输出模式*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
    
    /*设置引脚的输出类型为推挽输出*/
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    
    /*设置引脚为上拉模式，默认LED亮*/
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

		/*设置引脚速率为50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 

		/*调用库函数，使用上面配置的GPIO_InitStructure初始化GPIO*/
		GPIO_Init(ANGULAR_TRANSDUCER_GPIO_PORT, &GPIO_InitStructure);
		
		//使能角度传感器的12V电源
		ANGULAR_TRANSDUCER_ENABLE;
}



/**
  * @brief  配置TIM复用输出PWM时用到的I/O
  * @param  无
  * @retval 无
  */
static void TIM1_3_GPIO_Config(void) 
{
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*开启LED相关的GPIO外设时钟*/ 
	RCC_AHB1PeriphClockCmd (ADVANCE_ICPWM_A1_GPIO_CLK, ENABLE);
 	RCC_AHB1PeriphClockCmd (TIM3_ICPWM_A2_GPIO_CLK, ENABLE);

	/* 定时器复用引脚 */
	GPIO_PinAFConfig(ADVANCE_ICPWM_A1_GPIO_PORT,ADVANCE_ICPWM_A1_PINSOURCE,ADVANCE_ICPWM_AF); 
	GPIO_PinAFConfig(TIM3_ICPWM_A2_GPIO_PORT,TIM3_ICPWM_A2_PINSOURCE,TIM3_ICPWM_AF); 
	
	/* 通用定时器PWM输出引脚 */															   	
	GPIO_InitStructure.GPIO_Pin = ADVANCE_ICPWM_A1_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;    
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 	
	/* 高级控制定时器PWM输入捕获引脚 */
	GPIO_Init(ADVANCE_ICPWM_A1_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = TIM3_ICPWM_A2_PIN;
	/* 高级控制定时器PWM输入捕获引脚 */
	GPIO_Init(TIM3_ICPWM_A2_GPIO_PORT, &GPIO_InitStructure);
}

 /**
  * @brief  高级控制定时器 TIMx,x[1,8]中断优先级配置
  * @param  无
  * @retval 无
  */
static void TIM1_3_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // 设置中断组为0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);		
		// 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = ADVANCE_TIM_IRQn; 	
		// 设置抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	  // 设置子优先级
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
	// 开启TIMx_CLK,x[1,8] 
  RCC_APB2PeriphClockCmd(ADVANCE_TIM_CLK, ENABLE); 
	RCC_APB1PeriphClockCmd(TIM3_TIM_CLK, ENABLE);
  TIM_TimeBaseStructure.TIM_Period =0xFFFF-1; 	
	// 高级控制定时器时钟源TIMxCLK = HCLK=180MHz 
	// 设定定时器频率为=TIMxCLK/(TIM_Prescaler+1)=1MHz
  TIM_TimeBaseStructure.TIM_Prescaler = 180-1;	
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  // 计数方式
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;	
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	// 初始化定时器TIMx, x[1,8]
	TIM_TimeBaseInit(ADVANCE_TIM, &TIM_TimeBaseStructure);	
	TIM_TimeBaseStructure.TIM_Prescaler = 90-1;
	// 初始化定时器TIMx, x[1,8]
	TIM_TimeBaseInit(TIM3_TIM, &TIM_TimeBaseStructure);
	/* 高级定时器IC1捕获：上升沿触发 TI1FP1 */
  TIM_ICInitStructure.TIM_Channel = ADVANCE_IC1PWM_CHANNEL;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;
  TIM_PWMIConfig(ADVANCE_TIM, &TIM_ICInitStructure);
	/* 高级定时器IC2捕获：下降沿触发 TI1FP2 */	
	TIM_ICInitStructure.TIM_Channel = ADVANCE_IC2PWM_CHANNEL;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_IndirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;
  TIM_PWMIConfig(ADVANCE_TIM, &TIM_ICInitStructure);
	/* 通用定时器IC1捕获：上升沿触发 TI1FP1 */
  TIM_ICInitStructure.TIM_Channel = TIM3_IC1PWM_CHANNEL;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;
  TIM_PWMIConfig(TIM3_TIM, &TIM_ICInitStructure);
	/* 通用定时器IC2捕获：下降沿触发 TI1FP2 */	
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
  /* 使能高级控制定时器 */
  TIM_Cmd(ADVANCE_TIM, ENABLE);
	/* 使能通用控制定时器 */
  TIM_Cmd(TIM3_TIM, ENABLE);
  /* 使能高级定时器捕获/比较2中断请求 */
  TIM_ITConfig(ADVANCE_TIM, TIM_IT_CC1, ENABLE);
	/* 使能通用定时器捕获/比较2中断请求 */
  TIM_ITConfig(TIM3_TIM, TIM_IT_CC1, ENABLE);
}


/**
  * @brief  初始化高级控制定时器定时，1ms产生一次中断
  * @param  无
  * @retval 无
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
		IC1Value_3 = TIM_GetCapture1(ADVANCE_TIM);//频率
		IC1Value_1 = TIM_GetCapture2(ADVANCE_TIM);//占空比
		if(IC1Value_3 != 0)
		{
			DutyCycle_A1 = (IC1Value_1 * 1000) / IC1Value_3;
			
		}
		/* 清除定时器捕获/比较1中断 */
		TIM_ClearITPendingBit(ADVANCE_TIM, TIM_IT_CC1);
	}
}
int yy1=0,yy2=0,yy3=0;
void  TIM3_TIM_IRQHandler (void)
{
	if(TIM_GetITStatus(TIM3_TIM,TIM_IT_CC1)!= RESET)
	{
		IC2Value_3 = TIM_GetCapture1(TIM3_TIM);//频率
		IC2Value_1 = TIM_GetCapture2(TIM3_TIM);//占空比
		
		if((IC2Value_1<1750)&&(IC2Value_1>250))
		{
			yy2++;
		}

		
		if((IC2Value_3 != 0)&&(IC2Value_3<2200)&&(IC2Value_1<1750)&&(IC2Value_1>250))
		{
			DutyCycle_A2 = (float)(IC2Value_1 * 1000) / IC2Value_3;
			yy1++;
		}
		
		/* 清除定时器捕获/比较1中断 */
		TIM_ClearITPendingBit(TIM3_TIM, TIM_IT_CC1);
	}
	
}

int32_t AngleTransducer(uint16_t Duty)
{
	static int32_t Angle_final = 0,Angle_Last = 0;//扩大100倍
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













