/*-------------------------------------------------------------------------

							�������

    
-------------------------------------------------------------------------*/

#include "bsp.h"
#include "App.h"
#include "stdio.h"
#include "stm32f10x_gpio.h"

extern void MainTask(void);

static  OS_TCB      taskStartTCB;
static  CPU_STK     taskStartStk[STARTUP_TASK_STK_SIZE]; 		//��������ĳ���ռ�

static OS_TCB		taskLogicTCB;
static  CPU_STK     taskLogicStk[TASK3_STK_SIZE];/*�߼�����ջ�ռ�*/

static OS_TCB       taskOnenetTCB;
static  CPU_STK     taskOnenetStk[TASK5_STK_SIZE];/*�߼�����ջ�ռ�*/

volatile AppInfo mdAppInfo;


void SysVersionInfoShow()
{
	printf("########################��ӭʹ�����������ٽ��뿪��ƽ̨########################\n");
	printf("#SoftVersion:%s\n",SYS_SOFT_VERSION);
	printf("#HadVersion:%s\n",SYS_HAD_VERSION);
	printf("#����:л��\n");
	printf("#������ַ:1048070562\n");
	printf("#����ϵͳ����:UCOSIII\n");
	printf("########################��ӭʹ�����������ٽ��뿪��ƽ̨########################\n");

}

void SpearkInit()
{
	GPIO_InitTypeDef GPIO_Init_Structure;
	
	
	GPIO_Init_Structure.GPIO_Pin = GPIO_Pin_13; 
	GPIO_Init_Structure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init_Structure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOB,&GPIO_Init_Structure);
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_13);
}
void AppInit()
{
	SpearkInit();
}
/*1���������ģ���Ƿ���Ҫ�����豸,
������豸��Ҫ����������ģʽ��Ȼ���ϴ����ʱ��
Ӳ�����ӹ�ϵ��
usart2 --- ����ģ��*/
static void TaskStart(void)
{
	OS_ERR 		err;
	SysVersionInfoShow();
	AppInit();
	ESP8266_Init();/*wifiģ���ʼ��*/
	
}

/*�����񣬸��𴴽�������*/
void KernelMain(void)
{
    OS_ERR err;
	
    CPU_Init();
    OSInit(         (OS_ERR	    *)&err);

    OSTaskCreate(   (OS_TCB     *)&taskStartTCB,
                    (CPU_CHAR   *)"Task Start",
                    (OS_TASK_PTR)TaskStart,/*������*/
                    (void       *)0,
                    (OS_PRIO    ) STARTUP_TASK_PRIO,
                    (CPU_STK    *)&taskStartStk[0],
                    (CPU_STK_SIZE)STARTUP_TASK_STK_SIZE / 10,
                    (CPU_STK_SIZE)STARTUP_TASK_STK_SIZE,
                    (OS_MSG_QTY )0,
                    (OS_TICK    )0,
                    (void       *)0,
                    (OS_OPT     )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), 
                    (OS_ERR     *)&err);    
    
    OSStart(        (OS_ERR	    *)&err);
}





