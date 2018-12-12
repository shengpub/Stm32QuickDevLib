#ifndef _SOCKET_H
#define _SOCKET_H

#include "BSP/bsp_Esp8266.h"

#define NET_LINK_OK                0x0001 /*wifi��������*/

#define MAX_SOCKET_FD           0X4

typedef enum 
{
	NET_HARDWARE_LEVEL_ESP8266 = 0,
	NET_HARDWARE_LEVEL_GSMA6
}NET_HARDWARE_TYPE;
/*����Ӳ������������*/

#define NET_HARDWARE_LEVEL_TYPE  NET_HARDWARE_LEVEL_ESP8266


/*�����ص�����*/
typedef int (*SocketCallFunc)(int *arg1,int *arg2,int *arg3,int *arg4);

typedef struct ragSocketInfo
{
   /*��Ӧ����Ϣ����*/
   uint8_t flage;
}SocketInfo;
/*ÿ��ESP8266ģ����Դ����ĸ�socket 0 -4*/
typedef struct tagSocket_s
{
	/*�����豸����*/
	   uint8_t age;
       uint16_t ucCenterNetState;
       SocketInfo SocketInfos[MAX_SOCKET_FD];
	   SocketCallFunc *SocketClientCreate;/*����һ���ͻ���*/
	   SocketCallFunc *SocketServerCreate;/*����һ��������*/
	   SocketCallFunc *SocketClose;/*�ر�socketr*/
	   SocketCallFunc *SocketRead;/*��*/
	   SocketCallFunc *SocketWrite;/*д*/
       /*��ǰ����״̬
       1��wifi����
       2������������Ӧ
       3�����ӶϿ�
       4��������������*/
}Socket;/* ���������豸��ʼ��*/

int8_t SocketLevelInit( void );

/**************************************************************************
��������
1��Ӳ����:��Ҫ����wifiģ�飬gsmģ�����ɣ��ò���Ҫ��ɵĹ�����
	1������ά���������ܣ����̶ȱ������糩ͨ��
	2�������ṩ����tcp/ipЭ��Ļ����ӿ�
2������㣺��Ҫ��ʵ���������ӵı�׼�ӿڣ���Ҫ����һ�½ӿ�
	1��write
	2��read
	3��close
	4��client�����ͷ������Ĵ���




**************************************************************************/
#endif