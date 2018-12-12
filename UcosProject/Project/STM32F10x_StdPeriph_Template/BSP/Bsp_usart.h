#ifndef _BSP_USART_H_
#define _BSP_USART_H_

#include "BspConfig.h"
#include "stdio.h"
#include "os.h"
#include "Lib/Queue/Queue.h"
#include "bsp_atCmdCofig.h"
#define RECV_IS_ING	 			0X02			/*���ڽ���*/
#define RECV_NEW_DATA   		0X04			/*���ܵ�������*/
#define RECV_FULL				0X08			/*���ܳ���*/
#define SEND_IS_ING				0X10			/*���ڷ���*/
#define SEND_OVER				0X20			/*�������*/
#define SEND_ERROR				0X40			/*����ʧ��*/


/*��Щ��Ӧ���Ǻ�Ӳ����صģ�ֻ�����޸���ϵ��ʱ�Ż��޸ĵ�����*/
/*����1*/
#define USART1_PORT        			 GPIOA
#define USART1_TXD 					GPIO_Pin_9
#define USART1_RXD					GPIO_Pin_10
/*����2*/
#define USART2_PORT 				GPIOA
#define USART2_TXD 					GPIO_Pin_2
#define USART2_RXD					GPIO_Pin_3
/*����3*/
#ifdef STM32F10X_HD
/*����3*/
#define USART3_PORT 				GPIOB
#define USART3_TXD 					GPIO_Pin_10
#define USART3_RXD					GPIO_Pin_11

/*����4*/
#define USART4_PORT					GPIOC
#define USART4_TXD 					GPIO_Pin_10
#define USART4_RXD					GPIO_Pin_11

/*����5*/
#define USART5_PORT_A				GPIOC
#define USART5_TXD_A 				GPIO_Pin_12
#define USART5_RXD_A				GPIO_Pin_2

#define USART5_PORT_B				GPIOD
#define USART5_TXD_B 				GPIO_Pin_12
#define USART5_RXD_B				GPIO_Pin_2


#endif



typedef uint16_t (*USARTCallFunc)(uint16_t arg1,uint16_t arg2);
typedef struct tagUSARTInfo_s
{
	char       		UsartName[10];		/*��������*/
	USART_TypeDef*  USARTx;			/*��Ӧ���ڿ��е�USART1��USART2*/
	uint16_t 		GPIO_Pin_txd;	/*txd����*/
	uint16_t 		GPIO_Pin_rxd;	/*rxd����*/
	GPIO_TypeDef* 	GPIO_USART;		/*�������Ǹ��˿���*/
	uint32_t 		USART_BaudRate; /*������*/
	OS_SEM			USART_SendSem;	/*���ڷ����ź���,��Ҫ���������Ƶ���������ʴ���*/
	OS_SEM			USART_RecvSem;	/*���ڽ����ź���*/
	OS_SEM 			USART_RecvWiteSem;	/*���ڽ��յȴ��ź���*/
	USARTCallFunc   USARTUserDel;	/*�ص�����*/
	Queue			USART_SendQueue;/*���Ͷ���2k*/
	
	Queue			USART_RecvQueue;/*���Ͷ���2k*/
	OS_TCB     taskSendTCB;			/*��������tcb*/
  CPU_STK    taskSendStk[TASK3_STK_SIZE];/*��������ռ�ռ�*/
}USARTInfo_s;	

typedef struct tagUSART_s
{
	USARTInfo_s USART1_s;
	USARTInfo_s USART2_s;
#ifdef STM32F10X_HD
	USARTInfo_s USART3_s;
	USARTInfo_s USART4_s;
	USARTInfo_s USART5_s;
#endif
}USART_s;

extern USART_s mdUsart;

void USARTInit(USART_TypeDef* USARTx,uint32_t USART_BaudRate,
					USARTCallFunc CallBackFun);
int8_t UsartPutChar(USARTInfo_s *Usart,int16_t SendData,uint16_t TimeOut);
int8_t UsartWaitReciveEnd(USARTInfo_s * Usart);/*��ѯ��Ӧ�����Ƿ�������*/
int16_t UsartWaitRecviceData(USARTInfo_s * Usart,uint8_t WaitPort,uint16_t RecvDatLen,uint8_t *RecvBuf,uint16_t TimeOut );
/*�ȴ���������*/
int16_t UsartPutString(USARTInfo_s *Usart,uint8_t *SendString,uint16_t Len,uint16_t TimeOut);
int8_t UsartSendCmd(USARTInfo_s *Usart,char *Cmd,char *Restrict,uint16_t len,
                                        int8_t Mode,AtCmdCallBack CallBack,void *cArg1,void *cArg2,uint16_t TimeOut);
int8_t UsartPutCharFush(uint8_t ch);
extern void USART1_IRQHandler(void);
extern void USART2_IRQHandler(void);
extern void USART3_IRQHandler(void);
extern void UART4_IRQHandler(void);
extern void UART5_IRQHandler(void);



extern USART_s mdUsart;

#define LOG_DEBUG				1
#define LOG_RUN					2
#define LOG_CLOSE				3

#define LOG_CENTER_LEVE	 LOG_DEBUG	/*��ǰ��ӡ�ȼ�Ϊ����ģʽ*/

#define LogMsgStr(LogLevel,MsgStr)\
{\
	if(LogLevel == LOG_CENTER_LEVE)\
	{\
		printf(MsgStr);\
	}\
}
#define LogMsgInt(LogLevel,FormatStr,MsgInt)\
{\
	if(LogLevel == LOG_CENTER_LEVE)\
	{\
		printf(FormatStr,MsgInt);\
	}\
}
#endif
