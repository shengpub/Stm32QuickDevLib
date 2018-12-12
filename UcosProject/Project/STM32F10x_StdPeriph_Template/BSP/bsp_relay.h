#ifndef BSP_RELAY_H
#define BSP_RELAY_H



#include "BspConfig.h"
#include "os.h"
#include "bsp_Timer.h"

#define RELAY1_PORT  GPIOC
#define RELAY1_PIN   GPIO_Pin_0

#define RELAY2_PORT  GPIOC
#define RELAY2_PIN  GPIO_Pin_1

#define RELAY3_PORT  GPIOC
#define RELAY3_PIN  GPIO_Pin_2

#define RELAY4_PORT  GPIOC
#define RELAY4_PIN  GPIO_Pin_3

#define RELAY2_BIND_TIMER &mdTimer.Timer3

#define MAX_RELAY_NUM  4
typedef struct tagRelayNodeInfo
{
	GpioInfo Gpio;/*�ü̵�����Ӳ��λ��*/
	uint8_t ucRelayStatus;	/*�̵���״̬*/
	uint8_t ucRelayBusy;
	/*��ʱ��*/
}RelayNodeInfo;

typedef struct tagRelayInfo
{
	RelayNodeInfo RelayInfo[MAX_RELAY_NUM];
	TimerNodeInfo *Timer;/*�󶨵���ʱ����*/
}RelayInfo;

void RelayOpen(uint8_t RelayId,uint32_t ucDelayMs);
void RelayClose(uint8_t RelayId,uint32_t ucDelayMs);
void RelayInit();

extern RelayInfo mdRelayInfo;
#endif