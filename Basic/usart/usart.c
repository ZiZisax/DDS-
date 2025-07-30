#include "usart.h"

/*串口1接收储存*/
uint8_t HMI_DMA_RX_BUF[HMI_DMA_REC_LEN]= {0};
/*单次接收数据长度*/
uint8_t HMI_DMA_RX_LEN=0;
/*串口1接收完成标志位*/
uint8_t HMI_Receive_FLAG=0; /*0:空 1：接收完成 2：接收错误*/
uint8_t	Freq_Output_MODE=0;	/*0:空 1：固定频率输出模式 2：扫频模式*/

//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE {
	int handle; 
}; 
FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void	_sys_exit(int x){ 
	x = x; 
} 
/**
* @brief  重定向c库函数printf到串口，重定向后可使用printf函数
* @param  无
* @retval 无
*/
int fputc(int ch, FILE *f){      
	while((DEBUG_USARTx->SR&0X40)==0){};//循环发送,直到发送完毕   
    DEBUG_USARTx->DR = (u8) ch;      
	return ch;
}
#endif
/**
  * @brief  重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
  * @param  无
  * @retval 无
  */
int fgetc(FILE *f)
{
		/* 等待串口输入数据 */
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(DEBUG_USARTx);
}








	/**
  * @brief  USART1的DMA初始化设置
  * @param  无
  * @retval 无
  */
static void USART3_DMA_Init(void)
{ 
	/*定义DMA初始化结构体*/
	DMA_InitTypeDef DMA_InitStructure;
  /*使能DMA时钟（用于串口1数据的传送）*/
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  /*复位DMA1通道5 //要用谁就复位谁*/
	DMA_DeInit(DMA1_Channel3);
	
	/*定义 DMA通道外设基地址(强制装换为32位地址)*/
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART3->DR); 
	/*设置DMA读取的数据宽度（定义外设数据宽度8位）*/
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	/*当前外设寄存器地址是否递增（是否自动加1）*/
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	
	/*这一步就是在告诉DMA要把数据放到什么地方！(强制装换为32位地址)*/
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&HMI_DMA_RX_BUF;
	/*设置存储器数据宽度（定义DMA存储数据宽度8位）*/
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	
	/*当前DMA存储器的地址是否递增（是否自动加1）*/
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	
	/*//指定外设为源地址(传输方向)*/
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	/*定义DMA缓冲区大小，根据DMA需要读取的数据量决定*/
	DMA_InitStructure.DMA_BufferSize = HMI_DMA_REC_LEN;
	/*DMA通道操作模式为普通模式(不循环接收) */
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; 
	 	
	/*DMA通道优先级高*/
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	/*禁止DMA通道存储器到存储器传输*/
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	
	/*初始化DMA通道5 把以上所有的配置写入DMA*/
	DMA_Init(DMA1_Channel3, &DMA_InitStructure);
	/*使能DMA通道5*/
	DMA_Cmd(DMA1_Channel3, ENABLE); 
}
 /**
  * @brief  配置嵌套向量中断控制器NVIC
  * @param  无
  * @retval 无
  */
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* 嵌套向量中断控制器组选择 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* 配置USART为中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;
  /* 抢断优先级*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* 子优先级 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* 使能中断 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* 初始化配置NVIC */
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  进行数据处理并关闭DMA
  * @param  无
  * @retval 无
  */
void	Data_Process(void)
{
	/*关闭DMA(为了让DMA重头计数)*/
	DMA_Cmd(DMA1_Channel3, DISABLE);	
	/*重新设置DMA_Buffersize的大小*/
	DMA1_Channel3->CNDTR = HMI_DMA_REC_LEN; 
	/*开启DMA*/
	DMA_Cmd(DMA1_Channel3, ENABLE); 
}

	/**
  * @brief  USART1中断函数
  * @param  无
  * @retval 无
  */
void DEBUG_USART_IRQHandler(void)
{	
	if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET) //USART_IT_RXNE
	{   
			
			/*得到一次数据的长度*/
	    HMI_DMA_RX_LEN=HMI_DMA_REC_LEN - DMA_GetCurrDataCounter(DMA1_Channel3);
			/*接收完一次数据标志*/
		if(HMI_DMA_RX_LEN==9 )
		{		
			/*接收完一次数据*/
			Freq_Output_MODE=1;
		}
		else if(HMI_DMA_RX_LEN==18)
		{
			/*接收完一次数据*/
			Freq_Output_MODE=2;	
		}
		else if(HMI_DMA_RX_LEN==3 )
		{
			/*接收完一次数据*/
			Freq_Output_MODE=3;	
		}
		Data_Process();
	}
		
		/*清除标志位（只能用读取寄存器清除）*/
		USART3->SR;
    USART3->DR;
}



 /**
  * @brief  USART GPIO 配置,工作参数配置
  * @param  无
  * @retval 无
  */
void USART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// 打开串口GPIO的时钟
	DEBUG_USART_GPIO_APBxClkCmd(DEBUG_USART_GPIO_CLK, ENABLE);
	
	// 打开串口外设的时钟
	DEBUG_USART_APBxClkCmd(DEBUG_USART_CLK, ENABLE);

	// 将USART Tx的GPIO配置为推挽复用模式
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  // 将USART Rx的GPIO配置为浮空输入模式
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	
	// 配置串口的工作参数
	// 配置波特率
	USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
	// 配置 针数据字长
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// 配置停止位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// 配置校验位
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// 配置硬件流控制
	USART_InitStructure.USART_HardwareFlowControl = 
	USART_HardwareFlowControl_None;
	// 配置工作模式，收发一起
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// 完成串口的初始化配置
	USART_Init(DEBUG_USARTx, &USART_InitStructure);
	
		//串口中断优先级配置
		NVIC_Configuration();
		//DMA配置
		USART3_DMA_Init();
		/*接收DMA使能*/
		USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);
		/*接收空闲中断使能*/
		USART_ITConfig(DEBUG_USARTx, USART_IT_IDLE, ENABLE);	//USART_IT_RXNE
	
	//使能串口
	USART_Cmd(DEBUG_USARTx, ENABLE);	    
}

/*****************  发送一个字节 **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* 发送一个字节数据到USART */
	USART_SendData(pUSARTx,ch);
		
	/* 等待发送数据寄存器为空 */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

/****************** 发送8位的数组 ************************/
void Usart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num)
{
  uint8_t i;
	
	for(i=0; i<num; i++)
  {
	    /* 发送一个字节数据到USART */
	    Usart_SendByte(pUSARTx,array[i]);	
  
  }
	/* 等待发送完成 */
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
}

/*****************  发送字符串 **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
  do 
  {
      Usart_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  
  /* 等待发送完成 */
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
  {}
}

/*****************  发送一个16位数 **********************/
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
{
	uint8_t temp_h, temp_l;
	
	/* 取出高八位 */
	temp_h = (ch&0XFF00)>>8;
	/* 取出低八位 */
	temp_l = ch&0XFF;
	
	/* 发送高八位 */
	USART_SendData(pUSARTx,temp_h);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
	
	/* 发送低八位 */
	USART_SendData(pUSARTx,temp_l);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}



