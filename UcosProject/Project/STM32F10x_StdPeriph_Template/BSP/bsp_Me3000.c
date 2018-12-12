/***********************************************************************************
 * �� �� ��   : bsp_Me3000.c
 * �� �� ��   : л��
 * ��������   : 2017��7��17��
 * �ļ�����   : ����ME3000ģ��ĳ���
 * ��Ȩ˵��   : Copyright (c) 2008-2017   xx xx xx xx �������޹�˾
 * ��    ��   : 
 * �޸���־   : 
***********************************************************************************/

#include "bsp_Me3000.h"

Me3000 mdMe3000;
/*#####################������������#######################*/
AtCmdParInfo AtMe3000Cmd[] =
{
        /*0*/{"AT\r\n","OK",1,1000,NULL},/*ģ������״̬ȷ��*/
        /*1*/{"AT+ZPWROFF\r\n","OK",1,1000,NULL},/*�ػ�*/
        /*2*/{"AT+CGMR=?\r\n","+CGMR",1,1000,Me3000ReadModeVersionCallBack},/*��ȡ�汾����*/
        /*3*/{"AT*TSIMINS?\r\n","*TSIMINS:0,1",1,1000,NULL},/*��ѯ��״̬���޿�����0*/
        /*4*/{"AT+CSQ\r\n","+CSQ:",1,2000,Me3000ReadSingalValueCallBack},
        /*5*/{"ATE0\r\n","OK",1,2000,NULL},/*�����������*/
        /*6*/{"AT+CNUM\r\n" ,"+CNUM:",1,1000,Me3000GetPhoneNumberCallBack},
        /*7*/{"AT+ZDNSGETIP=","ERROR",1,1000,Me3000GetHostByNameCallBack},
};

/*#####################������������#######################*/


/*gsmģ��io�ڸߵ͵�ƽ����*/
uint16_t GsmGpioControl(GPIO_TypeDef* GPIO,uint16_t GPIO_Pin,uint8_t State)
{
	if(State)
	{
		GPIO_SetBits(GPIO,GPIO_Pin);
	}
	else
	{
		GPIO_ResetBits(GPIO,GPIO_Pin);
	}
}


int8_t Me3000OtherGpioInit()
{
    /*GSMģ������GPIO��ʼ��*/
    
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GSM_ON;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GSM_ON_PORT,&GPIO_InitStructure);
	
	GsmGpioControl(GSM_ON_PORT,GSM_ON,1);              
	 /*������һ�ϵ�ʹ��ڿ���״̬*/
     
	GPIO_InitStructure.GPIO_Pin = GSM_REST;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;		 
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GSM_REST_PORT,&GPIO_InitStructure);
	
	GsmGpioControl(GSM_REST_PORT,GSM_REST,0);       /*��λ����*/
}

/*****************************************************************************
 * �� �� ��  : Me3000ParmInit
 * �� �� ��  : л��
 * ��������  : 2017��7��17��
 * ��������  : me3000Ӳ��������ʼ��
 * �������  : ��
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
int8_t Me3000ParmInit()
{
    mdMe3000.Usart = ME3000_BIND_USART_PORT;/*GSMģ��󶨶˿�*/

    USARTInit(USART2,115200,NULL);/*��ʼ�������������Ҫע��ص�����*/
	
    mdMe3000.Usart->USART_RecvQueue.QueueType = QueueArryType;
    printf("\nMe3000ģ��˿ڳ�ʼ���ɹ�\n");
    Me3000OtherGpioInit();
    printf("\nMe3000ģ�鿪���͸�λ�˿ڳ�ʼ���ɹ�\n");
		Me3000TaskInit();
}

