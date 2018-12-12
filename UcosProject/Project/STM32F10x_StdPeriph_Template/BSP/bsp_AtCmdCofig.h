/***********************************************************************************
 * �� �� ��   : bsp_AtCmdCofig.h
 * �� �� ��   : л��
 * ��������   : 2017��7��17��
 * �ļ�����   : ����ļ���Ҫ������ʹ��atָ�����õ��豸�����������ļ�
 * ��Ȩ˵��   : Copyright (c) 2008-2017   xx xx xx xx �������޹�˾
 * ��    ��   : 
 * �޸���־   : 
***********************************************************************************/

#ifndef _BSP_ATCMDCONFIG_H
#define _BSP_ATCMDCONFIG_H

#include "Bsp_usart.h"

typedef void * (*AtCmdCallBack)(void *arg1,void *arg2,void *arg3);

typedef struct tagAtCmdParInfo
{	
	char CmdInfo[30];               /*��������*/
	char CmdRsp[30];                /*������Ӧ*/
	uint8_t CmdMode;	              /**���ܵ����ݺ��ģʽ*/
	uint16_t CmdTimeOut;        /*��ʱʱ��*/
	AtCmdCallBack CallBack;   /*�ص�����*/
}AtCmdParInfo;



#endif