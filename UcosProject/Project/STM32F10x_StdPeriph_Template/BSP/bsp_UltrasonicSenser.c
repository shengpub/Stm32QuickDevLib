#include  "bsp_UltrasonicSenser.h"

volatile UltrasonicSenserInfo mdUltrasonicSenserInfo;
float cm = 0.0;
GpioInfo UltrasonicSenserGpioCfg[2] = 
{
	{UltrasonicSenserTxdPort,UltrasonicSenserTxd},	
	{UltrasonicSenserRxdPort,UltrasonicSenserRxd}
};

/*****************************************************************************
 * �� �� ��  : UltrasonicSenserTask
 * �� �� ��  : xiegui
 * ��������  : 2018��1��21��
 * ��������  : ������������1ms��ʱ����û1ms����һ�θú���
 * �������  : uint16_t *arg1  �������1
               uint16_t *arg2  �������2
 * �������  : ��
 * �� �� ֵ  : uint16_t
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
uint16_t UltrasonicSenserTask(uint16_t *arg1,uint16_t *arg2)
{
 	uint16_t SenerId = 0;
	
 	if(arg1 != NULL)
 	{
		SenerId  = *arg1;
 	}
	mdUltrasonicSenserInfo.SenserNode[0].uliTimerMsCount++;
}

/*****************************************************************************
 * �� �� ��  : GetUltrasonicSenserEchoTime
 * �� �� ��  : xiegui
 * ��������  : 2018��1��21��
 * ��������  : ��ȡָ���������Ļظ�ʱ�䣬��ȡ������Ҫ���
 * �������  : uint8_t SenserId  ������ID
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
uint32_t GetUltrasonicSenserEchoTime(uint8_t SenserId)
{
	uint32_t SenserEchoTime = 0;

	SenserEchoTime = mdUltrasonicSenserInfo.SenserNode[SenserId].uliTimerMsCount * 1000;/*us*/
	
	SenserEchoTime += TIM_GetCounter(TIM2);
	
	TIM2->CNT=0;
	
	mdUltrasonicSenserInfo.SenserNode[SenserId].uliTimerMsCount = 0;
	mdUltrasonicSenserInfo.SenserNode[SenserId].uliTimerBeginUsCount = 0;
	return SenserEchoTime;/*���ص�ǰ��ֵ*/
}

void UltrasonicSenserDelay_Us(uint16_t time)  //��ʱ����
{ 
    uint16_t i,j;
    for(i=0;i<time;i++)
          for(j=0;j<9;j++);
}

