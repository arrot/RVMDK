#ifndef __SPEED_H__
#define __SPEED_H__

#include "stm32f4xx.h"
#include <stdio.h>
#include <includes.h>

/****************************** 继电器 引脚配置参数定义***************************************/
#define             RELAY1_GPIO_xClock_FUN                 RCC_AHB1PeriphClockCmd
#define             RELAY1_GPIO_CLK                        RCC_AHB1Periph_GPIOA
#define             RELAY1_GPIO_PORT                       GPIOA
#define             RELAY1_GPIO_PIN                        GPIO_Pin_9

#define             RELAY2_GPIO_xClock_FUN                 RCC_AHB1PeriphClockCmd
#define             RELAY2_GPIO_CLK                        RCC_AHB1Periph_GPIOA
#define             RELAY2_GPIO_PORT                       GPIOA
#define             RELAY2_GPIO_PIN                        GPIO_Pin_5

#define             RELAY3_GPIO_xClock_FUN                 RCC_AHB1PeriphClockCmd
#define             RELAY3_GPIO_CLK                        RCC_AHB1Periph_GPIOA
#define             RELAY3_GPIO_PORT                       GPIOA
#define             RELAY3_GPIO_PIN                        GPIO_Pin_6

#define             RELAY4_GPIO_xClock_FUN                 RCC_AHB1PeriphClockCmd
#define             RELAY4_GPIO_CLK                        RCC_AHB1Periph_GPIOA
#define             RELAY4_GPIO_PORT                       GPIOA
#define             RELAY4_GPIO_PIN                        GPIO_Pin_7

#define             RELAY5_GPIO_xClock_FUN                 RCC_AHB1PeriphClockCmd
#define             RELAY5_GPIO_CLK                        RCC_AHB1Periph_GPIOB
#define             RELAY5_GPIO_PORT                       GPIOB
#define             RELAY5_GPIO_PIN                        GPIO_Pin_12

#define             RELAY6_GPIO_xClock_FUN                 RCC_AHB1PeriphClockCmd
#define             RELAY6_GPIO_CLK                        RCC_AHB1Periph_GPIOB
#define             RELAY6_GPIO_PORT                       GPIOB
#define             RELAY6_GPIO_PIN                        GPIO_Pin_13

#define             RELAY7_GPIO_xClock_FUN                 RCC_AHB1PeriphClockCmd
#define             RELAY7_GPIO_CLK                        RCC_AHB1Periph_GPIOB
#define             RELAY7_GPIO_PORT                       GPIOB
#define             RELAY7_GPIO_PIN                        GPIO_Pin_14

#define             RELAY8_GPIO_xClock_FUN                 RCC_AHB1PeriphClockCmd
#define             RELAY8_GPIO_CLK                        RCC_AHB1Periph_GPIOB
#define             RELAY8_GPIO_PORT                       GPIOB
#define             RELAY8_GPIO_PIN                        GPIO_Pin_15




/****************************** 继电器 函数宏定义***************************************/
#define             RELAY1_OFF()                            GPIO_ResetBits ( RELAY1_GPIO_PORT, RELAY1_GPIO_PIN )
#define             RELAY1_ON()                           GPIO_SetBits ( RELAY1_GPIO_PORT, RELAY1_GPIO_PIN )
#define             RELAY1_TOGGLE()                        GPIO_ReadOutputDataBit ( RELAY1_GPIO_PORT, RELAY1_GPIO_PIN ) ? \
                                                            GPIO_ResetBits ( RELAY1_GPIO_PORT, RELAY1_GPIO_PIN ) : GPIO_SetBits ( RELAY1_GPIO_PORT, RELAY1_GPIO_PIN )

#define             RELAY2_OFF()                            GPIO_ResetBits ( RELAY2_GPIO_PORT, RELAY2_GPIO_PIN )
#define             RELAY2_ON()                           GPIO_SetBits ( RELAY2_GPIO_PORT, RELAY2_GPIO_PIN )
#define             RELAY2_TOGGLE()                        GPIO_ReadOutputDataBit ( RELAY2_GPIO_PORT, RELAY2_GPIO_PIN ) ? \
                                                            GPIO_ResetBits ( RELAY2_GPIO_PORT, RELAY2_GPIO_PIN ) : GPIO_SetBits ( RELAY2_GPIO_PORT, RELAY2_GPIO_PIN )

