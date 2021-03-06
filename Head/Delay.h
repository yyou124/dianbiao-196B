/*==============================================================================
            Copyright(C) 1997-2019.  Sinowealth Tech. Co., Ltd.
--------------------------------------------------------------------------------
Project No.   : XXX
File Name     : Delay..H
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
#ifndef  DELAY_H
#define  DELAY_H


#ifdef DELAY_C
   #define  DELAY_EXT
#else
   #define  DELAY_EXT   extern
#endif
//------------------------------------------------------------------------------

//-----------------------



/*******************************************************************************************
** 函数名称: DelayUs
** 函数描述: 延时dlyTime微秒
** 输入参数: dlyTime:延时微秒数
** 输出参数: 无
*******************************************************************************************/
//DELAY_EXT void DelayUs (unsigned int dlyTime);

/*******************************************************************************************
** 函数名称: Delay_us
** 函数描述: 延时cnt微秒
** 输入参数: cnt:延时微秒数
** 输出参数: 无
*******************************************************************************************/
DELAY_EXT void Delay_us(uint8 cnt);

/*******************************************************************************************
** 函数名称: Delay_ms
** 函数描述: 延时cnt毫秒
** 输入参数: cnt:延时毫秒数
** 输出参数: 无
******************************************************************************************/
DELAY_EXT void Delay_ms(uint8 cnt);



#endif