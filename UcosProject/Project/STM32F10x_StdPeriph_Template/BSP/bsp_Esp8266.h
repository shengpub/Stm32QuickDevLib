#ifndef BSP_ESP8266_H
#define BSP_ESP8266_H


/*���ļ���Ҫ����ESP8266WIFIģ��Ŀ��ƣ�Ϊ�ϲ�������ṩ֧��*/
#include "Bsp_usart.h"
#include "time.h"

#define ESP8266_BIND_USART_PORT (&mdUsart.USART3_s)

/*wifiģʽ*/
#define ESP8266_WIFI_MODE_STATION		0X01
#define ESP8266_WIFI_MODE_AP			0X02
#define ESP8266_WIFI_MODE_AP_STATIO 	0X03

typedef void * (*Esp8266CallBack)(void *arg1,void *arg2);

typedef struct tagEsp8266ParInfo
{	
	uint8_t CmdInfo[30];				/*��������*/
	uint8_t CmdRsp[30]; 			/*������Ӧ*/
	uint8_t CmdMode;			/*����ģʽ*/
	uint16_t CmdTimeOut;	/*���ʱʱ��*/
	Esp8266CallBack CallBack;/*�ص�����*/
}Esp8266ParInfo;


typedef struct tagEsp8266
{
	 USARTInfo_s * Usart;/*ģ��󶨵Ĵ���*/
	 uint8_t       	  WifiName[20];/*wifi����*/
	 uint8_t            WifiPasswd[20];/*wifi��Կ*/
}Esp8266;

extern int8_t ESP8266_Init();
extern Esp8266 mdEsp8266;
int8_t ESP8266_SendCmd(char *Cmd,char *Restrict,uint16_t len,int8_t Mode,uint16_t TimeOut);
int8_t ESP8266_Check();
int8_t ESP8266_Reset();
int8_t ESP8266_CmdDisplay(uint8_t Mode);
int8_t ESP8266_WifiModeSet(uint8_t Mode);
int8_t ESP8266_WifiConnect(char *WifiSsid,char *WifiPasswd);
int8_t ESP8266_WifiConnectStatus();
int8_t ESP8266_TcpLocalIp();
int8_t ESP8266_TcpSend(uint8_t Id,char *Buf,uint16_t Len);
int8_t ESP8266_TcpMux(int8_t Mode);
int8_t ESP8266_TcpClientConnect(uint8_t Socket,char *ServerIp,uint16_t ServerPort);
int8_t ESP8266_TcpClientDisconnect(uint8_t Id);
int8_t ESP8266_TcpSend(uint8_t Id,char *Buf,uint16_t Len);
int8_t ESP8266_GetTimeFromNet( void );
int8_t ESP8266_TcpConnectStatus();


#endif
