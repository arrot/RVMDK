#include "bsp_speed.h"


static void                         Relay_GPIO_Config                  ( void );



 /**
  * @brief  配置 Relay 的 GPIO 功能
  * @param  无
  * @retval 无
  */
static void Relay_GPIO_Config ( void )
{		
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;


	/* 配置 RELAY1 引脚 */
	RELAY1_GPIO_xClock_FUN ( RELAY1_GPIO_CLK, ENABLE ); 															   
	GPIO_InitStructure.GPIO_Pin = RELAY1_GPIO_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init ( RELAY1_GPIO_PORT, & GPIO_InitStructure );	

	/* 配置 RELAY2 引脚 */
	RELAY2_GPIO_xClock_FUN ( RELAY2_GPIO_CLK, ENABLE ); 															   
	GPIO_InitStructure.GPIO_Pin = RELAY2_GPIO_PIN;	
	GPIO_Init ( RELAY2_GPIO_PORT, & GPIO_InitStructure );	

	/* 配置 RELAY3 引脚 */
	RELAY2_GPIO_xClock_FUN ( RELAY3_GPIO_CLK, ENABLE ); 															   
	GPIO_InitStructure.GPIO_Pin = RELAY3_GPIO_PIN;	
	GPIO_Init ( RELAY3_GPIO_PORT, & GPIO_InitStructure );		

	/* 配置 RELAY4 引脚 */
	RELAY2_GPIO_xClock_FUN ( RELAY4_GPIO_CLK, ENABLE ); 															   
	GPIO_InitStructure.GPIO_Pin = RELAY4_GPIO_PIN;	
	GPIO_Init ( RELAY4_GPIO_PORT, & GPIO_InitStructure );	

	/* 配置 RELAY5 引脚 */
	RELAY2_GPIO_xClock_FUN ( RELAY5_GPIO_CLK, ENABLE ); 															   
	GPIO_InitStructure.GPIO_Pin = RELAY5_GPIO_PIN;	
	GPIO_Init ( RELAY5_GPIO_PORT, & GPIO_InitStructure );
	
	/* 配置 RELAY6 引脚 */
	RELAY2_GPIO_xClock_FUN ( RELAY6_GPIO_CLK, ENABLE ); 															   
	GPIO_InitStructure.GPIO_Pin = RELAY6_GPIO_PIN;	
	GPIO_Init ( RELAY6_GPIO_PORT, & GPIO_InitStructure );	

	/* 配置 RELAY7 引脚 */
	RELAY2_GPIO_xClock_FUN ( RELAY7_GPIO_CLK, ENABLE ); 															   
	GPIO_InitStructure.GPIO_Pin = RELAY7_GPIO_PIN;	
	GPIO_Init ( RELAY7_GPIO_PORT, & GPIO_InitStructure );

	/* 配置 RELAY8 引脚 */
	RELAY2_GPIO_xClock_FUN ( RELAY8_GPIO_CLK, ENABLE ); 															   
	GPIO_InitStructure.GPIO_Pin = RELAY8_GPIO_PIN;	
	GPIO_Init ( RELAY8_GPIO_PORT, & GPIO_InitStructure );
}


 /**
  * @brief  Realay 初始化函数
  * @param  无
  * @retval 无
  */
void Relay_Init ( void )
{
  Relay_GPIO_Config ();
	
	RELAY1_OFF();
	RELAY2_OFF();
	RELAY3_OFF();
	RELAY4_OFF();
	RELAY5_OFF();
	RELAY6_OFF();
	RELAY7_OFF();
	RELAY8_OFF();
}



/*********************************************END OF FILE**********************/




















