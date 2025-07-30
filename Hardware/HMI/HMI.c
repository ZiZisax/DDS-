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
  * @brief  ������յ�������
  * @param  ��
  * @retval ��
  */
void DataPack_Process(void)
{
	/*�յ������������Ĺ̶�ģʽ���ָ��*/
	if(Freq_Output_MODE==1)
		{	
			Freq_Output_MODE=0;
			
			/*�յ�9���ֽڵ�����*/
			if(HMI_DMA_RX_LEN == 9)
			{
				/*Ƶ��֡ͷ��7E    Ƶ��֡β��EF    ��λ֡ͷ��9E  ��λ֡β��FC          */
				if((HMI_DMA_RX_BUF[0]==0x7e) && (HMI_DMA_RX_BUF[5]==0xef) && (HMI_DMA_RX_BUF[6]==0x9e) && (HMI_DMA_RX_BUF[8]==0xfc))
				{
					LED(ON); // ����LED
					delay_ms(125);// ��ʱ0.125s
					LED(OFF); // Ϩ��LED
					delay_ms(125);// ��ʱ0.125s
					
					u32 Freq=(HMI_DMA_RX_BUF[4]<<24) |\
						 (HMI_DMA_RX_BUF[3]<<16) |\
						 (HMI_DMA_RX_BUF[2]<<8)  |\
						 (HMI_DMA_RX_BUF[1]<<0);
					AD9851_WR_SERIAL(0x01,Freq);
					
//					printf("Freq=%d\r\n",Freq);
					
					/*��������*/
					memset(HMI_DMA_RX_BUF, 0, sizeof(HMI_DMA_RX_BUF));
					
					printf("Fixed.b2.txt=\"OK\"\xff\xff\xff");
					
					delay_ms(1000);
					printf("Fixed.b2.txt=\"Wait\"\xff\xff\xff");
					
					for(int i = 0; i < 16; i++)
					{
						LED(ON); // ����LED
						delay_ms(125);// ��ʱ0.125s
						LED(OFF); // Ϩ��LED
						delay_ms(125);// ��ʱ0.125s
					}
				}
			}
		}
		else if(Freq_Output_MODE==2)
		{
			Freq_Output_MODE=0;
			
			/*�յ�9���ֽڵ�����*/
			if(HMI_DMA_RX_LEN == 18)
			{
				/*Ƶ��֡ͷ��7E    Ƶ��֡β��EF         */
				if((HMI_DMA_RX_BUF[0]==0x7e) && (HMI_DMA_RX_BUF[17]==0xef))
				{
					LED(ON); // ����LED
					delay_ms(500);// ��ʱ0.125s
					LED(OFF); // Ϩ��LED
					delay_ms(500);// ��ʱ0.125s
					
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
					

					/*��������*/
					memset(HMI_DMA_RX_BUF, 0, sizeof(HMI_DMA_RX_BUF));
					
					for(int i = 0; i < 4; i++)
					{
						LED(ON); // ����LED
						delay_ms(500);// ��ʱ0.125s
						LED(OFF); // Ϩ��LED
						delay_ms(500);// ��ʱ0.125s
					}
			    }
		   }
		}
		else if(Freq_Output_MODE==3)
		{
			Freq_Output_MODE=0;
			
			/*�յ�9���ֽڵ�����*/
			if(HMI_DMA_RX_LEN == 3)
			{
				
				/*Ƶ��֡ͷ��8E    Ƶ��֡β��FD*/
				if((HMI_DMA_RX_BUF[0]==0x8e) && (HMI_DMA_RX_BUF[1]==0xff) && (HMI_DMA_RX_BUF[2]==0xfd))
				{
					LED(ON); // ����LED
					delay_ms(1000);// ��ʱ0.125s
					LED(OFF); // Ϩ��LED
					delay_ms(1000);// ��ʱ0.125s
					
					HMI_SineWave();
					for(int i = 0; i < 8; i++)
					{
						LED(ON); // ����LED
						delay_ms(250);// ��ʱ0.125s
						LED(OFF); // Ϩ��LED
						delay_ms(250);// ��ʱ0.125s
					}
					/*��������*/
					memset(HMI_DMA_RX_BUF, 0, sizeof(HMI_DMA_RX_BUF));
				}
			}
		}
} 

 /**
  * @brief  �ڴ���������ʾ���Ҳ�
	add  �����߿ؼ��������

	add objid,ch,val

	objid:���߿ؼ�ID���(�˴�������ID�ţ���֧��ʹ�ÿؼ�����)

	ch:���߿ؼ�ͨ����

	val:���� (���255����С0)
  * @param  ��
  * @retval ��
  */
void	HMI_SineWave(void)
{
	// ���Ҳ����ݵ�����
	// Y��ƫ����
	// �洢���Ҳ�����
	 /*�������Ҳ�����*/
	for(int k=0;k<360;k++)
	{
		int y=125.0*(1+sin(2*k*3.14/360));
		printf("add 5,0,");
		/* �������߿ؼ�����*/
		printf("%d\xff\xff\xff",y);
		delay_ms(10);
	}
}

/**
  * @brief  ɨƵ
  * @param  start_freq : ɨƵ��ʼƵ��
			end_freq   : ɨƵ����Ƶ��
			step_freq  : ɨƵ����Ƶ��
			delay_time : ÿ��Ƶ��ͣ��ʱ��
  * @retval ��
  */
void	AD9851_FreqScan(double start_freq,double end_freq,double step_freq,int delay_time)
{
	int	num=(end_freq-start_freq)/step_freq+1;
	for (double freq = start_freq; freq <= end_freq; freq += step_freq) 
	{
		/*����AD9851���Ƶ��*/
		AD9851_WR_SERIAL(0x01,freq); 
		/*�ӳ�ͣ��ʱ��*/
		delay_ms(delay_time);
		
		printf("wave.n3.val=%d\xff\xff\xff",(int)freq);
		delay_ms(10);
	}
}