int8_t Me3000ControlTask()
{
    OS_ERR err;
    uint16_t uiLoop = 0;
    printf("\nME3000�������񴴽��ɹ�\n");
    /*��Ҫ�������豸���ߺ��շ�����*/
    /*1���ȴ�3SȻ���ж��ж��豸�Ƿ�����*/
    
    OSTimeDly(  (OS_TICK    )4000, (OS_OPT     )OS_OPT_TIME_DLY,  (OS_ERR     *)&err);  
        
    mdMe3000.Me3000ReadyOk = 1;/*ģ�鹤������*/

    Me3000SetShowMode();
    while(1)
    {
        OSTimeDly(  (OS_TICK    )5000, (OS_OPT     )OS_OPT_TIME_DLY,  (OS_ERR     *)&err);
				LedClose(LED2);
        if(Me3000CheckOnline())
        {
						LedOpen(LED3);
            mdMe3000.Me3000Online = 1;
        }
        else
        {
            mdMe3000.Me3000Online = 0;      /*ģ�����*/
            mdMe3000.Me3000ReadyOk = 0;     /*ģ�鹤������*/
        }
        OSTimeDly(  (OS_TICK    )5000, (OS_OPT     )OS_OPT_TIME_DLY,  (OS_ERR     *)&err);
        LedClose(LED3);
        /*�ź�ǿ�ȡ�ģ��״̬����ͳ��*/
        OSTimeDly(  (OS_TICK    )5000, (OS_OPT     )OS_OPT_TIME_DLY,  (OS_ERR     *)&err);
        /*�ź�����ȡ*/
				
        Me3000ReadSingalValue();
        //Me3000GetHostByName("www.baidu.com",NULL);
    }
}

/*****************************************************************************
 * �� �� ��  : Me3000TaskInit
 * �� �� ��  : л��
 * ��������  : 2017��7��17��
 * ��������  : ME3000ά���̳߳�ʼ��
 * �������  : ��
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
int8_t Me3000TaskInit()
{
    OS_ERR err;

	OSTaskCreate(   (OS_TCB     *)&mdMe3000.taskMe3000TCB,
                    (CPU_CHAR   *)"Me3000Task",
                    (OS_TASK_PTR)Me3000ControlTask,
                    (void       *)0,
                    (OS_PRIO    )TASK3_PRIO,
                    (CPU_STK    *)&mdMe3000.taskMe3000Stk[0],
                    (CPU_STK_SIZE)TASK3_STK_SIZE / 10,
                    (CPU_STK_SIZE)TASK3_STK_SIZE,
                    (OS_MSG_QTY )0,
                    (OS_TICK    )0,
                    (void       *)0,
                    (OS_OPT     )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), 
                    (OS_ERR     *)&err);
}

/*****************************************************************************
 * �� �� ��  : ME3000Init
 * �� �� ��  : л��
 * ��������  : 2017��7��17��
 * ��������  : ME3000ģ���ʼ������Ҫ�Ƕ˿�����
 * �������  : ��
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
int8_t Me3000Init()
{
      OS_ERR err;
      
       Me3000ParmInit();   /*������ʼ��*/
       
       Me3000TaskInit();  /*�������񴴽�*/
}

/*****************************************************************************
 * �� �� ��  : Me3000CheckOnline
 * �� �� ��  : л��
 * ��������  : 2017��7��17��
 * ��������  : ���ģ���Ƿ�����
 * �������  : ��
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
int8_t Me3000CheckOnline()
{
    AtCmdParInfo *cmdInfo = &AtMe3000Cmd[0];
    
    return UsartSendCmd(mdMe3000.Usart,cmdInfo->CmdInfo, cmdInfo->CmdRsp, 
                            0, cmdInfo->CmdMode,cmdInfo->CallBack,NULL,NULL,cmdInfo->CmdTimeOut);
}

/*****************************************************************************
 * �� �� ��  : Me3000Open
 * �� �� ��  : л��
 * ��������  : 2017��7��17��
 * ��������  : Me300ģ�鿪������
 * �������  : ��
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
int8_t Me3000Open()
{
		OS_ERR err;
    /*1�������ж��Ƿ��Ѿ�����*/
    if(Me3000CheckOnline())
   {
        mdMe3000.Me3000Online = 1;/*ģ���Ѿ���������*/
        printf("\nģ���Ѿ��ǿ���״̬\n");
        return 1;
    }
    else
    {
        mdMe3000.Me3000Online = 0;/*���¿���*/
    }
    
    do
    {
        GsmGpioControl(GSM_ON_PORT,GSM_ON,0);       /*����*/
        OSTimeDly(  (OS_TICK    )500, (OS_OPT     )OS_OPT_TIME_DLY,  (OS_ERR     *)&err);
        GsmGpioControl(GSM_ON_PORT,GSM_ON,1);       /*���Ϳ���*/ 
        OSTimeDly(  (OS_TICK    )5000, (OS_OPT     )OS_OPT_TIME_DLY,  (OS_ERR     *)&err);
        //GsmGpioControl(GSM_ON_PORT,GSM_ON,0);       /*����*/
        
    }
    while(!Me3000CheckOnline());
    
     return 1;/*���ڿ����ɹ�*/
}

