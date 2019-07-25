/*******************************************************************************
  * @file    xxx.c
  * @author  郭栋
  * @version V1.0
  * @date    2019-04-15
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
  
#include "./tim/bsp_general_tim.h"

/**
  * @brief  初始化控制电机方向的IO
  * @param  无
  * @retval 无
  */
static void DIR_GPIO_Config(void) 
{		
		/*定义一个GPIO_InitTypeDef类型的结构体*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*开启方向控制相关的GPIO外设时钟*/
		RCC_AHB1PeriphClockCmd ( DIR_A_GPIO_CLK|DIR_B_GPIO_CLK|DIR_C_GPIO_CLK|DIR_D_GPIO_CLK, ENABLE); 

		/*选择要控制的GPIO引脚*/															   
		GPIO_InitStructure.GPIO_Pin = DIR_A_PIN;	 

		/*设置引脚模式为输出模式*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
    
    /*设置引脚的输出类型为推挽输出*/
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    
    /*设置引脚为上拉模式，默认正向旋转*/
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

		/*设置引脚速率为50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 

		/*调用库函数，使用上面配置的GPIO_InitStructure初始化GPIO*/
		GPIO_Init(DIR_A_GPIO_PORT, &GPIO_InitStructure);	
    
    /*选择要控制的GPIO引脚*/															   
		GPIO_InitStructure.GPIO_Pin = DIR_B_PIN;	
    GPIO_Init(DIR_B_GPIO_PORT, &GPIO_InitStructure);	
    
    /*选择要控制的GPIO引脚*/															   
		GPIO_InitStructure.GPIO_Pin = DIR_C_PIN;	
    GPIO_Init(DIR_C_GPIO_PORT, &GPIO_InitStructure);	
		
		/*选择要控制的GPIO引脚*/															   
		GPIO_InitStructure.GPIO_Pin = DIR_D_PIN;	
    GPIO_Init(DIR_D_GPIO_PORT, &GPIO_InitStructure);	
		
}


/**
  * @brief  配置TIM复用输出PWM时用到的I/O
  * @param  无
  * @retval 无
  */
static void TIMx_GPIO_Config(void) 
{
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*开启相关的GPIO外设时钟*/
	RCC_AHB1PeriphClockCmd (GENERAL_OCPWM_1_GPIO_CLK | GENERAL_OCPWM_2_GPIO_CLK | GENERAL_OCPWM_3_GPIO_CLK | GENERAL_OCPWM_4_GPIO_CLK, ENABLE); 
  
	/* 定时器通道引脚复用 1 */
	GPIO_PinAFConfig(GENERAL_OCPWM_1_GPIO_PORT,GENERAL_OCPWM_1_PINSOURCE,GENERAL_OCPWM_1_AF); 
  /* 定时器通道引脚复用 2 */
	GPIO_PinAFConfig(GENERAL_OCPWM_2_GPIO_PORT,GENERAL_OCPWM_2_PINSOURCE,GENERAL_OCPWM_2_AF); 
  /* 定时器通道引脚复用 3 */
	GPIO_PinAFConfig(GENERAL_OCPWM_3_GPIO_PORT,GENERAL_OCPWM_3_PINSOURCE,GENERAL_OCPWM_3_AF); 
  /* 定时器通道引脚复用 4 */
	GPIO_PinAFConfig(GENERAL_OCPWM_4_GPIO_PORT,GENERAL_OCPWM_4_PINSOURCE,GENERAL_OCPWM_4_AF); 
  
	/* 定时器通道引脚 1 配置 */															   
	GPIO_InitStructure.GPIO_Pin = GENERAL_OCPWM_1_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;    
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
	GPIO_Init(GENERAL_OCPWM_1_GPIO_PORT, &GPIO_InitStructure);
	
	/* 定时器通道引脚 2 配置 */	
	GPIO_InitStructure.GPIO_Pin = GENERAL_OCPWM_2_PIN;
	GPIO_Init(GENERAL_OCPWM_2_GPIO_PORT, &GPIO_InitStructure);
	
	/* 定时器通道引脚 3 配置 */	
	GPIO_InitStructure.GPIO_Pin = GENERAL_OCPWM_3_PIN;
	GPIO_Init(GENERAL_OCPWM_3_GPIO_PORT, &GPIO_InitStructure);
	
	/* 定时器通道引脚 4 配置 */	
	GPIO_InitStructure.GPIO_Pin = GENERAL_OCPWM_4_PIN;
	GPIO_Init(GENERAL_OCPWM_4_GPIO_PORT, &GPIO_InitStructure);
}


