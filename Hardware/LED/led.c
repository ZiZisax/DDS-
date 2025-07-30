#include "led.h"

/**
  * @brief  初始化控制LED的IO
  * @param  无
  * @retval 无
  */
void LED_GPIO_Config(void)
{		
		/*定义一个GPIO_InitTypeDef类型的结构体*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*开启控制LED的GPIO的端口时钟*/
		RCC_APB2PeriphClockCmd( LED_GPIO_CLK, ENABLE); 

		/*选择要控制LED的GPIO*/															   
		GPIO_InitStructure.GPIO_Pin = LED_GPIO_PIN;	

		/*设置GPIO模式为通用推挽输出*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

		/*设置GPIO速率为50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*调用库函数，初始化控制LED的GPIO*/
		GPIO_Init(LED_GPIO_PORT, &GPIO_InitStructure);			 
    
		/* 关闭LED*/
		GPIO_SetBits(LED_GPIO_PORT, LED_GPIO_PIN);	 
}
