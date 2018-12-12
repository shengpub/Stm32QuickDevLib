#include "bsp_Timer.h"
#include "string.h"
TimerInfo mdTimer;


void TimerNvicConfig(uint8_t TimerIrqChannel,uint8_t SubPriority)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);   // ��ռʽ���ȼ��� 
	
	NVIC_InitStructure.NVIC_IRQChannel = TimerIrqChannel;//ָ���ж�Դ
	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = SubPriority;// ָ����Ӧ���ȼ���1
	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);
}

/*****************************************************************************
 * �� �� ��  : TimerInit
 * �� �� ��  : xiegui
 * ��������  : 2017��11��28��
 *@brief ��������  :  ��ʱ����ʼ��
 * �������  : ��
 * �������  : ��
 * �� �� ֵ  : void
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
void TimerAdvanceInit(TIM_TypeDef* Timer,uint16_t TimerPeriod,uint16_t TimerPrescaler,
				uint16_t TimerMode,uint16_t TimerClockDivision,
				TimerCallFunc callFunc,uint8_t ucFlage,uint8_t ucPwmEnable)/*TIM_CounterMode_Up*/
{
     TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	 
	 
	 TIM_TimeBaseStructure.TIM_Period = TimerPeriod - 1;/*��ʱ������ֵ*/
	 
	 TIM_TimeBaseStructure.TIM_Prescaler = TimerPrescaler - 1;
	 /*��ʱ��Ԥ�ȷ�Ƶֵ��ʱ����72MHZ�������Ҫ��ʱ1ms,���Բ��������ֵ�������ã�����ò�Ҫ
	 ����1MS����Ϊ�ж�̫Ƶ�����������һֱû������ʱ��
	 72000000/36000 = 2000HZ= 0.5mS,���TimerPeriod == 2000�������ʱ1S
	 ���Ҫ��ʱ10ms,TimerPeriod = 200
	 ��ʱ1us�ķ�����
	 72000000/72 = 1000000Hz = 1Mhz   1us*/
	 TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	 
	 TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

	 switch((uint32_t)Timer)
	 {
		case (uint32_t)TIM2:
			{
				TIM_InternalClockConfig(TIM2);
				if(ucFlage)/*ǿ�Ƴ�ʼ��*/
				{
					mdTimer.Timer2.TimerUseSate =0;
				}
				if(!mdTimer.Timer2.TimerUseSate)
				{
					mdTimer.Timer2.TimerPeriod = TimerPeriod;
					mdTimer.Timer2.TimerPrescaler = TimerPrescaler;
					mdTimer.Timer2.TimerCounterMode = TimerMode;
					mdTimer.Timer2.IntBackFun = callFunc;
					mdTimer.Timer2.TimerUseSate = 1;
					mdTimer.Timer2.TimetBaseMs = (TimerPeriod / 2);
					mdTimer.Timer2.TimerInitOk = 1;
					TIM_DeInit(TIM2);
					TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
					if(!ucPwmEnable)
					{
						TIM_ClearFlag(TIM2,TIM_FLAG_Update);
						 TIM_ClearITPendingBit(TIM2 , TIM_IT_Update);
						TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
						//TIM_Cmd(TIM2, ENABLE);
						TimerNvicConfig(TIM2_IRQn,0);
					}
				}
			};break;
		case (uint32_t)TIM3:
			{
				TIM_InternalClockConfig(TIM3);
				if(ucFlage)/*ǿ�Ƴ�ʼ��*/
				{
					mdTimer.Timer3.TimerUseSate =0;
				}
				if(!mdTimer.Timer3.TimerUseSate)
				{
					mdTimer.Timer3.TimerPeriod = TimerPeriod;
					mdTimer.Timer3.TimerPrescaler = TimerPeriod;
					mdTimer.Timer3.TimerCounterMode = TimerMode;
					mdTimer.Timer3.IntBackFun = callFunc;
					mdTimer.Timer3.TimerUseSate = 1;
					mdTimer.Timer3.TimetBaseMs = (TimerPeriod / 2);
					mdTimer.Timer3.TimerInitOk = 1;
					TIM_DeInit(TIM3);
					TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
					if(!ucPwmEnable)
					{
						TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
						 TIM_ClearITPendingBit(TIM3 , TIM_IT_Update);
						TIM_Cmd(TIM3, ENABLE);
						TimerNvicConfig(TIM3_IRQn,1);
					}
				}
			};break;
	 }
	  
}

