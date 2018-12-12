/***********************************************************************************
 * �� �� ��   : LockControl.c
 * �� �� ��   : л��
 * ��������   : 2017��7��12��
 * �ļ�����   : ������Ӧ�ã���Ҫ���Ħ��ͼ���д
 * ��Ȩ˵��   : Copyright (c) 2008-2017   xx xx xx xx �������޹�˾
 * ��    ��   : 
 * �޸���־   : 
***********************************************************************************/
#include "LockControl.h"

/*****************************************************************************
 * �� �� ��  : LockControlInit
 * �� �� ��  : л��
 * ��������  : 2017��7��12��
 * ��������  : ������ģ���ʼ������
 * �������  : void  ��
 * �������  : ��
 * �� �� ֵ  : int8_t
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/

LockControl mdLockControl;

/*****************************************************************************
 * �� �� ��  : CalcCrc
 * �� �� ��  : л��
 * ��������  : 2017��7��13��
 * ��������  : ����CRC����
 * �������  : char *arg1  ����CRC��
               int sizes   ����
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
uint8_t CalcCrc(char *arg1,int sizes)
{
	uint8_t uiLoop = 0;
	uint8_t ucCrcValue = arg1[0];

	if(arg1 == NULL)
	{
		return 0;
	}

	if(sizes == 0 || sizes == 1)
	{
		return ucCrcValue;
	}
	
	for(uiLoop = 1;uiLoop < sizes;uiLoop++)
	{
		ucCrcValue ^=  arg1[uiLoop];
	}
	
	return ucCrcValue;
}

/*****************************************************************************
 * �� �� ��  : Bit2Swap
 * �� �� ��  : л��
 * ��������  : 2017��7��19��
 * ��������  : �������50��ר�ú���������bit0��bit1���н���
                                ����������״̬��BUG��ɵġ�
 * �������  : uint8_t dat  ���������ֽ���
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
uint8_t Bit2Swap(uint8_t dat)
{
	utype temp;
	utype temp2;
	
	temp.data_char = dat;
	
	temp2.data_bits.bit0 = temp.data_bits.bit1;
	temp2.data_bits.bit1 = temp.data_bits.bit0;
	
	return temp2.data_char;
}

/*****************************************************************************
 * �� �� ��  : BitSwap
 * �� �� ��  : л��
 * ��������  : 2017��7��19��
 * ��������  : ���������50��ר�ú���������һ���ֽ���
                                �ߵ�λȫ����λ
 * �������  : uint8_t dat  ���������������
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
uint8_t BitSwap(uint8_t dat)
{
	utype temp;
	utype temp2;
	
	temp.data_char = dat;
	
	temp2.data_bits.bit0 = temp.data_bits.bit7;
	temp2.data_bits.bit1 = temp.data_bits.bit6;
	temp2.data_bits.bit2 = temp.data_bits.bit5;
	temp2.data_bits.bit3 = temp.data_bits.bit4;
	temp2.data_bits.bit4 = temp.data_bits.bit3;
	temp2.data_bits.bit5 = temp.data_bits.bit2;
	temp2.data_bits.bit6 = temp.data_bits.bit1;
	temp2.data_bits.bit7 = temp.data_bits.bit0;
	
	return temp2.data_char;
}

/*****************************************************************************
 * �� �� ��  : LockControlGetFreeMsgBuf
 * �� �� ��  : л��
 * ��������  : 2017��7��13��
 * ��������  : void
 * �������  : ��
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
 LockMsgType *LockControlGetFreeMsgBuf()
{
    uint8_t ucLoop = 0;

    for(ucLoop = 0;ucLoop < LOCK_MSG_NUM;ucLoop++)
    {
        if(mdLockControl.MsgBuf[ucLoop].stau == LOCK_MSG_FREE)
         {
            mdLockControl.MsgBuf[ucLoop].stau = LOCK_MSG_USE;
            return &mdLockControl.MsgBuf[ucLoop];
         }
    }
    return NULL;
}

/*****************************************************************************
 * �� �� ��  : LockControlFreeMsgBuf
 * �� �� ��  : л��
 * ��������  : 2017��7��13��
 * ��������  : �ͷ���Ϣ������
 * �������  : ��
 * �������  : ��
 * �� �� ֵ  : int8_t
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
int8_t LockControlFreeMsgBuf( LockMsgType * MsgBuf)
{
    if(MsgBuf != NULL)
    {
        MsgBuf->arg1 = 0;
        MsgBuf->arg2 = 0;
        MsgBuf->cmd =  0;
        MsgBuf->stau = LOCK_MSG_FREE;

        return 1;
    }

    return 0;
}

/*****************************************************************************
 * �� �� ��  : LockControlParmInit
 * �� �� ��  : л��
 * ��������  : 2017��7��12��
 * ��������  : ������ģ�������ʼ��
 * �������  : void  ��
 * �������  : ��
 * �� �� ֵ  : int8_t
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
int8_t LockControlParmInit( void )
{
	uint8_t ucLoop = 0;
	OS_ERR err;
    
    	mdLockControl.Usart = LOCK_CONTROL_USART;/*���ڰ�*/

	USARTInit(USART3,9600,NULL);
       
	mdLockControl.Usart->USART_RecvQueue.QueueType = QueueArryType;

	for(ucLoop = 1; ucLoop <= BOARD_NUM; ucLoop++)
	{
		mdLockControl.sAllBoardInfo[ucLoop].ucBoardAddr = ucLoop;/*����ַ��ʼ��*/
	}

        /*��Ϣ���г�ʼ��,���Դ��10����Ϣ*/
        OSQCreate((OS_Q*)&mdLockControl.msg,(CPU_CHAR*)"LockMsg",(OS_MSG_QTY)10,(OS_ERR*)&err);
}

