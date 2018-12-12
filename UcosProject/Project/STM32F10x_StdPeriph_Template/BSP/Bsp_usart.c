#include "Bsp_usart.h"
#include "string.h"

USART_s mdUsart;	/*����ģ��*/

void USARTGpioConfig(USARTInfo_s *UsartInfo)
{
	GPIO_InitTypeDef GPIO_InitStructure;		//����һ���ṹ�����

	GPIO_InitStructure.GPIO_Pin = UsartInfo->GPIO_Pin_txd;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(UsartInfo->GPIO_USART,&GPIO_InitStructure);	

	GPIO_InitStructure.GPIO_Pin = UsartInfo->GPIO_Pin_rxd; 	//PA10---RXD
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	 //��������
	GPIO_Init(UsartInfo->GPIO_USART,&GPIO_InitStructure);		
} 

void USARTNvicConfig(uint8_t NVIC_IRQChannel,uint8_t MasterPro,uint8_t SlaPro)
{
		/*NVIC �����÷����Լ�ѭ��
		1��NVIC_IRQChannel;�����жϺ�
		2��NVIC_IRQChannelPreemptionPriority;������ռ���ȼ� 
		3��NVIC_IRQChannelSubPriority;//���ô����ȼ� 
		4��NVIC_IRQChannelCmdʹ��
	*/
	NVIC_InitTypeDef NVIC_Config;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	NVIC_Config.NVIC_IRQChannel = NVIC_IRQChannel;  //ѡ���ж�Դ 
	NVIC_Config.NVIC_IRQChannelPreemptionPriority= MasterPro;  //��ռ���ȼ�0
	NVIC_Config.NVIC_IRQChannelSubPriority = SlaPro; //�����ȼ�Ϊ0
	NVIC_Config.NVIC_IRQChannelCmd = ENABLE;//ʹ��
	
	NVIC_Init (&NVIC_Config);	
}

void USARTConfig(USART_TypeDef* USARTx,USARTInfo_s *UsartInfo)
{
	USART_InitTypeDef USART_Init_Structure;
	USART_ClockInitTypeDef USART_ClockInitStructure;

	USART_DeInit(USARTx);
	
	USART_Init_Structure.USART_BaudRate = UsartInfo->USART_BaudRate;//������
	USART_Init_Structure.USART_WordLength = USART_WordLength_8b;
	USART_Init_Structure.USART_StopBits = USART_StopBits_1;	 //ֹͣλλ1
	USART_Init_Structure.USART_Parity = USART_Parity_No;	//żУ��
	USART_Init_Structure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//������Ӳ��������
	USART_Init_Structure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    
	//USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
	//USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
	//USART_ClockInitStructure.USART_CPHA = USART_CPHA_1Edge;
	//USART_ClockInitStructure.USART_LastBit = USART_LastBit_Enable;

	USART_Init(USARTx,&USART_Init_Structure);
    
	//USART_ClockInit(USARTx,&USART_ClockInitStructure);

	USART_Cmd(USARTx,ENABLE);
	USART_ClearFlag(USARTx,USART_FLAG_TC);
	USART_ClearFlag(USARTx,USART_IT_RXNE);
	USART_ClearITPendingBit(USARTx,USART_IT_TC);
	USART_ITConfig(USARTx,USART_IT_RXNE,ENABLE); //��������ж�

}

#if 0
/*����*/
	 if(USART_GetITStatus(USART1,USART_FLAG_TXE) != RESET)  
	 {
	 	/*���ӳɹ�*/
		if(DeQueue(&mdUsart.USART1_s.USART_SendQueue,&SendData))
		{
			USART_SendData(USART1,SendData);/*��������*/
		}
		else
		{
			USART_ITConfig(USART1, USART_FLAG_TXE, DISABLE);
		}
		
	 }
	 #endif
