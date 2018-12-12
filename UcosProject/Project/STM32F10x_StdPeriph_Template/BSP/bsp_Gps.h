#ifndef _GPS_H_
#define _GPS_H_
#include "main.h"

typedef struct tagNmea_slmsg
{
	uint8_t num;			/*���Ǳ��*/
	uint8_t eledeg;		/*��������*/
	uint16_t azideg;	/*���Ƿ�λ��*/
	uint8_t sn;				/*�����*/
}Nmea_slmsg;

typedef struct tagNmea_utc_time
{
	uint16_t year;	
	uint8_t month;	
	uint8_t date;	
	uint8_t hour; 	
	uint8_t min; 	
	uint8_t sec; 	
}Nmea_utc_time;

/*ͨ����ض��壬��Ҫ����������صĶ���*/
typedef struct tagGpsComInfo_s
{
	uint8_t  RecvMessage[1024];		/*���ܻ���*/
	//	uint8_t  SendMessage[GSM_SEND_BUF_LEN];										/*���ͻ���*/
	uint16_t RecvCount;															/*��ǰ���ܸ���*/
	uint16_t SendCount;															/*���η������ݳ���*/
	uint32_t  WorkFlage;	/*���ڹ���״̬��־*/														/*״̬*/
	
}GpsComInfo;


/*������������Ҫ�ı���*/
typedef struct tagGPSFastCmd_s
{
	uint8_t   RecvBuf[512];
	uint8_t   RecvCount;									/*��������洢�ռ�*/
	uint8_t   RecvFlage;									/*���������־��1��־��⵽����ͷ��0����ʾδ��⵽*/
	uint8_t   CheckHead;									/*��⵽����ͷ��*/
}GPSFastCmdInfo;

typedef struct tagGPS_s
{
	uint8_t svnum;/*�ɼ����Ǹ���*/
	Nmea_slmsg slMsg[12];	/*������Ϣ*/
	Nmea_utc_time utc;/*ʱ����Ϣ*/
	double latitude;				/**γ��*/
	
	uint8_t nshemi;					//	��γ/��γN����S		  
	double longitude;			    /*����*/
	uint8_t ewhemi;					//E����W����
	uint8_t gpssta;					/*gps״̬0δ��λ1�ǲ�ֶ�λ2��ֶ�λ6���ڹ���*/
 	uint8_t posslnum;				//���Ǹ���0~12.
 	uint8_t possl[12];				//��λ���Ǳ��
	uint8_t fixmode;					//��λ����1û�ж�λ2 2D��λ 3D��λ
	uint16_t pdop;					//λ�þ�������0~500,ʵ��0~50.0
	uint16_t hdop;					//ˮƽ��������0~500,ʵ��0~50.0
	uint16_t vdop;					//��ֱ��������0~500,ʵ��0~50.0 

	int altitude;			 	  //���θ߶� 
	uint16_t speed;					//��������
	USARTInfo_s *Usart;				/*ģ��󶨵Ĵ���*/
	GPSFastCmdInfo GpsGpgsa;       /*gpgsa��Ϣ*/
}GPS;

extern int8_t  AnalysisGpgsa(uint8_t *buf,uint16_t len);
extern uint8_t GpsInit(void);
extern void GpsFastGPGSACheck(uint8_t arg);
extern uint8_t PaketGpsState(char *cGpsStatBuf,uint16_t bufSize);

extern GPS mdGps;

#endif