void UltrasonicSenserTasks(void *p_arg)
{
	OS_ERR     err;
	float    fCm = 0;
	float    fCemterCm = 0;
	uint32_t ulTimes = 0;
	uint8_t  ucCount = 0;
	while(1)
	{
		ucCount = 0;
		while(ucCount < 10)
		{
			
			/*���͸ߵ�ƽ*/
			GPIO_SetBits(mdUltrasonicSenserInfo.SenserNode[0].UltrasonicSenserTx.GPIO_Port,
			mdUltrasonicSenserInfo.SenserNode[0].UltrasonicSenserTx.GPIO_Pin);/*���͸ߵ�ƽ*/
			
			UltrasonicSenserDelay_Us(20);/*�ȴ�20us*/
			
			GPIO_ResetBits(mdUltrasonicSenserInfo.SenserNode[0].UltrasonicSenserTx.GPIO_Port,
			mdUltrasonicSenserInfo.SenserNode[0].UltrasonicSenserTx.GPIO_Pin);/*���͵͵�ƽ*/
			
			while(!GPIO_ReadInputDataBit(mdUltrasonicSenserInfo.SenserNode[0].UltrasonicSenserRx.GPIO_Port,
				mdUltrasonicSenserInfo.SenserNode[0].UltrasonicSenserRx.GPIO_Pin));
			
			OpenCheckSenserEchoTimer(0);/*��ʼ����*/
			
			while(GPIO_ReadInputDataBit(mdUltrasonicSenserInfo.SenserNode[0].UltrasonicSenserRx.GPIO_Port,
				mdUltrasonicSenserInfo.SenserNode[0].UltrasonicSenserRx.GPIO_Pin));
			
			ulTimes = GetUltrasonicSenserEchoTime(0);
			if(ulTimes <= 130)
			{
				ulTimes = 120;
			}
			fCemterCm = ((float)ulTimes / 58.0);
			if(fCemterCm > UltrasonicSenserMaxCm)
			{
				fCemterCm = UltrasonicSenserMaxCm;
			}
			fCm += fCemterCm;
			CloseCheckSenserEchoTimer(0);
			ucCount+=1;
			OSTimeDly(  (OS_TICK    )50, 
	                    (OS_OPT     )OS_OPT_TIME_DLY, 
	                    (OS_ERR     *)&err);/*1ms*/
		}
		fCm /= 10.0;
		//OSSemPend(&mdUltrasonicSenserInfo.Sem,OS_OPT_PEND_BLOCKING,1000,NULL,&err);/*�ȴ��ź���*/
		
		mdUltrasonicSenserInfo.SenserNode[0].fCenterCm = fCm;
		if(mdUltrasonicSenserInfo.SenserNode[0].fOldCm != fCm)
		{
			mdUltrasonicSenserInfo.SenserNode[0].fOldCm = mdUltrasonicSenserInfo.SenserNode[0].fCenterCm;
		}
		cm = fCm; 
		//OSSemPost(&mdUltrasonicSenserInfo.Sem,OS_OPT_POST_ALL,&err);/*�ͷ��ź���*/
		fCm = 0;
		OSTimeDly(  (OS_TICK    )500, 
	                    (OS_OPT     )OS_OPT_TIME_DLY, 
	                    (OS_ERR     *)&err);/*1ms*/
		
	}
}
/*��ȡˮ����봫�����ĸ߶�*/
float GetCenterSenserCm(uint8_t SenserId)
{
	OS_ERR     err;
	float CenterCm = 0.0;
	//OSSemPend(&mdUltrasonicSenserInfo.Sem,OS_OPT_PEND_BLOCKING,1000,NULL,&err);/*�ȴ��ź���*/
	
	CenterCm = cm;
	//OSSemPost(&mdUltrasonicSenserInfo.Sem,OS_OPT_POST_ALL,&err);/*�ͷ��ź���*/
	return CenterCm;
}
void UltrasonicSenserInit()
{
	uint8_t ucLoop = 0;
	OS_ERR err;
	GPIO_InitTypeDef GPIO_InitStructure;
	//OSSemCreate(&mdUltrasonicSenserInfo.Sem, "SenserSem", 1, &err);/*�ź�������*/
	//OSSemPost(&mdUltrasonicSenserInfo.Sem,OS_OPT_POST_ALL,&err);/*�ͷ��ź���*/
	 
	TimerAdvanceInit(TIM2,1000,72,0,0,UltrasonicSenserTask,1,0);/*1msһ���ж�*/
	//for(ucLoop = 0;ucLoop < 2;ucLoop++)
	{
		mdUltrasonicSenserInfo.SenserNode[0].UltrasonicSenserTx.GPIO_Port =  
		UltrasonicSenserGpioCfg[0].GPIO_Port;
		
		mdUltrasonicSenserInfo.SenserNode[0].UltrasonicSenserTx.GPIO_Pin =  
		UltrasonicSenserGpioCfg[0].GPIO_Pin;

		mdUltrasonicSenserInfo.SenserNode[0].UltrasonicSenserRx.GPIO_Port =  
		UltrasonicSenserGpioCfg[1].GPIO_Port;
		
		mdUltrasonicSenserInfo.SenserNode[0].UltrasonicSenserRx.GPIO_Pin =  
		UltrasonicSenserGpioCfg[1].GPIO_Pin;
		/*txd�����*/
		GPIO_InitStructure.GPIO_Pin = mdUltrasonicSenserInfo.SenserNode[0].UltrasonicSenserTx.GPIO_Pin;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(mdUltrasonicSenserInfo.SenserNode[0].UltrasonicSenserTx.GPIO_Port,&GPIO_InitStructure);
		GPIO_ResetBits(mdUltrasonicSenserInfo.SenserNode[0].UltrasonicSenserTx.GPIO_Port,
					mdUltrasonicSenserInfo.SenserNode[0].UltrasonicSenserTx.GPIO_Pin);

		/*rxd*/
		GPIO_InitStructure.GPIO_Pin = mdUltrasonicSenserInfo.SenserNode[0].UltrasonicSenserRx.GPIO_Pin;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(mdUltrasonicSenserInfo.SenserNode[0].UltrasonicSenserRx.GPIO_Port,&GPIO_InitStructure);
		GPIO_ResetBits(mdUltrasonicSenserInfo.SenserNode[0].UltrasonicSenserRx.GPIO_Port,
					mdUltrasonicSenserInfo.SenserNode[0].UltrasonicSenserRx.GPIO_Pin);
		
		mdUltrasonicSenserInfo.SenserNode[0].BindTimerNode = UltrasonicSenserBindTimer;
	}
	/*��ʱ����*/
	mdUltrasonicSenserInfo.SenserNode[0].BindInterruptNode = UltrasonicSenser1BindInter;
	mdUltrasonicSenserInfo.SenserNode[0].BindInterruptNode = UltrasonicSenser2BindInter;

	/*��������*/
	

	OSTaskCreate(   (OS_TCB     *)&mdUltrasonicSenserInfo.taskUltrasonicSenserTCB,
                    (CPU_CHAR   *)"UltrasonicSensers",
                    (OS_TASK_PTR)UltrasonicSenserTasks,
                    (void       *)0,
                    (OS_PRIO    )TASK2_PRIO,
                    (CPU_STK    *)&mdUltrasonicSenserInfo.taskUltrasonicSenserStk[0],
                    (CPU_STK_SIZE)TASK3_STK_SIZE / 10,
                    (CPU_STK_SIZE)TASK3_STK_SIZE,
                    (OS_MSG_QTY )0,
                    (OS_TICK    )0,
                    (void       *)0,
                    (OS_OPT     )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), 
                    (OS_ERR     *)&err);

	OSTimeDly(  (OS_TICK    )500, 
	                    (OS_OPT     )OS_OPT_TIME_DLY, 
	                    (OS_ERR     *)&err);/*100ms,�ó���������������������*/
	
}
/*****************************************************************************
 * �� �� ��  : OpenCheckSenserEchoTimer
 * �� �� ��  : xiegui
 * ��������  : 2018��1��21��
 * ��������  : �����������������ķ��ؽ��ܶ�ʱ����
 * �������  : ��
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
void OpenCheckSenserEchoTimer(uint8_t SenserId)
{
	TIM_SetCounter(TIM2,0);
	
	mdUltrasonicSenserInfo.SenserNode[SenserId].uliTimerMsCount = 0;

	TIM_Cmd(TIM2,ENABLE);
}
/*****************************************************************************
 * �� �� ��  : CloseCheckSenserEchoTimer
 * �� �� ��  : xiegui
 * ��������  : 2018��1��21��
 * ��������  : ɾ��ָ������
 * �������  : uint8_t SenserId  ������id
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
void CloseCheckSenserEchoTimer(uint8_t SenserId)
{
	TIM_Cmd(TIM2,ENABLE);
}