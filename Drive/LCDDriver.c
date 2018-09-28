/*==============================================================================
                Copyright(C) 1997-2019.  Sinowealth Tech. Co., Ltd.
--------------------------------------------------------------------------------
Project Name  : LCDDRIVER.c
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
#ifndef  LCDDRIVER_C
	#define  LCDDRIVER_C
#endif
//-------------------------------------------------------------------------------

//---------------

#include "config.h"
#include "Headconfig.h"



unsigned char xdata LCDBuf[32]	 _at_  0x0B00;



////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: LcdDataToICBufShow
** 函数描述:
** 输入参数: 无
** 输出参数: 无
** 返    回：无
** 条    件：无
** 将显示数据写入MCU LCD显示阵列，以此驱动LCD显示
** D_ICLcdSegAdd1 is MCU SEG ADDRESS IS seg0
** D_ICLcdSegAdd2 is MCU SEG ADDRESS IS seg5
** D_ICLcdSegAdd3 is MCU SEG ADDRESS IS seg12
** D_ICLcdSegAdd4 is MCU SEG ADDRESS IS seg24
*******************************************************************************************/
void LcdDataToICBufShow(unsigned char xdata *ptr)
{
	unsigned char i;

	for(i = D_ICLcdSegAdd1; i < (D_ICLcdSegAdd2+1); i++)		//MCU SEG1~SEG4
	{
		LCDBuf[i] = *ptr;
		ptr++;
	}
    for(i = D_ICLcdSegAdd3; i < (D_ICLcdSegAdd4+1); i++)		//MCU SEG7~SEG10
	{
		LCDBuf[i] = *ptr;
		ptr++;
	}
    for(i = D_ICLcdSegAdd5; i < (D_ICLcdSegAdd6+1); i++)		//MCU SEG21~SEG22
	{
		LCDBuf[i] = *ptr;
		ptr++;
	}
    // for(i = D_ICLcdSegAdd7; i < (D_ICLcdSegAdd8+1); i++)		//MCU SEG27~SEG28
	// {
	// 	LCDBuf[i] = *ptr;
	// 	ptr++;
	// }
}