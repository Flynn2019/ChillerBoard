#include "Include.h"
/* private------- ------------------------------------------------------------*/
int Config_Data[] = {1,0,220,280,100,450,20,20,0,0,0,100,100,100};//配置系统参数,依次为控制方式参数、语言选择、低温水、高温水、下限温度、上限温度等参数

#define SIZE sizeof(Config_Data)	 	//数组长度

#define FLASH_SAVE_ADDR  0x0803f800 	//设置FLASH 保存地址(必须为偶数，且其值要大于本代码所占用FLASH的大小+0X08000000),最后一页存储地址

/**
  * @brief	存储系统参数
  * @param 	
  * @retval : 无.
  */
void StoreSysPrama(void)
{
	/*
	*温度设定值
	*/
	Config_Data[0]=(int)Key.Model_count;//保存模式参数
	Config_Data[2]=TemperaSet[0];//保存低温设置值
	Config_Data[3]=TemperaSet[1];//保存高温设置值
	Config_Data[4]=TemperaSet[2];//保存下限温度设置值
	Config_Data[5]=TemperaSet[3];//保存上限温度设置值
	Config_Data[6]=TemperaSet[4];
	Config_Data[7]=TemperaSet[5];
	/*
	*温度校正参数
	*/
	Config_Data[8]=TemperaError[0];
	Config_Data[9]=TemperaError[1];
	Config_Data[10]=TemperaError[2];
	Config_Data[11]=TemperaErrorDis[0];
	Config_Data[12]=TemperaErrorDis[1];
	Config_Data[13]=TemperaErrorDis[2];
	STMFLASH_Write(FLASH_SAVE_ADDR,(u16*)Config_Data,SIZE);
}

/**
  * @brief	获得系统参数
  * @param 	
  * @retval : 无.
  */

void GetSysPrama(void)
{
	STMFLASH_Read(FLASH_SAVE_ADDR,(u16*)Config_Data,SIZE);
	Key.Model_count=(char)Config_Data[0];//保存模式参数
	/*
	*温度设定值
	*/
	TemperaSet[0]=Config_Data[2];//保存低温设置值
	TemperaSet[1]=Config_Data[3];//保存高温设置值
	TemperaSet[2]=Config_Data[4];//保存下限温度设置值
	TemperaSet[3]=Config_Data[5];//保存上限温度设置值
	TemperaSet[4]=Config_Data[6];//保存低温水对比温度差
	TemperaSet[5]=Config_Data[7];//保存高温水对比温度差
	/*
	*温度校正参数
	*/
	TemperaError[0]=Config_Data[8];
	TemperaError[1]=Config_Data[9];
	TemperaError[2]=Config_Data[10];
	TemperaErrorDis[0]=Config_Data[11];
	TemperaErrorDis[1]=Config_Data[12];
	TemperaErrorDis[2]=Config_Data[13];
}
