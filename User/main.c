#include	"stm32f10x.h"
#include    "rcc.h"
#include	"AD9851.h"
#include    "usart.h"
#include 	"led.h"
#include 	"delay.h"
#include 	"HMI.h"
#include    "stdio.h"

int main(void)
{
	RCC_Configuration();
	/* LED GPIO ≥ı ºªØ */
	LED_GPIO_Config();

	AD9851_Init();

	AD9851_WR_SERIAL(0x01,50000000);//50MHz–≈∫≈
	
  while(1)
	{	
		

	}	
}