/*���ͺ���*/
void SendTask(void *p_arg)
{
	USARTInfo_s *UsartInfo = (USARTInfo_s *)p_arg;
	
	uint16_t SendData = 0;
	OS_ERR     err;
	while(1)
	{
		OSTimeDly(  (OS_TICK    )1, 
                    (OS_OPT     )OS_OPT_TIME_DLY, 
                    (OS_ERR     *)&err);
		/*���ӳɹ�*/
					
		if(DeQueue(&UsartInfo->USART_SendQueue,&SendData))
		{
				USART_SendData(UsartInfo->USARTx,SendData);/*��������*/
		}
	}
}
/*����һ���������񣬸��𽫶����е�����ȡ�������з���*/
void USART_SendTaskCreate(USARTInfo_s *Usart)
{
	OS_ERR 		err;
	
	OSTaskCreate(   (OS_TCB     *)&Usart->taskSendTCB,
                    (CPU_CHAR   *)Usart->UsartName,
                    (OS_TASK_PTR)SendTask,/*��ں���*/
                    (void       *)Usart,
                    (OS_PRIO    )TASK1_PRIO,
                    (CPU_STK    *)&Usart->taskSendStk[0],
                    (CPU_STK_SIZE)TASK3_STK_SIZE / 10,
                    (CPU_STK_SIZE)TASK3_STK_SIZE,
                    (OS_MSG_QTY )0,
                    (OS_TICK    )0,
                    (void       *)0,
                    (OS_OPT     )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), 
                    (OS_ERR     *)&err);

}
void USARTInit(USART_TypeDef* USARTx,uint32_t USART_BaudRate,
					USARTCallFunc CallBackFun)
{
	OS_ERR     err;
	switch((uint32_t)USARTx)
	{
		case (uint32_t)USART1:/*GPIOA USART1 RXD-GPIO9 TXD-GPIO10*/
				{
					mdUsart.USART1_s.GPIO_Pin_rxd = USART1_RXD;/*����ǲ���Ҫ��������*/
					mdUsart.USART1_s.GPIO_Pin_txd = USART1_TXD;
					mdUsart.USART1_s.USART_BaudRate = USART_BaudRate;
					mdUsart.USART1_s.GPIO_USART = USART1_PORT; 
					mdUsart.USART1_s.USARTUserDel = CallBackFun;
					/*�û��ص�����ע��*/
					
					USARTGpioConfig(&mdUsart.USART1_s);/*io������*/
					USARTNvicConfig(USART1_IRQn,7,0);
					USARTConfig(USART1,&mdUsart.USART1_s);
					/*�����ź�������*/
					OSSemCreate(&mdUsart.USART1_s.USART_SendSem, "USART1_SendSem", 1, &err);
					OSSemCreate(&mdUsart.USART1_s.USART_RecvSem, "USART1_RecvSem", 1, &err);
                                    OSSemPost(&mdUsart.USART1_s.USART_SendSem,OS_OPT_POST_ALL,&err);/*�ͷ��ź���*/
                                    OSSemPost(&mdUsart.USART1_s.USART_RecvSem,OS_OPT_POST_ALL,&err);/*�ͷ���*/
					strcpy(mdUsart.USART1_s.UsartName,"USART1");
					mdUsart.USART1_s.USARTx = USART1;
					USART_SendTaskCreate(&mdUsart.USART1_s);
					break;
				}
		case (uint32_t)USART2:
				{
					mdUsart.USART2_s.GPIO_Pin_rxd = USART2_RXD;
					mdUsart.USART2_s.GPIO_Pin_txd = USART2_TXD;
					mdUsart.USART2_s.USART_BaudRate = USART_BaudRate;
					mdUsart.USART2_s.GPIO_USART = USART2_PORT;
					mdUsart.USART2_s.USARTUserDel = CallBackFun;
					/*�û��ص�����ע��*/
					
					USARTGpioConfig(&mdUsart.USART2_s);/*io������*/
					USARTNvicConfig(USART2_IRQn,0,0);
					USARTConfig(USART2,&mdUsart.USART2_s);
					OSSemCreate(&mdUsart.USART2_s.USART_SendSem, "USART2_SendSem", 1, &err);
					OSSemCreate(&mdUsart.USART2_s.USART_RecvSem, "USART2_RecvSem", 1, &err);
                                   OSSemPost(&mdUsart.USART2_s.USART_SendSem,OS_OPT_POST_ALL,&err);/*�ͷ��ź���*/
                                    OSSemPost(&mdUsart.USART2_s.USART_RecvSem,OS_OPT_POST_ALL,&err);/*�ͷ���*/
					strcpy(mdUsart.USART2_s.UsartName,"USART2");
					mdUsart.USART2_s.USARTx = USART2;
					USART_SendTaskCreate(&mdUsart.USART2_s);
					break;
				}
#ifdef STM32F10X_HD

	  case (uint32_t)USART3:
				{
					mdUsart.USART3_s.GPIO_Pin_rxd = USART3_RXD;
					mdUsart.USART3_s.GPIO_Pin_txd = USART3_TXD;
					mdUsart.USART3_s.USART_BaudRate = USART_BaudRate;
					mdUsart.USART3_s.GPIO_USART = USART3_PORT;
					mdUsart.USART3_s.USARTUserDel = CallBackFun;
					/*�û��ص�����ע��*/
					
					USARTGpioConfig(&mdUsart.USART3_s);/*io������*/
					USARTNvicConfig(USART3_IRQn,0,0);
					USARTConfig(USART3,&mdUsart.USART3_s);
					OSSemCreate(&mdUsart.USART3_s.USART_SendSem, "USART3_SendSem", 1, &err);
					OSSemCreate(&mdUsart.USART3_s.USART_RecvSem, "USART3_RecvSem", 1, &err);
                                    OSSemPost(&mdUsart.USART3_s.USART_SendSem,OS_OPT_POST_ALL,&err);/*�ͷ��ź���*/
                                    OSSemPost(&mdUsart.USART3_s.USART_RecvSem,OS_OPT_POST_ALL,&err);/*�ͷ���*/
                                   strcpy(mdUsart.USART3_s.UsartName,"USART3");
					mdUsart.USART3_s.USARTx = USART3;
                                   ;
                                   USART_SendTaskCreate(&mdUsart.USART3_s);
					break;
				}

				
	 case (uint32_t)UART4:
				{
					mdUsart.USART4_s.GPIO_Pin_rxd = USART4_RXD;
					mdUsart.USART4_s.GPIO_Pin_txd = USART4_TXD;
					mdUsart.USART4_s.USART_BaudRate = USART_BaudRate;
					mdUsart.USART4_s.GPIO_USART = USART4_PORT;
					mdUsart.USART4_s.USARTUserDel = CallBackFun;
					/*�û��ص�����ע��*/
					
					USARTGpioConfig(&mdUsart.USART4_s);/*io������*/
					USARTNvicConfig(UART4_IRQn,0,0);
					USARTConfig(UART4,&mdUsart.USART4_s);
					OSSemCreate(&mdUsart.USART4_s.USART_SendSem, "USART4_SendSem", 1, &err);
					OSSemCreate(&mdUsart.USART4_s.USART_RecvSem, "USART4_RecvSem", 1, &err);
					break;
				}

				
	case (uint32_t)UART5:
				{
					mdUsart.USART5_s.GPIO_Pin_rxd = USART5_RXD_A;
					mdUsart.USART5_s.GPIO_Pin_txd = USART5_TXD_A;
					mdUsart.USART5_s.USART_BaudRate = USART_BaudRate;
					mdUsart.USART5_s.GPIO_USART = USART5_PORT_A;
					mdUsart.USART5_s.USARTUserDel = CallBackFun;
					/*�û��ص�����ע��*/
					
					USARTGpioConfig(&mdUsart.USART5_s);/*io������*/
					USARTNvicConfig(UART5_IRQn,0,0);
					USARTConfig(UART5,&mdUsart.USART5_s);
					OSSemCreate(&mdUsart.USART5_s.USART_SendSem, "USART5_SendSem", 1, &err);
					OSSemCreate(&mdUsart.USART5_s.USART_RecvSem, "USART5_RecvSem", 1, &err);
					break;
				}


#endif
	}
}
/*****************************************************************************
 * �� �� ��  : UsartGetChar
 * �� �� ��  : Gavin
 * ��������  : 2017��5��8��
 * ��������  : �Ӵ��ڻ����л�ȡһ���ַ���һ�㵥���ַ��������ʹ������ӿ�
 				�����Ҫ�ڸ���ͨ�ţ�����ʹ�ûص�����
 * �������  : ��
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
int8_t UsartGetChar(USARTInfo_s *Usart,uint16_t TimeOut)
{
	uint16_t RecvData = 0;
	OS_ERR err; 
	if(IsQueueEmpty(&Usart->USART_RecvQueue))
	{
		/*��ʱ��֪����ôЭ���������������Ҫ��������*/
		//OSSemPend(&Usart->USART_RecvWiteSem,TimeOut,&err);/*��ʱ�ȴ�*/
	}
	if(DeQueue(&Usart->USART_RecvQueue,&RecvData))
	{
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);/*��������*/
		return RecvData;/*���ظ��ַ�*/
	}
}

