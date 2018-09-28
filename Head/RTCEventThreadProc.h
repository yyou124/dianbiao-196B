/*==============================================================================
            Copyright(C) 1997-2019.  Sinowealth Tech. Co., Ltd.
--------------------------------------------------------------------------------
Project No.   : XXX
File Name     : RTCEVENTTHREADPROC..H
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
#ifndef	RTCEVENTTHREADPROC_H
#define	RTCEVENTTHREADPROC_H

#ifndef RTCEVENTTHREADPROC_C
	#define RTCEVENTTHREADPROC_EXT	extern
#else
	#define RTCEVENTTHREADPROC_EXT
#endif
//------------------------------------------------------------------------------

//----------------



RTCEVENTTHREADPROC_EXT unsigned int xdata  gBClsKeyCount;
RTCEVENTTHREADPROC_EXT unsigned char xdata gBClsKeyEnable; 		//meter sum cls enable command word
RTCEVENTTHREADPROC_EXT unsigned char xdata gBClsKeyEnable1;		//meter sum cls enable command word


RTCEVENTTHREADPROC_EXT void JumpMeterClsZeroThread(void);		//跳线电量清零线程处理
RTCEVENTTHREADPROC_EXT void BatteryVolThread(void);
//RTCEVENTTHREADPROC_EXT void BatModeThreadProc(void);


#endif