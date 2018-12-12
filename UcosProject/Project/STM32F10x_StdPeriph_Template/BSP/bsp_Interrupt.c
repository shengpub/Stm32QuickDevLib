/**
@note ���ļ���Ҫ�����ⲿ�жϷ���	  
*/
#include "bsp_Interrupt.h"

InterruptInfo mdInterruptInfo;

/*�ж�IO����*/
GpioInfo InterruptIoInfo[INTERRUPT_MAX_NUM] = 
{
	{InterruptChannel2Port,InterruptChannel2Pin},
	{InterruptChannel2Port,InterruptChannel2Pin},
	{InterruptChannel2Port,InterruptChannel2Pin},
	{InterruptChannel2Port,InterruptChannel2Pin}
};

uint32_t InterruptInitLineCfg[INTERRUPT_MAX_NUM] =
{
	InterruptChannel2Line,
	InterruptChannel2Line,
	InterruptChannel2Line,
	InterruptChannel2Line
};

uint8_t InterruptPinSourcecfg[INTERRUPT_MAX_NUM] = 
{
	InterruptChannel2PinSource,
	InterruptChannel2PinSource,
	InterruptChannel2PinSource,
	InterruptChannel2PinSource
};

uint8_t InterruptPortSourceccfg[INTERRUPT_MAX_NUM] =
{
	InterruptChannel2PortSource,
	InterruptChannel2PortSource,
	InterruptChannel2PortSource,
	InterruptChannel2PortSource
};
uint8_t InterruptIqrcfg[INTERRUPT_MAX_NUM] = 
{
	InterruptChannel2Irq,
	InterruptChannel2Irq,
	InterruptChannel2Irq,
	InterruptChannel2Irq
};

/*�ж�����*/
static void GPIO_NVIC_InterruptInit(uint8_t ucChannel,uint8_t PreemptionPriority,
																	  uint8_t SubPriority)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitStructure.NVIC_IRQChannel = mdInterruptInfo.sInterruptInfo[ucChannel].ucInterruptIrq; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PreemptionPriority; 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = SubPriority;       
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;     
  NVIC_Init(&NVIC_InitStructure);
	
}
/*�ⲿ�жϳ�ʼ��*/
static void GPIO_EXTI_InterruptInit(uint8_t ucChannel)
{
	 EXTI_InitTypeDef EXTI_InitStructure;
  /*����ж���*/
	EXTI_ClearITPendingBit(mdInterruptInfo.sInterruptInfo[ucChannel].ullInterruptLine);
	
	/*ѡ���ж�IO*/
  GPIO_EXTILineConfig(mdInterruptInfo.sInterruptInfo[ucChannel].ucInterruptPortSource, 
	mdInterruptInfo.sInterruptInfo[ucChannel].ucInterruptPinSource);

  EXTI_InitStructure.EXTI_Line = mdInterruptInfo.sInterruptInfo[ucChannel].ullInterruptLine;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; 
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;                          
  EXTI_Init(&EXTI_InitStructure);
	
}

