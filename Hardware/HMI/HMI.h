#ifndef	__HMI_H
#define __HMI_H
#include "stm32f10x.h"
#include "sys.h"

/*�������������������*/
extern u8	End_CMD[3];

/*������յ�������*/
void 	DataPack_Process(void);
/*��ʾ���Ҳ�*/
void	HMI_SineWave(void);

/*ɨƵ*/
void	AD9851_FreqScan(double start_freq,double end_freq,double step_freq,int delay_time);

#endif	/* __HMI_H */
