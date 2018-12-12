#ifndef BSP_DEVICE_LIST_H
#define BSP_DEVICE_LIST_H

#include "stdio.h"
#include "stdlib.h"
#include "string.h"



typedef struct tagDeviceOperations
{	
	/*ͨ���豸�����������ж�Ӧ,�򿪳ɹ��󷵻��豸�ļ��������ţ�
	�������Ϊ�豸�������е�λ�� + 1*/
	int (*DeviceOpen) (char *DeviceName,int *arg);
	/*�򿪲���*/

	/*��ȡ����*/
	int (*DeviceRead)(int DeviceFd,char *UserBuf,int len);
	/*д����*/
	int (*DeviceWrite)(int DeviceFd,char *UserBuf,int len);
}DeviceOperations;/*�豸�����ṹ��*/

/*�豸��ϸ��Ϣ*/
typedef struct tagDeviceInfo
{
	char DeviceName[10];	/*�豸���ƣ���Ҫ��ͨ���������Ӧ�豸*/
	int  DeviceId;			/*�豸id����*/
}DeviceInfo;

typedef struct tagSysDevStrutInfo
{
	DeviceOperations DevOpeatationInfo;	/*�豸�ڵ���ϸ��Ϣ*/
	DeviceInfo       NodeInfo;			/*�ڵ���Ϣ*/
	struct tagSysDevStrutInfo *next;	/*��һ���ڵ�*/
}SysDevStrutInfo;

typedef void (*ModeCallBack)(SysDevStrutInfo *DeviceNode);
SysDevStrutInfo * DeviceListInit( void );
void mode_init(ModeCallBack Func);

extern SysDevStrutInfo *DeviceListHead;

#endif
