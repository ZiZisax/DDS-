#include	"AD9851.h"
#include	"stdio.h"
#include 	"delay.h"

/*AD9851����6��Ƶ,AD9850��֧�ֱ�Ƶ��AD9851֧�ֱ�Ƶ*/
u8 AD9851_FD=0x01; 

/*AD9851��ʼ��*/
void	AD9851_Init(void)
{
	/*IO�˿ڳ�ʼ��*/
	AD9851_GPIO_Config();
	/*AD9851ϵͳ��λ*/
	AD9851_RESET_SERIAL();
}

/**
  * @brief  ��ʼ������AD9851��IO
  * @param  ��
  * @retval ��
  */
void	AD9851_GPIO_Config(void)
{
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*��������AD9851��GPIO�Ķ˿�ʱ��*/
	RCC_APB2PeriphClockCmd( FQ_UD_GPIO_CLK | W_CLK_GPIO_CLK | DATA_GPIO_CLK | RST_GPIO_CLK, ENABLE); 
	
	/*ʹ��GPIOBʱ�Ӻ͸��ù��ܣ�Ҫ�ȴ򿪸��ò����޸ĸ��ù��ܣ�*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE); 

	/*Ҫ�ȿ�ʱ�ӣ�����ӳ�䣻����ʾ�ر�JTAG��ʹ��SWD��*/
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);      
	
	/*����GPIOģʽΪͨ���������*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

	/*����GPIO����Ϊ50MHz */   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	/*ѡ��Ҫ����AD9851��GPIO*/															   
	GPIO_InitStructure.GPIO_Pin = FQ_UD_GPIO_PIN;	

	/*���ÿ⺯������ʼ������AD9851��GPIO*/
	GPIO_Init(FQ_UD_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = W_CLK_GPIO_PIN;
	GPIO_Init(W_CLK_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = DATA_GPIO_PIN;
	GPIO_Init(DATA_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = RST_GPIO_PIN;
	GPIO_Init(RST_GPIO_PORT, &GPIO_InitStructure);		
}

/**
  * @brief  AD9851ϵͳ��λ
  * @param  ��
  * @retval ��
  */
void	AD9851_RESET_SERIAL(void)
{
	W_CLK_LOW;
	FQ_UD_LOW;
	
	/*RST�ź�*/
	RST_LOW;
	RST_HIGH;
	RST_LOW;
	
	/*W_CLK�ź�*/
	W_CLK_LOW;
	W_CLK_HIGH;
	W_CLK_LOW;
	
	/*FQ_UD�ź�*/
	FQ_UD_LOW;
	FQ_UD_HIGH;
	FQ_UD_LOW;
}

/**
  * @brief  ��AD9851�д���д�����������
  * @param  w0 :
			frequense : Ƶ��
  * @retval ��
  */
void	AD9851_WR_SERIAL(u8 w0,double frequence)
	
{
	u8 i,w;
	long int y;
	
	/*��frequence��2^32/180000000����*/
	frequence=frequence*4294967295/180000000;
	y=frequence;
	
	/*дW4����*/
	w=(y>>0);
	for(i=0;i<8;i++)
	{
		if((w>>i)&0x01)
			DATA_UD_HIGH;
		else DATA_UD_LOW;
		W_CLK_HIGH;
		W_CLK_LOW;
	}
	
	/*дW3����*/
	w=(y>>8);
	for(i=0;i<8;i++)
	{
		if((w>>i)&0x01)
			DATA_UD_HIGH;
		else DATA_UD_LOW;
		W_CLK_HIGH;
		W_CLK_LOW;
	}
	
	/*дW2����*/
	w=(y>>16);
	for(i=0;i<8;i++)
	{
		if((w>>i)&0x01)
			DATA_UD_HIGH;
		else DATA_UD_LOW;
		W_CLK_HIGH;
		W_CLK_LOW;
	}
	
	/*дW1����*/
	w=(y>>24);
	for(i=0;i<8;i++)
	{
		if((w>>i)&0x01)
			DATA_UD_HIGH;
		else DATA_UD_LOW;
		W_CLK_HIGH;
		W_CLK_LOW;
	}
	
	/*дW0���ݣ�1/(2^5)*360��=11.25��*/
	w=w0;
	for(i=0;i<8;i++)
	{
		if((w>>i)&0x01)
			DATA_UD_HIGH;
		else DATA_UD_LOW;
		W_CLK_HIGH;
		W_CLK_LOW;
	}
	
	/*����ʹ��*/
	FQ_UD_HIGH;
	FQ_UD_LOW;
}

/**
  * @brief  Ƶ�����
  * @param  freq : Ƶ��
  * @retval ��
  */
void AD9851_SetFreq(double freq)
{
	AD9851_WR_SERIAL(AD9851_FD,freq);
}	












