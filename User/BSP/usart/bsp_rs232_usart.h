#ifndef __RS232_USART_H
#define	__RS232_USART_H

#include "stm32f4xx.h"
#include <stdio.h>



//����2�����Ŷ���
/*******************************************************/
#define RS232_USART                             USART2
#define RS232_USART_CLK                         RCC_APB1Periph_USART2


#define RS232_USART_RX_GPIO_PORT                GPIOD
#define RS232_USART_RX_GPIO_CLK                 RCC_AHB1Periph_GPIOD
#define RS232_USART_RX_PIN                      GPIO_Pin_6
#define RS232_USART_RX_AF                       GPIO_AF_USART2
#define RS232_USART_RX_SOURCE                   GPIO_PinSource6

#define RS232_USART_TX_GPIO_PORT                GPIOD
#define RS232_USART_TX_GPIO_CLK                 RCC_AHB1Periph_GPIOD
#define RS232_USART_TX_PIN                      GPIO_Pin_5
#define RS232_USART_TX_AF                       GPIO_AF_USART2
#define RS232_USART_TX_SOURCE                   GPIO_PinSource5

//����1�����Ŷ���
/*******************************************************/
#define RS232_USART1                             USART1
#define RS232_USART1_CLK                         RCC_APB2Periph_USART1

#define RS232_USART1_RX_GPIO_PORT                GPIOA
#define RS232_USART1_RX_GPIO_CLK                 RCC_AHB1Periph_GPIOA
#define RS232_USART1_RX_PIN                      GPIO_Pin_10
#define RS232_USART1_RX_AF                       GPIO_AF_USART1
#define RS232_USART1_RX_SOURCE                   GPIO_PinSource10

#define RS232_USART1_TX_GPIO_PORT                GPIOA
#define RS232_USART1_TX_GPIO_CLK                 RCC_AHB1Periph_GPIOA
#define RS232_USART1_TX_PIN                      GPIO_Pin_9
#define RS232_USART1_TX_AF                       GPIO_AF_USART1
#define RS232_USART1_TX_SOURCE                   GPIO_PinSource9




#define RS232_USART_IRQHandler                   USART2_IRQHandler
#define RS232_USART111_IRQHandler                USART1_IRQHandler

#define RS232_USART_IRQ                 					USART2_IRQn
#define RS232_USART1_IRQ                 					USART1_IRQn
/************************************************************/


//���ڲ�����
#define RS232_USART_BAUDRATE                    115200



void Debug_USART_Config(void);
void Usart_SendStr_length( USART_TypeDef * pUSARTx, uint8_t *str,uint32_t strlen );
void Usart_SendString( USART_TypeDef * pUSARTx, uint8_t *str);

//int fputc(int ch, FILE *f);

#endif /* __USART1_H */
