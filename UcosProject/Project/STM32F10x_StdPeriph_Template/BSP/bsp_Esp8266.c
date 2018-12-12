/*���ڴ�����������������������*/
#include "bsp_Esp8266.h"
#include "string.h"

Esp8266 mdEsp8266;

Esp8266ParInfo Esp8266ParmInfos[] =
{
		/*0*/{"AT\r\n","OK",1,2000,NULL},/*�����豸����*/
	/*1*/{"AT+RST\r\n","OK",1,4000,NULL},
	/*2*/{"ATE","OK",1,1000,NULL},
	
	/*������WIFI��ص�����*/
	/*3*/{"AT+CWMODE=","OK",1,1000,NULL},
	/*4*/{"AT+CWLAP","OK",1,2000,NULL},/*���Ҫ�ӻص���������wifi����*/
	/*5*/{"AT+CWJAP=","OK",1,8000,NULL},/*���ӵ��ƶ���wifi*/
	/*6*/{"AT+CWQAP","OK",1,1000,NULL},/*�Ͽ�wifi*/
	/*7*/{"AT+CIPMUX=","OK",1,3000,NULL},
	/*8*/{"AT+CIPSTART=","OK",1,2000,NULL},
	/*9*/{"AT+CIPSEND=","SEND OK",1,2000,NULL},
	/*10*/{"AT+CIPCLOSE=","OK",1,1000,NULL},
	/*11*/{"AT+CWJAP?\r\n","OK",1,4000,NULL},
	/*12*/{"AT+CIFSR\r\n","OK",1,2000,NULL},

	/*13*/{"AT+CIPSERVER=","OK",1,2000,NULL},
	/*14*/{"AT+CIPSTATUS","+CIPSTATUS:0",1,4000,NULL}
};

/*ģ�������ʼ��*/
int8_t ESP8266_ParmInit()
{
	mdEsp8266.Usart = ESP8266_BIND_USART_PORT; /*���ڴ���2��*/
	/*��Ӧ���ڵĳ�ʼ��*/
	USARTInit(USART3,115200,NULL);
	/*���ö�Ӧ���ڵĽ���ģʽΪ����ģʽ*/
	mdEsp8266.Usart->USART_RecvQueue.QueueType = QueueArryType;
	//printf("\nWIFIģ�������·��ʼ���ɹ�\n");
}
/*ģ���ʼ��*/
int8_t ESP8266_Init()
{
	OS_ERR err;
	int8_t status = 0;
	
	ESP8266_ParmInit();/*������ʼ��*/
	
	/*1����λ*/
	#if 1
	do
	{
		OSTimeDly(  (OS_TICK    )500, 
                    (OS_OPT     )OS_OPT_TIME_DLY, 
                    (OS_ERR     *)&err);
		
		status = ESP8266_Reset();
	}while(!status);
	printf("�豸�����ɹ�\n");
        #endif
    
	/*wifiģ��Ӳ����س�ʼ��*/
	/*2���鿴�豸�Ƿ�����*/
	do
	{
		OSTimeDly(  (OS_TICK    )500, 
                    (OS_OPT     )OS_OPT_TIME_DLY, 
                    (OS_ERR     *)&err);
		
		status = ESP8266_Check();
	}while(!status);
	
	printf("wifi�豸����\n");
	do
	{
	OSTimeDly(  (OS_TICK    )500, 
                    (OS_OPT     )OS_OPT_TIME_DLY, 
                    (OS_ERR     *)&err);
	status = ESP8266_CmdDisplay(1);
	}while(!status);
	
	/*3������ģʽ������*/ /*1������Ϊ������ģʽ*//*2������Ϊstationģʽ*/
	do
	{
		OSTimeDly(  (OS_TICK    )500, 
                    (OS_OPT     )OS_OPT_TIME_DLY, 
                    (OS_ERR     *)&err);
		
		status = ESP8266_TcpMux(1);/*����������*/
	}while(!status);
	//printf("������ģʽ���óɹ�\n");
	//do
	//{
		OSTimeDly(  (OS_TICK    )500, 
                    (OS_OPT     )OS_OPT_TIME_DLY, 
                    (OS_ERR     *)&err);
		
		status = ESP8266_WifiModeSet(ESP8266_WIFI_MODE_STATION);/*wifiģ�鹤��ģʽ*/
	//}while(!status);
	//printf("Wifiģ�鹤��ģʽΪSTATION\n");
	do
	{
		/*4��wifi�������ã�����WiFi����*/
		
		
		//status = ESP8266_WifiConnect("Xiaomi_1F14","11111111");
		status = ESP8266_WifiConnect("TP-LINK_9111","123456789");
		OSTimeDly(  (OS_TICK    )500, 
                    (OS_OPT     )OS_OPT_TIME_DLY, 
                    (OS_ERR     *)&err);
		
		//status = ESP8266_WifiConnect("phone","123456788");
	}while(!status);
	printf("wifi���ӳɹ�\n");
}
/*ģ�鷢�������ⷵ��*/
int8_t ESP8266_SendCmd(char *Cmd,char *Restrict,uint16_t len,int8_t Mode,uint16_t TimeOut)
{
	return  UsartSendCmd(mdEsp8266.Usart,Cmd,Restrict,len,Mode,NULL,NULL,NULL,TimeOut);
}


