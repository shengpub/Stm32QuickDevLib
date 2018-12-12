/***********************************************************************************
 * �� �� ��   : BspDeviceList.c
 * �� �� ��   : Gavin
 * ��������   : 2017��6��7��
 * �ļ�����   : �豸����ʵ�֣�һ������²���Ҫ�޸ĸ��ļ�
 * ��Ȩ˵��   : Copyright (c) 2008-2017   xx xx xx xx �������޹�˾
 * ��    ��   : 
 * �޸���־   : 
***********************************************************************************/

#include "BspDeviceList.h"

/*�豸ȫ������*/
SysDevStrutInfo *DeviceListHead = NULL;

/*****************************************************************************
 * �� �� ��  : DeviceListInit
 * �� �� ��  : Gavin
 * ��������  : 2017��6��19��
 * ��������  : �豸�����ʼ�������е��豸����ע��ķ�ʽע�ᵽϵͳ�У�ͳһ��
               �й���
 * �������  : void  ��
 * �������  : ��
 * �� �� ֵ  : ��������ڵ�
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
SysDevStrutInfo * DeviceListInit( void )
{
    SysDevStrutInfo *jDeviceListHead = NULL;

	jDeviceListHead = (SysDevStrutInfo *)malloc(sizeof(SysDevStrutInfo));
	if(NULL == jDeviceListHead )
	{
		return NULL;
	}

	jDeviceListHead->next = NULL;

	return jDeviceListHead;
}

/*****************************************************************************
 * �� �� ��  : FindSelectNode
 * �� �� ��  : Gavin
 * ��������  : 2017��6��19��
 * ��������  : ��������Ľڵ���ҵ���Ӧ������ڵ�
 * �������  : ��
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
SysDevStrutInfo *FindSelectNode( int NodeId)
{
	SysDevStrutInfo *Node = DeviceListHead;
	
	if(Node == NULL)
	{
		return NULL;
	}
	
	if( NodeId <= 0 )
	{
		return NULL;
	}

	while(Node != NULL)
	{
		if(Node->NodeInfo.DeviceId == NodeId)
		{
			return Node;
		}
		Node = Node->next;
	}

	return NULL;
}

/*****************************************************************************
 * �� �� ��  : FindEndNode
 * �� �� ��  : Gavin
 * ��������  : 2017��6��23��
 * ��������  : �������������һ���ڵ�
 * �������  : ��
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
SysDevStrutInfo* FindEndNode()
{
    SysDevStrutInfo *Node = DeviceListHead;

	while(Node->next != NULL)
	{
		Node = Node->next;
	}
	return Node;
	/*�������һ���ڵ�*/
}

/*****************************************************************************
 * �� �� ��  : FindSelectNodeFromName
 * �� �� ��  : Gavin
 * ��������  : 2017��6��23��
 * ��������  : �������Ʋ��������е��豸
 * �������  : ��
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
SysDevStrutInfo* FindSelectNodeFromName( char *DeviceName)
{
	SysDevStrutInfo *Node = DeviceListHead;
	
	if(Node == NULL)
	{
		return NULL;
	}

	while(Node != NULL)
	{
		if(NULL != strstr(Node->NodeInfo.DeviceName,DeviceName))
		{
			return Node;
		}
		Node = Node->next;
	}

	return NULL;
}

/*****************************************************************************
 * �� �� ��  : device_open
 * �� �� ��  : Gavin
 * ��������  : 2017��6��19��
 * ��������  : �豸�򿪲���
 * �������  : ��
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
int  device_open(char *DeviceName,int *arg)
{
	SysDevStrutInfo *Node = NULL;
    /*1��ͨ���豸�������������ҵ���Ӧ������*/
	/*2���ص���Ӧ�Ĵ򿪺���*/
	Node = FindSelectNodeFromName(DeviceName);
	Node->DevOpeatationInfo.DeviceOpen(DeviceName,arg);
}

/*****************************************************************************
 * �� �� ��  : device_write
 * �� �� ��  : Gavin
 * ��������  : 2017��6��23��
 * ��������  : �豸д����
 * �������  : ��
 * �������  : ��
 * �� �� ֵ  : int
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
int device_write(int DeviceFd,char *UserBuf,int len)
{
    SysDevStrutInfo *Node = NULL;
    /*ͨ���豸fd�Ҵ��Ӧ���豸�����ص���ȡ����*/
	if(DeviceFd > 0)
	{
		Node = FindSelectNode(DeviceFd);
		return Node->DevOpeatationInfo.DeviceWrite(DeviceFd,UserBuf,len);
	}

	return -1;
}

/*****************************************************************************
 * �� �� ��  : device_read
 * �� �� ��  : Gavin
 * ��������  : 2017��6��19��
 * ��������  : �豸��ȡ����
 * �������  : ��
 * �������  : ��
 * �� �� ֵ  : int
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
int device_read(int DeviceFd,char *UserBuf,int len)
{
	SysDevStrutInfo *Node = NULL;
    /*ͨ���豸fd�Ҵ��Ӧ���豸�����ص���ȡ����*/
	if(DeviceFd > 0)
	{
		Node = FindSelectNode(DeviceFd);
		return Node->DevOpeatationInfo.DeviceRead(DeviceFd,UserBuf,len);
	}

	return -1;
}
/*�������غ���*/
void mode_init(ModeCallBack Func)
{
	/*�����豸id*/
	SysDevStrutInfo *Node = NULL;
	SysDevStrutInfo *NodeNew = NULL;
	
	Node = FindEndNode();

	NodeNew = (SysDevStrutInfo *)malloc(sizeof(SysDevStrutInfo));
	if(NULL == NodeNew )
	{
		while(1);
	}
	Node->next = NodeNew;

	if(NULL == Func)
	{
		while(1);
	}

	Func(NodeNew);/*�����û�ģ���ʼ��*/
	
	/*1��������Ľڵ��������*/
}
/*ģ���˳�����*/
void mode_exit(ModeCallBack Func)
{
	/*�ҵ��ýڵ�*/
	/*��������ɾ���ýڵ�*/
	
}

