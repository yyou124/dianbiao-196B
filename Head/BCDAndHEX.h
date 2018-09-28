/*==============================================================================
            Copyright(C) 1997-2019.  Sinowealth Tech. Co., Ltd.
--------------------------------------------------------------------------------
Project No.   : XXX
File Name     : BCDANDHEX..H
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
/*
#ifndef  BCDANDHEX_H
#define  BCDANDHEX_H


#ifdef BCDANDHEX_C
   #define  BCDANDHEX_EXT
#else
   #define  BCDANDHEX_EXT   extern
#endif
*/
//------------------------------------------------------------------------------



#ifndef		BCDANDHEX_H
#define		BCDANDHEX_H

/*******************************************************************************************
** 函数名称: Delay_us
** 函数描述: 延时cnt微秒
** 输入参数: cnt:延时微秒数
** 输出参数: 无
*******************************************************************************************/
extern void Delay_us(uint8 cnt);

/*******************************************************************************************
** 函数名称: Delay_ms
** 函数描述: 延时cnt毫秒
** 输入参数: cnt:延时毫秒数
** 输出参数: 无
******************************************************************************************/
extern void Delay_ms(uint8 cnt);

/*******************************************************************************************
** 函数名称: BCD2HEX
** 函数描述: 将多字节BCD数据转换为HEX数据
** 输入参数: BCDdata:要处理的BCD数据
** 输出参数: 处理后的HEX
*******************************************************************************************/
extern uint32 WriteToHex(uint8 *BCDdata,uint8 num);

/*********************************************************************************************************
** 函数名称: Write_Array
** 功能描述: 将数据按BCD格式写入数组将多字节HEX码转换成BCD码
**
********************************************************************************************************/
//extern void Write_Array (uint8 *address,uint32 tdat,uint8 num);
extern void Write_ArrayRes (uint8 *address,uint32 tdat,uint8 num);

/*******************************************************************************************
** 函数名称: BCD2HEX
** 函数描述: 将1字节BCD数据转换为HEX数据
** 输入参数: BCDdata:要处理的BCD数据
** 输出参数: 处理后的HEX
*******************************************************************************************/
extern uint8 HEX2BCD(uint8 Hexdata);

/*******************************************************************************************
** 函数名称: BCD2HEX
** 函数描述: 将1字节BCD数据转换为HEX数据
** 输入参数: BCDdata:要处理的BCD数据
** 输出参数: 处理后的HEX
*******************************************************************************************/
extern uint8 BCD2HEX(uint8 BCDdata);

unsigned char DataToWeek(unsigned char *ptr);
/*******************************************************************************************
** 函数名称: BCDDEC
** 函数描述: BCD减法
** 输入参数: BCDdata:要处理的BCD数据
** 输出参数: 返回进位位
*******************************************************************************************/
extern uint8 BCDDEC(uint8 *sSub,uint8 len,uint8 step);
extern unsigned char BCDCheck(unsigned char *buff, unsigned char len);
extern unsigned int BCD2toINT(unsigned char *BCD);
//extern void INTtoBCD2(unsigned int INT, unsigned char *BCD);


#endif