/***********************************************************************************
 * �� �� ��   : Update.c
 * �� �� ��   : л��
 * ��������   : 2017��7��6��
 * �ļ�����   : ��Ҫ�������Զ�����������ͷ��������ֳ����ӣ�
 				������������
 * ��Ȩ˵��   : Copyright (c) 2008-2017   xx xx xx xx �������޹�˾
 * ��    ��   : 
 * �޸���־   : 
***********************************************************************************/

#include "Update.h"


static  OS_TCB      taskUpdateTCB;
static  CPU_STK     taskUpdateStk[TASK3_STK_SIZE];

/*****************************************************************************
 * �� �� ��  : UpdateTask
 * �� �� ��  : л��
 * ��������  : 2017��7��6��
 * ��������  : ϵͳԶ�̸�������
 * �������  : void *p_arg  �������
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
void UpdateTask(void *p_arg)
{
	/*��ʼ��*/
	uint8_t *Ptr;
	OS_ERR err;
	char buf[20] = {0};
	int16_t len = 0;
	int8_t ConnectStatus;
	sCmdPaket CmdPakets;
	
	/*��ȡһ�����е�socket*/
	do
	 {
			OSTimeDly(  (OS_TICK    )500, 
                    (OS_OPT     )OS_OPT_TIME_DLY, 
                    (OS_ERR     *)&err);
		   ConnectStatus = ESP8266_TcpClientConnect(1,"192.168.31.33",8001);
		    if(ConnectStatus)
		    {
			printf("Զ�̸��·��������ӳɹ�\n");
		    }
			
	 }while(!ConnectStatus);
	
	/*���ӵ�������*/
	while(1)
	{
#if 0
		len = ESP8266_WaitRecviceData(1,sizeof(buf),buf,100000);/*������ʱ*/
		if(len)
		{
			printf("%d\n",len);
			QueueSetEmpty(&mdEsp8266.Usart->USART_RecvQueue);/*�������*/
			
		}
#else
		OSTimeDly(  (OS_TICK    )100, 
                    (OS_OPT     )OS_OPT_TIME_DLY, 
                    (OS_ERR     *)&err);
		CmdPakets.uiHead = 0x7e7e;
		CmdPakets.uiEnd = 0x5aa5;
		CmdPakets.uiPaketCmd++;
		ESP8266_TcpSend(1,(char *)&CmdPakets,sizeof(sCmdPaket));
#endif
	}
}

void UpdateTaskInit()
{
	OS_ERR err;

	OSTaskCreate(   (OS_TCB     *)&taskUpdateTCB,
                    (CPU_CHAR   *)"UpdateTask",
                    (OS_TASK_PTR)UpdateTask,
                    (void       *)0,
                    (OS_PRIO    )TASK3_PRIO,
                    (CPU_STK    *)&taskUpdateStk[0],
                    (CPU_STK_SIZE)TASK3_STK_SIZE / 10,
                    (CPU_STK_SIZE)TASK3_STK_SIZE,
                    (OS_MSG_QTY )0,
                    (OS_TICK    )0,
                    (void       *)0,
                    (OS_OPT     )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), 
                    (OS_ERR     *)&err);
	
}

