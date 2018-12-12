/***********************************************************************************
 * �� �� ��   : bsp_Me3000.h
 * �� �� ��   : л��
 * ��������   : 2017��7��17��
 * �ļ�����   : ����ļ���Ҫ��������ME3000ģ��
 * ��Ȩ˵��   : Copyright (c) 2008-2017   xx xx xx xx �������޹�˾
 * ��    ��   : 
 * �޸���־   : 
***********************************************************************************/

#ifndef _BSP_ME3000_H
#define _BSP_ME3000_H

#include "Bsp_usart.h"
#include "time.h"
#include "bsp_AtCmdCofig.h"
#include "os.h"
#include "Bsp_led.h"
#include "string.h"

#define ME3000_BIND_USART_PORT (&mdUsart.USART2_s)

#define GSM_ON					GPIO_Pin_3
#define GSM_ON_PORT 			        GPIOC
#define GSM_REST 				GPIO_Pin_2
#define GSM_REST_PORT   		GPIOC

#define GSM_READ_SIGNEL_INT							15000					/*��ǰ�ź����������ʱ��*/


typedef struct tagMe3000
{
	 USARTInfo_s * Usart;/*ģ��󶨵Ĵ���*/
        uint16_t          Me3000ReadyOk;        /*����ģ���Ѿ�׼����*/
        uint16_t          SingnelValue;            /*�ź�ǿ��*/
        uint16_t          SingnelBer;               /*�ź�������*/
	 uint8_t       	  PhoneNumber[15];/*�����绰����*/
        uint8_t            Me3000Online;    /*ģ����������������ǿ���״̬���ǹػ�״̬*/
        OS_TCB      taskMe3000TCB;
        CPU_STK     taskMe3000Stk[TASK3_STK_SIZE];
}Me3000;



extern Me3000 mdMe3000;

int8_t Me3000ParmInit();
int8_t Me3000CheckOnline();
int8_t Me3000TaskInit();
int8_t Me3000Open();
int8_t Me3000SoftClose();
void *Me3000ReadModeVersionCallBack(void *arg1,void *arg2,void *arg3);
void *Me3000ReadSingalValueCallBack(void *arg1,void *arg2,void *arg3);
int8_t Me3000ReadSingalValue();
int8_t Me3000SetShowMode();
int8_t Me3000GetPhoneNumber(void);
void * Me3000GetPhoneNumberCallBack(void *arg1,void *arg2,void *arg3);
int8_t Me3000GetHostByName( const char *Name,char *HostBuf);
void *Me3000GetHostByNameCallBack(void *arg1,void *arg2,void *arg3);
#endif