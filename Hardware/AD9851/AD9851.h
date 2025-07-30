#ifndef		__AD9851_H
#define		__AD9851_H

#include "stm32f10x.h"
#include "sys.h"

/**************** AD9851�������Ŷ��� ********************/

/*����AD9851���ģʽ*/
#define Serial_Mode	    1
#define	Parallel_Mode	0

/*����ģʽ���Ŷ���*/
#if	Serial_Mode

/* FQ_UD ����Ƶ�ʸ��� */
#define      FQ_UD_GPIO_CLK              RCC_APB2Periph_GPIOB  
#define      FQ_UD_GPIO_PORT             GPIOB
#define      FQ_UD_GPIO_PIN              GPIO_Pin_5
#define		 FQ_UD_HIGH					 GPIO_SetBits(FQ_UD_GPIO_PORT,FQ_UD_GPIO_PIN)
#define		 FQ_UD_LOW					 GPIO_ResetBits(FQ_UD_GPIO_PORT,FQ_UD_GPIO_PIN)

/* W_CLK ����ʱ��ͬ�� */
#define      W_CLK_GPIO_CLK              RCC_APB2Periph_GPIOB  
#define      W_CLK_GPIO_PORT             GPIOB
#define      W_CLK_GPIO_PIN              GPIO_Pin_4
#define		 W_CLK_HIGH					 GPIO_SetBits(W_CLK_GPIO_PORT,W_CLK_GPIO_PIN)
#define		 W_CLK_LOW					 GPIO_ResetBits(W_CLK_GPIO_PORT,W_CLK_GPIO_PIN)

/* D7 ��������д�� */
#define      DATA_GPIO_CLK               RCC_APB2Periph_GPIOB  
#define      DATA_GPIO_PORT              GPIOB
#define      DATA_GPIO_PIN               GPIO_Pin_7
#define		 DATA_UD_HIGH			     GPIO_SetBits(DATA_GPIO_PORT,DATA_GPIO_PIN)
#define		 DATA_UD_LOW				 GPIO_ResetBits(DATA_GPIO_PORT,DATA_GPIO_PIN)

/* RST ������λ */
#define      RST_GPIO_CLK                RCC_APB2Periph_GPIOB  
#define      RST_GPIO_PORT               GPIOB
#define      RST_GPIO_PIN                GPIO_Pin_6
#define		 RST_HIGH					 GPIO_SetBits(RST_GPIO_PORT,RST_GPIO_PIN)
#define		 RST_LOW					 GPIO_ResetBits(RST_GPIO_PORT,RST_GPIO_PIN)

/* GPIOBֱ�Ӳ��� (Bit Set/Reset Register)�Ĵ��� */
#define AD9851_DataBus	GPIOB->BSRR

/*AD9851��ʼ��*/
void	AD9851_Init(void);
/*��ʼ������AD9851��IO*/
void	AD9851_GPIO_Config(void);
/*AD9851ϵͳ��λ*/
void	AD9851_RESET_SERIAL(void);
/*Ƶ�����*/
void    AD9851_SetFreq(double freq);
/*��AD9851�д���д�����������*/
void	AD9851_WR_SERIAL(u8 w0,double frequence);

#endif

/*����ģʽ���Ŷ���*/
#if	Parallel_Mode


#endif



#endif	/* __AD9851_H*/

