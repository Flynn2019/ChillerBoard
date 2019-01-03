/**
  *****************************************************************************
  * @file  main.c
  * @author  Feng wenyun
  * @version  V1.0.0
  * @date  24/08/2016
  * @brief	������.
  *****************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/

#include "Include.h"

/**
  * @brief  ������
  * @param None
  * @retval : 0
  */

int main(void)
{ 
	int *pf;
	pf=(int *)0x0803f800;
	/*��ʼ������*/
	SystemInit();					// ����ϵͳʱ��Ϊ72M 
	DelayInit(72000000);
	NVIC_Configuration();			//TIM2 ��ʱ���� 
	TIM2_Configuration(); 	
	START_TIME;	 					 //TIM2 ��ʱ��ʼ��ʱ	
	Key_GPIO_Config();				//�����˿ڳ�ʼ��
	IOputConfig();					//��ʼ���������IO
	PerifStop();					//��ֹͣ���蹤��
	LCD_PORT_Init();
	LCD_Init();						//��ʼ��Һ��
	ADC1_Init();					//��ʼ��ADC
	PerifStop();					//��ʼ���������״̬�ر�
	LogoTime=0;
	while(LogoTime<LogoFreshTime)
	{
		DisLogo();					//��ʾ��˾LOGO
	}
	/*�ж��Ƿ�Ϊ��һ����д�����оƬ*/
	if((*(pf+2)==0xFFFFFFFF)&&(*(pf+3)==0xFFFFFFFF)&&(*(pf+4)==0xFFFFFFFF)&&(*(pf+5)==0xFFFFFFFF))
	{
		StoreSysPrama();/*��һ����д�����оƬ����Ҫ�ȴ洢��ʼֵ*/
	}
	GetSysPrama();
  AntifreezePro();				//��������
	while(1)
	{
		Key_Process();				//��������
		if(MenuTime>=MenuFreshTime)	//0.5����ˢ��һ��
		{
			if(Key.MenuSetFlag==1)//�رչ���ģʽKey.MenuSetFlag������ʾ������ٲ����¶�
			{
				ADC1_Tempera();			//ADC����
				TemperaPro();			//�¶ȴ���
			}
			Dis_Menu(Key.MenuSelect);	//��ʾ����
			MenuTime=0;				//��ռ�����
		}
		OutPro();					//������ƴ���
	}
	return 0;
}

