#include "usart.h"

/*����1���մ���*/
uint8_t HMI_DMA_RX_BUF[HMI_DMA_REC_LEN]= {0};
/*���ν������ݳ���*/
uint8_t HMI_DMA_RX_LEN=0;
/*����1������ɱ�־λ*/
uint8_t HMI_Receive_FLAG=0; /*0:�� 1��������� 2�����մ���*/
uint8_t	Freq_Output_MODE=0;	/*0:�� 1���̶�Ƶ�����ģʽ 2��ɨƵģʽ*/

//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE {
	int handle; 
}; 
FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void	_sys_exit(int x){ 
	x = x; 
} 
/**
* @brief  �ض���c�⺯��printf�����ڣ��ض�����ʹ��printf����
* @param  ��
* @retval ��
*/
int fputc(int ch, FILE *f){      
	while((DEBUG_USARTx->SR&0X40)==0){};//ѭ������,ֱ���������   
    DEBUG_USARTx->DR = (u8) ch;      
	return ch;
}
#endif
/**
  * @brief  �ض���c�⺯��scanf�����ڣ���д����ʹ��scanf��getchar�Ⱥ���
  * @param  ��
  * @retval ��
  */
int fgetc(FILE *f)
{
		/* �ȴ������������� */
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(DEBUG_USARTx);
}








	/**
  * @brief  USART1��DMA��ʼ������
  * @param  ��
  * @retval ��
  */
static void USART3_DMA_Init(void)
{ 
	/*����DMA��ʼ���ṹ��*/
	DMA_InitTypeDef DMA_InitStructure;
  /*ʹ��DMAʱ�ӣ����ڴ���1���ݵĴ��ͣ�*/
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  /*��λDMA1ͨ��5 //Ҫ��˭�͸�λ˭*/
	DMA_DeInit(DMA1_Channel3);
	
	/*���� DMAͨ���������ַ(ǿ��װ��Ϊ32λ��ַ)*/
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART3->DR); 
	/*����DMA��ȡ�����ݿ�ȣ������������ݿ��8λ��*/
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	/*��ǰ����Ĵ�����ַ�Ƿ�������Ƿ��Զ���1��*/
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	
	/*��һ�������ڸ���DMAҪ�����ݷŵ�ʲô�ط���(ǿ��װ��Ϊ32λ��ַ)*/
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&HMI_DMA_RX_BUF;
	/*���ô洢�����ݿ�ȣ�����DMA�洢���ݿ��8λ��*/
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	
	/*��ǰDMA�洢���ĵ�ַ�Ƿ�������Ƿ��Զ���1��*/
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	
	/*//ָ������ΪԴ��ַ(���䷽��)*/
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	/*����DMA��������С������DMA��Ҫ��ȡ������������*/
	DMA_InitStructure.DMA_BufferSize = HMI_DMA_REC_LEN;
	/*DMAͨ������ģʽΪ��ͨģʽ(��ѭ������) */
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; 
	 	
	/*DMAͨ�����ȼ���*/
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	/*��ֹDMAͨ���洢�����洢������*/
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	
	/*��ʼ��DMAͨ��5 ���������е�����д��DMA*/
	DMA_Init(DMA1_Channel3, &DMA_InitStructure);
	/*ʹ��DMAͨ��5*/
	DMA_Cmd(DMA1_Channel3, ENABLE); 
}
 /**
  * @brief  ����Ƕ�������жϿ�����NVIC
  * @param  ��
  * @retval ��
  */
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Ƕ�������жϿ�������ѡ�� */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* ����USARTΪ�ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;
  /* �������ȼ�*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* �����ȼ� */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* ʹ���ж� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* ��ʼ������NVIC */
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  �������ݴ����ر�DMA
  * @param  ��
  * @retval ��
  */
