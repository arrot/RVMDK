#include "bsp_speed.h"


static void                         Relay_GPIO_Config                  ( void );



 /**
  * @brief  ���� Relay �� GPIO ����
  * @param  ��
  * @retval ��
  */
static void Relay_GPIO_Config ( void )
{		
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;


	/* ���� RELAY1 ���� */
	RELAY1_GPIO_xClock_FUN ( RELAY1_GPIO_CLK, ENABLE ); 															   
	GPIO_InitStructure.GPIO_Pin = RELAY1_GPIO_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init ( RELAY1_GPIO_PORT, & GPIO_InitStructure );	

	/* ���� RELAY2 ���� */
	RELAY2_GPIO_xClock_FUN ( RELAY2_GPIO_CLK, ENABLE ); 															   
	GPIO_InitStructure.GPIO_Pin = RELAY2_GPIO_PIN;	
	GPIO_Init ( RELAY2_GPIO_PORT, & GPIO_InitStructure );	

	/* ���� RELAY3 ���� */
	RELAY2_GPIO_xClock_FUN ( RELAY3_GPIO_CLK, ENABLE ); 															   
	GPIO_InitStructure.GPIO_Pin = RELAY3_GPIO_PIN;	
	GPIO_Init ( RELAY3_GPIO_PORT, & GPIO_InitStructure );		

	/* ���� RELAY4 ���� */
	RELAY2_GPIO_xClock_FUN ( RELAY4_GPIO_CLK, ENABLE ); 															   
	GPIO_InitStructure.GPIO_Pin = RELAY4_GPIO_PIN;	
	GPIO_Init ( RELAY4_GPIO_PORT, & GPIO_InitStructure );	

	/* ���� RELAY5 ���� */
	RELAY2_GPIO_xClock_FUN ( RELAY5_GPIO_CLK, ENABLE ); 															   
	GPIO_InitStructure.GPIO_Pin = RELAY5_GPIO_PIN;	
	GPIO_Init ( RELAY5_GPIO_PORT, & GPIO_InitStructure );
	
	/* ���� RELAY6 ���� */
	RELAY2_GPIO_xClock_FUN ( RELAY6_GPIO_CLK, ENABLE ); 															   
	GPIO_InitStructure.GPIO_Pin = RELAY6_GPIO_PIN;	
	GPIO_Init ( RELAY6_GPIO_PORT, & GPIO_InitStructure );	

	/* ���� RELAY7 ���� */
	RELAY2_GPIO_xClock_FUN ( RELAY7_GPIO_CLK, ENABLE ); 															   
	GPIO_InitStructure.GPIO_Pin = RELAY7_GPIO_PIN;	
	GPIO_Init ( RELAY7_GPIO_PORT, & GPIO_InitStructure );

	/* ���� RELAY8 ���� */
	RELAY2_GPIO_xClock_FUN ( RELAY8_GPIO_CLK, ENABLE ); 															   
	GPIO_InitStructure.GPIO_Pin = RELAY8_GPIO_PIN;	
	GPIO_Init ( RELAY8_GPIO_PORT, & GPIO_InitStructure );
}


 /**
  * @brief  Realay ��ʼ������
  * @param  ��
  * @retval ��
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
/**
  * @brief  ���ó���
  * @param  Speed- 0����+100���ٶȴ�С
  * @retval ��
  */
extern uint32_t DualSine12bit;
void CarSpeedSet(uint8_t Speed)
{
	//�������ĺ�����
	if(Speed>100)
		Speed = 100;
	//����DAC�����
	DualSine12bit = Speed*4096.0/100.0;
}
/*
@brief:ģʽ�л�
@param:Mode=MANUAL_CONTROL�ֶ�ģʽ     Mode=AUTO_FORMARD�Զ�ǰ��ģʽ Mode=AUTO_BACKMARD�Զ�����ģʽ
@return:ģʽ
*/
int8_t SwitchMode(int8_t Mode)
{
	int8_t state = 0;
	if(Mode)//�Զ�ģʽ
	{
		RELAY4_ON();
		RELAY5_ON();
		RELAY1_ON();
		RELAY3_ON();
		if(Mode == 1)//ǰ��
		{
			RELAY2_OFF();
			state = 1;
		}
		else if(Mode == 2)//����
		{
			RELAY2_ON();
			state = 1;
		}
	}
	else//�ֶ�ģʽ
	{
		RELAY4_OFF();
		RELAY5_OFF();
		RELAY1_OFF();
		RELAY3_OFF();
		state = 1;
	}
	return state;
}
/*********************************************END OF FILE**********************/




















