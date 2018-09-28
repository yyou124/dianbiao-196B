/*==============================================================================
                Copyright(C) 1997-2019.  Sinowealth Tech. Co., Ltd.
--------------------------------------------------------------------------------
Project Name  : MAIN.c
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
#ifndef  MAIN_C
	#define  MAIN_C
#endif
//-------------------------------------------------------------------------------

//---------------

#include "config.h"
#include "Headconfig.h"




////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: main
** 函数描述: 程序主函数
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////

	//unsigned char i = 500;
void main(void)
{
	IEN0 = 0x00;           					//关闭总中断，ADC和TPS中断，定时器0、1、2中断，外部1中断，串口0中断
    IEN1 = 0x00;           					//关闭LPD中断，PWM中断，串口1中断，RTC中断，EMU中断
	PSW = 0x00;                             //清溢出标志位，UART通信偶校验(BIT0 = 0)

   Change32KToPLL();	   					//系统2分频，时钟频率4.096MHz

//    Delay_ms(500);                          //延时500ms，等待系统稳定
//    Delay_ms(500);                          //延时500ms，等待系统稳定
    Init_IO();			   					//IO口初始化0
	Init_Timer();		   					//TIME初始化，目前仅使用定时器1
	Init_Uart();		   					//UART初始化，波特率2400，数据位8，偶校验，停止位1


//PWONInit_LCD();					//LCD初始化

/*
目前启用的中断情况：
Timer1      溢出中断
UART0       SBUF中断
RTC         SEC中断
LPD         中断
EMU         EEMU(EMU总中断，在Init_EMU中启用)PF脉冲中断
*/
	IEN1 = (BIT7 | BIT3);    				//开启LPD中断、RTC中断
	EA =1;                                  //总中断打开

//NB_Init();

//目前尚无通信校表功能(在Init_RAM()中被注释掉)
//全局标志位，gbFgKeyProg =F001表示进入工厂模式，=0000表示退出工厂模式
//UART中需要添加直接读写EEPROM功能，以方便测试
//注意处理.h文件中的函数预定义

	while (1)
	{


            ACModeWDTProc();				//AC MODE WDT PROC            修改完成
            EA = 1;
            //监测中断
        test_nb();

		Delay_ms(500);
Delay_ms(500);
            ACInit_IO();					//IO SFR刷新                   修改完成
            ACInit_Timer();             	//Timer SFR刷新                修改完成

            Change32KToPLL();				//AC下监测 PLL                 修改完成
            UARTProcess();                  //UART通讯处理	                修改完成

	}
}
