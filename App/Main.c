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

//unsigned char temp[18] = {0x11,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18};
//unsigned char temp_read[18] = {0x00,0x00};
//unsigned char temp[10] = {0x12,0x12,0x13,0x04};
//unsigned char DataRecieve[256];
//	unsigned char nb_receive_len;
//	 unsigned char temp[] = "\r\n+QLWDATARECV: 19,1,0,27,01010000000000000011001F19\r\n";
void main(void)
{
	IEN0 = 0x00;                        //关闭总中断，ADC和TPS中断，定时器0、1、2中断，外部1中断，串口0中断
	IEN1 = 0x00;                        //关闭LPD中断，PWM中断，串口1中断，RTC中断，EMU中断
	PSW = 0x00;                         //清溢出标志位，UART通信偶校验(BIT0 = 0)
	Change32KToPLL();                   //系统2分频，时钟频率4.096MHz

	Delay_ms(500);                      //延时500ms，等待系统稳定
	Delay_ms(500);                      //延时500ms，等待系统稳定

	Init_IO();                          //IO口初始化
	Init_Uart();                        //UART初始化，波特率9600，数据位8，无校验，停止位1
	PWONInit_LCD();                     //LCD初始化
	Init_RTC();                         //RTC初始化
	ReadRTCProc();                      //读RTC处理
	CheckRTC();                         //RTC校验处理,改为默认时间2018-01-01 12: 00: 00
	Init_Timer();                       //TIME初始化，目前仅使用定时器1
	g_Flag.Run &= ~F_PWRUP;             //AC MODE

/*
目前启用的中断情况：
Timer1      溢出中断
UART0       SBUF中断
RTC         SEC中断
LPD         中断
EMU         EEMU(EMU总中断，在Init_EMU中启用)PF脉冲中断
*/
	IEN1 = (BIT7 | BIT3);               //开启LPD中断、RTC中断
	EA =1;                              //总中断打开

	while (1)
	{
	//	NBdata_Receive_MTK((char *)temp,DataRecieve, (unsigned char *)&nb_receive_len);
	//E_to_RAM(EE_PROG_FLAG, &temp[4], 3);

        if((g_Flag.Run & F_PWRUP) == 0) //首次上电
        {
            ACModeWDTProc();            //AC模式喂狗                修改完成
            Change32KToPLL();           //时钟频率控制4.096MHz       修改完成
            Init_IO();                  //IO口初始化                修改完成
            Init_Uart();                //UART初始化                修改完成
            PWONInit_LCD();             //LCD初始化
            Init_RTC();                 //RTC初始化                 修改完成
            Init_RAM();                 //一些参数的初始化           修改完成，校表代码已被注释掉
            g_Flag.Run |= F_PWRUP;      //置上电的标志
            Init_Timer();               //TIME初始化                修改完成
            ReadRTCProc();              //读RTC处理                 修改完成
            CheckRTC();                 //RTC校验处理                修改完成
            if((LPDCON & 0x08) == 0x08) //VCC>2.7V
            {
                Init_EMU();             //初始化EMU                  修改完成
            }
            else                        //VCC<2.7V
            {
                g_Flag.Run &= ~F_PWRUP; //置下电的标志
            }
            //启用LPD中断,主程序不需要

            //CheckPowerOff();//掉电检测，此处仅检测VCC<2.7v  修改完成
        }
        else
        {
					
            ACModeWDTProc();            //AC MODE WDT PROC            修改完成
            EA = 1;
            //监测中断
            TimeTaskThread();           //时间任务电量线程处理				 修改完成,包含校表程序
            ACInit_IO();                //IO SFR刷新                   修改完成
            ACInit_RTC();               //RTC SFR刷新                  修改完成
            ACInit_Timer();             //Timer SFR刷新                修改完成
            DisplayProc();              //LCD显示线程
            EMUTampProc();              //每秒的电量参数瞬时读取，计算	修改完成，计算细节待完善
            EnergyProc();               //电量处理，总用电量累加         修改完成，每0.01度累加一次
            Change32KToPLL();           //AC下监测 PLL                 修改完成
            ACInit_EMUPluse();          //AC 刷新EMU                   修改完成
            UART1Process();             //UART1通讯处理	                修改完成
            CommuProcess();             //通信处理
            //启用LPD中断,主程序不需要
            //CheckPowerOff();          //掉电检测			            修改完成
        }
	}/*while finish*/
}/*main finish*/
