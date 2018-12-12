#ifndef _UPDATE_H
#define _UPDATE_H
/*��Ҫ�������Զ��������
ʹ�õ����ļ�
socket.c
bsp_esp8266.c
bsp_usart.c��*/
#include "bsp.h"

typedef struct tagCmdPaket
{
	uint16_t  uiHead;			/*Э��ͷ�����̶�0x5aa5*/
	uint16_t  uiPaketCmd;		/*���������*/
	uint8_t    uiDat[512];		/*����*/
	uint16_t  uiCrc;			/*У��*/
	uint16_t  uiEnd;			/*������*/
}sCmdPaket;

/*������*/
typedef struct tagHeartCmd
{
	uint16_t   times;				/*������*/
	uint16_t   DeviceId[20];		/*�豸ID����*/
	struct tm localTime;			/*����ʱ���*/
	uint8_t     SoftVersion[20];		/*����汾*/
	uint8_t     SoftVersionLog[100];	/*�汾������־*/
}HeartCmd;

/*���°�*/
typedef struct tagUpdateCmd
{
	uint16_t  uiTotalPaket;		/*�ܰ���*/
	uint16_t  uiCenterPaket;		/*��ǰ����*/
	uint16_t  uiCenterPaketLen;	/*��ǰ������*/
	uint8_t    ucData[512 - 6];
}UpdateCmd;
void UpdateTask(void *p_arg);
void UpdateTaskInit();

#endif