/*****************************************************************************
 * �� �� ��  : LockControlTaskInit
 * �� �� ��  : л��
 * ��������  : 2017��7��12��
 * ��������  : �������������ʼ��
 * �������  : void  ��
 * �������  : ��
 * �� �� ֵ  : int8_t
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
int8_t LockControlTaskInit( void )
{
	OS_ERR err;

	OSTaskCreate(   (OS_TCB     *)&mdLockControl.taskTCB,
                    (CPU_CHAR   *)"LockControlTask",
                    (OS_TASK_PTR)ControlControlTask,
                    (void       *)0,
                    (OS_PRIO    )TASK3_PRIO,
                    (CPU_STK    *)&mdLockControl.taskStk[0],
                    (CPU_STK_SIZE)TASK_MINI_STK_SIZE / 10,
                    (CPU_STK_SIZE)TASK_MINI_STK_SIZE,
                    (OS_MSG_QTY )0,
                    (OS_TICK    )0,
                    (void       *)0,
                    (OS_OPT     )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), 
                    (OS_ERR     *)&err);

	printf("\n������ģ��������񴴽��ɹ�\n");
	
}

int8_t LockControlInit( void )
{
	/*������ʼ��*/
	LockControlParmInit();/*������ʼ��*/

	/*�����ʼ��*/
	LockControlTaskInit();
	
	printf("\n������ģ���ʼ���ɹ�\n");

}
/*****************************************************************************
 * �� �� ��  : LockControlOpenALock
 * �� �� ��  : л��
 * ��������  : 2017��7��13��
 * ��������  : ��ָ������
 * �������  : uint8_t BoradAddr  Ҫ���������İ忨��
               uint8_t LockAddr   ����ַ
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
int8_t LockControlOpenALock(uint8_t BoardAddr,uint8_t LockAddr)
{
    	OpenBoardLock OpenBoardLockCmd;
	/*�������ֻ���������������ã�����ᷢ��ԭ�Ӳ���*/
	if(BoardAddr < 1 || BoardAddr > BOARD_NUM)
	{
		return 0;
	}
    
	memset(&OpenBoardLockCmd,0,sizeof(OpenBoardLock));
        
	if(LockAddr < 1 || LockAddr > BOARD_LOCK_NUMS)
	{
		return 0;
	}
	
	OpenBoardLockCmd.ucHead = OPEN_BOARD_LOCK_HEAD;
	OpenBoardLockCmd.ucBoardAddr = BoardAddr;
	OpenBoardLockCmd.ucLockAddr = LockAddr;
	OpenBoardLockCmd.ucState = 0x11;
	OpenBoardLockCmd.ucCrc = CalcCrc((char *)&OpenBoardLockCmd,(sizeof(OpenBoardLock) -1));
    
	UsartPutString(mdLockControl.Usart,(char *)&OpenBoardLockCmd,sizeof(OpenBoardLock),100);
    
	return 1;
}

