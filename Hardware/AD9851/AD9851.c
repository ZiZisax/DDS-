#include	"AD9851.h"
#include	"stdio.h"
#include 	"delay.h"

/*AD9851启动6倍频,AD9850不支持倍频，AD9851支持倍频*/
u8 AD9851_FD=0x01; 

/*AD9851初始化*/
void	AD9851_Init(void)
{
	/*IO端口初始化*/
	AD9851_GPIO_Config();
	/*AD9851系统复位*/
	AD9851_RESET_SERIAL();
}

/**
  * @brief  初始化控制AD9851的IO
  * @param  无
  * @retval 无
  */
void	AD9851_GPIO_Config(void)
{
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*开启控制AD9851的GPIO的端口时钟*/
	RCC_APB2PeriphClockCmd( FQ_UD_GPIO_CLK | W_CLK_GPIO_CLK | DATA_GPIO_CLK | RST_GPIO_CLK, ENABLE); 
	
	/*使能GPIOB时钟和复用功能（要先打开复用才能修改复用功能）*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE); 

	/*要先开时钟，再重映射；这句表示关闭JTAG，使能SWD。*/
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);      
	
	/*设置GPIO模式为通用推挽输出*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

	/*设置GPIO速率为50MHz */   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	/*选择要控制AD9851的GPIO*/															   
	GPIO_InitStructure.GPIO_Pin = FQ_UD_GPIO_PIN;	

	/*调用库函数，初始化控制AD9851的GPIO*/
	GPIO_Init(FQ_UD_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = W_CLK_GPIO_PIN;
	GPIO_Init(W_CLK_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = DATA_GPIO_PIN;
	GPIO_Init(DATA_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = RST_GPIO_PIN;
	GPIO_Init(RST_GPIO_PORT, &GPIO_InitStructure);		
}

/**
  * @brief  AD9851系统复位
  * @param  无
  * @retval 无
  */
void	AD9851_RESET_SERIAL(void)
{
	W_CLK_LOW;
	FQ_UD_LOW;
	
	/*RST信号*/
	RST_LOW;
	RST_HIGH;
	RST_LOW;
	
	/*W_CLK信号*/
	W_CLK_LOW;
	W_CLK_HIGH;
	W_CLK_LOW;
	
	/*FQ_UD信号*/
	FQ_UD_LOW;
	FQ_UD_HIGH;
	FQ_UD_LOW;
}

/**
  * @brief  向AD9851中串行写入命令和数据
  * @param  w0 :
			frequense : 频率
  * @retval 无
  */
void	AD9851_WR_SERIAL(u8 w0,double frequence)
	
{
	u8 i,w;
	long int y;
	
	/*将frequence按2^32/180000000量化*/
	frequence=frequence*4294967295/180000000;
	y=frequence;
	
	/*写W4数据*/
	w=(y>>0);
	for(i=0;i<8;i++)
	{
		if((w>>i)&0x01)
			DATA_UD_HIGH;
		else DATA_UD_LOW;
		W_CLK_HIGH;
		W_CLK_LOW;
	}
	
	/*写W3数据*/
	w=(y>>8);
	for(i=0;i<8;i++)
	{
		if((w>>i)&0x01)
			DATA_UD_HIGH;
		else DATA_UD_LOW;
		W_CLK_HIGH;
		W_CLK_LOW;
	}
	
	/*写W2数据*/
	w=(y>>16);
	for(i=0;i<8;i++)
	{
		if((w>>i)&0x01)
			DATA_UD_HIGH;
		else DATA_UD_LOW;
		W_CLK_HIGH;
		W_CLK_LOW;
	}
	
	/*写W1数据*/
	w=(y>>24);
	for(i=0;i<8;i++)
	{
		if((w>>i)&0x01)
			DATA_UD_HIGH;
		else DATA_UD_LOW;
		W_CLK_HIGH;
		W_CLK_LOW;
	}
	
	/*写W0数据，1/(2^5)*360度=11.25度*/
	w=w0;
	for(i=0;i<8;i++)
	{
		if((w>>i)&0x01)
			DATA_UD_HIGH;
		else DATA_UD_LOW;
		W_CLK_HIGH;
		W_CLK_LOW;
	}
	
	/*移入使能*/
	FQ_UD_HIGH;
	FQ_UD_LOW;
}

/**
  * @brief  频率输出
  * @param  freq : 频率
  * @retval 无
  */
void AD9851_SetFreq(double freq)
{
	AD9851_WR_SERIAL(AD9851_FD,freq);
}	