/*****************************************************************************
 * �� �� ��  : Me3000SoftClose
 * �� �� ��  : л��
 * ��������  : 2017��7��17��
 * ��������  : ͨ��atָ�������йػ�����
 * �������  : ��
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
int8_t Me3000SoftClose()
{
		OS_ERR err;
    int8_t State = 0;
    AtCmdParInfo *cmdInfo = &AtMe3000Cmd[1];
    
   State =  UsartSendCmd(mdMe3000.Usart,cmdInfo->CmdInfo, cmdInfo->CmdRsp, 
                            0, cmdInfo->CmdMode,NULL,NULL,NULL,cmdInfo->CmdTimeOut);

    OSTimeDly(  (OS_TICK    )1000, (OS_OPT     )OS_OPT_TIME_DLY,  (OS_ERR     *)&err);

    
   State = Me3000CheckOnline();/*����Ƿ�ػ��ɹ�*/
   if(State)
   {
        mdMe3000.Me3000Online = 0;/*ģ��ػ�*/
        return 1;
   }
   else
   {
        return 0;
   }
    
}

void *Me3000ReadModeVersionCallBack(void *arg1,void *arg2,void *arg3)
{
    /*�汾����*/
    char *Pointe = (char *)arg1;
    char *VersionPointe = NULL;
    char Version[10];
    
    if((VersionPointe = strstr(Pointe,"MG2639")) != NULL)
    {
        memcpy(Version,VersionPointe,10);
        printf("\nGSM�汾:%s\n",Version);
        return VersionPointe;
    }
    else if((VersionPointe = strstr(Pointe,"ME3000")) != NULL)
    {
        memcpy(Version,VersionPointe,10);
        printf("\nGSM�汾:%s\n",Version);
        return VersionPointe;
    }
    return NULL;
}

