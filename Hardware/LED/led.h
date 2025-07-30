#ifndef __LED_H
#define __LED_H	 

#include "stm32f10x.h"

/* 定义LED连接的GPIO端口, 用户只需要修改下面的代码即可改变控制的LED引脚 */
#define LED_GPIO_PORT    	GPIOA			              /* GPIO端口 */
#define LED_GPIO_CLK 	    RCC_APB2Periph_GPIOA		/* GPIO端口时钟 */
#define LED_GPIO_PIN		  GPIO_Pin_11			        /* 连接到蜂鸣器的GPIO */

/* 低电平时，LED亮 */
#define ON  1
#define OFF 0

/* 带参宏，可以像内联函数一样使用 */
#define LED(a)	if (a)	\
					GPIO_ResetBits(LED_GPIO_PORT,LED_GPIO_PIN);\
					else		\
					GPIO_SetBits(LED_GPIO_PORT,LED_GPIO_PIN)

void LED_GPIO_Config(void);
		 				    
#endif /* __LED_H*/