/*****************************************************************************
 * �� �� ��  : TimerInit
 * �� �� ��  : xiegui
 * ��������  : 2017��11��30��
 * ��������  : 
 * �������  : ��
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
void  TimerInit(TIM_TypeDef* Timer,uint16_t uiMsDelay,TimerCallFunc callFunc,uint8_t ucFlage,uint8_t ucPwmEnable)
{
	/*��ʱ��Χ0.1MS - 65536 * 0.5ms ---- 0.1ms - 32.768s*/
		//memset(&mdTimer,0,sizeof(TimerInfo));
    TimerAdvanceInit(Timer,uiMsDelay * 2,36000,0,0,callFunc,ucFlage,ucPwmEnable);
}
/*��ȡһ�����еĶ�ʱ����洢�ռ�*/
int8_t TimerTaskGetFreeReg(TimerNodeInfo *TimerNode)
{
	int8_t ucLoop = 0;
	
	for(ucLoop = 0;ucLoop < MAX_TIMER_TASK;ucLoop++)
	{
		if(TimerNode->TimerTask[ucLoop].TimerNodeStatus == TIMER_TASK_BUF_FREE)
		{
			TimerNode->TimerTask[ucLoop].TimerNodeStatus = TIMER_TASK_BUF_USE;
			return ucLoop;
		}
	}
	return -1;
}

/*��ʱ�����������һ����ʱ����
��ӳɹ����ض�Ӧ��Id�����򷵻�-1
�����������1��Ҫ��ӵ��Ķ�ʱ����2��Ҫ��ӵĻص�������
						3��Ҫ��ӵĲ���1��   4��Ҫ��ӵĲ���2
						5����ʱ����ģʽ			 6����ʱ����ʱִ��ʱ��*/
int8_t TimerTaskAdd(TimerNodeInfo *TimerNode,TimerCallFunc hook,
										uint16_t arg1,uint16_t arg2,uint8_t TaskMode,
										uint32_t uiTime)
{
	int8_t TaskId = 0;
	/*��ȡ��ǰ��ʱ�����е�λ�ã�*/
	TaskId = TimerTaskGetFreeReg(TimerNode);
	if(TaskId != -1)
	{
		TimerNode->TimerTask[TaskId].Func = hook;
		TimerNode->TimerTask[TaskId].arg1 = arg1;
		TimerNode->TimerTask[TaskId].arg2 = arg2;
		TimerNode->TimerTask[TaskId].TimerMode = TaskMode;
		TimerNode->TimerTask[TaskId].uiDelayMs = uiTime;
		TimerNode->TimerTask[TaskId].uiDelayCount = 0;
		return TaskId;
	}
	return -1;
}
/*��ʱ����ɾ���������ɾ�����ƺܼ򵥣�ֻ��Ҫ�������ģʽ�����޸�Ϊִֻ��һ��
���ɣ�ǧ��Ҫȥ���������������������*/
int8_t TimerTaskDelete(TimerNodeInfo *TimerNode,int8_t TimerTaskId)
{
	TimerNode->TimerTask[TimerTaskId].TimerMode = TIMER_TASK_ONECE; /*�´�ִ�����Զ��˳�*/
}
/*****************************************************************************
 * �� �� ��  : TIM1_IRQHandler
 * �� �� ��  : xiegui
 * ��������  : 2017��11��28��
 * ��������  : ��ʱ��1�жϷ����
 * �������  : ��
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
void TIM1_IRQHandler()
{
	
}
/*****************************************************************************
 * �� �� ��  : TIM3_IRQHandler
 * �� �� ��  : xiegui
 * ��������  : 2017��11��28��
 * ��������  : ��ʱ��1�жϷ����
 * �������  : ��
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 
*****************************************************************************/

