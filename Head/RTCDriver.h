/*==============================================================================
            Copyright(C) 1997-2019.  Sinowealth Tech. Co., Ltd.
--------------------------------------------------------------------------------
Project No.   : XXX
File Name     : RTCDRIVER..H
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
#ifndef RTCDRIVER_H
#define RTCDRIVER_H

#ifndef RTCDRIVER_C
	#define RTCDRIVER_EXT	extern
#else
	#define RTCDRIVER_EXT
#endif
//------------------------------------------------------------------------------

//----------------



RTCDRIVER_EXT unsigned char xdata  _UINT8_RTC[7];

/*
_UINT8_RTC[0]   --->    秒
_UINT8_RTC[1]   --->    分
_UINT8_RTC[2]   --->    时
_UINT8_RTC[3]   --->    天
_UINT8_RTC[4]   --->    月
_UINT8_RTC[5]   --->    年
_UINT8_RTC[6]   --->    星期
*/
RTCDRIVER_EXT unsigned char  xdata mPWRDN_PTM;     // 上电时时钟补偿禁止开关

RTCDRIVER_EXT void Init_RTC(void);
RTCDRIVER_EXT void SetRealTime(void);

RTCDRIVER_EXT void SetRealTime_Day(void);
RTCDRIVER_EXT void SetRealTime_Time(void);
RTCDRIVER_EXT void GetRealTime(void);
//RTCDRIVER_EXT void RTCAdjuatEnable(void);
//RTCDRIVER_EXT void RTCAdjuatDisable(void);
RTCDRIVER_EXT void Init_RTCAdjustProc(void);
//symbol=0xff，负误差符号
//symbol=0x00，正误差符号
//RtcErr 0.0~22.0
//RTCDRIVER_EXT void RTCAdjustProc(unsigned char symbol);



#endif
