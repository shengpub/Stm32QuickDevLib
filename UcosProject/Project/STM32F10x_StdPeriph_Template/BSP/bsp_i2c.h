/***********************************************************************************
 * �� �� ��   : bsp_i2c.h
 * �� �� ��   : л��
 * ��������   : 2017��7��14��
 * �ļ�����   : I2C���������㣬���а���ģ�ⷽʽ�ڲ�ģ��������ʽ
 * ��Ȩ˵��   : Copyright (c) 2008-2017   xx xx xx xx �������޹�˾
 * ��    ��   : 
 * �޸���־   : 
***********************************************************************************/
#ifndef BSP_I2C_H
#define BSP_I2C_H

#include "BspConfig.h"
#include "stdio.h"
#include "os.h"
#include "Lib/Queue/Queue.h"

#define SCL_H         GPIO_SetBits(GPIOB,GPIO_Pin_6) 
#define SCL_L         GPIO_ResetBits(GPIOB,GPIO_Pin_6)  
    
#define SDA_H         GPIO_SetBits(GPIOB,GPIO_Pin_7) 
#define SDA_L         GPIO_ResetBits(GPIOB,GPIO_Pin_7)

#define SCL_STATE      GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6) 
#define SDA_STATE      GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7) 


int8_t SimulationI2cInit( );
void SimulationI2cStart();
void SimulationI2cStop();
void SimulationI2cSendACK(uint8_t ack);
uint8_t SimulationI2cRecvACK();
void SimulationI2cSendByte(uint8_t dat);
uint8_t Simulationl2cRecvByte();

#endif
