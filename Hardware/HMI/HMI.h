#ifndef	__HMI_H
#define __HMI_H
#include "stm32f10x.h"
#include "sys.h"

/*串口屏发送命令结束符*/
extern u8	End_CMD[3];

/*处理接收到的数据*/
void 	DataPack_Process(void);
/*显示正弦波*/
void	HMI_SineWave(void);

/*扫频*/
void	AD9851_FreqScan(double start_freq,double end_freq,double step_freq,int delay_time);

#endif	/* __HMI_H */