/*****************************************************************************
 * �� �� ��  : Me3000ReadModeVersion
 * �� �� ��  : л��
 * ��������  : 2017��7��17��
 * ��������  : ��ȡģ��汾����
 * �������  : ��
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
int8_t Me3000ReadModeVersion()
{
    OS_ERR err;
    int8_t State = 0;
    
    AtCmdParInfo *cmdInfo = &AtMe3000Cmd[2];

    
    State =  UsartSendCmd(mdMe3000.Usart,cmdInfo->CmdInfo, cmdInfo->CmdRsp, 
                            0, cmdInfo->CmdMode,cmdInfo->CallBack,NULL,NULL,cmdInfo->CmdTimeOut);
    return State;
}
/*****************************************************************************
 * �� �� ��  : Me3000FindSimCard
 * �� �� ��  : л��
 * ��������  : 2017��7��17��
 * ��������  : ��ѯSIM��״̬
 * �������  : ��
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
int Me3000FindSimCard()
{
    OS_ERR err;
    int8_t State = 0;
    
    AtCmdParInfo *cmdInfo = &AtMe3000Cmd[3];

    
    State =  UsartSendCmd(mdMe3000.Usart,cmdInfo->CmdInfo, cmdInfo->CmdRsp, 
                            0, cmdInfo->CmdMode,cmdInfo->CallBack,NULL,NULL,cmdInfo->CmdTimeOut);
    return State;
}

void *Me3000ReadSingalValueCallBack(void *arg1,void *arg2,void *arg3)
{
    uint16_t uiKVaue;
    
    char *Pointe = (char *)arg1;
    char *SingnelPointe = NULL;
    char TempBuf[4] = {0};
    char bits = 0;
    if((SingnelPointe = strstr(Pointe,"+CSQ:")) != NULL)
    {
        memcpy(TempBuf,&SingnelPointe[6],2);
        uiKVaue = atoi(TempBuf);
        if(uiKVaue >= 0 && uiKVaue <= 99)
        {
            mdMe3000.SingnelValue = uiKVaue;
        }
        memset(TempBuf,0,sizeof(TempBuf));
				bits = (mdMe3000.SingnelValue >= 10)?2:1;
        memcpy(TempBuf,&SingnelPointe[6 + bits + 2],2);
        
        uiKVaue = atoi(TempBuf);
        if(uiKVaue >= 0 && uiKVaue <= 99)
        {
            mdMe3000.SingnelBer = uiKVaue;
        }
        printf("\n####�ź���:%d,�źŴ�����%d####\n",mdMe3000.SingnelValue,
                  mdMe3000.SingnelBer);
    }
    
}
/*****************************************************************************
 * �� �� ��  : Me3000ReadSingalValue
 * �� �� ��  : л��
 * ��������  : 2017��7��17��
 * ��������  : ��ȡ�ź�ǿ��
 * �������  : ��
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
int8_t Me3000ReadSingalValue()
{
    OS_ERR err;
    int8_t State = 0;
    
    AtCmdParInfo *cmdInfo = &AtMe3000Cmd[4];

    
    State =  UsartSendCmd(mdMe3000.Usart,cmdInfo->CmdInfo, cmdInfo->CmdRsp, 
                            0, cmdInfo->CmdMode,cmdInfo->CallBack,NULL,NULL,cmdInfo->CmdTimeOut);
    return State;
}

int8_t Me3000SetShowMode()
{
    OS_ERR err;
    int8_t State = 0;
    
    AtCmdParInfo *cmdInfo = &AtMe3000Cmd[5];

    
    State =  UsartSendCmd(mdMe3000.Usart,cmdInfo->CmdInfo, cmdInfo->CmdRsp, 
                            0, cmdInfo->CmdMode,cmdInfo->CallBack,NULL,NULL,cmdInfo->CmdTimeOut);
    return State;

}

/*****************************************************************************
 * �� �� ��  : Me3000GetPhoneNumberCallBack
 * �� �� ��  : л��
 * ��������  : 2017��7��19��
 * ��������  : ��ȡ��������ص�������
 * �������  : void 
 * �������  : ��
 * �� �� ֵ  : int8_t
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
void * Me3000GetPhoneNumberCallBack(void *arg1,void *arg2,void *arg3)
{
    
    return NULL;
}
/*****************************************************************************
 * �� �� ��  : Me3000GetPhoneNumber
 * �� �� ��  : л��
 * ��������  : 2017��7��19��
 * ��������  : void
 * �������  : ��
 * �������  : ��
 * �� �� ֵ  : ��
 * ���ù�ϵ  : 
 * ��    ��  :�ú�����Ҫ�ύ���õ绰����

*****************************************************************************/
int8_t Me3000GetPhoneNumber(void)
{
    OS_ERR err;
    int8_t State = 0;
    
    AtCmdParInfo *cmdInfo = &AtMe3000Cmd[6];

    
    State =  UsartSendCmd(mdMe3000.Usart,cmdInfo->CmdInfo, cmdInfo->CmdRsp, 
                            0, cmdInfo->CmdMode,cmdInfo->CallBack,NULL,NULL,cmdInfo->CmdTimeOut);
    return State;
}

void *Me3000GetHostByNameCallBack(void *arg1,void *arg2,void *arg3)
{
    printf("122:%s\n",(char *)arg1);
}
/*****************************************************************************
 * �� �� ��  : Me3000GetHostByName
 * �� �� ��  : л��
 * ��������  : 2017��7��19��
 * ��������  : ����������ȡIp��ַ
 * �������  : const char *Name  ��������
 * �������  : ��
 * �� �� ֵ  :      char *
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
int8_t Me3000GetHostByName( const char *Name,char *HostBuf)
{
    OS_ERR err;
    int8_t State = 0;
    char buf[50] ={0};
    
    AtCmdParInfo *cmdInfo = &AtMe3000Cmd[7];

    sprintf(buf,"%s%\"s\"",cmdInfo->CmdInfo,Name);
    
    State =  UsartSendCmd(mdMe3000.Usart,buf, cmdInfo->CmdRsp, 
                            0, cmdInfo->CmdMode,cmdInfo->CallBack,NULL,NULL,cmdInfo->CmdTimeOut);
    return State;
}