/*****************************************************************************
 * �� �� ��  : LockControlOpenALockMsg
 * �� �� ��  : л��
 * ��������  : 2017��7��13��
 * ��������  : �ú������ṩ�ⲿ������е��ã�
 ���ᷢ��һ����Ϣ��������ģ�飬
               �����ұ���ԭ�Ӳ����;�׼�������
 * �������  : ��
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
int8_t LockControlOpenALockMsg(uint8_t BoradAddr,uint8_t LockAddr)
{
    LockMsgType *MsgBuf = NULL ;
    OS_ERR err;
     
    MsgBuf = LockControlGetFreeMsgBuf();
    
    if(NULL != MsgBuf)
    {
        MsgBuf->cmd = OPEN_BOARD_LOCK_HEAD;/*����*/
        MsgBuf->arg1 = BoradAddr; /*�忨��ַ*/
        MsgBuf->arg2 = LockAddr; /*����ַ*/
        
        printf("������Ϣ����:�忨��ַ:0x%x,����ַ:0x%x\n",BoradAddr,LockAddr);
        
        OSQPost((OS_Q*)&mdLockControl.msg,(void*)MsgBuf,(OS_MSG_SIZE)(sizeof(LockMsgType)),
            (OS_OPT)OS_OPT_POST_FIFO,(OS_ERR*)&err); 
    }
}

/*****************************************************************************
 * �� �� ��  : ReadAllLock
 * �� �� ��  : л��
 * ��������  : 2017��7��13��
 * ��������  : ��ѯָ���忨����״̬
 * �������  : uint8_t BoardAddr  ��ѯ�忨��ַ
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
uint16_t LockControlReadAllLock(uint8_t BoardAddr)
{
	ReadBoardLockState ReadLockState;
	
	memset(&ReadLockState,0,sizeof(ReadBoardLockState));
	
	ReadLockState.ucHead = READ_BOARD_LOCK_HEAD;
	ReadLockState.ucBoardAddr = BoardAddr;
	ReadLockState.ucLockAddr = 0x00;
	ReadLockState.ucCmd = 0x33;
	ReadLockState.ucCrc = CalcCrc((char *)&ReadLockState,(sizeof(ReadBoardLockState) - 1));

	UsartPutString(mdLockControl.Usart,(char *)&ReadLockState,sizeof(ReadBoardLockState),100);
	return 1;
}

/*****************************************************************************
 * �� �� ��  : LockCheckReadLockRespParm
 * �� �� ��  : л��
 * ��������  : 2017��7��13��
 * ��������  : ����ѯ�忨״̬����Ӧ֡��
 * �������  : char *Buf  ���뻺������
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
int8_t LockCheckRspParm(uint8_t Head,char *LockRspParm,uint8_t sizes)
{
    uint8_t CrcTemp = 0;/*crcֵ*/
    if(LockRspParm[0] == Head)
    {
        CrcTemp = CalcCrc((char *)&LockRspParm[0],sizes);
        if(CrcTemp == LockRspParm[sizes])
        {
            return 1;
        }
    }
    
    return 0;
}

