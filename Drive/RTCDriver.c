/*==============================================================================
                Copyright(C) 1997-2019.  Sinowealth Tech. Co., Ltd.
--------------------------------------------------------------------------------
Project Name  : RTCDRIVER.c
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
#ifndef RTCDRIVER_C
	#define RTCDRIVER_C
#endif
//-------------------------------------------------------------------------------

//---------------


#include "config.h"
#include "Headconfig.h"




////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
 * Function:     Init_RTC
 * Description:
 * Calls:
 * Called By:
 * Data Accessed:
 * Data Updated:
 * Input:   strCalendar，
 * Output:
 * Return:
 * Others:
*******************************************************************************************/
void Init_RTC(void)
{
   RTCCON = 0x00;			//关闭RTC内部定时器，CALOUT引脚作为普通IO
   RTCALM = 0x00;           //关闭闹铃0和1的比较功能
   RTCIE = Bin(01000000);   //开启RTC天中断，关闭RTC内部定时器中断、闹铃0和1中断、RTC秒和分钟和小数中断
   RTCIF = 0x00;            //所有的RTC中断标志清零
}

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
 * Function:     SetRealTime
 * Description:  设置日历时间
 * Calls:
 * Called By:
 * Data Accessed:
 * Data Updated:
 * Input:   strCalendar，设置的日历时间,年只支持单字节
 * Output:
 * Return:
 * Others:
*******************************************************************************************/
void SetRealTime(void)
{
    RTCWR = 0x69;             //使能时间日历寄存器和RTC密码寄存器RTCPSW的写入功能

    SEC = _UINT8_RTC[0];
    MIN = _UINT8_RTC[1];
    HR =  _UINT8_RTC[2];
    DAY = _UINT8_RTC[3];
    MTH = _UINT8_RTC[4];
    YR =  _UINT8_RTC[5];
    DOW = _UINT8_RTC[6];	  //week

    RTCPSW = 0x5A;            //RTCPSW寄存器写入0x5A后，设置的值才真正加载到时间日历计时电路
}
//void SetRealTime_Day(void)
//{
//    RTCWR = 0x69;             //使能时间日历寄存器和RTC密码寄存器RTCPSW的写入功能

//    // SEC = _UINT8_RTC[0];
//    // MIN = _UINT8_RTC[1];
//    // HR =  _UINT8_RTC[2];
//    DAY = _UINT8_RTC[3];
//    MTH = _UINT8_RTC[4];
//    YR =  _UINT8_RTC[5];
//    DOW = _UINT8_RTC[6];	  //week

//    RTCPSW = 0x5A;            //RTCPSW寄存器写入0x5A后，设置的值才真正加载到时间日历计时电路
//}
//void SetRealTime_Time(void)
//{
//    RTCWR = 0x69;             //使能时间日历寄存器和RTC密码寄存器RTCPSW的写入功能

//    SEC = _UINT8_RTC[0];
//    MIN = _UINT8_RTC[1];
//    HR =  _UINT8_RTC[2];
//    // DAY = _UINT8_RTC[3];
//    // MTH = _UINT8_RTC[4];
//    // YR =  _UINT8_RTC[5];
//    // DOW = _UINT8_RTC[6];	  //week

//    RTCPSW = 0x5A;            //RTCPSW寄存器写入0x5A后，设置的值才真正加载到时间日历计时电路
//}
////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
 * Function:     ReadCalendarTime
 * Description:  读取当前日历时间
 * Calls:
 * Called By:
 * Data Accessed:
 * Data Updated:
 * Input:
 * Output:       strCalendar，输出日历时间
 * Return:
 * Others:
*******************************************************************************************/
void GetRealTime(void)
{
    RTCCON |= 0x80;              //置位RTCRD位，将最新时间日历更新到对应寄存器，并停止更新
    Delay_us(50);                //RTCRD写1后不会立即变为1，有不超过32us的延时

    _UINT8_RTC[0] = SEC&0x7F;
    _UINT8_RTC[1] = MIN&0x7F;
    _UINT8_RTC[2] = HR&0x3F;
    _UINT8_RTC[3] = DAY&0x3F;
    _UINT8_RTC[4] = MTH&0x1F;
    _UINT8_RTC[5] = YR;
    _UINT8_RTC[6] = DOW&0x07;    //week

    RTCCON &= ~0x80;             //RTCRD位清零，否则时间日历寄存器将一直保持上次读取的值
}


////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: RTCAdjuatEnable
** 函数描述:
** 输入参数:
** 输出参数:
*******************************************************************************************/
// void RTCAdjuatEnable(void)
// {
// 	if(0xA5==mPWRDN_PTM)
// 	{
// 		EMUIE &=0xBF;
//         P2CR = 0x86;   //Bin(10000110)
//         P3CR = 0xF7;   //Bin(11110111)
// 		RTCCON = 0x0B;          //CALOUT引脚输出秒脉冲，高电平有效     //Bin(00001010)
// 	}
// }


////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: RTCAdjuatDisable
** 函数描述:
** 输入参数:
** 输出参数:
*******************************************************************************************/
// void RTCAdjuatDisable(void)
// {
//     RTCCON = 0x00;             	//CALOUT引脚作为普通IO
//     P2CR = D_P2CR;
//     P3CR = D_P3CR;
//     EMUIE |=0x48;  				//Bin(01001000);
//     mPWRDN_PTM = 0x00;
// }

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: Init_RTCAdjustProc
** 函数描述:
** 输入参数:
** 输出参数:
*******************************************************************************************/
void Init_RTCAdjustProc(void)
{
/*
    unsigned int temp;

    if(VER_RDbytes(EE_RTCadjust, (unsigned char *)&temp, 2)!=0)
    {
        RTCDATL = temp & 0x00FF;
        RTCDATH = (temp>>8) & 0x3F;
    }
    else
    {
        RTCDATL = 0x00;
        RTCDATH = 0x00;
    }
*/
    RTCDATL = 0x00;
    RTCDATH = 0x00;

    mPWRDN_PTM = 0x00; //关闭时钟补偿变量值。
}

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: RTCAdjustProc
** 函数描述:
** 输入参数:
** 输出参数:
** 补偿范围为+/-1024PPM，时钟的频率误差和补偿寄存器值计算关系如下:
** Err为正时：E[13:0]= (Err)*11.574/0.127公式中，Err：时钟的频率误差，单位为秒/天；
** Err为负时：E[13:0]= ~[(|Err|)*11.574/0.127] + 1
*******************************************************************************************/
// void RTCAdjustProc(unsigned char symbol)
// {
//     unsigned int temp;

//     if ((symbol & 0x80) == 0x80)   //为负数
//     {
//         symbol = ~symbol + 1;   //负数取反，加1，为正数
//         temp = symbol*16;       //时钟频率误差为0.175*symbol，校准值需要再乘以11.574/0.127
//         temp = ~temp + 1;
//     }
//     else
//     {
//         temp = symbol*16;       //时钟频率误差为0.175*symbol，校准值需要再乘以11.574/0.127
//     }

//     RTCDATL = temp & 0x00FF;
//     RTCDATH = (temp>>8) & 0x3F;

// //    VER_WRbytes(EE_RTCadjust, (unsigned char *)&temp, 2, 1);
// }