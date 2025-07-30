#ifndef __USART_H
#define	__USART_H
#include "stm32f10x.h"
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "stdio.h"

 
/**********************串口3-USART3*************************/
#define  DEBUG_USARTx                   USART3
#define  DEBUG_USART_CLK                RCC_APB1Periph_USART3
#define  DEBUG_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
#define  DEBUG_USART_BAUDRATE           9600

// USART GPIO 引脚宏定义
#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOB)
#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  DEBUG_USART_TX_GPIO_PORT       GPIOB   
#define  DEBUG_USART_TX_GPIO_PIN        GPIO_Pin_10
#define  DEBUG_USART_RX_GPIO_PORT       GPIOB
#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_11

#define  DEBUG_USART_IRQ                USART3_IRQn
#define  DEBUG_USART_IRQHandler         USART3_IRQHandler

/*DMA最大缓冲长度*/
#define HMI_DMA_REC_LEN    32
/*串口1接收储存*/
extern uint8_t HMI_DMA_RX_BUF[HMI_DMA_REC_LEN];
/*单次接收数据长度*/
extern uint8_t HMI_DMA_RX_LEN;
/*串口1接收完成标志位*/
extern uint8_t HMI_Receive_FLAG; /*0:空 1：接收完成 2：接收错误*/
/*AD9851频率输出模式*/
extern uint8_t	Freq_Output_MODE;	/*0:空 1：固定频率输出模式 2：扫频模式*/

void USART_Config(void);
/* 发送8位的数组 */
void Usart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num);
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void Usart_SendString( USART_TypeDef * pUSARTx, char *str);
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch);

#endif /* __USART_H */