/*ģ�����߼��*/
int8_t ESP8266_Check()
{
	Esp8266ParInfo *sInfo = NULL;
	
	sInfo = &Esp8266ParmInfos[0];
	
	return ESP8266_SendCmd(sInfo->CmdInfo,sInfo->CmdRsp,0, sInfo->CmdMode,
				sInfo->CmdTimeOut);/*���������豸�Ƿ�����*/
}
/*ģ�鸴λ*/
int8_t ESP8266_Reset()
{
	Esp8266ParInfo *sInfo = NULL;
	
	sInfo = (Esp8266ParInfo *)&Esp8266ParmInfos[1];
	
	return ESP8266_SendCmd(sInfo->CmdInfo,sInfo->CmdRsp,0, sInfo->CmdMode,
				sInfo->CmdTimeOut);/*��λ�豸*/
	return 1;
}
int8_t ESP8266_CmdDisplay(uint8_t Mode)
{
	char CmdBuf[10] = {0};
	Esp8266ParInfo *sInfo = NULL;
	
	sInfo = &Esp8266ParmInfos[2];
	
	sprintf(CmdBuf,"%s%d\r\n",sInfo->CmdInfo,Mode);
	
	return  ESP8266_SendCmd(CmdBuf,sInfo->CmdRsp,0, sInfo->CmdMode,
				sInfo->CmdTimeOut);/*����ʾ*/
	
}
/*�����Ǻ�wifi��ص�ָ��*/
int8_t ESP8266_WifiModeSet(uint8_t Mode)
{
	Esp8266ParInfo *sInfo = NULL;
	char CmdBuf[30] = {0};
	
	sInfo = &Esp8266ParmInfos[3];
	
	sprintf(CmdBuf,"%s%d\r\n",sInfo->CmdInfo,Mode);
	
	return  ESP8266_SendCmd(CmdBuf,sInfo->CmdRsp,0 ,sInfo->CmdMode,
				sInfo->CmdTimeOut);/*wifiģʽ����*/
}
int8_t ESP8266_WifiConnect(char *WifiSsid,char *WifiPasswd)
{
	Esp8266ParInfo *sInfo = NULL;
	char CmdBuf[50] = {0};
	
	sInfo = &Esp8266ParmInfos[5];
	
	sprintf(CmdBuf,"%s\"%s\",\"%s\"\r\n",sInfo->CmdInfo,WifiSsid,WifiPasswd);
	
	return  ESP8266_SendCmd(CmdBuf,sInfo->CmdRsp,0, sInfo->CmdMode,
				sInfo->CmdTimeOut);/*wifiģʽ����*/
}
/*wifi״̬��ѯ*/
int8_t ESP8266_WifiConnectStatus()
{
	
	Esp8266ParInfo *sInfo = NULL;
	
	sInfo = &Esp8266ParmInfos[11];
	
	return  ESP8266_SendCmd(sInfo->CmdInfo,sInfo->CmdRsp,0 ,sInfo->CmdMode,
				sInfo->CmdTimeOut);/*wifiģʽ����*/
}
/*�Ͽ�wifi*/
int8_t ESP8266_WifiDisconnect()
{
	Esp8266ParInfo *sInfo = NULL;
	
	sInfo = &Esp8266ParmInfos[6];
	
	return  ESP8266_SendCmd(sInfo->CmdInfo,sInfo->CmdRsp,0, sInfo->CmdMode,
				sInfo->CmdTimeOut);/*wifiģʽ����*/
}