void	Data_Process(void)
{
	/*�ر�DMA(Ϊ����DMA��ͷ����)*/
	DMA_Cmd(DMA1_Channel3, DISABLE);	
	/*��������DMA_Buffersize�Ĵ�С*/
	DMA1_Channel3->CNDTR = HMI_DMA_REC_LEN; 
	/*����DMA*/
	DMA_Cmd(DMA1_Channel3, ENABLE); 
}

	/**
  * @brief  USART1�жϺ���
  * @param  ��
  * @retval ��
  */
void DEBUG_USART_IRQHandler(void)
{	
	if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET) //USART_IT_RXNE
	{   
			
			/*�õ�һ�����ݵĳ���*/
	    HMI_DMA_RX_LEN=HMI_DMA_REC_LEN - DMA_GetCurrDataCounter(DMA1_Channel3);
			/*������һ�����ݱ�־*/
		if(HMI_DMA_RX_LEN==9 )
		{		
			/*������һ������*/
			Freq_Output_MODE=1;
		}
		else if(HMI_DMA_RX_LEN==18)
		{
			/*������һ������*/
			Freq_Output_MODE=2;	
		}
		else if(HMI_DMA_RX_LEN==3 )
		{
			/*������һ������*/
			Freq_Output_MODE=3;	
		}
		Data_Process();
	}
		
		/*�����־λ��ֻ���ö�ȡ�Ĵ��������*/
		USART3->SR;
    USART3->DR;
}



 /**
  * @brief  USART GPIO ����,������������
  * @param  ��
  * @retval ��
  */
void USART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// �򿪴���GPIO��ʱ��
	DEBUG_USART_GPIO_APBxClkCmd(DEBUG_USART_GPIO_CLK, ENABLE);
	
	// �򿪴��������ʱ��
	DEBUG_USART_APBxClkCmd(DEBUG_USART_CLK, ENABLE);

	// ��USART Tx��GPIO����Ϊ���츴��ģʽ
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  // ��USART Rx��GPIO����Ϊ��������ģʽ
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	
	// ���ô��ڵĹ�������
	// ���ò�����
	USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
	// ���� �������ֳ�
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// ����ֹͣλ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// ����У��λ
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// ����Ӳ��������
	USART_InitStructure.USART_HardwareFlowControl = 
	USART_HardwareFlowControl_None;
	// ���ù���ģʽ���շ�һ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// ��ɴ��ڵĳ�ʼ������
	USART_Init(DEBUG_USARTx, &USART_InitStructure);
	
		//�����ж����ȼ�����
		NVIC_Configuration();
		//DMA����
		USART3_DMA_Init();
		/*����DMAʹ��*/
		USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);
		/*���տ����ж�ʹ��*/
		USART_ITConfig(DEBUG_USARTx, USART_IT_IDLE, ENABLE);	//USART_IT_RXNE
	
	//ʹ�ܴ���
	USART_Cmd(DEBUG_USARTx, ENABLE);	    
}

/*****************  ����һ���ֽ� **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* ����һ���ֽ����ݵ�USART */
	USART_SendData(pUSARTx,ch);
		
	/* �ȴ��������ݼĴ���Ϊ�� */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

/****************** ����8λ������ ************************/
void Usart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num)
{
  uint8_t i;
	
	for(i=0; i<num; i++)
  {
	    /* ����һ���ֽ����ݵ�USART */
	    Usart_SendByte(pUSARTx,array[i]);	
  
  }
	/* �ȴ�������� */
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
}

/*****************  �����ַ��� **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
  do 
  {
      Usart_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  
  /* �ȴ�������� */
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
  {}
}

/*****************  ����һ��16λ�� **********************/
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
{
	uint8_t temp_h, temp_l;
	
	/* ȡ���߰�λ */
	temp_h = (ch&0XFF00)>>8;
	/* ȡ���Ͱ�λ */
	temp_l = ch&0XFF;
	
	/* ���͸߰�λ */
	USART_SendData(pUSARTx,temp_h);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
	
	/* ���͵Ͱ�λ */
	USART_SendData(pUSARTx,temp_l);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}



