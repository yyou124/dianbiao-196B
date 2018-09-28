/*==============================================================================
            Copyright(C) 1997-2019.  Sinowealth Tech. Co., Ltd.
--------------------------------------------------------------------------------
Project No.   : XXX
File Name     : LCDRIVER..H
Description   :
              £»
              £»
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
#ifndef LCDRIVER_H
#define RTCDRIVER_H

#ifndef LCDRIVER_C
	#define LCDRIVER_EXT	extern
#else
	#define LCDRIVER_EXT
#endif
//------------------------------------------------------------------------------

//----------------



#define D_MCUSEG1       	1
#define D_MCUSEG4       	4

#define D_MCUSEG7      		7
#define D_MCUSEG10      	10

#define D_MCUSEG21      	21
#define D_MCUSEG24      	24

// #define D_MCUSEG27      	27
// #define D_MCUSEG28      	28

//lcd segment is seg1~seg6,seg13~seg24
#define D_ICLcdSegAdd1		(D_MCUSEG1 - 1)	//address 00H		 //MCU SEG ADDRESS IS seg1
#define D_ICLcdSegAdd2		(D_MCUSEG4 - 1)	//address 03H		 //MCU SEG ADDRESS IS seg6		6

#define D_ICLcdSegAdd3		(D_MCUSEG7 - 1)	//address 06H	     //MCU SEG ADDRESS IS seg13
#define D_ICLcdSegAdd4		(D_MCUSEG10 - 1)//address 09H		 //MCU SEG ADDRESS IS seg24		12	 SEG SUM =6+12=18 SEG

#define D_ICLcdSegAdd5		(D_MCUSEG21 - 1)//address 14H		 //MCU SEG ADDRESS IS seg13
#define D_ICLcdSegAdd6		(D_MCUSEG24 - 1)//address 17H		 //MCU SEG ADDRESS IS seg24		12	 SEG SUM =6+12=18 SEG

// #define D_ICLcdSegAdd7		(D_MCUSEG27 - 1)		 //MCU SEG ADDRESS IS seg13
// #define D_ICLcdSegAdd8		(D_MCUSEG28 - 1)		 //MCU SEG ADDRESS IS seg24		12	 SEG SUM =6+12=18 SEG



//LCD
LCDRIVER_EXT unsigned char xdata LCDBuf[32];
LCDRIVER_EXT void LcdDataToICBufShow(unsigned char xdata *ptr);




#endif