/*
 * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
 * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
 * 另外三个成员是通用定时器和高级定时器才有.
 *-----------------------------------------------------------------------------
 * TIM_Prescaler         都有
 * TIM_CounterMode			 TIMx,x[6,7]没有，其他都有（基本定时器）
 * TIM_Period            都有
 * TIM_ClockDivision     TIMx,x[6,7]没有，其他都有(基本定时器)
 * TIM_RepetitionCounter TIMx,x[1,8]才有(高级定时器)
 *-----------------------------------------------------------------------------
 */
/**
  * @brief  PWM定时器配置.
  * @param  None
  * @retval None
  * @notice None
  */
static void TIM_PWMOUTPUT_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef        TIM_OCInitStructure;
	
	// 开启TIMx_CLK,x[2,3,4,5,12,13,14] 
  RCC_APB1PeriphClockCmd(GENERAL_TIM_CLK, ENABLE); 

  /* 累计 TIM_Period个后产生一个更新或者中断*/		
  //当定时器从0计数到8999，即为9000次，为一个定时周期
  TIM_TimeBaseStructure.TIM_Period = 9000-1;       
	
	// 高级控制定时器时钟源TIMxCLK = HCLK/2=90MHz 
	// 设定定时器频率为=TIMxCLK/(TIM_Prescaler+1)=100KHz
  TIM_TimeBaseStructure.TIM_Prescaler = 9-1;	
  // 采样时钟分频
  TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
  // 计数方式
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	
	// 初始化定时器TIMx, x[2,3,4,5,12,13,14] 
	TIM_TimeBaseInit(GENERAL_TIM, &TIM_TimeBaseStructure);
	
	/*PWM模式配置*/
	/* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //配置为PWM模式1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  	  //当定时器计数值小于CCR1_Val时为高电平
  
	
	TIM_OC1Init(GENERAL_TIM, &TIM_OCInitStructure);	 //使能通道1
  TIM_OC2Init(GENERAL_TIM, &TIM_OCInitStructure);	 //使能通道2
	TIM_OC3Init(GENERAL_TIM, &TIM_OCInitStructure);	 //使能通道3
	TIM_OC4Init(GENERAL_TIM, &TIM_OCInitStructure);	 //使能通道4
	
	/*使能通道1重载*/
	TIM_OC1PreloadConfig(GENERAL_TIM, TIM_OCPreload_Enable);
	/*使能通道2重载*/
	TIM_OC2PreloadConfig(GENERAL_TIM, TIM_OCPreload_Enable);
	/*使能通道3重载*/
	TIM_OC3PreloadConfig(GENERAL_TIM, TIM_OCPreload_Enable);
	/*使能通道4重载*/
	TIM_OC4PreloadConfig(GENERAL_TIM, TIM_OCPreload_Enable);
	
	
	//TIM_SetCompare3(GENERAL_TIM, 0);
	// 使能定时器
	TIM_Cmd(GENERAL_TIM, ENABLE);	
}

/**
  * @brief  初始化高级控制定时器定时，1ms产生一次中断
  * @param  无
  * @retval 无
  */
void TIMx_Configuration(void)
{
	DIR_GPIO_Config();
	
	TIMx_GPIO_Config();
  
  TIM_PWMOUTPUT_Config();
}
/**
  * @brief  制动电机转速设置
	* @param  Speed: -100 -- +100  速度由负VCC电压最大到正的VCC最大
  * @retval 无
  */


void BrakeSpeedSet(int8_t Speed)
{
	uint16_t my_Speed = 0;
	if(Speed < 0)//电机电压为正值
	{
		Speed = -Speed;
		DIR_B_P;
	}
	else//电机电压为负值
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
	if(Speed < 0)//电机电压为正值
	{
		Speed = -Speed;
		DIR_C_P;
	}
	else//电机电压为负值
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
