#ifndef BSP_ONENETSERVER_H
#define BSP_ONENETSERVER_H

#include "stm32f10x.h"			
#include "os.h"				/*��Ҫ���ڲ���ϵͳ*/
#include "BspConfig.h"
#include "Bsp_Esp8266.h"

#define MQTT_SERVER_IP 		"183.230.40.39"
#define MQTT_SERVER_PORT  6002

/** MQTT���ݰ����� */
enum MqttPacketType {
    MQTT_PKT_CONNECT = 1, /**< �����������ݰ� */
    MQTT_PKT_CONNACK,     /**< ����ȷ�����ݰ� */
    MQTT_PKT_PUBLISH,     /**< �����������ݰ� */
    MQTT_PKT_PUBACK,      /**< ����ȷ�����ݰ� */
    MQTT_PKT_PUBREC,      /**< ���������ѽ������ݰ���Qos 2ʱ���ظ�MQTT_PKT_PUBLISH */
    MQTT_PKT_PUBREL,      /**< ���������ͷ����ݰ��� Qos 2ʱ���ظ�MQTT_PKT_PUBREC */
    MQTT_PKT_PUBCOMP,     /**< ����������ݰ��� Qos 2ʱ���ظ�MQTT_PKT_PUBREL */
    MQTT_PKT_SUBSCRIBE,   /**< �������ݰ� */
    MQTT_PKT_SUBACK,      /**< ����ȷ�����ݰ� */
    MQTT_PKT_UNSUBSCRIBE, /**< ȡ���������ݰ� */
    MQTT_PKT_UNSUBACK,    /**< ȡ������ȷ�����ݰ� */
    MQTT_PKT_PINGREQ,     /**< ping ���ݰ� */
    MQTT_PKT_PINGRESP,    /**< ping ��Ӧ���ݰ� */
    MQTT_PKT_DISCONNECT   /**< �Ͽ��������ݰ� */
};


/*mqttЭ��ͷ��*/
typedef struct tagMQTTHeader_s
{
	uint8_t ucPacketType;					/*mqtt����������*/
	
	uint8_t ucRemainingLenth;				/*ʣ�೤��*/
}MQTTHeader;

/*mqtt����Э��*/
typedef struct tagMQTTConnect_s
{
	MQTTHeader sHeader;						/*�̶�ͷ��*/
	uint16_t uiProtocolNameLength;		    /*Э�����Ƴ���*/
	uint8_t 	uiMqttName[4];				/*Э������*/
	uint8_t ucProtocolLevel;				/*Э��ȼ�*/
	uint8_t ucConnectFlag;					/*״̬��ʶ*/
	uint16_t ucKeepAliveTime;				/*����ʱ��*/
											/*��������Ϣ�ֶ���д*/
}MQTTConnect;

typedef struct tagMQTTHeart_s
{
	MQTTHeader sHeader;							/*�̶�ͷ��*/
}MQTTHeart;

typedef struct tagMQTTPush_s
{
	MQTTHeader sHeader;							/*�̶�ͷ��*/
	uint8_t ucRemainingLenth2;				/*ʣ�೤��*/
}MQTTPush;


#define USER_BUFF_LEN									25	
#define CONNECT_USER_FLAGE 								0X80					/*�û�flag*/
#define CONNECT_PASSWD_FLAGE							0X40


void OnenetConnectMqttServer(uint8_t *ucMqttServerIp,uint16_t uiMqttServerPort);
void OnenetMqttConnect(uint16_t KeepOnlineTime,char *DeviceId,
							 char *UserName,char *Password);
void OnenetKeepAliveInit();
uint16_t MQTTPublist(char *TopicName,float fCenterCm,float fHightCm,float fLowCm,
									uint16_t uiSysStatusCode,uint16_t uiWaterPercent);


#endif