/*����������*/
int8_t ESP8266_TcpMux(int8_t Mode)
{
	Esp8266ParInfo *sInfo = NULL;
	char CmdBuf[30] = {0};
	
	sInfo = &Esp8266ParmInfos[7];
	
	sprintf(CmdBuf,"%s%d\r\n",sInfo->CmdInfo,Mode);
	
	return  ESP8266_SendCmd(CmdBuf,sInfo->CmdRsp, 0,sInfo->CmdMode,
				sInfo->CmdTimeOut);/*wifi����������*/
	
}
/*tcp����*/
int8_t ESP8266_TcpClientConnect(uint8_t Socket,char *ServerIp,uint16_t ServerPort)
{
	Esp8266ParInfo *sInfo = NULL;
	char CmdBuf[50] = {0};
	
	sInfo = &Esp8266ParmInfos[8];
	
	sprintf(CmdBuf,"%s%d,\"TCP\",\"%s\",%d\r\n",sInfo->CmdInfo,Socket,ServerIp,ServerPort);
	
	return  ESP8266_SendCmd(CmdBuf,sInfo->CmdRsp,0 ,sInfo->CmdMode,
				sInfo->CmdTimeOut);/*wifi����������*/
}
/*���socket����״̬*/
int8_t ESP8266_TcpConnectStatus()
{
	Esp8266ParInfo *sInfo = NULL;
	char CmdBuf[20] = {0};
	
	sInfo = &Esp8266ParmInfos[14];
	
	sprintf(CmdBuf,"%s\r\n",sInfo->CmdInfo);
	
	return  ESP8266_SendCmd(CmdBuf,sInfo->CmdRsp,0 ,sInfo->CmdMode,
				sInfo->CmdTimeOut);/*wifi����������*/
}
/*��ѯ����IP��ַ*/
int8_t ESP8266_TcpLocalIp()
{
	Esp8266ParInfo *sInfo = NULL;
	
	sInfo = &Esp8266ParmInfos[12];
	
	return  ESP8266_SendCmd(sInfo->CmdInfo,sInfo->CmdRsp,0, sInfo->CmdMode,
				sInfo->CmdTimeOut);/*wifiģʽ����*/
}
int8_t ESP8266_TcpSend(uint8_t Id,char *Buf,uint16_t Len)
{
	Esp8266ParInfo *sInfo = NULL;
	char CmdBuf[512] = {0};
	OS_ERR err;
	int8_t status = 0;
	
	sInfo = &Esp8266ParmInfos[9];
	
	sprintf(CmdBuf,"%s%d,%d\r\n",sInfo->CmdInfo,Id,Len);
	status = ESP8266_SendCmd(CmdBuf,">", 0,sInfo->CmdMode,
			sInfo->CmdTimeOut);/*wifi����������*/
	
	QueueSetEmpty(&mdEsp8266.Usart->USART_RecvQueue);/*�������*/
	
	status = ESP8266_SendCmd(Buf,sInfo->CmdRsp, Len,sInfo->CmdMode,
			sInfo->CmdTimeOut);/*wifi����������*/;

}
/*�Ͽ�TCP����*/
int8_t ESP8266_TcpClientDisconnect(uint8_t Id)
{
	Esp8266ParInfo *sInfo = NULL;
	char CmdBuf[50] = {0};
	
	sInfo = &Esp8266ParmInfos[10];
	
	sprintf(CmdBuf,"%s%d\r\n",sInfo->CmdInfo,Id);
	
	return  ESP8266_SendCmd(CmdBuf,sInfo->CmdRsp,0 ,sInfo->CmdMode,
				sInfo->CmdTimeOut);/*wifi����������*/
}
/*������TCP��������ز���0,�ر�1����*/
int8_t ESP8266_TcpServerConfig(uint8_t SwitchMode,uint16_t ServerPort)
{
	Esp8266ParInfo *sInfo = NULL;
	char CmdBuf[50] = {0};
	
	sInfo = &Esp8266ParmInfos[13];
	
	sprintf(CmdBuf,"%s%d,%d\r\n",sInfo->CmdInfo,SwitchMode,ServerPort);
	
	return  ESP8266_SendCmd(CmdBuf,sInfo->CmdRsp,0, sInfo->CmdMode,
				sInfo->CmdTimeOut);/*wifi����������*/
}

