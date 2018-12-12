#ifndef _BSP_ULTRASONIC_SENSER_H
#define _BSP_ULTRASONIC_SENSER_H


#include "stm32f10x.h"			
#include "os.h"				/*��Ҫ���ڲ���ϵͳ*/
#include "BspConfig.h"
#include "bsp_Timer.h"		/*��Ҫʹ�ö�ʱ��*/
#include "bsp_Interrupt.h" /*��Ҫʹ���ⲿ�ж�*/
#include "bsp_SysDelay.h"

/*����������������ԭ��
���ȷ��������źţ�������һ����ʱ�������жϣ��յ��жϺ������ʱ���������������*/

#define UltrasonicSenserMaxCm 450			/*��Զ450cm*/
#define UltrasonicSenserMinCm 2				/*��С2cm*/

#define UltrasonicSenserTxdPort GPIOA
#define UltrasonicSenserTxd GPIO_Pin_1

#define UltrasonicSenserRxdPort GPIOA
#define UltrasonicSenserRxd GPIO_Pin_0

#define UltrasonicSenserBindTimer &mdTimer.Timer2

#define UltrasonicSenser1BindInter &mdInterruptInfo.sInterruptInfo[0]
#define UltrasonicSenser2BindInter &mdInterruptInfo.sInterruptInfo[1]

typedef struct tagUltrasonicSenserNodeInfo_s
{
	GpioInfo        UltrasonicSenserTx;	/*��������*/
	GpioInfo		UltrasonicSenserRx;	/*����������Ӧ��*/
	InterruptNodeInf *BindInterruptNode;/*�󶨵��ⲿ�жϵĽڵ�*/
	TimerNodeInfo    *BindTimerNode;/*�󶨵��Ķ�ʱ��*/ 
	uint32_t        uliTimerMsCount;/*��ʱ��ms������*/
	uint32_t        uliTimerBeginUsCount;/*��ʼ���ز�֮ǰ��ʱ����us������Ϊ����ȥ���
										 ��ʱ����ֵ��ֻ�ܼ�¼������*/
	uint8_t         ucTimerFlag;
	int8_t          cTimerTaskId;
	volatile float           fCenterCm;		/*��ǰ����*/
	volatile float 					fOldCm;
}UltrasonicSenserNodeInfo;

typedef struct UltrasonicSenserInfo
{
  UltrasonicSenserNodeInfo SenserNode[2];//��·
  OS_SEM	 Sem;
  OS_TCB     taskUltrasonicSenserTCB;	
  CPU_STK    taskUltrasonicSenserStk[TASK3_STK_SIZE];
}UltrasonicSenserInfo;

void OpenCheckSenserEchoTimer(uint8_t SenserId);
void CloseCheckSenserEchoTimer(uint8_t SenserId);
void UltrasonicSenserInit();
float GetCenterSenserCm(uint8_t SenserId);/*�����ṩ�ľ����ȡ*/

extern volatile UltrasonicSenserInfo mdUltrasonicSenserInfo;

#endif