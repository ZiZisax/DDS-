#include "led.h"

/**
  * @brief  ��ʼ������LED��IO
  * @param  ��
  * @retval ��
  */
void LED_GPIO_Config(void)
{		
		/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*��������LED��GPIO�Ķ˿�ʱ��*/
		RCC_APB2PeriphClockCmd( LED_GPIO_CLK, ENABLE); 

		/*ѡ��Ҫ����LED��GPIO*/															   
		GPIO_InitStructure.GPIO_Pin = LED_GPIO_PIN;	

		/*����GPIOģʽΪͨ���������*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

		/*����GPIO����Ϊ50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*���ÿ⺯������ʼ������LED��GPIO*/
		GPIO_Init(LED_GPIO_PORT, &GPIO_InitStructure);			 
    
		/* �ر�LED*/
		GPIO_SetBits(LED_GPIO_PORT, LED_GPIO_PIN);	 
}