void TIM2_IRQHandler()
{
		OS_ERR err;
	uint8_t ucLoop = 0;
	static uint16_t ucCount = 0;
	OSIntEnter();    //֪ͨUCOS�����ж�
	
	if(TIM_GetITStatus(TIM2 , TIM_IT_Update) == SET)
	{
	  TIM_ClearITPendingBit(TIM2 , TIM_IT_Update);
	  /*do user thing*/
	  if(mdTimer.Timer2.IntBackFun != NULL)
	  {
			mdTimer.Timer2.IntBackFun(NULL,NULL);
	  }
		for(ucLoop = 0;ucLoop < MAX_TIMER_TASK;ucLoop++)
		{
			if( (((++mdTimer.Timer2.TimerTask[ucLoop].uiDelayCount)) >= 
				mdTimer.Timer2.TimerTask[ucLoop].uiDelayMs) &&
				mdTimer.Timer2.TimerTask[ucLoop].TimerNodeStatus == TIMER_TASK_BUF_USE &&
				mdTimer.Timer2.TimerTask[ucLoop].Func != NULL)
			{
				/*do thing*/
				mdTimer.Timer2.TimerTask[ucLoop].uiDelayCount =0;
				mdTimer.Timer2.TimerTask[ucLoop].Func(&mdTimer.Timer2.TimerTask[ucLoop].arg1,
				&mdTimer.Timer2.TimerTask[ucLoop].arg2);
				if(mdTimer.Timer2.TimerTask[ucLoop].TimerMode == TIMER_TASK_ONECE)
				{
					//memset(&mdTimer.Timer2.TimerTask[0],0,sizeof(TimerCallFunInfo));
					mdTimer.Timer2.TimerTask[ucLoop].TimerNodeStatus  = TIMER_TASK_BUF_FREE;
				}
			}
		}
		/*��ʱ����*/
	}
	OSIntExit();
}

/*****************************************************************************
 * �� �� ��  : TIM3_IRQHandler
 * �� �� ��  : xiegui
 * ��������  : 2017��11��28��
 * ��������  : ��ʱ��1�жϷ����
 * �������  : ��
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/

void TIM3_IRQHandler()
{
	OS_ERR err;
	uint8_t ucLoop = 0;
	OSIntEnter();    //֪ͨUCOS�����ж�
	
	if(TIM_GetITStatus(TIM3 , TIM_IT_Update) == SET)
	{
	  TIM_ClearITPendingBit(TIM3 , TIM_FLAG_Update);
	  /*do user thing*/
	  if(mdTimer.Timer3.IntBackFun != NULL)
	  {
			mdTimer.Timer3.IntBackFun(NULL,NULL);
	  }
		for(ucLoop = 0;ucLoop < MAX_TIMER_TASK;ucLoop++)
		{
			
			if( ((mdTimer.Timer3.TimetBaseMs * (++mdTimer.Timer3.TimerTask[ucLoop].uiDelayCount)) >= 
				mdTimer.Timer3.TimerTask[ucLoop].uiDelayMs) &&
				mdTimer.Timer3.TimerTask[ucLoop].TimerNodeStatus == TIMER_TASK_BUF_USE &&
				mdTimer.Timer3.TimerTask[ucLoop].Func != NULL)
			{
				/*do thing*/
				mdTimer.Timer3.TimerTask[ucLoop].uiDelayCount =0;
				mdTimer.Timer3.TimerTask[ucLoop].Func(&mdTimer.Timer3.TimerTask[ucLoop].arg1,
				&mdTimer.Timer3.TimerTask[ucLoop].arg2);
				if(mdTimer.Timer3.TimerTask[ucLoop].TimerMode == TIMER_TASK_ONECE)
				{
					memset(&mdTimer.Timer3.TimerTask[ucLoop],0,sizeof(TimerCallFunInfo));
				}
			}
		}
		/*��ʱ����*/
	}
	OSIntExit();
}


