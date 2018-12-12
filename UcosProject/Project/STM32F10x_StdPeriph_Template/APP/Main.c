   /*
********************************************************************************
*                                  uC/OS-III
*                              ARM Cortex-M3 Port
*
* File      	: Main.C
* Version   	: V1.01
* By        	: 
*
* For       	: Stm32f10x
* Mode      	: 
* Toolchain 	: 
*             		RealView Microcontroller Development Kit (MDK)
*             		Keil uVision
* Description   : ����STM32F10xоƬ ��uC/OS-IIIƽ̨�ϵ��������ģ��
*
*					1��STM32F10x 3.5��
*					2��uC/OS-III v3.03
*				
* Date          : 
*******************************************************************************/

#include "BSP.h"
#include "APP.h"


int fputc(int ch,FILE *f)
{
	USART_SendData(USART1,ch);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)!=SET);
	return(ch);
}

int main()
{
	BspInit();
	
 	KernelMain();
	return 0;
}





