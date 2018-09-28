/*==============================================================================
            Copyright(C) 1997-2019.  Sinowealth Tech. Co., Ltd.
--------------------------------------------------------------------------------
Project No.   : XXX
File Name     : _642B_INT..H
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
#ifndef  _642B_INT_H
#define  _642B_INT_H


#ifdef _642B_INT_C
   #define  _642B_INT_EXT
#else
   #define  _642B_INT_EXT   extern
#endif
//------------------------------------------------------------------------------

//-------------------


#define MachineCycle        (FSYSCLK/12)		 //FSYSCLK= 32.768*125=4.096m
#define	TCLK				MachineCycle
#define T0OV_FREQ           120//40			 2.9929US		5MS/2.9929US=1670，65536-1670=63866=F97A
#define T0_25MS_CNT         63866//(65536 - TCLK/T0OV_FREQ + 2)



_642B_INT_EXT unsigned char data gBTime0Flag;
_642B_INT_EXT unsigned char data gBCountFlag;
_642B_INT_EXT unsigned char data gBCount10ms;
_642B_INT_EXT unsigned int data NBCount10ms;//NB模块超时处理
_642B_INT_EXT unsigned char data WDTCount10ms;

/*********************************************************************************************************
** 函数名称: Init_Timer
** 功能描述: 初始化TIMER
** 入口参数: 无
** 出口参数: 无
********************************************************************************************************/
_642B_INT_EXT void Init_Timer(void);



#endif