/*****************************************************************************
 * �� �� ��  : ESP8266_GetTimeFromStr
 * �� �� ��  : л��
 * ��������  : 2017��7��5��
 * ��������  : �����緵�ص��ַ�������ȡ��ʱ����Ϣ
 * �������  : struct tm *localTime  ����ʱ��ָ��
               uint8_t *DataPoint  �ַ���ָ��
 * �������  : ��
 * �� �� ֵ  : int8_t
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
int8_t ESP8266_GetTimeFromStr( struct tm *localTime, uint8_t *DataPoint )
{
    /*57939 17-07-05 13:03:47 50 0 0   0.0 UTC(NIST) * */
    uint8_t *Str = NULL;
    char time[3] = {0, 0, 0};								//ʱ�仺����
    Str = strstr(DataPoint," ");
    if(NULL != Str)
    {
	Str++;
	time[0] = Str[0];
	time[1] = Str[1];
	localTime->tm_year = atoi(time);
	/*��*/
	Str+=3;

	time[0] = Str[0];
	time[1] = Str[1];
	localTime->tm_mon = atoi(time);
	/*��*/
	Str+=3;

	time[0] = Str[0];
	time[1] = Str[1];
	localTime->tm_mday = atoi(time);
	/*��*/
	Str+=3;


	time[0] = Str[0];
	time[1] = Str[1];
	localTime->tm_hour = atoi(time);
	/*ʱ*/
	Str+=3;

	time[0] = Str[0];
	time[1] = Str[1];
	localTime->tm_min = atoi(time);
	/*��*/
	Str+=3;

	time[0] = Str[0];
	time[1] = Str[1];
	localTime->tm_sec = atoi(time);
	/*��*/
	localTime->tm_isdst = 1;
	
    }
    
    
}
/*****************************************************************************
 * �� �� ��  : ESP8266_GetTimeFromNet
 * �� �� ��  : л��
 * ��������  : 2017��7��5��
 * ��������  : �������л�ȡʱ����Ϣ
 * �������  : void  ��
 * �������  : ��
 * �� �� ֵ  : int8_t
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
int8_t ESP8266_GetTimeFromNet( void )
{
   OS_ERR err;
    int8_t ConnectStatus = 0;
    uint8_t *TimeStr = NULL;
    struct tm localTime;
    uint16_t  Len = 0;
    uint8_t buf[512] = 0;
	
    /*1�����ӵ���ʱ����*/
    ESP8266_TcpMux(1);/*���ö�·����*/

   do
   {
	    ConnectStatus = ESP8266_TcpClientConnect(0,"24.56.178.140",13);
	    if(ConnectStatus)
	    {
			printf("��ʱ�������ӳɹ�\n");
	    }
    }
    while(!ConnectStatus);
    
    if(-1 != (Len =  UsartWaitRecviceData(mdEsp8266.Usart,0,Len,buf,2000)))
    {
			ESP8266_GetTimeFromStr(&localTime,TimeStr);
			OSTimeDly(  (OS_TICK    )2000, 
                    (OS_OPT     )OS_OPT_TIME_DLY, 
                    (OS_ERR     *)&err);

    	ESP8266_TcpClientDisconnect(0);
    }    
}