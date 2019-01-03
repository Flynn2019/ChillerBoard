#include "Include.h"
/* private------- ------------------------------------------------------------*/
int Config_Data[] = {1,0,220,280,100,450,20,20,0,0,0,100,100,100};//����ϵͳ����,����Ϊ���Ʒ�ʽ����������ѡ�񡢵���ˮ������ˮ�������¶ȡ������¶ȵȲ���

#define SIZE sizeof(Config_Data)	 	//���鳤��

#define FLASH_SAVE_ADDR  0x0803f800 	//����FLASH �����ַ(����Ϊż��������ֵҪ���ڱ�������ռ��FLASH�Ĵ�С+0X08000000),���һҳ�洢��ַ

/**
  * @brief	�洢ϵͳ����
  * @param 	
  * @retval : ��.
  */
void StoreSysPrama(void)
{
	/*
	*�¶��趨ֵ
	*/
	Config_Data[0]=(int)Key.Model_count;//����ģʽ����
	Config_Data[2]=TemperaSet[0];//�����������ֵ
	Config_Data[3]=TemperaSet[1];//�����������ֵ
	Config_Data[4]=TemperaSet[2];//���������¶�����ֵ
	Config_Data[5]=TemperaSet[3];//���������¶�����ֵ
	Config_Data[6]=TemperaSet[4];
	Config_Data[7]=TemperaSet[5];
	/*
	*�¶�У������
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
  * @brief	���ϵͳ����
  * @param 	
  * @retval : ��.
  */

void GetSysPrama(void)
{
	STMFLASH_Read(FLASH_SAVE_ADDR,(u16*)Config_Data,SIZE);
	Key.Model_count=(char)Config_Data[0];//����ģʽ����
	/*
	*�¶��趨ֵ
	*/
	TemperaSet[0]=Config_Data[2];//�����������ֵ
	TemperaSet[1]=Config_Data[3];//�����������ֵ
	TemperaSet[2]=Config_Data[4];//���������¶�����ֵ
	TemperaSet[3]=Config_Data[5];//���������¶�����ֵ
	TemperaSet[4]=Config_Data[6];//�������ˮ�Ա��¶Ȳ�
	TemperaSet[5]=Config_Data[7];//�������ˮ�Ա��¶Ȳ�
	/*
	*�¶�У������
	*/
	TemperaError[0]=Config_Data[8];
	TemperaError[1]=Config_Data[9];
	TemperaError[2]=Config_Data[10];
	TemperaErrorDis[0]=Config_Data[11];
	TemperaErrorDis[1]=Config_Data[12];
	TemperaErrorDis[2]=Config_Data[13];
}
