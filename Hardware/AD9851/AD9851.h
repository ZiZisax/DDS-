#ifndef		__AD9851_H
#define		__AD9851_H

#include "stm32f10x.h"
#include "sys.h"

/**************** AD9851控制引脚定义 ********************/

/*设置AD9851编程模式*/
#define Serial_Mode	    1
#define	Parallel_Mode	0

/*串行模式引脚定义*/
#if	Serial_Mode

/* FQ_UD ——频率更新 */
#define      FQ_UD_GPIO_CLK              RCC_APB2Periph_GPIOB  
#define      FQ_UD_GPIO_PORT             GPIOB
#define      FQ_UD_GPIO_PIN              GPIO_Pin_5
#define		 FQ_UD_HIGH					 GPIO_SetBits(FQ_UD_GPIO_PORT,FQ_UD_GPIO_PIN)
#define		 FQ_UD_LOW					 GPIO_ResetBits(FQ_UD_GPIO_PORT,FQ_UD_GPIO_PIN)

/* W_CLK ——时钟同步 */
#define      W_CLK_GPIO_CLK              RCC_APB2Periph_GPIOB  
#define      W_CLK_GPIO_PORT             GPIOB
#define      W_CLK_GPIO_PIN              GPIO_Pin_4
#define		 W_CLK_HIGH					 GPIO_SetBits(W_CLK_GPIO_PORT,W_CLK_GPIO_PIN)
#define		 W_CLK_LOW					 GPIO_ResetBits(W_CLK_GPIO_PORT,W_CLK_GPIO_PIN)

/* D7 ——数据写入 */
#define      DATA_GPIO_CLK               RCC_APB2Periph_GPIOB  
#define      DATA_GPIO_PORT              GPIOB
#define      DATA_GPIO_PIN               GPIO_Pin_7
#define		 DATA_UD_HIGH			     GPIO_SetBits(DATA_GPIO_PORT,DATA_GPIO_PIN)
#define		 DATA_UD_LOW				 GPIO_ResetBits(DATA_GPIO_PORT,DATA_GPIO_PIN)

/* RST ——复位 */
#define      RST_GPIO_CLK                RCC_APB2Periph_GPIOB  
#define      RST_GPIO_PORT               GPIOB
#define      RST_GPIO_PIN                GPIO_Pin_6
#define		 RST_HIGH					 GPIO_SetBits(RST_GPIO_PORT,RST_GPIO_PIN)
#define		 RST_LOW					 GPIO_ResetBits(RST_GPIO_PORT,RST_GPIO_PIN)

/* GPIOB直接操作 (Bit Set/Reset Register)寄存器 */
#define AD9851_DataBus	GPIOB->BSRR

/*AD9851初始化*/
void	AD9851_Init(void);
/*初始化控制AD9851的IO*/
void	AD9851_GPIO_Config(void);
/*AD9851系统复位*/
void	AD9851_RESET_SERIAL(void);
/*频率输出*/
void    AD9851_SetFreq(double freq);
/*向AD9851中串行写入命令和数据*/
void	AD9851_WR_SERIAL(u8 w0,double frequence);

#endif

/*并行模式引脚定义*/
#if	Parallel_Mode


#endif



#endif	/* __AD9851_H*/

