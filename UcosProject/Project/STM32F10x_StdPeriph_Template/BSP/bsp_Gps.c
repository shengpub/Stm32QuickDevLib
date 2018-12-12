#include "Gps.h"
#include "usart.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
GPS mdGps;
/*����GPGSA��Ϣ*/

int8_t  AnalysisGpgsa(uint8_t *buf,uint16_t len)
{
	uint8_t temp = 0;
	uint32_t temp32 = 0;
	/*gpsʱ�����ʵʱ���8��Сʱ*/
	temp = (8 + ((buf[7] - 0x30) * 10) +  (buf[8] - 0x30));
	if(temp >= 0 && temp <= 24)
	{
		mdGps.utc.hour = temp;
	}
	
	temp = (((buf[9] - 0x30) * 10) + (buf[10] - 0x30));
	if(temp >=0 && temp < 24)
	{
		mdGps.utc.min = temp;
	}
	
	temp = (((buf[11] - 0x30) * 10) + (buf[12] - 0x30));
	if(temp >=0 && temp < 24)
	{
		mdGps.utc.sec = temp;
	}
	
	buf[27] = 0;/*�����ַ�*/
	temp32 = (uint32_t)(atof((char *)&buf[17])* 10000);
	if(temp32 >= 0 && temp32 <= 4294967295)
	{
		mdGps.latitude = temp32;/*γ����Ϣ*/
	}
	if(buf[28] == 'N' || buf[28] == 'S')
	{
		mdGps.nshemi = buf[28];/*�ϱ�ά*/
	}

	buf[41] = 0;/*�����ַ�*/
	temp32 = (uint32_t)(atof((char *)&buf[30])* 10000);
	if(temp32 >= 0 && temp32 <= 4294967295)
	{
		mdGps.longitude = temp32;/*γ����Ϣ*/
	}
	if(buf[42] == 'E' || buf[42] == 'W')
	{
		mdGps.ewhemi = buf[42];/*�ϱ�ά*/
	}
	/*��λ����*/
	mdGps.fixmode = buf[44] - 0x30;
	/*�������ʱ������*/
	return 1;
	
}
/*���ڿ��ټ��GPGSAC��Ϣ*/
void GpsFastGPGSACheck(uint8_t arg)
{
	if(arg == '$')/**/
	{
		mdGps.GpsGpgsa.RecvCount = 0;
		mdGps.GpsGpgsa.CheckHead = 0; 
		mdGps.GpsGpgsa.RecvFlage = 1;/*��ʱ�����⵽ͷ����Ϣ*/
		mdGps.GpsGpgsa.RecvBuf[mdGps.GpsGpgsa.RecvCount] = arg;
	  return;
	}
	else if(mdGps.GpsGpgsa.RecvFlage == 1 && mdGps.GpsGpgsa.CheckHead == 0)
	{
		mdGps.GpsGpgsa.RecvCount++;
		mdGps.GpsGpgsa.RecvBuf[mdGps.GpsGpgsa.RecvCount] = arg;
		/*ͷ����Ϣ*/
		if(mdGps.GpsGpgsa.RecvCount >= 7)
		{
			/*��ʼ����Ƿ���һ��������GPGSA��Ϣ*/
			if(NULL != strstr(mdGps.GpsGpgsa.RecvBuf,"$GPGGA"))
			{
				mdGps.GpsGpgsa.CheckHead = 1;/*��⵽������ͷ����Ϣ*/
				return;
			}
			else
			{
				memset((char *)&mdGps.GpsGpgsa,0,sizeof(GPSFastCmdInfo));
				return;
			}
		}
	}
	if(mdGps.GpsGpgsa.CheckHead == 1)
	{
		mdGps.GpsGpgsa.RecvCount++;
		mdGps.GpsGpgsa.RecvBuf[mdGps.GpsGpgsa.RecvCount] = arg;
		/*��ʼ���β��*/
		if(arg == '\n' && mdGps.GpsGpgsa.RecvBuf[mdGps.GpsGpgsa.RecvCount - 1] == '\r')
		{
			AnalysisGpgsa(mdGps.GpsGpgsa.RecvBuf,mdGps.GpsGpgsa.RecvCount);
			/*��⵽һ��������GPGSA���ݣ�����ɽ��н���*/
			//printf("�������\n");
			memset((char *)&mdGps.GpsGpgsa,0,sizeof(GPSFastCmdInfo));
		}
	}
}
/*���GPS״̬*/
uint8_t PaketGpsState(char *cGpsStatBuf,uint16_t bufSize)
{
	/*�����GPS��Ϣ���д������*/
	memset(cGpsStatBuf,0,bufSize);
	/*�����ʽ����ά��*/
	sprintf(cGpsStatBuf,"%c%d-%c%d",mdGps.ewhemi,mdGps.longitude,mdGps.nshemi,mdGps.latitude);
	
	
}

uint16_t GpsRecvCmdDel(uint16_t arg1,uint16_t arg2)
{
	mdGps.gpsComInfos.WorkFlage |= RECV_IS_ING;	/*���ڽ���*/
	
	mdGps.gpsComInfos.RecvMessage[mdGps.gpsComInfos.RecvCount] = arg1;
	GpsFastGPGSACheck(arg1);/*����������*/
	mdGps.gpsComInfos.RecvCount++;
	
	if(mdGps.gpsComInfos.RecvCount > 2)
	{
		mdGps.gpsComInfos.WorkFlage |= 0x01;/*���ܵ�������*/
	}
	if(mdGps.gpsComInfos.RecvCount > 1023)
	{
		/*δ����*/
		mdGps.gpsComInfos.RecvCount = 0;
	}
	mdGps.gpsComInfos.WorkFlage &=~RECV_IS_ING;	/*���ڽ���*/
	return 1;
}

uint8_t GpsInit(void)
{
	memset(&mdGps,0,sizeof(GPS));
	
	USARTInit(USART2, 9600);
	mdGps.gpsComInfos
	mdUsart.USART3_s.USARTUserDel = GpsRecvCmdDel;
	/**/
	
}