/*****************************************************************************
 * �� �� ��  : UsartPutCharFush
 * �� �� ��  : Gavin
 * ��������  : 2017��5��8��
 * ��������  : �򴮿������һ���ַ���ʵʱ�ȴ��������,ˢ��
 * �������  : ��
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
int8_t UsartPutCharFush(uint8_t ch)
{
	USART_SendData(USART1,ch);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)!=SET);
	return(ch);
}

/*****************************************************************************
 * �� �� ��  : UsartPutChar
 * �� �� ��  : Gavin
 * ��������  : 2017��5��8��
 * ��������  : �򴮿ڻ����������һ���ַ�
 * �������  : ��
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
int8_t UsartPutChar(USARTInfo_s *Usart,int16_t SendData,uint16_t TimeOut)
{
		OS_ERR err; 
	
         /*�������Ϊ����ȴ�*/
   OSSemPend(&Usart->USART_SendSem,OS_OPT_PEND_BLOCKING,TimeOut,NULL,&err);/*�ȴ��ź���*/
         
	 if(IsQueueFull(&Usart->USART_SendQueue))
	 {
			OSSemPost(&Usart->USART_SendSem,OS_OPT_POST_ALL,&err);/*�ͷ��ź���*/
			return -1;
	 }
	 if(EnQueue(&Usart->USART_SendQueue,&SendData))
	 {
	     OSSemPost(&Usart->USART_SendSem,OS_OPT_POST_ALL,&err);/*�ͷ��ź���*/
			/*����гɹ�*/
			return 1;
	 }
	 /*�ͷ��ź���*/
   OSSemPost(&Usart->USART_SendSem,OS_OPT_POST_ALL,&err);/*�ͷ��ź���*/
	 
	 return ERROR;
}
/*����һ���ַ���*/
int16_t UsartPutString(USARTInfo_s *Usart,uint8_t *SendString,uint16_t Len,uint16_t TimeOut)
{
	uint16_t uiLoop = 0;
	
	if(Len == 0)
	{
		while(*SendString != '\0')
		{
			UsartPutChar(Usart,*SendString,0);
			SendString++;
		}
	}
	else
	{
		while(uiLoop < Len)
		{
			UsartPutChar(Usart,*SendString,0);
			SendString++;
			uiLoop++;
		}
	}
}

