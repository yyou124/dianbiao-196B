/*==============================================================================
            Copyright(C) 1997-2019.  Sinowealth Tech. Co., Ltd.
--------------------------------------------------------------------------------
Project No.   : XXX
File Name     : ADJUSTMETERPROC..H
Description   :
              ；
              ；
Notice        :
             1:
             2:

Author        : James (email: james.wang@sinowealth.com)
Start Date    : 2015/11/08
Release Date  :
Approve Date  :
Version       : V1.0
CheckSum      : XXXX
Function List :
             1:
             2:

RevisionHistory:
Rev#  CheckSum   Date     Author     Comments(Function+Date)
-----+--------+----------+---------+--------------------------------------------
0.0 2015/11/08 James Just build the function
0.1
==============================================================================*/


//------------------------------------------------------------------------------
#ifndef ADJUSTMETERPROC_H
#define ADJUSTMETERPROC_H

#ifndef ADJUSTMETERPROC_C
	#define ADJUSTMETERPROC_EXT	extern
#else
	#define ADJUSTMETERPROC_EXT
#endif
//------------------------------------------------------------------------------

//----------------



typedef struct
{
    unsigned char Step;
    unsigned char Key;
    unsigned char Flag;

    unsigned char ClsKeyStep;
    unsigned char ClsKey;
    unsigned char ClsKeyFlag;
}_CAL;

typedef struct
{
    unsigned long I1Buf;
    unsigned long I2Buf;
    unsigned long VBuf;
    unsigned long P2Buf;
}_OffsetCal;

typedef union
{
    unsigned char Buffer[16];
    _OffsetCal Offset;
}_CALTMP;

typedef struct
{
    unsigned long Buf[5];
    unsigned long LastVal;
    unsigned char FPtr;
}_FILTERV;

typedef struct
{
	float Vrms;
	float Irms_A;
	float Irms_B;
	float Power;
}_INSF;

#define K_Cal        0xA5

#define D_InitStart  0x01
#define D_Initing    0x02
#define D_InitAdjust 0x03
#define D_InitOk     0x04
#define D_InitEnd    0x05

#define D_Auto		 0x08


//校表初始化步骤
#define S_DataInit  0x01  		//数据初始化
#define S_ADCShort  0x02		//ADC短接
#define S_Wait      0x03
#define S_Wait1     0x04
#define S_GetOfst1  0x05		//连续采样i1,i2,v四次
#define S_GetOfst2  0x06
#define S_GetOfst3  0x07
#define S_GetOfst4  0x08
#define S_Calculate 0x09		//计算连续采样i1,i2,v四次

//校表参数校准步骤
#define Adj0_EMU_INIT		0x10FF//EMU_INIT
#define Adj1_VRMS_GAIN  	0x10C0//VRMS GAIN
#define Adj2_I1RMS_GAIN 	0x10C1//I1RMS GAIN
#define Adj3_I2RMS_GAIN 	0x10C2//I2RMS GAIN
#define Adj4_CHA_Power_GAIN 0x10C3//CHA Power GAIN
#define Adj5_CHB_Power_GAIN 0x10C4//CHB Power GAIN
#define Adj6_CHA_Phase 		0x10C5//CHA Phase
#define Adj7_CHB_Phase 		0x10C6//CHB Phase
#define Adj8_CHA_Offset 	0x10C7//CHA Offset
#define Adj9_CHB_Offset 	0x10C8//CHB Offset
#define Adj10_adcos_NULL    0x10C9//adcos_NULL
#define Adj11_adcos 		0x10CA//adcos
#define Adj12_irms1os 		0x10CB//irms1os
#define Adj13_irms2os		0x10CC//irms2os
#define Adj14_cal_L_mode	0x0F0A//cal_L_mode	 cal_1P3L_mode
#define Adj15_cal_N_mode    0x0F0B//cal_N_mode   cal_1P3L_mode
#define Adj16_cal_L_N_mode  0x0F0C//cal_L_N_mode

#define Adj5_CHB_Power_GAIN_OK  0x20C4
#define Adj7_CHB_Phase_OK       0x20C6
#define Adj9_CHB_Offset_OK      0x20C8


ADJUSTMETERPROC_EXT unsigned int xdata gbFgKeyProg;	    //全局标志位，=F001表示进入工厂模式，=0000表示退出工厂模式
ADJUSTMETERPROC_EXT unsigned int xdata gbFgClsKeyProg;	//全局标志位，=F002表示进入工厂总清模式，=0000表示退出工厂总清模式

ADJUSTMETERPROC_EXT _CAL 		g_Cal;
ADJUSTMETERPROC_EXT _CALTMP 	g_CalTmp;
ADJUSTMETERPROC_EXT _FILTERV 	g_FilterV;

ADJUSTMETERPROC_EXT void AdjustMeterInit(void);
//ADJUSTMETERPROC_EXT unsigned char AdjustMeterProc(unsigned int ID,unsigned char *Src);

#endif