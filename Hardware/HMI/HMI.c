#include 	"HMI.h"

#include    "usart.h"
#include	"AD9851.h"
#include 	"led.h"
#include 	"delay.h"
#include    "stdio.h"
#include    "string.h"
#include	"math.h"

u8	End_CMD[3]={0XFF,0XFF,0XFF};

 /**
  * @brief  处理接收到的数据
  * @param  无
  * @retval 无
  */
void DataPack_Process(void)
{
	/*收到串口屏发来的固定模式输出指令*/
	if(Freq_Output_MODE==1)
		{	
			Freq_Output_MODE=0;
			
			/*收到9个字节的数据*/
			if(HMI_DMA_RX_LEN == 9)
			{
				/*频率帧头：7E    频率帧尾：EF    相位帧头：9E  相位帧尾：FC          */
				if((HMI_DMA_RX_BUF[0]==0x7e) && (HMI_DMA_RX_BUF[5]==0xef) && (HMI_DMA_RX_BUF[6]==0x9e) && (HMI_DMA_RX_BUF[8]==0xfc))
				{
					LED(ON); // 点亮LED
					delay_ms(125);// 延时0.125s
					LED(OFF); // 熄灭LED
					delay_ms(125);// 延时0.125s
					
					u32 Freq=(HMI_DMA_RX_BUF[4]<<24) |\
						 (HMI_DMA_RX_BUF[3]<<16) |\
						 (HMI_DMA_RX_BUF[2]<<8)  |\
						 (HMI_DMA_RX_BUF[1]<<0);
					AD9851_WR_SERIAL(0x01,Freq);
					
//					printf("Freq=%d\r\n",Freq);
					
					/*清零数组*/
					memset(HMI_DMA_RX_BUF, 0, sizeof(HMI_DMA_RX_BUF));
					
					printf("Fixed.b2.txt=\"OK\"\xff\xff\xff");
					
					delay_ms(1000);
					printf("Fixed.b2.txt=\"Wait\"\xff\xff\xff");
					
					for(int i = 0; i < 16; i++)
					{
						LED(ON); // 点亮LED
						delay_ms(125);// 延时0.125s
						LED(OFF); // 熄灭LED
						delay_ms(125);// 延时0.125s
					}
				}
			}
		}
		else if(Freq_Output_MODE==2)
		{
			Freq_Output_MODE=0;
			
			/*收到9个字节的数据*/
			if(HMI_DMA_RX_LEN == 18)
			{
				/*频率帧头：7E    频率帧尾：EF         */
				if((HMI_DMA_RX_BUF[0]==0x7e) && (HMI_DMA_RX_BUF[17]==0xef))
				{
					LED(ON); // 点亮LED
					delay_ms(500);// 延时0.125s
					LED(OFF); // 熄灭LED
					delay_ms(500);// 延时0.125s
					
					u32 start_freq=(HMI_DMA_RX_BUF[4]<<24) |\
								   (HMI_DMA_RX_BUF[3]<<16) |\
								   (HMI_DMA_RX_BUF[2]<<8)  |\
								   (HMI_DMA_RX_BUF[1]<<0);
					
//					printf("start_freq=%d\r\n",start_freq);

					u32 end_freq=(HMI_DMA_RX_BUF[8]<<24) |\
								 (HMI_DMA_RX_BUF[7]<<16) |\
								 (HMI_DMA_RX_BUF[6]<<8)  |\
								 (HMI_DMA_RX_BUF[5]<<0);
//					printf("end_freq=%d\r\n",end_freq);

					u32 step_freq=(HMI_DMA_RX_BUF[12]<<24) |\
								  (HMI_DMA_RX_BUF[11]<<16) |\
							      (HMI_DMA_RX_BUF[10]<<8)  |\
							      (HMI_DMA_RX_BUF[9]<<0);
//					printf("step_freq=%d\r\n",step_freq);
								  

					u32 delay_time=(HMI_DMA_RX_BUF[16]<<24) |\
							       (HMI_DMA_RX_BUF[15]<<16) |\
								   (HMI_DMA_RX_BUF[14]<<8)  |\
							       (HMI_DMA_RX_BUF[13]<<0);
//					printf("delay_time=%d\r\n",delay_time);
								   
					AD9851_FreqScan(start_freq,end_freq,step_freq,delay_time);		   
					

					/*清零数组*/
					memset(HMI_DMA_RX_BUF, 0, sizeof(HMI_DMA_RX_BUF));
					
					for(int i = 0; i < 4; i++)
					{
						LED(ON); // 点亮LED
						delay_ms(500);// 延时0.125s
						LED(OFF); // 熄灭LED
						delay_ms(500);// 延时0.125s
					}
			    }
		   }
		}
		else if(Freq_Output_MODE==3)
		{
			Freq_Output_MODE=0;
			
			/*收到9个字节的数据*/
			if(HMI_DMA_RX_LEN == 3)
			{
				
				/*频率帧头：8E    频率帧尾：FD*/
				if((HMI_DMA_RX_BUF[0]==0x8e) && (HMI_DMA_RX_BUF[1]==0xff) && (HMI_DMA_RX_BUF[2]==0xfd))
				{
					LED(ON); // 点亮LED
					delay_ms(1000);// 延时0.125s
					LED(OFF); // 熄灭LED
					delay_ms(1000);// 延时0.125s
					
					HMI_SineWave();
					for(int i = 0; i < 8; i++)
					{
						LED(ON); // 点亮LED
						delay_ms(250);// 延时0.125s
						LED(OFF); // 熄灭LED
						delay_ms(250);// 延时0.125s
					}
					/*清零数组*/
					memset(HMI_DMA_RX_BUF, 0, sizeof(HMI_DMA_RX_BUF));
				}
			}
		}
} 

 /**
  * @brief  在串口屏上显示正弦波
	add  往曲线控件添加数据

	add objid,ch,val

	objid:曲线控件ID序号(此处必须是ID号，不支持使用控件名称)

	ch:曲线控件通道号

	val:数据 (最大255，最小0)
  * @param  无
  * @retval 无
  */
void	HMI_SineWave(void)
{
	// 正弦波数据点数量
	// Y轴偏移量
	// 存储正弦波数据
	 /*生成正弦波数据*/
	for(int k=0;k<360;k++)
	{
		int y=125.0*(1+sin(2*k*3.14/360));
		printf("add 5,0,");
		/* 发送曲线控件数据*/
		printf("%d\xff\xff\xff",y);
		delay_ms(10);
	}
}

/**
  * @brief  扫频
  * @param  start_freq : 扫频起始频率
			end_freq   : 扫频结束频率
			step_freq  : 扫频步进频率
			delay_time : 每个频率停留时间
  * @retval 无
  */
void	AD9851_FreqScan(double start_freq,double end_freq,double step_freq,int delay_time)
{
	int	num=(end_freq-start_freq)/step_freq+1;
	for (double freq = start_freq; freq <= end_freq; freq += step_freq) 
	{
		/*设置AD9851输出频率*/
		AD9851_WR_SERIAL(0x01,freq); 
		/*延迟停留时间*/
		delay_ms(delay_time);
		
		printf("wave.n3.val=%d\xff\xff\xff",(int)freq);
		delay_ms(10);
	}
}



