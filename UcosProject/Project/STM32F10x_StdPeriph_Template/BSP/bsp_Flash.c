#include "bsp_Flash.h"


uint16_t STMFLASH_BUF[STM_SECTOR_SIZE/2];

/*flash������ʹ��4k FLASHģ��errrom����������1k����*/

u16 FLASH_ReadHalfWord(u32 faddr)
{
	return *(vu16*)faddr; 
}

void FLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead)   	
{
	u16 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i] = FLASH_ReadHalfWord(ReadAddr);
		ReadAddr+=2;
	}
}
void FLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)   
{ 			 		 
	u16 i;
	for(i=0;i<NumToWrite;i++)
	{
		FLASH_ProgramHalfWord(WriteAddr,pBuffer[i]);
	    WriteAddr+=2;
	}  
}
/*****************************************************************************
 * �� �� ��  : FlashWrite
 * �� �� ��  : xiegui
 * ��������  : 2018��1��25��
 * ��������  : flash д��
 * �������  : uint32_t uiWriteAddr   д������
               uint16_t *pBuffer      ���ݵ�ַ
               uint16_t uiNumOfWrite  ���� 2ֽ
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
void FlashWrite(uint32_t uiWriteAddr,uint16_t *pBuffer,uint16_t uiNumOfWrite)
{

	uint32_t uiOffAddr = 0;/*ʵ��ƫ�Ƶ�ַ*/
	uint32_t uiSecPos = 0;/*������ַ*/
	uint16_t uiSecOff = 0;/*������ƫ�Ƶ�ַ*/
	uint16_t uiSecRemain = 0;/*������ʣ���ַ*/
	uint16_t uiLoop = 0;
	
	if(uiWriteAddr < STM32_FLASH_BASE || uiWriteAddr >= (STM32_FLASH_BASE + 1024 * STM32_FLASH_SIZE))
	{
		return;
	}
	/**����flash*/
	FLASH_Unlock();
	uiOffAddr = uiWriteAddr - STM32_FLASH_BASE;
	uiSecPos = uiOffAddr / STM_SECTOR_SIZE;
	uiSecOff =  (uiOffAddr % STM_SECTOR_SIZE) / 2;
	uiSecRemain = (STM_SECTOR_SIZE /2 - uiSecOff); 


	if(uiNumOfWrite <= uiSecRemain) 
	{
		uiSecRemain = uiNumOfWrite;
	}
	while(1)
	{
		FLASH_Read(uiSecPos * STM_SECTOR_SIZE + STM32_FLASH_BASE,
						STMFLASH_BUF,STM_SECTOR_SIZE/2);
		for(uiLoop = 0; uiLoop < uiSecRemain;uiLoop++)
		{
			if(STMFLASH_BUF[uiSecOff + uiLoop] != 0xffff)
			{
				break;
			}
		}
		if(uiLoop < uiSecRemain)
		{
			FLASH_ErasePage(uiSecPos * STM_SECTOR_SIZE+STM32_FLASH_BASE);
			for(uiLoop = 0;uiLoop < uiSecRemain;uiLoop++)
			{
				STMFLASH_BUF[uiLoop + uiSecOff]=pBuffer[uiLoop];	  
			}
			FLASH_Write_NoCheck(uiSecPos * STM_SECTOR_SIZE+STM32_FLASH_BASE,
								STMFLASH_BUF,STM_SECTOR_SIZE/2);
		}
		else
		{
			FLASH_Write_NoCheck(uiOffAddr,pBuffer,uiSecRemain);
		}
		if(uiNumOfWrite == uiSecRemain)
		{
			break;
		}
		else
		{
			uiSecPos++;
			uiSecOff = 0;
			pBuffer += uiSecRemain;
			uiWriteAddr += uiSecRemain;
			uiNumOfWrite -=uiSecRemain;
			if(uiNumOfWrite > STM_SECTOR_SIZE / 2)
			{
				uiSecRemain = STM_SECTOR_SIZE / 2;
			}
			else
			{
				uiSecRemain = uiNumOfWrite;
			}
		}
	}
	FLASH_Lock();
}

