#ifndef _LOCK_CONTROL_H
#define _LOCK_CONTROL_H

#include "bsp.h"


#define FIND_BOARD_ADDR_HEAD 						0X81		/*�忨��ַ��ѯ*/
#define OPEN_BOARD_LOCK_HEAD						0X8A		/*����*/
#define READ_BOARD_LOCK_HEAD 						0X80		/*��ȡ��״̬*/
#define READ_BOARD_PROTCOL_VERSION_HEAD             0X90		/*��ȡ����Э��*/

#undef CONTROL_LOCK_BORAD_RSP_TYPE                                     /*�ú궨���ʾ�������Ƿ�1s�᷵������*/

#define LOCK_CONTROL_USART                                               (&mdUsart.USART3_s)		/*���������󶨴���*/

#define  BOARD_NUM										2				/*�ܹ����Ű忨*/
#define  LOCK_MSG_NUM                                                          15
#define  LOCK_MSG_FREE                                                          0X00
#define  LOCK_MSG_USE                                                           0X01
#define   BOARD_LOCK_NUM_24 						        25				/*24��*/
#define  BOARD_LOCK_NUM_50							 51				/*50��*/
#define  READ_LOCK_STAT_INT							200						/*��ȡ��״̬���ʱ��*/
#define BOARD_LOCK_NUMS 								BOARD_LOCK_NUM_50   

#define BOARD_LOCK_OPEN_STATE           			               0X11		/*��Ϊ����״̬*/
#define BOARD_LOCK_CLOSE_STATE 					        0X22		/*��Ϊ�ر�״̬*/


typedef struct tagBoardInfo_s
{
	uint8_t ucBoardAddr;	/**��Ӧ�忨�ĵ�ַ*/
	uint8_t ucBoardLockState[BOARD_LOCK_NUMS];/*�ð忨�����е���״̬*/
	uint8_t ucBoardUsed;													/*��?1?2��?����?��??����??��?����1��???��??��*/
}BoardInfo;


typedef struct{  
    uint8_t bit0:1;  
    uint8_t bit1:1;  
    uint8_t bit2:1;  
    uint8_t bit3:1;  
    uint8_t bit4:1;  
    uint8_t bit5:1;  
    uint8_t bit6:1;  
    uint8_t bit7:1;  
}bitss;

typedef union{  
    uint8_t data_char;  
    bitss  data_bits;   
}utype;


/*��ѯ�忨��ַ*/
typedef struct tagFindBoardAddr_s
{
	uint8_t ucHead;			/*Э��ͷ��*/
	uint8_t ucBoardAddr;		/*�忨��ַ*/
	uint8_t ucStatic;			/*�̶�ֵ*/
	uint8_t ucState;			/*״̬*/
	uint8_t ucCrc;				/*crcУ��*/
}FindBoardAddr;

/*��ѯ�忨��ַ��Ӧ֡*/
typedef struct tagFindBoardAddrRsp_s
{
	uint8_t ucHead;			/*ͷ��*/
	uint8_t ucBoardAddr;		/*�忨��ַ*/
	uint8_t ucStatic;			/*�̶�ֵ*/
	uint8_t ucState;			/*״̬*/
	uint8_t ucCrc;				/*crcУ��*/
}FindBoardAddrRsp;

/*����ָ��*/
typedef struct tagOpenBoardLock_s
{
	uint8_t ucHead;			/*ͷ��*/
	uint8_t ucBoardAddr;		/*�忨��ַ*/
	uint8_t ucLockAddr;		 /*����ַ*/
	uint8_t ucState;			/*״̬*/
	uint8_t ucCrc;				/*crcУ��*/
}OpenBoardLock;

/*����ָ���*/
typedef struct tagOpenBoardLockRsp_s
{
	uint8_t ucHead;				/*ͷ��*/
	uint8_t ucBoardAddr;			/*�忨��ַ*/
	uint8_t ucLockAddr;		    /*����ַ*/
	uint8_t ucState;					/*״̬*/
	uint8_t ucCrc;						/*crcУ��*/
}OpenBoardLockRsp;

