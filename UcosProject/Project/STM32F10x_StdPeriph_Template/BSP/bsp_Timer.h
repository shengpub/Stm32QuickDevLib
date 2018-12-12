/***********************************************************************************
 * �� �� ��   : bsp_Timer.h
 * �� �� ��   : xiegui
 * ��������   : 2017��11��28��
 * �ļ�����   : ����Ƕ�ʱ������
 * ��Ȩ˵��   : Copyright (c) 2008-2017   xx xx xx xx �������޹�˾
 * ��    ��   : 
 * �޸���־   : 
***********************************************************************************/
#ifndef BSP_TIMER_H
#define BSP_TIMER_H


#include "BspConfig.h"
#include "stdio.h"
#include "os.h"

typedef uint16_t (*TimerCallFunc)(uint16_t *arg1,uint16_t *arg2);

#define MAX_TIMER_TASK   8
#define TIMER_TASK_BUF_USE  1
#define TIMER_TASK_BUF_FREE  0

/*��ʱ����ģʽ*/
#define TIMER_TASK_ONECE 0
#define TIMER_TASK_FOREVER 1

typedef struct tagTimerCallFunInfo
{
	uint32_t uiDelayMs; /*��ʱ����ʱʱ��:��λBastMs*/
	uint32_t uiDelayCount;/*��ʱ������*/
	TimerCallFunc Func; /*��ʱ�ص�ʱ��*/
	uint16_t arg1;			/*����1*/
	uint16_t arg2;		  /*����2*/
	uint8_t  TimerNodeStatus; /*��ǰ״̬*/
	uint8_t  TimerMode; /*0������ִ�У�1�����ִ��*/
}TimerCallFunInfo;

typedef struct tagTimerNodeInfo
{
	/*Ԥ��Ƶ��*/
	uint16_t TimerPeriod;
	uint16_t TimerPrescaler;
	/*����ģʽ*/
	uint16_t TimerCounterMode;
	uint8_t TimerUseSate;
	uint8_t TimerInitOk;
	uint16_t TimetBaseMs;/*��ʱ���Ļ�׼Msʱ��*/
	/*��ʱ��ʹ��״̬������Ѿ�����ʼ�����������ٽ��г�ʼ��
	����ǿ�Ƴ�ʼ��*/
	/*��ʱ���жϷ���ִ���û���������ò��Ǻ�ʱ��ĺ���*/
	TimerCallFunc IntBackFun; /*���ص�����*/
	/*�ص������Ĳ���*/
	uint16_t TimerCallFunArg1;
	uint16_t TimerCallFunArg2;
	TimerCallFunInfo TimerTask[MAX_TIMER_TASK];/*��ʱ�������5��*/
	
}TimerNodeInfo;
/*������ʱ����Ҫ�������Ϣ*/

typedef struct tagTimerInfo
{
	TimerNodeInfo Timer1;/*��ʱ��1*/
	TimerNodeInfo Timer2;/*��ʱ��1*/
	TimerNodeInfo Timer3;/*��ʱ��1*/
}TimerInfo;

void TimerAdvanceInit(TIM_TypeDef* Timer,uint16_t TimerPeriod,uint16_t TimerPrescaler,
				uint16_t TimerMode,uint16_t TimerClockDivision,
				TimerCallFunc callFunc,uint8_t ucFlage,uint8_t ucPwmEnable);


void  TimerInit(TIM_TypeDef* Timer,uint16_t uiMsDelay,TimerCallFunc callFunc,uint8_t ucFlage,uint8_t ucPwmEnable);
int8_t TimerTaskAdd(TimerNodeInfo *TimerNode,TimerCallFunc hook,
										uint16_t arg1,uint16_t arg2,uint8_t TaskMode,
										uint32_t uiTime);
int8_t TimerTaskDelete(TimerNodeInfo *TimerNode,int8_t TimerTaskId);

extern TimerInfo mdTimer;

#endif
