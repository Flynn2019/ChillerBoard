#ifndef  __flash_H__
#define  __flash_H__

extern int Config_Data[];//配置系统参数,依次为控制方式参数、低温水、高温水、下限温度、上限温度等参数
void StoreSysPrama(void);		//存储系统参数	
void GetSysPrama(void);		//获得系统参数	

#endif