/*����һ���ַ���*/
int16_t UsartPutString2(USARTInfo_s *Usart,uint8_t *SendString,uint16_t Len,uint16_t TimeOut)
{
	uint16_t uiLoop = 0;
	
	if(Len == 0)
	{
		while(*SendString != '\0')
		{
			//UsartPutChar(Usart,*SendString,0);
			USART_SendData(USART3,*SendString);
			while(USART_GetFlagStatus(USART3,USART_FLAG_TXE)!=SET);
			SendString++;
		}
	}
	else
	{
		while(uiLoop < Len)
		{
			//UsartPutChar(Usart,*SendString,0);
			USART_SendData(USART3,*SendString);
			while(USART_GetFlagStatus(USART3,USART_FLAG_TXE)!=SET);
			SendString++;
			uiLoop++;
		}
	}
}

/*****************************************************************************
 * �� �� ��  : UsartWaitReciveEnd
 * �� �� ��  : л��
 * ��������  : 2017��7��13��
 * ��������  : �����Ƿ�������
 * �������  : ��
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
int8_t UsartWaitReciveEnd(USARTInfo_s * Usart)
{
	 uint16_t dat = 0;
	/*˵��������û�н����κ�ֵ*/
	if(IsQueueEmpty(&Usart->USART_RecvQueue))
	{
		return 0;
	}
	if(Usart->USART_RecvQueue.QueueTail ==
		Usart->USART_RecvQueue.QueueOldTail)
	{
		/*��һ�ν��ܵ�ֵ����ν��ܵ�ֵһ���������������*/
		return 1;/*�������*/
	}
	
	Usart->USART_RecvQueue.QueueOldTail = 
	Usart->USART_RecvQueue.QueueTail;
	/*��Ϊ��ͬ־*/
	return 0;
}


