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
void main(void)
{
	IEN0 = 0x00;           					//关闭总中断，ADC和TPS中断，定时器0、1、2中断，外部中断0、1，串口0中断
    IEN1 = 0x00;           					//关闭LPD中断，串口1、2中断，外部中断2、3，PWM中断，EMU中断  
	PSW = 0x00;

	BatSupplySwitch();     					//开启LPD检测、供电自动切换、LPD中断
    Change32KToPLL();	   					//系统2分频
    
	Init_IO();			   					//IO口初始化
    Init_PWM();			   					//PWM初始
    Init_TPS();			   					//TPS初始化
	Init_ADC();			   					//ADC初始化
	Init_LCD();			   					//LCD初始化
	Init_RAM();			   					//RAM初始化
	Init_RTC();			   					//RTC初始化
	ReadRTCProc();		   					//读RTC处理
    CheckRTC();			   					//RTC校验处理
	Init_Timer();		   					//TIME初始化
	Init_Uart();		   					//UART初始化

//  AP_RSTCNTINC();
 
    gbLowPWRFlag = 0;
    g_Flag.Run &= ~F_PWRUP;					//AC MODE
	IEN1 = (BIT7 | BIT3);    				//开启LPD中断、RTC中断	
	 
	EA =1;                                  //总中断打开    



//TEST  
/*
	KWH_PN[0] = 0XA5;
	KWH_PN[1] = 0XA5;
	KWH_PN[2] = 0XA5;
	KWH_PN[3] = 0XA5;
    VER_WRbytes(LMON_EC, &KWH_PN[0], 4, 1);    	
	KWH_PN[0] = 0X00;
	KWH_PN[1] = 0X00;
	KWH_PN[2] = 0X00;
	KWH_PN[3] = 0X00;
    VER_RDbytes(LMON_EC, &KWH_PN[0],  4);   		
	_nop_();
	_nop_();

*/
//TEST  

	while (1)
	{
		if (gbLowPWRFlag == 0) 		  			//交流电压模式, gbLowPWRFlag = 0 为交流电压模式，gbLowPWRFlag = 1 为电池模式
		{      
            if((g_Flag.Run & F_PWRUP) == 0)		//首次上电 
            {
                ACModeWDTProc();				//AC MODE WDT PROC
                Change32KToPLL();				//切32 TO PLL
				Init_IO();						//IO口初始化
				Init_RTC();						//RTC初始化
				Init_ADC();						//ADC初始化
                PWONInit_LCD();					//LCD初始化
				Init_RAM();						//RAM初始化
				g_Flag.Run |= F_PWRUP;			//置上电的标志
				Init_Timer();					//TIME初始化
				Init_Uart();					//UART初始化
                ReadRTCProc();					//读RTC处理
                CheckRTC();						//RTC校验处理

                if((LPDCON & 0x40) == 0x40)		//VIN>1.2V
                {
                    Init_EMU();					//初始化EMU

                    PwrCnt = 0x00;                   
                    while(PwrCnt != 0x0A);		//For delay 250ms

                    g_AllDisDly = C_AllDispDly;	//置全屏显示标志
                }
                else							//VIN<1.2V
                {
                    g_Flag.Run &= ~F_PWRUP;		//置下电的标志
                }
                CheckPowerOff();				//检测下电处理
            }
            else
            {             
                ACModeWDTProc();				//AC MODE WDT PROC
                EA = 1;

                BatSupplySwitch();				//开启LPD检测、供电自动切换、LPD中断 					
				  
                TimeTaskThread();        		//年，月，日，小时，分钟，秒的相关时间任务线程处理               
				DisplayProc();       			//LCD显示处理
         
                ACInit_IO();					//IO SFR刷新
                ACInit_RTC();					//RTC SFR刷新
                ACInit_LCD();					//LCD SFR刷新
                ACInit_Timer();             	//Timer SFR刷新
                  
                EMUTampProc();			  		//每秒的瞬时量读取，计量通道选择，事件判断
                EnergyProc();					//电量处理

                Change32KToPLL();				//AC下监测 PLL
                ACInit_EMUPluse();				//AC 刷新EMU

				PIMAAckProc();					//PIMA码输出处理
					                   
	            CheckPowerOff();		        //掉电检测			 
            } 			
		}
		else									//电池模式, gbLowPWRFlag = 0 为交流电压模式，gbLowPWRFlag = 1 为电池模式
		{
            PDModeWDTProc();					//BAT MODE WDT PROC,看门狗复位  4096ms
			BatteryInitMode(); 				    //低功耗处理

			BatModeThreadProc();  	            //掉电流程中的秒任务
			ReadRTCProc();				  		//读RTC处理
//            g_Flag.Run |= F_DISP;				//置一秒钟刷新LCD屏的标志
//			  DisplayProc();					//掉电显示处理
			BatSupplySwitch();       			//开启LPD检测、供电自动切换、LPD中断
			ChangePLLTo32K();				    //PLL TO 32K处理

			RTCCON = 0x00;			 			//关闭RTC内部定时器，CALOUT引脚作为普通IO
            RTCIE = 0x08;            			//开启RTC秒中断，关闭RTC内部定时器中断、闹铃0和1中断、RTC分钟和小数和天中断
            IEN0 = 0;                			//关闭总中断，ADC和TPS中断，定时器0、1、2中断，外部中断0、1，串口0中断
            IEN1 = 0;                			//关闭LPD中断，串口1、2中断，外部中断2、3，PWM中断，EMU中断  
			IEN1 = (BIT7 | BIT3);    			//开启LPD中断、RTC中断

            g_Flag.Run &= ~F_PWRUP;				//置下电的标志
            PwrCnt = 0x00;

            EA = 1;                  			//开启总中断

			SUSLO = 0x55;
			PCON = 0x01;             			//进入空闲模式
			_nop_();
			_nop_();
			_nop_();
			_nop_();
			_nop_();

			CheckPowerOn();         			//检测上电处理
	 	}
	}
}
