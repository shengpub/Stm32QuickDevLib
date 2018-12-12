/***********************************************************************************
 * �� �� ��   : Queue.h
 * �� �� ��   : Gavin
 * ��������   : 2017��5��8��
 * �ļ�����   : ����ļ���Ҫ������ʵ�ֻ��ζ��У��ö���Ϊ���Ի��ζ��У���Ҫ��
                �������豸ȥ�����豸֮��Ļ�������
 * ��Ȩ˵��   : Copyright (c) 2008-2017   xx xx xx xx �������޹�˾
 * ��    ��   : 
 * �޸���־   : 
***********************************************************************************/

#include "stm32f10x.h"

#define QueueStandardType           0 /*��׼����*/
#define QueueArryType			1 /*�����Ͷ���*/



typedef struct s_Queue
{ 
	uint8_t  QueueBuf[1024];
	uint16_t QueueSize;	/*������д�С*/
	uint8_t  QueueType;	/*�������ͣ�0��ȫѭ������ 1:ѭ���������Ͷ���*/
	uint16_t QueueHead;	/*�������ͷ��*/
	uint16_t QueueTail;	/*�������β��*/
	uint16_t QueueOldTail;/*��һ�ζ���β��*/
}Queue;

uint16_t EnQueue(Queue *Queues,uint16_t *arg1);
uint16_t DeQueue(Queue *Queues,uint16_t *arg1);
int IsQueueEmpty(Queue *Queues);
int IsQueueFull(Queue *Queues);
int QueueSetEmpty(Queue *Queues);


