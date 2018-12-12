/***********************************************************************************
 * �� �� ��   : Bsp_led.h
 * �� �� ��   : Gavin
 * ��������   : 2017��4��1��
 * �ļ�����   : ���ļ���������LEDָʾ�Ƶĺ궨��
 * ��Ȩ˵��   : Copyright (c) 2008-2017   xx xx xx xx �������޹�˾
 * ��    ��   : 
 * �޸���־   : 
***********************************************************************************/

#ifndef _BSP_LED_H
#define _BSP_LED_H

#include "BspConfig.h"

#define LED_NUM 4

#define LED1_PROT GPIOA
#define LED2_PROT GPIOA
#define LED3_PROT GPIOA
#define LED4_PROT GPIOA

#define LED1_Pin  GPIO_Pin_8
#define LED2_Pin 	GPIO_Pin_11
#define LED3_Pin  GPIO_Pin_12
#define LED4_Pin 	GPIO_Pin_8

typedef enum
{
	LED1 = 1,
	LED2,
	LED3,
	LED4
}LED;


typedef struct tagLedInfo_s
{
	uint16_t GPIO_Pin;				/*led��������*/
	GPIO_TypeDef* GPIO_Port;	/*���Ǹ��˿���*/
	uint8_t UseFlage;					/*���ʹ���������д1*/
	
}LedInfo;

typedef struct tagLedControl_s
{
	LedInfo LedInfo[LED_NUM];					/*ϵͳ���ƿ���4��LED��*/
}LedControl;


void SysLedInit(void);

/*���Ʋ���*/
void LedOpen(uint8_t bits);
void LedClose(uint8_t bits);
#endif
