#include "Bsp_led.h"
#include "stdio.h"
#include "string.h"

LedControl mdLed;

void EnableLedPort(uint8_t bits)
{
	GPIO_InitTypeDef GPIO_Init_Structure;
	
	if(mdLed.LedInfo[bits].UseFlage == 1)
	{
		GPIO_Init_Structure.GPIO_Pin = mdLed.LedInfo[bits].GPIO_Pin; 
		GPIO_Init_Structure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init_Structure.GPIO_Speed = GPIO_Speed_50MHz;

		GPIO_Init(mdLed.LedInfo[bits].GPIO_Port,&GPIO_Init_Structure); /*led1*/
	}
}
/*****************************************************************************
 * �� �� ��  : SysLedParmInit
 * �� �� ��  : Gavin
 * ��������  : 2017��4��1��
 * ��������  : ϵͳLED�Ʋ�����ʼ������Ҫ����LEDָʾ�Ƶ�Ӳ�����ӹ�ϵ
 * �������  : ��
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
void SysLedParmInit()
{
	/*ledok*/
	mdLed.LedInfo[0].GPIO_Port = LED1_PROT;
	mdLed.LedInfo[0].GPIO_Pin = LED1_Pin;
	mdLed.LedInfo[0].UseFlage = 1;
	
	mdLed.LedInfo[1].GPIO_Port = LED2_PROT;
	mdLed.LedInfo[1].GPIO_Pin = LED2_Pin;
	mdLed.LedInfo[1].UseFlage = 1;
	
	mdLed.LedInfo[2].GPIO_Port = LED3_PROT;
	mdLed.LedInfo[2].GPIO_Pin = LED3_Pin;
	mdLed.LedInfo[2].UseFlage = 1;

}
/*****************************************************************************
 * �� �� ��  : SysLedInit
 * �� �� ��  : Gavin
 * ��������  : 2017��4��1��
 * ��������  : ϵͳָʾ�Ƴ�ʼ��
 * �������  : ��
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
void SysLedInit()
{
	uint8_t ucLoop = 0;
	memset(&mdLed,0,sizeof(mdLed));
	
	SysLedParmInit();
	
	for(ucLoop = 0; ucLoop < 3; ucLoop++)
	{
		EnableLedPort(ucLoop);
		LedClose(ucLoop + 1);
	}
}
/*****************************************************************************
 * �� �� ��  : LedIoControl
 * �� �� ��  : Gavin
 * ��������  : 2017��4��1��
 * ��������  : �ú������ڿ���LED��
 * �������  : uint8_t bits   LED�Ʊ�ʾ
               uint8_t flage  ȡֵ��Χ��0-1��0����رգ�1������
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
void LedIoControl(uint8_t bits,uint8_t flage)
{
	if(mdLed.LedInfo[bits].UseFlage == 1)
	{
		if(flage)
		{
			GPIO_SetBits(mdLed.LedInfo[bits].GPIO_Port,
									mdLed.LedInfo[bits].GPIO_Pin);
		}
		else
		{
			GPIO_ResetBits(mdLed.LedInfo[bits].GPIO_Port,
									mdLed.LedInfo[bits].GPIO_Pin);
		}
	}
}
/*****************************************************************************
 * �� �� ��  : LedOpen
 * �� �� ��  : Gavin
 * ��������  : 2017��4��1��
 * ��������  : LEDָʾ�ƿ�������
 * �������  : uint8_t bits  LED��
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
void LedOpen(uint8_t bits)
{
	LedIoControl(bits -1,0);
}

/*****************************************************************************
 * �� �� ��  : LedClose
 * �� �� ��  : Gavin
 * ��������  : 2017��4��1��
 * ��������  : LEDָʾ�ƹرղ���
 * �������  : uint8_t bits  LEDָʾ��
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
void LedClose(uint8_t bits)
{
	LedIoControl(bits -1,1);
}
