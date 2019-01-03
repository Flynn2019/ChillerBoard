/**
  *****************************************************************************
  * @file  key.c
  * @author  Feng wenyun
  * @version  V1.0.0
  * @date  24/08/2016
  * @brief	按键应用函数库        .
  *****************************************************************************
  */ 
/* Includes ------------------------------------------------------------------*/
#include "Include.h"

/* private------- ------------------------------------------------------------*/
Key_Typedef Key={0,0,0,0,0,1,0,0};//初始化按键标志
int TemperaSet[6]={220,280,100,450,20,20};//低温,高温,下限,上限报警等预设值
#define LongTime  20	//长按时间2秒


/**
  * @brief	配置按键用到的I/O口 
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
   * @brief  检测是否有按键按下，并判断是哪个按键
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
	/*检测是否有按键按下 */
	if((GPIO_ReadInputData(GPIOx)&(GPIO_Pin))!=(GPIO_Pin))
	{
		/*延时消抖*/
		Delayms(200);
		if((GPIO_ReadInputData(GPIOx)&(GPIO_Pin))!=(GPIO_Pin))
		{
			Key_judge=(GPIO_ReadInputData(GPIOx)&(GPIO_Pin));
			switch(Key_judge)
			{
				/*判断是否为设置键*/
				case (Key_up|Key_down)://如果等于UP键和DOWN键只和的值那么说明set键被按下
					Key.Set_flag=1;	
					Key.Up_flag=0; 
					Key.Down_flag=0;
					DubleKeyFlag=0;
				break;
				/*判断是否为加键*/
				case (Key_set|Key_down):	//如果等于SET键和DOWN键只和的值那么说明UP键被按下
					Key.Set_flag=0;	
					Key.Up_flag=1; 
					Key.Down_flag=0;
					DubleKeyFlag=0;
				break;
				/*判断是否为减键*/
				case (Key_set|Key_up):		//如果等于UP键和SET键只和的值那么说明DOWN键被按下
					Key.Set_flag=0;	
					Key.Up_flag=0; 
					Key.Down_flag=1; 
					DubleKeyFlag=0;
				break;
				case (Key_set):				//UP键和Down键同时按下
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
			/*等待按键释放 */
			while((GPIO_ReadInputData(GPIOx)&(GPIO_Pin))!=(GPIO_Pin))
			{
				/*
				*任何两种情况不可能同时为1
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
						if(Key.Model_flag>=2)//Key.Model_flag为1可设置，为0不可设置
						{
							Key.Model_flag=0;
							Key.MenuSelect=5;
							Key.DubleKeyflag=0;
						}
						SetFlag=0;
						DubleKeyFlag=0;
					}
					if((LongTimeFlag==1)&&(FastAddTime>=1))//实现连加
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
   * @brief  显示设置处理
   * @param None
   * @retval : None
   */
 void Set_Pro(void)
 {
	static char i=0;
	static int *ptemper=TemperaSet;//初始化指针
	static int *pt=TemperaError;//初始化指针
	if(Key.Model_flag==1)//是否进入设置模式
	{					
		if(Key.Set_flag==1)
		{
			Key.Set_flag=0;
			Key.MenuSelect++;
			if(Key.DubleKeyflag==1)//双按键隐藏界面数据3个
			{
				if(Key.MenuSelect>=MenuNum)
				{
					Key.MenuSelect=(MenuNum-3);
				}
			}
			else
			{
				if(Key.MenuSelect>=(MenuNum-3))//界面有MenuNum个
				{
					Key.MenuSelect=6;
				}
			}		
		}
		if((Key.Up_flag==1)||(Key.Down_flag==1))//按键加减计数处理
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
				*@确定各个温度的设定范围
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
					*(ptemper+1)=*ptemper;/*高温水设定值一定要大于或等于低温水*/
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
			StoreSysPrama();	//保存设置值
		}
	}
 }
 /**
   * @brief  按键输入处理
   * @param None
   * @retval : None
   */
 void Key_Process(void)
{
	if(Config_Data[0]==3)										//外控操作模式界面
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
			if(Key.MenuSetFlag==2)								//Key.MenuSetFlag只有三个值1,2加上初始化的0值
			{
				Alarm.flag=0;
			}
			if(Key.MenuSetFlag>=3)								//Key.MenuSetFlag只有三个值1,2加上初始化的0值
			{
				Key.MenuSetFlag=1;
			}								
		}
		while(GPIO_ReadInputDataBit(GPIOD,Key_OnOf) == KEY_ON);//等待按键释放
	}
	 if(Key.MenuSetFlag==1)
	 {
		if( Key_Scan(GPIOC,(Key_set|Key_up |Key_down)) == KEY_ON  )//判断是否有键按下
		{
			Set_Pro();
		}
	 }
}
 
