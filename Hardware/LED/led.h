#ifndef __LED_H
#define __LED_H	 

#include "stm32f10x.h"

/* ����LED���ӵ�GPIO�˿�, �û�ֻ��Ҫ�޸�����Ĵ��뼴�ɸı���Ƶ�LED���� */
#define LED_GPIO_PORT    	GPIOA			              /* GPIO�˿� */
#define LED_GPIO_CLK 	    RCC_APB2Periph_GPIOA		/* GPIO�˿�ʱ�� */
#define LED_GPIO_PIN		  GPIO_Pin_11			        /* ���ӵ���������GPIO */

/* �͵�ƽʱ��LED�� */
#define ON  1
#define OFF 0

/* ���κ꣬��������������һ��ʹ�� */
#define LED(a)	if (a)	\
					GPIO_ResetBits(LED_GPIO_PORT,LED_GPIO_PIN);\
					else		\
					GPIO_SetBits(LED_GPIO_PORT,LED_GPIO_PIN)

void LED_GPIO_Config(void);
		 				    
#endif /* __LED_H*/
