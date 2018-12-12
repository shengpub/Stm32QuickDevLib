/***********************************************************************************
 * �� �� ��   : BspConfig.h
 * �� �� ��   : Gavin
 * ��������   : 2017��6��7��
 * �ļ�����   : ���ļ���Ҫ����������Ӳ�������豸����ϸ����,�������û�ֻ��Ҫ
                ���ø��ļ��Ϳ��Կ��ٵ������������
 * ��Ȩ˵��   : Copyright (c) 2008-2017   xx xx xx xx �������޹�˾
 * ��    ��   : 
 * �޸���־   : 
***********************************************************************************/
#ifndef BSP_CONFIG_H
#define BSP_CONFIG_H

#include "stm32f10x.h"

typedef struct tagGpioInfo
{
	GPIO_TypeDef*   GPIO_Port;
	uint16_t 		GPIO_Pin;		
}GpioInfo;


#define GPIOControl(Port,Pin,Flage) \
	if(Flage) GPIO_SetBits(Port,Pin);\
	else GPIO_ResetBits(Port,Pin);

#endif
