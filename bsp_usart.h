#ifndef _BSP_USART_H
#define _BSP_USART_H
#include "stm32f10x.h"
#include <stdio.h>
//串口1
#define DEBUG_USARTx                 USART1
#define DEBUG_USART_CLK              RCC_APB2Periph_USART1
#define DEBUG_USART_APBxClkCmd       RCC_APB2PeriphClockCmd
#define  DEBUG_USART_BAUDRATE        115200
//usart引脚定义
#define DEBUG_USART_GPIO_CLK         (RCC_APB2Periph_GPIOA)
#define DEBUG_USART_GPIO_APBxClkcmd   RCC_APB2PeriphClockcmd
#define DEBUG_USART_TX_GPIO_PORT      GPIOA
#define DEBUG_USART_TX_GPIO_PIN       GPIO_Pin_9
#define DEBUG_USART_RX_GPIO_PORT      GPIOA
#define DEBUG_UASRT_RX_GPIO_PIN       GPIO_Pin_10

#define DEBUG_USART_IRQ               USART1_IRQn
#define DEBUG_USART_IRQHandler        USART1_IQHandler
void USART_Config(void );
void ADC_SEND(u16 ADC_ConvertedValue );
void Usart_SendBB(USART_TypeDef*pUSARTx,uint8_t data);
void USART_IRQHandler(void);
void Usart_sendhalfword(USART_TypeDef* pUSARTx, u16 Data);
void Usart_Sendstr(USART_TypeDef* pUSARTx, uint8_t *str);
void Usart_Sendarray(USART_TypeDef* pUSARTx, uint8_t *arr,uint8_t num);
u8 Check_Sum( u8* buf ,u16 len);

int fputc(int ch,FILE*f);
#endif


