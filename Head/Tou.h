/*==============================================================================
            Copyright(C) 1997-2019.  Sinowealth Tech. Co., Ltd.
--------------------------------------------------------------------------------
Project No.   : XXX
File Name     : TOU..H	
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
#ifndef TOU_H
#define TOU_H

#ifndef TOU_C
	#define TOU_EXT	extern
#else
	#define TOU_EXT         
#endif
//------------------------------------------------------------------------------

//----------------


#define PwrUpBill 0x01
#define NorBill   0x02
#define ManuBill  0x03


extern void CheckRTC(void);
extern void TimeTaskThread();
//extern void AP_Bill(unsigned char Mode);
extern void AP_ClearEvent(void);
//extern void PwrDn_Bill(void);
extern void SaveHistorys(void);
extern void ReadRTCProc(void);
extern void AP_ClearAllData(unsigned char Cmd);
extern void AP_RSTCNTINC(void);
#if TCoverChk == ENABLE
extern void AP_PowerDown_TermialChk(void);
#endif
#endif