/*�ⲿ�жϳ�ʼ������Ҫ��֪�ж��ߺ����ȼ�����Ϣ*/
void InterruptInit(uint8_t ucChannel,InterruptCallFunc func,uint8_t ucMastPriority,uint8_t ucSubPriority)
{
	uint8_t ucLoop = 0;
	static uint8_t ucParmFlag = 0;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*�ж�IO��Ϣ��������*/
	for(ucLoop = 0;ucLoop < INTERRUPT_MAX_NUM && 
			ucParmFlag == 0;ucLoop++)
	{
		mdInterruptInfo.sInterruptInfo[ucLoop].InterruptIo.GPIO_Pin =
		InterruptIoInfo[ucLoop].GPIO_Pin;
		
		mdInterruptInfo.sInterruptInfo[ucLoop].InterruptIo.GPIO_Port =
		InterruptIoInfo[ucLoop].GPIO_Port;
		
		
		mdInterruptInfo.sInterruptInfo[ucLoop].ullInterruptLine = 
						InterruptInitLineCfg[ucLoop];/*�ж�������*/
		
		mdInterruptInfo.sInterruptInfo[ucLoop].ucInterruptPinSource = 
						InterruptPinSourcecfg[ucLoop];/*�ж�Դ����*/
		
		mdInterruptInfo.sInterruptInfo[ucLoop].ucInterruptPortSource = 
						InterruptPortSourceccfg[ucLoop];/*�ж�Դ����*/
						
		mdInterruptInfo.sInterruptInfo[ucLoop].ucInterruptIrq = 
						InterruptIqrcfg[ucLoop];/*�ж�Դ����*/
	}
	ucParmFlag = 1;/*ֻ��ʼ��һ��*/
	
	GPIO_InitStructure.GPIO_Pin = 
	mdInterruptInfo.sInterruptInfo[ucChannel].InterruptIo.GPIO_Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;/*��������*/
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(mdInterruptInfo.sInterruptInfo[ucChannel].InterruptIo.GPIO_Port,
							&GPIO_InitStructure);	
	
	mdInterruptInfo.sInterruptInfo[ucChannel].func = func;
	
	GPIO_EXTI_InterruptInit(ucChannel);/*���ø�ͨ�����ⲿ�ж�*/
	GPIO_NVIC_InterruptInit(ucChannel,ucMastPriority,
													ucSubPriority);
}
/*�ж�*/
void EXTI0_IRQHandler(void)
{
	/*do thing*/
	/*�жϼ���������*/
	if(mdInterruptInfo.sInterruptInfo[0].ullInterruptCount < 4294967295)
	{
	//	mdInterruptInfo.sInterruptInfo[0].ullInterruptCount++;
	}
	else
	{
		//mdInterruptInfo.sInterruptInfo[0].ucInterruptFullFlg = 1;
		//mdInterruptInfo.sInterruptInfo[0].ullInterruptCount = 0;
	}
	if(mdInterruptInfo.sInterruptInfo[0].func != NULL)
	{
		mdInterruptInfo.sInterruptInfo[0].func(NULL,NULL);/*�ص�����*/
	}
	
	EXTI_ClearITPendingBit(EXTI_Line0);
}
/*��ȡ��ǰ�жϸ���*/
uint32_t InterruptGetCenterCount(InterruptNodeInf *InterruptNode)
{
	return InterruptNode->ullInterruptCount;
}
/*����жϼ���*/
void IneterruptCleanCount(InterruptNodeInf *InterruptNode)
{
	InterruptNode->ullInterruptCount = 0;
	InterruptNode->ucInterruptFullFlg = 0;
}
void EXTI1_IRQHandler(void)
{
	/*do thing*/
	/*�жϼ���������*/
	OSIntEnter();    //֪ͨUCOS�����ж�
	if(mdInterruptInfo.sInterruptInfo[1].ullInterruptCount < 4294967295)
	{
		mdInterruptInfo.sInterruptInfo[1].ullInterruptCount++;
	}
	else
	{
		mdInterruptInfo.sInterruptInfo[1].ucInterruptFullFlg = 1;
		mdInterruptInfo.sInterruptInfo[1].ullInterruptCount = 0;
	}
	if(mdInterruptInfo.sInterruptInfo[1].func != NULL)
	{
		mdInterruptInfo.sInterruptInfo[1].func(NULL,NULL);/*�ص�����*/
	}
	
	EXTI_ClearITPendingBit(EXTI_Line1);
	OSIntExit();
}

void EXTI2_IRQHandler(void)
{
	/*do thing*/
	/*�жϼ���������*/
	OSIntEnter();    //֪ͨUCOS�����ж�
	EXTI_ClearITPendingBit(EXTI_Line2);
	if(mdInterruptInfo.sInterruptInfo[0].ullInterruptCount < 4294967295)
	{
		mdInterruptInfo.sInterruptInfo[0].ullInterruptCount++;
		
	}
	else
	{
		mdInterruptInfo.sInterruptInfo[0].ucInterruptFullFlg = 1;
		mdInterruptInfo.sInterruptInfo[0].ullInterruptCount = 0;
	}
	OSIntExit();
}

void EXTI3_IRQHandler(void)
{
	/*do thing*/
	/*�жϼ���������*/
	OSIntEnter();    //֪ͨUCOS�����ж�
	if(mdInterruptInfo.sInterruptInfo[3].ullInterruptCount < 4294967295)
	{
		mdInterruptInfo.sInterruptInfo[3].ullInterruptCount++;
	}
	else
	{
		mdInterruptInfo.sInterruptInfo[3].ucInterruptFullFlg = 1;
		mdInterruptInfo.sInterruptInfo[3].ullInterruptCount = 0;
	}
	if(mdInterruptInfo.sInterruptInfo[3].func != NULL)
	{
		mdInterruptInfo.sInterruptInfo[3].func(NULL,NULL);/*�ص�����*/
	}
	
	EXTI_ClearITPendingBit(EXTI_Line3);
	OSIntExit();
}