/*****************************************************************************
 * �� �� ��  : UsartWaitRecviceData
 * �� �� ��  : л��
 * ��������  : 2017��7��13��
 * ��������  : �ȴ��������ݣ����ܳɹ����ؽ��ܵ��ĸ���
 * �������  : uint8_t WaitPort     �ȴ��˿�
               uint16_t RecvDatLen  ���ܳ���
               uint8_t *RecvBuf     ���ܻ�����
               uint16_t TimeOut     ���ܳ�ʱʱ��
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
int16_t UsartWaitRecviceData(USARTInfo_s * Usart,uint8_t WaitPort,uint16_t RecvDatLen,uint8_t *RecvBuf,uint16_t TimeOut )
{
    OS_ERR err;
    char *IpdStr = NULL;
    char *IpdStr1 = NULL;
    char *StrPort = NULL;
    uint16_t  RecvDatNum = 0;/*�����ַ�����*/
    uint8_t Temp[4] = {0};/*������1024�ֽ�*/
    uint8_t RecvDatPort = 0;
    while(TimeOut--)
    {
    	/*���ܽ���*/
	if(UsartWaitReciveEnd(Usart))
	{
		IpdStr = strstr((char *)Usart->USART_RecvQueue.QueueBuf,"+IPD,");
		IpdStr+=5;/*��ת���˿ڿ�ʼλ��*/
		if(NULL != IpdStr)
		{
			RecvDatPort = IpdStr[0] - 48;
			
			if(WaitPort == RecvDatPort)
			{		
				IpdStr+=2;
				/*ȡ�����ܵ��ĳ�����Ϣ*/
				IpdStr1 = strchr(IpdStr, ':');/*�ҵ�:*/	
				if(IpdStr1 != NULL)
				{
					memcpy(Temp,IpdStr,(IpdStr1 - IpdStr));
					RecvDatNum = atoi(Temp);
					printf("�˿�:%d���ܵ����ݣ����ݳ���%d\n",RecvDatPort,RecvDatNum);
					IpdStr+=4;
					memset(RecvBuf,0,RecvDatLen);
					if(RecvDatNum <= RecvDatLen)
					{
						memcpy(RecvBuf,IpdStr,RecvDatNum);/*��������*/
					}
					else
					{
						memcpy(RecvBuf,IpdStr,RecvDatLen);/*��������*/
					}
                                    /*�����ÿ�*/
					QueueSetEmpty(&Usart->USART_RecvQueue);
					return RecvDatNum;
				}
				else
				{
					return (-1);
				}
			}
			else
			{
				return (-1);
			}
		}
	}
	OSTimeDly(  (OS_TICK    )1, 
                    (OS_OPT     )OS_OPT_TIME_DLY, 
                    (OS_ERR     *)&err);
    }
    return (-1);
}

/*****************************************************************************
 * �� �� ��  : UsartSendCmd
 * �� �� ��  : л��
 * ��������  : 2017��7��17��
 * �������  : ��
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
int8_t UsartSendCmd(USARTInfo_s *Usart,char *Cmd,char *Restrict,uint16_t len,
                                        int8_t Mode,AtCmdCallBack CallBack,void *cArg1,void *cArg2,uint16_t TimeOut)
{
    OS_ERR err;
    char *Poite = NULL;
	//nt8_t CheckStrLen = strlen()
	if(!len)
	{
		QueueSetEmpty(&Usart->USART_RecvQueue);/*���*/	
	}
	UsartPutString2(Usart,(uint8_t *)Cmd,len,0);
	while(TimeOut--)
	{
		if(UsartWaitReciveEnd(Usart))
		{
		       
			if((Poite = strstr((char *)Usart->USART_RecvQueue.QueueBuf,Restrict)) != NULL)
			{   
			    if(CallBack != NULL)
			    {
                      CallBack(Poite,cArg1,cArg2);
                 }
				if(Mode)
				{
					QueueSetEmpty(&Usart->USART_RecvQueue);/*���*/
				}
				return 1;/*���سɹ�*/
			}
			if((Poite = strstr((char *)Usart->USART_RecvQueue.QueueBuf,"ALREAY CONNECT"))!= NULL)
			{
				/*�Ѿ�����*/
				if(CallBack != NULL)
			    {
                      CallBack(Poite,cArg1,cArg2);
                 }
				if(Mode)
				{
					QueueSetEmpty(&Usart->USART_RecvQueue);/*���*/
				}
				return 1;/*���سɹ�*/
			}
		}
        
		OSTimeDly(  (OS_TICK    )1, 
                    (OS_OPT     )OS_OPT_TIME_DLY, 
                    (OS_ERR     *)&err);
	}
	
	return 0;
}



