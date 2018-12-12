#include "Bsp_epprom.h"
/*****************************************************************************
 * �� �� ��  : EppromInit
 * �� �� ��  : л��
 * ��������  : 2017��7��14��
 * ��������  : Epprom��ʼ������
 * �������  : void  ��
 * �������  : ��
 * �� �� ֵ  : void
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
void EppromInit( void )
{
    /*errom������ʼ������*/
    SimulationI2cInit();
}

/*****************************************************************************
 * �� �� ��  : EppromWriteOneByte
 * �� �� ��  : л��
 * ��������  : 2017��7��14��
 * ��������  : ��洢�ռ�дһ���ֽ�����
 * �������  : uint8_t slave_addr  �ӻ���ַ
               uint8_t byte_addr   �洢��ַ
               uint8_t byte_dat    ����
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
void EppromWriteOneByte(uint8_t slave_addr,uint8_t byte_addr,uint8_t byte_dat)
{
   uint16_t uiLoop = 0;
   
    SimulationI2cStart();     
    
    slave_addr &= 0xfe;
    
    SimulationI2cSendByte(slave_addr);     
    SimulationI2cSendByte(byte_addr);     	
    
    SimulationI2cSendByte(byte_dat);
    
    for(uiLoop = 0;uiLoop < 1000;uiLoop++);
    
    SimulationI2cStop();             		
}
/*****************************************************************************
 * �� �� ��  : EppromReadOneByte
 * �� �� ��  : л��
 * ��������  : 2017��7��14��
 * ��������  : Epprom��ȡһ���ֽ�
 * �������  : uint8_t slave_addr  �ӻ��豸
               uint8_t byte_addr   ��ȡ��ַ
               uint8_t *byte_dat   ��ȡ�����ݴ洢λ��
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
void EppromReadOneByte(uint8_t slave_addr,uint8_t byte_addr,uint8_t *byte_dat)
{
    SimulationI2cStart();            		
    slave_addr&=0xfe;
    
    SimulationI2cSendByte(slave_addr);     	
    SimulationI2cSendByte(byte_addr);     
    SimulationI2cStart();  
    
    slave_addr|=0x01;
    
    SimulationI2cSendByte(slave_addr);     
    
    *byte_dat = Simulationl2cRecvByte();
    
    SimulationI2cSendACK(1); 	
    
    SimulationI2cStop();             	
}


/*****************************************************************************
 * �� �� ��  : EppromReadBuf
 * �� �� ��  : Gavin
 * ��������  : 2017��6��4��
 * ��������  : epprom�洢����ȡָ�����ݵ���������
 * �������  : uint16_t StartAddr  ��ʼ��ַ
               uint8_t * Buf  �洢����
               uint16_t Len  ��ȡ����
 * �������  : ��
 * �� �� ֵ  : int16_t
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
int16_t EppromReadBuf( uint16_t StartAddr, uint8_t * Buf, uint16_t Len )
{
	uint16_t StartAddr_n = 0;
	uint16_t uiLoop = 0;
	uint8_t dat = 0;
	StartAddr_n = StartAddr;
	
	while(uiLoop < Len)
	{
		EppromReadOneByte(0xa0,StartAddr_n,&dat);
		Buf[uiLoop++] = dat;
		//SysDelayUs_d(1000);
		StartAddr_n++;
		dat = 0;
		
	}
    
}

/*****************************************************************************
 * �� �� ��  : EppromWriteBuf
 * �� �� ��  : Gavin
 * ��������  : 2017��6��4��
 * ��������  : ��ָ���洢�ռ������д��EPPROMָ����ַ��
 * �������  : uint16_t StartAddr  ��ʼ��ַ
               uint8_t *Buf  �ڴ��ַ
               uint16_t Len  ������Ϣ
 * �������  : ��
 * �� �� ֵ  : int16_t
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
int16_t EppromWriteBuf( uint16_t StartAddr, uint8_t *Buf, uint16_t Len )
{
	uint16_t StartAddr_n = 0;
	uint16_t uiLoop = 0;
	uint8_t dat = 0;
	StartAddr_n = StartAddr % 2048;
	
	while(uiLoop < Len)
	{
		EppromWriteOneByte(0xa0,StartAddr_n % 2048,Buf[uiLoop++]);
		//SysDelayUs_d(10000);
		StartAddr_n++;	
	}
}

void EppromEase()
{
	uint16_t uiLoop = 0;
	
	for(uiLoop = 0;uiLoop<1;uiLoop++)
	{
		EppromWriteOneByte(0xa0,uiLoop,0);
	}
	
}


