/**
  *****************************************************************************
  * @file  main.c
  * @author  Feng wenyun
  * @version  V1.0.0
  * @date  24/08/2016
  * @brief	主函数.
  *****************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/

#include "Include.h"

/**
  * @brief  主函数
  * @param None
  * @retval : 0
  */

int main(void)
{ 
	int *pf;
	pf=(int *)0x0803f800;
	/*初始化外设*/
	SystemInit();					// 配置系统时钟为72M 
	DelayInit(72000000);
	NVIC_Configuration();			//TIM2 定时配置 
	TIM2_Configuration(); 	
	START_TIME;	 					 //TIM2 定时开始计时	
	Key_GPIO_Config();				//按键端口初始化
	IOputConfig();					//初始化输入输出IO
	PerifStop();					//现停止外设工作
	LCD_PORT_Init();
	LCD_Init();						//初始化液晶
	ADC1_Init();					//初始化ADC
	PerifStop();					//初始化外设输出状态关闭
	LogoTime=0;
	while(LogoTime<LogoFreshTime)
	{
		DisLogo();					//显示公司LOGO
	}
	/*判断是否为第一次烧写程序的芯片*/
	if((*(pf+2)==0xFFFFFFFF)&&(*(pf+3)==0xFFFFFFFF)&&(*(pf+4)==0xFFFFFFFF)&&(*(pf+5)==0xFFFFFFFF))
	{
		StoreSysPrama();/*第一次烧写程序的芯片，需要先存储初始值*/
	}
	GetSysPrama();
  AntifreezePro();				//防冻处理
	while(1)
	{
		Key_Process();				//按键处理
		if(MenuTime>=MenuFreshTime)	//0.5秒钟刷新一次
		{
			if(Key.MenuSetFlag==1)//关闭工作模式Key.MenuSetFlag正常显示界面后再测试温度
			{
				ADC1_Tempera();			//ADC采样
				TemperaPro();			//温度处理
			}
			Dis_Menu(Key.MenuSelect);	//显示更新
			MenuTime=0;				//清空计数器
		}
		OutPro();					//输出控制处理
	}
	return 0;
}