void USART1_IRQHandler(void)
{
	uint16_t BackData = 0;
	
	OS_ERR err; 
  OSIntEnter();    //֪ͨUCOS�����ж�
  
	if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET)
	 { 
	 	BackData = USART_ReceiveData(USART1);
		#ifndef 	UCOS_SUPPORT
	 		USART_ClearITPendingBit(USART1,USART_IT_RXNE);/*�������Ҫ�ˣ�ϵͳ��Ϊ�������*/
		#endif
		/*�����֧�������û����ٴ����յ��Ĵ�����Ϣ*/
		if(mdUsart.USART1_s.USARTUserDel != NULL)
		{
			/*ִ���û��жϴ�����:�ṩ�����Ľӿڣ����û�ʵ���Լ���Э��*/
			mdUsart.USART1_s.USARTUserDel(BackData,0);
		}
		/*����ǳ��洦��*/
		{
			/*���������Ͷ��У����õ�����������Ϊ�ڲ��Ѿ�����ѭ��*/
			EnQueue(&mdUsart.USART1_s.USART_RecvQueue,&BackData);/*���������*/
			/*�ͷ��ź���*/
			//mdUsart.USART1_s.USART_RecvQueue.QueueTail++;
			OSSemPost(&mdUsart.USART1_s.USART_RecvWiteSem,2,&err);
		}
	}
	OSIntExit();
}
void USART2_IRQHandler(void)
{
	OS_ERR err; 
	uint16_t BackData;
	
	OSIntEnter();    //֪ͨUCOS�����ж�
	
	if(USART_GetITStatus(USART2,USART_IT_RXNE) != RESET)
	 { 
		BackData = USART_ReceiveData(USART2);
		#ifndef 	UCOS_SUPPORT
	 		USART_ClearITPendingBit(USART2,USART_IT_RXNE);/*�������Ҫ�ˣ�ϵͳ��Ϊ�������*/
		#endif
		
		if(mdUsart.USART2_s.USARTUserDel != NULL)
		{
			mdUsart.USART2_s.USARTUserDel(BackData,0);/*ִ���û��жϴ�����*/
		}
		
		{
			/*���������Ͷ��У����õ�����������Ϊ�ڲ��Ѿ�����ѭ��*/
			EnQueue(&mdUsart.USART2_s.USART_RecvQueue,&BackData);/*���������*/
			/*�ͷ��ź���*/
			OSSemPost(&mdUsart.USART2_s.USART_RecvWiteSem,2,&err);
		}
	 } 
	OSIntExit();
}

#ifdef STM32F10X_HD

void USART3_IRQHandler(void)
{
	OS_ERR err;
	uint16_t BackData;
	
	OSIntEnter();    //֪ͨUCOS�����ж�
	
	if(USART_GetITStatus(USART3,USART_IT_RXNE) != RESET)
	 { 
	 	USART_ClearITPendingBit(USART3,USART_IT_RXNE);
		BackData = USART_ReceiveData(USART3);

              #ifndef 	UCOS_SUPPORT
	 		USART_ClearITPendingBit(USART3,USART_IT_RXNE);/*�������Ҫ�ˣ�ϵͳ��Ϊ�������*/
		#endif
		if(mdUsart.USART3_s.USARTUserDel != NULL)
		{
			mdUsart.USART3_s.USARTUserDel(BackData,0);/*ִ���û��жϴ�����*/
		}

              /*���������Ͷ��У����õ�����������Ϊ�ڲ��Ѿ�����ѭ��*/
	       EnQueue(&mdUsart.USART3_s.USART_RecvQueue,&BackData);/*���������*/
			/*�ͷ��ź���*/
	       OSSemPost(&mdUsart.USART3_s.USART_RecvWiteSem,2,&err);
	 } 
	OSIntExit();
}

void UART4_IRQHandler(void)
{
	uint16_t BackData;
	if(USART_GetITStatus(UART4,USART_IT_RXNE) != RESET)
	 { 
	 	USART_ClearITPendingBit(UART4,USART_IT_RXNE);
		BackData = USART_ReceiveData(UART4);
		if(mdUsart.USART4_s.USARTUserDel != NULL)
		{
			mdUsart.USART4_s.USARTUserDel(BackData,0);/*ִ���û��жϴ�����*/
		}
	 } 
}

void UART5_IRQHandler(void)
{
	uint16_t BackData;
	if(USART_GetITStatus(UART5,USART_IT_RXNE) != RESET)
	 { 
	 	USART_ClearITPendingBit(UART5,USART_IT_RXNE);
		BackData = USART_ReceiveData(UART5);
		if(mdUsart.USART5_s.USARTUserDel != NULL)
		{
			mdUsart.USART5_s.USARTUserDel(BackData,0);/*ִ���û��жϴ�����*/
		}
	 } 
}
#endif



