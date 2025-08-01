#include "delay.h"


#define AHB_INPUT  72  //请按RCC中设置的AHB时钟频率填写到这里（单位MHz）


void delay_us(u32 uS){ //nS微秒级延时程序（参考值即是延时数，72MHz时最大值233015）	
  SysTick->LOAD=AHB_INPUT*uS;      //重装计数初值（当主频是72MHz，72次为1微秒）
   SysTick->CTRL=0x00000005;//时钟源HCLK，打开定时器
   while(!(SysTick->CTRL&0x00010000)); //等待计数到0
   SysTick->CTRL=0x00000004;//关闭定时器
}

void delay_ms(u16 ms){ //mS毫秒级延时程序（参考值即是延时数，最大值65535）	 		  	  
	while( ms-- != 0){
		delay_us(1000);	//调用1000微秒的延时
	}
}
 
void delay_s(u16 s){ //S秒级延时程序（参考值即是延时数，最大值65535）	 		  	  
	while( s-- != 0){
		delay_ms(1000);	//调用1000毫秒的延时
	}
} 