/*****************************************************************************
 * �� �� ��  : LockControlLockStateDel
 * �� �� ��  : л��
 * ��������  : 2017��7��19��
 * ��������  : ���ư忨״̬ά������
 * �������  : ��
 * �������  : ��
 * �� �� ֵ  : int8_t
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
int8_t LockControlLockStateDel(ReadBoardLockStateRsp *aBoardLockState)
{
#if BOARD_LOCK_NUMS == BOARD_LOCK_NUM_24
    uint8_t uiLoop = 0;
    uint32_t LockState;
    BoardInfo *BoardInfos;
    
    if(aBoardLockState == NULL)
    {
        return 0;/*ָ�����*/
    }
    
    BoardInfos =  (BoardInfo *)&mdLockControl.sAllBoardInfo[aBoardLockState->ucBoardAddr];
    
    LockState = ((uint32_t)aBoardLockState->ucState1 * 256 + aBoardLockState->ucState2) * 
                      256 + aBoardLockState->ucState3;
    
	for(uiLoop = 1;uiLoop <= BOARD_LOCK_NUMS; uiLoop++)
	{
		if((LockState & (0x00000001 << (uiLoop - 1))))
		{
		      BoardInfos->ucBoardLockState[uiLoop] = BOARD_LOCK_CLOSE_STATE;
		}
		else
		{	
			BoardInfos->ucBoardLockState[uiLoop]  =  BOARD_LOCK_OPEN_STATE;
		}
    }
    LedClose(LED2);
    return 1;
#else
    uint8_t uiLoop = 0;
    uint32_t Temp = 0;
    uint32_t Temp2 =0;
    BoardInfo *BoardInfos;
    
    if(aBoardLockState == NULL)
    {
        return 0;/*ָ�����*/
    }
    
    BoardInfos =  (BoardInfo *)&mdLockControl.sAllBoardInfo[aBoardLockState->ucBoardAddr];
    
    Temp2 = ((uint32_t)(Bit2Swap(aBoardLockState->ucState7) << 16)) | 
                  (uint32_t)(BitSwap(aBoardLockState->ucState6) << 8) |  
                  (uint32_t)(BitSwap(aBoardLockState->ucState5) << 0);
	
    Temp = (uint32_t)(BitSwap(aBoardLockState->ucState4) << 24) | 
                (uint32_t)(BitSwap(aBoardLockState->ucState3) << 16) | 
                (uint32_t)(BitSwap(aBoardLockState->ucState2) << 8)  |
	         (uint32_t)(BitSwap(aBoardLockState->ucState1) << 0);
    
	for(uiLoop = 1;uiLoop <= 32; uiLoop++)
	{
		if((Temp & ((uint32_t)0x01 << (uiLoop - 1))))
		{
			BoardInfos->ucBoardLockState[uiLoop] =  BOARD_LOCK_CLOSE_STATE;
		}
		else
		{
			BoardInfos->ucBoardLockState[uiLoop]=  BOARD_LOCK_OPEN_STATE;
		}
	}
	
	for(uiLoop = 33;uiLoop < BOARD_LOCK_NUMS; uiLoop++)
	{
		if((Temp2 & ((uint32_t)0x01 << (uiLoop - 33))))
		{
			BoardInfos->ucBoardLockState[uiLoop] =  BOARD_LOCK_CLOSE_STATE;
		}
		else
		{
			BoardInfos->ucBoardLockState[uiLoop] =  BOARD_LOCK_OPEN_STATE;
		}
	}
    LedClose(LED2);
    return 1;
#endif
}

