#include "bsp_relay.h"
/*�̵���������*/

GpioInfo RelayGpioInfo[MAX_RELAY_NUM] =
{
	{RELAY1_PORT,RELAY1_PIN},
	{RELAY2_PORT,RELAY2_PIN},
	{RELAY3_PORT,RELAY3_PIN},
	{RELAY4_PORT,RELAY4_PIN}
};

RelayInfo mdRelayInfo;

/*�̵�����ʼ��*/
void RelayInit()
{
	uint8_t ucLoop = 0;
	GPIO_InitTypeDef GPIO_InitStructure;
	/*gpio��Ϣ��ʼ��*/
	for(ucLoop = 0;ucLoop <MAX_RELAY_NUM;ucLoop++)
	{
		mdRelayInfo.RelayInfo[ucLoop].Gpio.GPIO_Port = 
			RelayGpioInfo[ucLoop].GPIO_Port;
		mdRelayInfo.RelayInfo[ucLoop].Gpio.GPIO_Pin = 
			RelayGpioInfo[ucLoop].GPIO_Pin;
		
		GPIO_InitStructure.GPIO_Pin =  mdRelayInfo.RelayInfo[ucLoop].Gpio.GPIO_Pin;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	  GPIO_Init(mdRelayInfo.RelayInfo[ucLoop].Gpio.GPIO_Port, &GPIO_InitStructure);
		
	  GPIO_SetBits(mdRelayInfo.RelayInfo[ucLoop].Gpio.GPIO_Port,
							mdRelayInfo.RelayInfo[ucLoop].Gpio.GPIO_Pin);
	}
	mdRelayInfo.Timer = RELAY2_BIND_TIMER;
}

/*�̵���״̬����ص������������ǰ״̬Ϊ��������ر�
������ڹر�զ��*/
uint16_t RelayStatusDel(uint16_t *arg1,uint16_t *arg2)
{
	uint16_t RelayId = *arg1;
	
	if(mdRelayInfo.RelayInfo[RelayId].ucRelayStatus)
	{
		GPIO_ResetBits(mdRelayInfo.RelayInfo[RelayId].Gpio.GPIO_Port,
							mdRelayInfo.RelayInfo[RelayId].Gpio.GPIO_Pin);
		
		mdRelayInfo.RelayInfo[RelayId].ucRelayStatus = 0;
		mdRelayInfo.RelayInfo[RelayId].ucRelayBusy = 0;/*����*/
	}
	else
	{
		GPIO_SetBits(mdRelayInfo.RelayInfo[RelayId].Gpio.GPIO_Port,
							mdRelayInfo.RelayInfo[RelayId].Gpio.GPIO_Pin);
		mdRelayInfo.RelayInfo[RelayId].ucRelayStatus = 1;
		mdRelayInfo.RelayInfo[RelayId].ucRelayBusy = 0;/*����*/
	}
}

/*�����̵������ڶ�������Ϊ�����󱣳�ʱ��*/
void RelayOpen(uint8_t RelayId,uint32_t ucDelayMs)
{
	GPIO_ResetBits(mdRelayInfo.RelayInfo[RelayId].Gpio.GPIO_Port,
							mdRelayInfo.RelayInfo[RelayId].Gpio.GPIO_Pin);
	
	mdRelayInfo.RelayInfo[RelayId].ucRelayStatus = 1;
	mdRelayInfo.RelayInfo[RelayId].ucRelayBusy = 1;/*æ*/
	if(ucDelayMs > 1 && mdRelayInfo.Timer->TimerInitOk == 1)
	{
		TimerTaskAdd(mdRelayInfo.Timer,RelayStatusDel,
								RelayId,0,0,ucDelayMs);
	}
}
/*�ر�״̬�����κδ���*/
void RelayClose(uint8_t RelayId,uint32_t ucDelayMs)
{
	GPIO_SetBits(mdRelayInfo.RelayInfo[RelayId].Gpio.GPIO_Port,
							mdRelayInfo.RelayInfo[RelayId].Gpio.GPIO_Pin);
	
	mdRelayInfo.RelayInfo[RelayId].ucRelayStatus = 0;
	if(ucDelayMs > 1 && mdRelayInfo.Timer->TimerInitOk == 1)
	{
		//TimerTaskAdd(mdRelayInfo.Timer,RelayStatusDel,
								//RelayId,0,0,ucDelayMs);
		mdRelayInfo.RelayInfo[RelayId].ucRelayBusy = 0;/*����*/
	}
}