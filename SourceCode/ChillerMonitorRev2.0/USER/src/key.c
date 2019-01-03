/**
  *****************************************************************************
  * @file  key.c
  * @author  Feng wenyun
  * @version  V1.0.0
  * @date  24/08/2016
  * @brief	����Ӧ�ú�����        .
  *****************************************************************************
  */ 
/* Includes ------------------------------------------------------------------*/
#include "Include.h"

/* private------- ------------------------------------------------------------*/
Key_Typedef Key={0,0,0,0,0,1,0,0};//��ʼ��������־
int TemperaSet[6]={220,280,100,450,20,20};//����,����,����,���ޱ�����Ԥ��ֵ
#define LongTime  20	//����ʱ��2��


/**
  * @brief	���ð����õ���I/O�� 
  * @param None
  * @retval : None
  */
void Key_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure,OnOf;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitStructure.GPIO_Pin = (Key_set|Key_up|Key_down); 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE); 
	OnOf.GPIO_Pin = Key_OnOf; 
	OnOf.GPIO_Speed = GPIO_Speed_10MHz; 
	OnOf.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(GPIOD, &OnOf);	
}
 /**
   * @brief  ����Ƿ��а������£����ж����ĸ�����
   * @param None
   * @retval : None
   */
u8 Key_Scan(GPIO_TypeDef* GPIOx,u16 GPIO_Pin)
{			
	uint16_t Key_judge;
	static char LongTimeFlag=0;
	static char SetFlag=0;
	static char UpFlag=0;
	static char DownFlag=0;
	static char DubleKeyFlag=0;
	/*����Ƿ��а������� */
	if((GPIO_ReadInputData(GPIOx)&(GPIO_Pin))!=(GPIO_Pin))
	{
		/*��ʱ����*/
		Delayms(200);
		if((GPIO_ReadInputData(GPIOx)&(GPIO_Pin))!=(GPIO_Pin))
		{
			Key_judge=(GPIO_ReadInputData(GPIOx)&(GPIO_Pin));
			switch(Key_judge)
			{
				/*�ж��Ƿ�Ϊ���ü�*/
				case (Key_up|Key_down)://�������UP����DOWN��ֻ�͵�ֵ��ô˵��set��������
					Key.Set_flag=1;	
					Key.Up_flag=0; 
					Key.Down_flag=0;
					DubleKeyFlag=0;
				break;
				/*�ж��Ƿ�Ϊ�Ӽ�*/
				case (Key_set|Key_down):	//�������SET����DOWN��ֻ�͵�ֵ��ô˵��UP��������
					Key.Set_flag=0;	
					Key.Up_flag=1; 
					Key.Down_flag=0;
					DubleKeyFlag=0;
				break;
				/*�ж��Ƿ�Ϊ����*/
				case (Key_set|Key_up):		//�������UP����SET��ֻ�͵�ֵ��ô˵��DOWN��������
					Key.Set_flag=0;	
					Key.Up_flag=0; 
					Key.Down_flag=1; 
					DubleKeyFlag=0;
				break;
				case (Key_set):				//UP����Down��ͬʱ����
					Key.Set_flag=0;	
					Key.Up_flag=0; 
					Key.Down_flag=0;
					DubleKeyFlag=1;
				break;
				default:
					Key.Set_flag=0;	
					Key.Up_flag=0; 				
					Key.Down_flag=0;
					Key.DubleKeyflag=0;
					DubleKeyFlag=0;
				break;
			}
			if((Key.Set_flag==1)||(Key.Up_flag==1)||(Key.Down_flag==1)||(DubleKeyFlag==1))
			{
				SetFlag=Key.Set_flag;			
				UpFlag=Key.Up_flag;				
				DownFlag=Key.Down_flag;
				//DubleKeyFlag=0;
				LongTimeFlag=0;					
				LongKeyTime=0;					
			}
			/*�ȴ������ͷ� */
			while((GPIO_ReadInputData(GPIOx)&(GPIO_Pin))!=(GPIO_Pin))
			{
				/*
				*�κ��������������ͬʱΪ1
				*/
				if((SetFlag==1)||(UpFlag==1)||(DownFlag==1)||(DubleKeyFlag==1))
				{
					if(LongKeyTime>=LongTime)
					{
						LongTimeFlag=1;
						LongKeyTime=0;
						Key.Model_flag +=(SetFlag+DubleKeyFlag);
						if((Key.Model_flag==1)&&(DubleKeyFlag==1))
						{
							Key.MenuSelect=13;
							Key.DubleKeyflag=1;
						}
						if(Key.Model_flag>=2)//Key.Model_flagΪ1�����ã�Ϊ0��������
						{
							Key.Model_flag=0;
							Key.MenuSelect=5;
							Key.DubleKeyflag=0;
						}
						SetFlag=0;
						DubleKeyFlag=0;
					}
					if((LongTimeFlag==1)&&(FastAddTime>=1))//ʵ������
					{
						Key.Up_flag=UpFlag;
						Key.Down_flag=DownFlag;
						FastAddTime=0;
					}
				}
				Set_Pro();
				Dis_Menu(Key.MenuSelect);	
			}
			return 	KEY_ON; 				
		}
		else
			return KEY_OFF;					
	}
	else
		return KEY_OFF;						
}
 /**
   * @brief  ��ʾ���ô���
   * @param None
   * @retval : None
   */
 void Set_Pro(void)
 {
	static char i=0;
	static int *ptemper=TemperaSet;//��ʼ��ָ��
	static int *pt=TemperaError;//��ʼ��ָ��
	if(Key.Model_flag==1)//�Ƿ��������ģʽ
	{					
		if(Key.Set_flag==1)
		{
			Key.Set_flag=0;
			Key.MenuSelect++;
			if(Key.DubleKeyflag==1)//˫�������ؽ�������3��
			{
				if(Key.MenuSelect>=MenuNum)
				{
					Key.MenuSelect=(MenuNum-3);
				}
			}
			else
			{
				if(Key.MenuSelect>=(MenuNum-3))//������MenuNum��
				{
					Key.MenuSelect=6;
				}
			}		
		}
		if((Key.Up_flag==1)||(Key.Down_flag==1))//�����Ӽ���������
		{
			if(Key.MenuSelect==6)
			{
				Key.Model_count += (Key.Up_flag-Key.Down_flag);
				if(Key.Model_count>=4)
					Key.Model_count=1;
				else if(Key.Model_count<=0) 
					Key.Model_count=3;
			}
			else if((Key.MenuSelect>=7)&&(Key.MenuSelect<(MenuNum-3)))
			{
				i=Key.MenuSelect-7;
				*(ptemper+i) += (Key.Up_flag-Key.Down_flag);
				/*
				*@ȷ�������¶ȵ��趨��Χ
				**/
				if(*ptemper>=350)
					*ptemper=350;
				if(*ptemper<=150)
					*ptemper=150;
				
				if(*(ptemper+1)>=350)
					*(ptemper+1)=350;
				if(*(ptemper+1)<=150)
					*(ptemper+1)=150;
				
				if(*(ptemper+2)>=140)
					*(ptemper+2)=140;
				if(*(ptemper+2)<=0)
					*(ptemper+2)=0;

				if(*(ptemper+3)>=500)
					*(ptemper+3)=500;
				if(*(ptemper+3)<=410)
					*(ptemper+3)=410;
				if(*ptemper>=*(ptemper+1))
				{
					*(ptemper+1)=*ptemper;/*����ˮ�趨ֵһ��Ҫ���ڻ���ڵ���ˮ*/
				}
			}
			else if((Key.MenuSelect>=(MenuNum-3))&&(Key.MenuSelect<MenuNum)&&(Key.DubleKeyflag==1))
			{
				i=Key.MenuSelect-(MenuNum-3);
				*(pt+i) += (Key.Up_flag-Key.Down_flag);
				TMonitor[i]=(((2361*ADC_SmoothCount[2*i])/(ADC_SmoothCount[2*i+1])-2597)+TemperaError[i]);
				TMonitorDis[i]=TMonitor[i];
				TemperaErrorDis[i]=100+TemperaError[i];
			}
			Key.Up_flag=0;
			Key.Down_flag=0;
			StoreSysPrama();	//��������ֵ
		}
	}
 }
 /**
   * @brief  �������봦��
   * @param None
   * @retval : None
   */
 void Key_Process(void)
{
	if(Config_Data[0]==3)										//��ز���ģʽ����
	{
		if(GPIO_ReadInputDataBit(GPIOA,ExCtr_IN) == Bit_SET)
		{
			Delayms(20);
			if(GPIO_ReadInputDataBit(GPIOA,ExCtr_IN) == Bit_SET)
		 	{
				Key.MenuSetFlag=2;
			}
		}
	}
 	if(GPIO_ReadInputDataBit(GPIOD,Key_OnOf) == KEY_ON)
	{
		Delayms(20);
		if(GPIO_ReadInputDataBit(GPIOD,Key_OnOf) == KEY_ON) 
		{
			Key.MenuSetFlag++;
			if(Key.MenuSetFlag==2)								//Key.MenuSetFlagֻ������ֵ1,2���ϳ�ʼ����0ֵ
			{
				Alarm.flag=0;
			}
			if(Key.MenuSetFlag>=3)								//Key.MenuSetFlagֻ������ֵ1,2���ϳ�ʼ����0ֵ
			{
				Key.MenuSetFlag=1;
			}								
		}
		while(GPIO_ReadInputDataBit(GPIOD,Key_OnOf) == KEY_ON);//�ȴ������ͷ�
	}
	 if(Key.MenuSetFlag==1)
	 {
		if( Key_Scan(GPIOC,(Key_set|Key_up |Key_down)) == KEY_ON  )//�ж��Ƿ��м�����
		{
			Set_Pro();
		}
	 }
}
 