#define             RELAY3_OFF()                            GPIO_ResetBits ( RELAY3_GPIO_PORT, RELAY3_GPIO_PIN )
#define             RELAY3_ON()                           GPIO_SetBits ( RELAY3_GPIO_PORT, RELAY3_GPIO_PIN )
#define             RELAY3_TOGGLE()                        GPIO_ReadOutputDataBit ( RELAY3_GPIO_PORT, RELAY3_GPIO_PIN ) ? \
                                                            GPIO_ResetBits ( RELAY3_GPIO_PORT, RELAY3_GPIO_PIN ) : GPIO_SetBits ( RELAY3_GPIO_PORT, RELAY3_GPIO_PIN )

#define             RELAY4_OFF()                            GPIO_ResetBits ( RELAY4_GPIO_PORT, RELAY4_GPIO_PIN )
#define             RELAY4_ON()                           GPIO_SetBits ( RELAY4_GPIO_PORT, RELAY4_GPIO_PIN )
#define             RELAY4_TOGGLE()                        GPIO_ReadOutputDataBit ( RELAY4_GPIO_PORT, RELAY4_GPIO_PIN ) ? \
                                                            GPIO_ResetBits ( RELAY4_GPIO_PORT, RELAY4_GPIO_PIN ) : GPIO_SetBits ( RELAY4_GPIO_PORT, RELAY4_GPIO_PIN )

#define             RELAY5_OFF()                            GPIO_ResetBits ( RELAY5_GPIO_PORT, RELAY5_GPIO_PIN )
#define             RELAY5_ON()                           GPIO_SetBits ( RELAY5_GPIO_PORT, RELAY5_GPIO_PIN )
#define             RELAY5_TOGGLE()                        GPIO_ReadOutputDataBit ( RELAY5_GPIO_PORT, RELAY5_GPIO_PIN ) ? \
                                                            GPIO_ResetBits ( RELAY5_GPIO_PORT, RELAY5_GPIO_PIN ) : GPIO_SetBits ( RELAY5_GPIO_PORT, RELAY5_GPIO_PIN )

#define             RELAY6_OFF()                            GPIO_ResetBits ( RELAY6_GPIO_PORT, RELAY6_GPIO_PIN )
#define             RELAY6_ON()                           GPIO_SetBits ( RELAY6_GPIO_PORT, RELAY6_GPIO_PIN )
#define             RELAY6_TOGGLE()                        GPIO_ReadOutputDataBit ( RELAY6_GPIO_PORT, RELAY6_GPIO_PIN ) ? \
                                                            GPIO_ResetBits ( RELAY6_GPIO_PORT, RELAY6_GPIO_PIN ) : GPIO_SetBits ( RELAY6_GPIO_PORT, RELAY6_GPIO_PIN )
 
#define             RELAY7_OFF()                            GPIO_ResetBits ( RELAY7_GPIO_PORT, RELAY7_GPIO_PIN )
#define             RELAY7_ON()                           GPIO_SetBits ( RELAY7_GPIO_PORT, RELAY7_GPIO_PIN )
#define             RELAY7_TOGGLE()                        GPIO_ReadOutputDataBit ( RELAY7_GPIO_PORT, RELAY7_GPIO_PIN ) ? \
                                                            GPIO_ResetBits ( RELAY7_GPIO_PORT, RELAY7_GPIO_PIN ) : GPIO_SetBits ( RELAY7_GPIO_PORT, RELAY7_GPIO_PIN )

#define             RELAY8_OFF()                            GPIO_ResetBits ( RELAY8_GPIO_PORT, RELAY8_GPIO_PIN )
#define             RELAY8_ON()                           GPIO_SetBits ( RELAY8_GPIO_PORT, RELAY8_GPIO_PIN )
#define             RELAY8_TOGGLE()                        GPIO_ReadOutputDataBit ( RELAY8_GPIO_PORT, RELAY8_GPIO_PIN ) ? \
                                                           GPIO_ResetBits ( RELAY8_GPIO_PORT, RELAY8_GPIO_PIN ) : GPIO_SetBits ( RELAY8_GPIO_PORT, RELAY8_GPIO_PIN )
/****************************** LED 函数声明 ***************************************/
void                               Relay_Init                         ( void );
 
void CarSpeedSet(int8_t Speed);









#endif