/*****************************************************************************
 * �� �� ��  : ControlControlTask
 * �� �� ��  : л��
 * ��������  : 2017��7��12��
 * ��������  : ������������������
 * �������  : void *p_arg  ����������Ϣ
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
void ControlControlTask(void *p_arg)
{
    OS_ERR err;
		ReadBoardLockStateRsp *aBoardLockState = NULL;
    LockMsgType *msg;
    uint8_t  ucLoop = 0;
    uint16_t uiJoop = 0;
    char buf[sizeof(ReadBoardLockStateRsp)] = {0};
    /*�ú�����Ҫʵ�ֿ�������״̬����Ȳ���*/
    /*��ѯ�忨����*/
   while(1)
    {
#if 1
        /*���ȵȴ���Ϣ�����еĿ�������*/
        if(NULL != (msg = OSQPend((OS_Q*)&mdLockControl.msg,(OS_TICK)0,(OS_OPT)OS_OPT_PEND_NON_BLOCKING,
                (OS_MSG_SIZE*)(sizeof(LockMsgType)),(CPU_TS*)0,(OS_ERR*)&err)))
         {
            /*���ܵ�һ����Ϣ*/
            if(msg->cmd == OPEN_BOARD_LOCK_HEAD)
            {
                printf("#####��������:�忨��ַ0x%x,����ַ0x%x#####\n",
                         msg->arg1,msg->arg2);
                
                LockControlOpenALock(msg->arg1,msg->arg2);/*����*/
                /*�ͷŸ���Ϣ*/
                LockControlFreeMsgBuf(msg);
#ifdef CONTROL_LOCK_BORAD_RSP_TYPE
                OSTimeDly(  (OS_TICK    )1200, 
                    (OS_OPT     )OS_OPT_TIME_DLY, 
                    (OS_ERR     *)&err);
#else
               OSTimeDly(  (OS_TICK    )1000, 
                    (OS_OPT     )OS_OPT_TIME_DLY, 
                    (OS_ERR     *)&err); 
#endif
            }
         }
#endif
        /*��ѯÿ����ӵ���״̬*/
        for(ucLoop = 1; ucLoop <= BOARD_NUM;  ucLoop++)
        {
            LockControlReadAllLock(ucLoop);/*���Ͳ�ѯ����*/
            LedOpen(LED2);
            /**�ȴ�*/
            OSTimeDly(  (OS_TICK    )(READ_LOCK_STAT_INT / 4), /*50ms,��Ӧʵ��ֵ50 * 4 = 200ms*/
                    (OS_OPT     )OS_OPT_TIME_DLY, 
                    (OS_ERR     *)&err);
            
            if(UsartWaitReciveEnd(mdLockControl.Usart))
             {
                for(uiJoop = 0;uiJoop < sizeof(mdLockControl.Usart->USART_RecvQueue.QueueBuf);uiJoop++)
                {
                    if(mdLockControl.Usart->USART_RecvQueue.QueueBuf[uiJoop] == READ_BOARD_LOCK_HEAD)
                    {
                        memcpy(buf,(char *)&mdLockControl.Usart->USART_RecvQueue.QueueBuf[uiJoop],sizeof(ReadBoardLockStateRsp));
                        /*����Ƿ����Э��*/
                        if(LockCheckRspParm(READ_BOARD_LOCK_HEAD,buf,(sizeof(ReadBoardLockStateRsp) -1)))
                        {	
			        aBoardLockState = (ReadBoardLockStateRsp *)buf; 
				LockControlLockStateDel(aBoardLockState);
				
                            memset((char *)&mdLockControl.Usart->USART_RecvQueue.QueueBuf[uiJoop],0,sizeof(ReadBoardLockStateRsp));
                        }
                     }
                 }
                
                QueueSetEmpty(&mdLockControl.Usart->USART_RecvQueue);
                 
                OSTimeDly(  (OS_TICK    )100, 
                    (OS_OPT     )OS_OPT_TIME_DLY, 
                    (OS_ERR     *)&err);
            }
        }
				
    }
}

/*****************************************************************************
 * �� �� ��  : LockShowAllLockState
 * �� �� ��  : Gavin
 * ��������  : 2017��5��1��
 * ��������  : �鿴���е���״̬
 * �������  : ��
 * �������  : ��
 * �� �� ֵ  : uint16_t
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
int8_t LockShowAllLockState()
{
	uint16_t uiLoop = 1;
    printf("\r\n");
    for(uiLoop = 1;uiLoop < BOARD_LOCK_NUMS;uiLoop++)
    {
	     printf(" %d--0x%.2x ",uiLoop,mdLockControl.sAllBoardInfo[0x01].ucBoardLockState[uiLoop]);
    }
	printf("\r\n");
}
