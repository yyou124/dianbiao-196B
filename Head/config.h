/*==============================================================================
            Copyright(C) 1997-2019.  Sinowealth Tech. Co., Ltd.
--------------------------------------------------------------------------------
Project No.   : XXX
File Name     : CONFIG..H
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

#ifndef  CONFIG_H
#define  CONFIG_H


#ifdef CONFIG_C
   #define  CONFIG_EXT
#else
   #define  CONFIG_EXT   extern
#endif

//------------------------------------------------------------------------------






#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif




/********************************/
/*      CPU无关性定义           */
/********************************/
typedef unsigned char      uint8;                   //无符号8位整型变量
typedef signed   char      int8;                    //有符号8位整型变量
typedef unsigned short     uint16;                  //无符号16位整型变量
typedef signed   short     int16;                   //有符号16位整型变量
typedef unsigned long      uint32;                  //无符号32位整型变量
typedef signed   long      int32;                   //有符号32位整型变量
typedef float              fp32;                    //单精度浮点数（32位长度）
typedef double             fp64;                    //双精度浮点数（64位长度）
typedef	bit				   Bit;
typedef	bit				   Bool;
typedef	unsigned char	   Byte;
typedef	unsigned int	   Word;
typedef	unsigned long	   Dword;


#define countof(a) (sizeof(a) / sizeof(*(a)))     //取字符串长度
#define		  MAKEDWORD(v1,v2,v3,v4) (((unsigned long)(v1)<<24) + ((unsigned long)(v2)<<16)+ ((unsigned long)(v3)<<8)+ (unsigned long)(v4))
#define	      MAKEWORD(v1,v2)	     (((Word)(v1)<<8) + (Word)(v2))	 //将两个8位合成16位
#define	      HIBYTE(v1)		     ((Byte)((v1)>>8))		         //取16位的高8位
#define	      LOBYTE(v1)		     ((Byte)((v1)&0xff))	         //取16位的低8位

#define       LongToBin(n) \
      (                    \
      ((n >> 21) & 0x80) | \
      ((n >> 18) & 0x40) | \
      ((n >> 15) & 0x20) | \
      ((n >> 12) & 0x10) | \
      ((n >>  9) & 0x08) | \
      ((n >>  6) & 0x04) | \
      ((n >>  3) & 0x02) | \
      ((n      ) & 0x01)   \
      )
#define       Bin(n)                 LongToBin(0x##n##l)	//write binary charactor set,exsample : Bin(11111111) = 0xff

typedef union
{
   uint32 val;
	struct
	{
      uint8 buf3:8;
      uint8 buf2:8;
      uint8 buf1:8;
      uint8 buf0:8;
	}TBuf;
}__U32_Def;                                        //字节定义


typedef union
{
	uint32  val;
	struct
	{
      uint8 buf3:8;
      uint8 buf2:8;
      uint8 buf1:8;
      uint8 buf0:8;
	}TBuf;
}__U32_U08;


#define       BIT0                   0x01
#define       BIT1                   0x02
#define       BIT2                   0x04
#define       BIT3                   0x08
#define       BIT4                   0x10
#define       BIT5                   0x20
#define       BIT6                   0x40
#define       BIT7                   0x80

#define       NOP                    _nop_()

//IEC 通信有效配置
#define 	ENABLE               	0x5A
#define 	DISABLE              	0xA5
#define 	IECComm 		        DISABLE

//DLT645 通信有效配置
#define 	ENABLE               	0x5A
#define 	DISABLE              	0xA5
#define 	DLT645Comm 		        DISABLE

//背光有效配置
#define 	ENABLE               	0x5A
#define 	DISABLE              	0xA5
#define 	BlackLedEn  			DISABLE

//计量模式配置
#define     MT_1P1L              	0x8A        			//单相单线
#define     MT_1P2L              	0x5A        			//单相两线
#define     MT_1P3L              	0xA5        			//单相三线
#define     MeterType            	MT_1P3L

//开表盖有效配置
#define     ENABLE               	0x5A
#define     DISABLE              	0xA5
#define     TCoverChk            	DISABLE

//需量有效配置
#define     ENABLE               	0x5A
#define     DISABLE              	0xA5
#define     DemandEn            	DISABLE

//电池欠压检测有效配置
#define     ENABLE               	0x5A
#define     DISABLE              	0xA5
#define     BatCheckEn            	DISABLE

//启动升级程序有效配置
#define     ENABLE               	0x5A
#define     DISABLE              	0xA5
#define     AutoUpdataProgram       DISABLE


//642B有效配置
#define     ENABLE               	0x5A
#define     DISABLE              	0xA5
#define     MCU_6432                DISABLE

//PIMA TEST有效配置
#define     ENABLE               	0x5A
#define     DISABLE              	0xA5
#define     PIMATestEn              DISABLE//ENABLE




#define     LCDFRAME    		      12	  			//LCD SEGMENT NUMBER
#define     D_ShowItemNum      		4          			//显示项目数
#define     D_PollingDisplayTime          5          			//轮显时间间隔5S

#include <intrins.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#if MCU_6432 == ENABLE
#include "SH79F169B.h"
#else
#include "SH79F169B.h"
#endif


#include "Delay.h"



#include "IO.h"
//#include "ExLigLib.h"
#include "isr.h"
#include "642B_MCU.h"
#include "642B_EMU.h"
#include "642B_INT.h"
//#include "adc.h"
#include "lcd.h"

#include "EEprom.h"

#include "DisplayProc.h"
#include "IIcDriver.h"
#include "RTCDriver.h"
#include "LCDDriver.h"
#include "UartDriver.h"
#include "NBDriver.h"
#include "RELAYDrive.h"
//#include "MeasureMD.h"
#include "Tou.h"
#include "MeasureProc.h"
#include "AdjustMeterProc.h"
#include "PowerAdjustMeter.h"
#include "RTCEventThreadProc.h"
//#include "CRC.h"
#include "COMMU.h"

//#include "Uart_DLT645.h"
//#include "DLT645_CommProc.h"
//#include "LocationForUpFirmCMD.H"




#endif
/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