/*��ȡ��״̬*/
typedef struct tagReadBoardLockState_s
{
	uint8_t ucHead;				/*ͷ��*/
	uint8_t ucBoardAddr;			/*�忨��ַ*/
	uint8_t ucLockAddr;		      /*����ַ*/
	uint8_t ucCmd;				/*״̬*/
	uint8_t ucCrc;					/*crcУ��*/
}ReadBoardLockState;


#if BOARD_LOCK_NUMS == BOARD_LOCK_NUM_24

typedef struct tagReadBoardLockStateRsp_s
{
	uint8_t ucHead;				/*ͷ��*/
	uint8_t ucBoardAddr;		 	/*�忨��ַ*/
	uint8_t ucState1;
	uint8_t ucState2;
	uint8_t ucState3;
	uint8_t ucCmd;				/*����*/
	uint8_t ucCrc;				     /*crc*/
}ReadBoardLockStateRsp;
#else
typedef struct tagReadBoardLockStateRsp_s
{
	uint8_t ucHead;				/*ͷ��*/
	uint8_t ucBoardAddr;			/*�忨��ַ*/
	uint8_t ucState1;
	uint8_t ucState2;
	uint8_t ucState3;
	uint8_t ucState4;
	uint8_t ucState5;
	uint8_t ucState6;
	uint8_t ucState7;
	uint8_t ucCmd;				/*����*/
	uint8_t ucCrc;				/*crc*/
}ReadBoardLockStateRsp;	
#endif
/*��ȡЭ��汾*/
typedef struct tagReadProtcolVersion_s
{
	uint8_t ucHead;				/*ͷ��*/
	uint8_t ucBoardAddr;			/*�忨��ַ*/
	uint8_t ucCmd1;
	uint8_t ucCmd2;
	uint8_t ucCrc;					/*crcУ��*/
}ReadProtcolVersion;

/*��ȡЭ��汾����*/
typedef struct tagReadProtcolVersionRsp_s
{
	uint8_t ucHead;				/*ͷ��*/
	uint8_t ucBoardAddr;		/*�忨��ַ*/
	uint8_t ucCmd1;
	uint8_t ucVersion;
	uint8_t ucCrc;				/*crcУ��*/
}ReadProtcolVersionRsp;

typedef struct tagLockMsgType
{
    uint8_t cmd;    /*����*/
    uint8_t arg1;   /*����1*/
    uint8_t arg2;   /*����2*/
    uint8_t stau;   /*����Ϣ�Ƿ���ʹ��*/
}LockMsgType;

typedef struct tagLockControl
{
	USARTInfo_s * Usart;	/*����һ�������豸����Ҫ���ð󶨵Ĵ���λ��*/
	BoardInfo  sAllBoardInfo[BOARD_NUM + 1];/*���еİ�״̬*/
	OS_TCB     taskTCB;			/*TCB*/
 	CPU_STK    taskStk[0x100];/*��������ռ�ռ�*/
  OS_Q         msg;/*��Ϣ�������ڿ����������������߳���Ϣͨ��*/
  LockMsgType MsgBuf[LOCK_MSG_NUM]; /*15����Ϣ�ռ�*/
}LockControl;



extern LockControl mdLockControl;
#ifdef __cplusplus
extern "C"
{
#endif
void ControlControlTask(void *p_arg);
uint16_t LockControlReadAllLock(uint8_t BoardAddr);
int8_t LockControlInit( void );
int8_t LockControlOpenALockMsg(uint8_t BoradAddr,uint8_t LockAddr);
int8_t LockControlOpenALock(uint8_t BoradAddr,uint8_t LockAddr);
int8_t LockShowAllLockState();
#ifdef __cplusplus
}
#endif
#endif
