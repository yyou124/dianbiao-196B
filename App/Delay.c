/*==============================================================================
                Copyright(C) 1997-2019.  Sinowealth Tech. Co., Ltd.
--------------------------------------------------------------------------------
Project Name  : DELAY.c
Description   :
Notice        :
			 1:
			 2:
Author        : James.wang (email: James.wang@sinowealth.com)
Start Date    : 2015/11/08
Approve Date  :
Version       : V0.0
Function List :
             1:
             2:
RevisionHistory:
Rev#  CheckSum    Date     Author     Comments(Function+Date)
-----+--------+----------+---------+------------------------------------------
0.0     XXXX   2015/11/08 james.wang Just build the file
==============================================================================*/

//-------------------------------------------------------------------------------
#ifndef  DELAY_C
	#define  DELAY_C
#endif
//-------------------------------------------------------------------------------

//---------------




#include "config.h"
#include "Headconfig.h"


////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** ��������: DelayUs
** ��������: ��ʱdlyTime΢��
** �������: dlyTime:��ʱ΢����
** �������: ��
*******************************************************************************************/
//void DelayUs (unsigned int dlyTime)
//{
//	xdata unsigned int  i;
//	for(i=dlyTime;i>0;i--)
//	{
//		_nop_();
//		_nop_();
//	}
//}

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** ��������: Delay_us
** ��������: ��ʱcnt΢��
** �������: cnt:��ʱ΢����
** �������: ��
*******************************************************************************************/
void Delay_us(uint8 cnt)
{
	uint8 i;

	for(i=cnt;i>0;i--)
	{
	  _nop_();
	  _nop_();
	  _nop_();
	  _nop_();
	  _nop_();
	  _nop_();
	  _nop_();
	  _nop_();
	  _nop_();
	  _nop_();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** ��������: Delay_ms
** ��������: ��ʱcnt����
** �������: cnt:��ʱ������
** �������: ��
*******************************************************************************************/
void Delay_ms(uint8 cnt)
{
	uint8 i;
	uint16 j;

	for(i=cnt;i>0;i--)
		for(j=151;j>0;j--);
	
}