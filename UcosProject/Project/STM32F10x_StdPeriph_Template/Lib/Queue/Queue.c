#include "Queue.h"
#include "stdio.h"
#include "string.h"
/*�����*/

uint16_t EnQueue(Queue *Queues,uint16_t *arg1)
{
	int Newtail = 0;

	Queues->QueueSize = sizeof(Queues->QueueBuf);
	if(Queues->QueueType ==  QueueStandardType)
	{
		Newtail = (Queues->QueueTail + 1) % Queues->QueueSize;
		if(Newtail == Queues->QueueHead)
		{
			return 0;
		}
		else
		{
			Queues->QueueBuf[Queues->QueueTail] = *arg1;
			Queues->QueueTail = Newtail;
		}
	}
	else
	{
		/*��������*/
		Newtail = (Queues->QueueTail + 1) % Queues->QueueSize;
		Queues->QueueBuf[Queues->QueueTail] = *arg1;
		Queues->QueueTail = Newtail;
	}
	return 1;
}
/*������*/
uint16_t DeQueue(Queue *Queues,uint16_t *arg1)
{
	Queues->QueueSize = sizeof(Queues->QueueBuf);
	
	if(Queues->QueueHead == Queues->QueueTail)
	{
		return 0;
	}
	else
	{
		*arg1 = Queues->QueueBuf[Queues->QueueHead];
		Queues->QueueHead = (Queues->QueueHead + 1) % Queues->QueueSize;
		return 1;
	}
}
/*��������Ƿ�Ϊ��*/
int IsQueueEmpty(Queue *Queues)
{
	if(Queues->QueueType == QueueStandardType)
	{
    if(Queues->QueueHead == Queues->QueueTail)
     {
        return 1;
     }
     else
     {
        return 0;
     }
	}
	else
	{
		if(Queues->QueueTail == 0)
		{
			return 1;
		}
	}
	return 0;
}
/*������*/
int IsQueueFull(Queue *Queues)
{
	if((Queues->QueueHead - 1) == Queues->QueueTail)
	{
		return 1;
	}
	return 0;
}
/*�������ÿ�*/
int QueueSetEmpty(Queue *Queues)
{
	
	if(Queues->QueueType == QueueArryType)
	{
		Queues->QueueTail = 0;
		memset(Queues->QueueBuf,0,sizeof(Queues->QueueBuf));
	}
}