/*==============================================================================
                Copyright(C) 1997-2019.  Sinowealth Tech. Co., Ltd.
--------------------------------------------------------------------------------
Project Name  : RTCEVENTTHREADPROC.c
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
#ifndef  RTCEVENTTHREADPROC_C
	#define  RTCEVENTTHREADPROC_C
#endif
//-------------------------------------------------------------------------------

//---------------


#include "config.h"
#include "Headconfig.h"






////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: BatModeThreadProc
** 函数描述: 掉电流程中的秒任务,一秒钟执行一次
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
/* void BatModeThreadProc(void)
{
/*
	  if (g_Disp.PollingDisplayCount != 0)
	  {
	      g_Disp.PollingDisplayCount--;
	  }
	  if (g_AllDisDly != 0)
	  {
	      g_AllDisDly--;
	  }

	  if (g_Disp.ShutDisplayCountSet != 0)
	  {
	      if (g_Disp.ShutDisplayCount < g_Disp.ShutDisplayCountSet)
	      {
	         g_Disp.ShutDisplayCount++;
	      }
	      else
	      {
	         PWOFFInit_LCD();
	      }
	  }
*/
/* 	  PWOFFInit_LCD();
}  */



////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: CheckRTCBCD
** 函数描述: RTC十进制转BCD码检查。正确返回0，错误返回1
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
unsigned char CheckRTCBCD(unsigned char xdata *ClkByte)
{
    unsigned char data Tmp;
    unsigned char xdata Rlt;
    Rlt = 0;

    Tmp = *ClkByte & 0x0f;
    if(Tmp > 0x09)
    {
        Rlt = 1;
    }
    Tmp = ((*ClkByte) >> 4) & 0x0f;
    if(Tmp > 0x09)
    {
        Rlt = 1;
    }
    return Rlt;
}


////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: ClkIsBad
** 函数描述: RTC寄存器数值检查。正确返回0.错误返回1
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
unsigned char ClkIsBad(unsigned char xdata *ClkBuf)
{
    unsigned char xdata Rlt = 0;
    unsigned char data i;

    if (ClkBuf[0] >0x59 || ClkBuf[1]>0x59 || ClkBuf[2]>0x23 || ClkBuf[3]>0x31 || ClkBuf[4]>0x12 || ClkBuf[5]>0x99 || ClkBuf[6] >0x06 ||
        ClkBuf[3]==0x00 || ClkBuf[4]==0x00)
        Rlt = 0x01;

    for(i = 0; i < 7; i++)
    {
        if(CheckRTCBCD(&ClkBuf[i]))
        {
            Rlt = 0x01;
            break;
        }
    }
    return Rlt;
}


////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: CheckRTC
** 函数描述:
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
void CheckRTC(void)
{
	GetRealTime();
	if (ClkIsBad(&_UINT8_RTC[0]) == 0x01)
	{
		_UINT8_RTC[0] = 0x00;
		_UINT8_RTC[1] = 0x00;
		_UINT8_RTC[2] = 0x12;
		_UINT8_RTC[3] = 0x01;
		_UINT8_RTC[4] = 0x01;
		_UINT8_RTC[5] = 0x18;
		_UINT8_RTC[6] = DataToWeek(&_UINT8_RTC[3]);
		SetRealTime();
		//g_Flag.Run |= F_CLR;
	}

    //memcpy(&g_Clk.Sec, &_UINT8_RTC[0], 7);
}


////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: RTC_ReadClock
** 函数描述:
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
void RTC_ReadClock(unsigned char xdata * ptr)
{
    RTCCON |= 0x80;              //置位RTCRD位，将最新时间日历更新到对应寄存器，并停止更新
    Delay_us(50);                //RTCRD写1后不会立即变为1，有不超过32us的延时

    *(ptr++) = SEC&0x7F;
    *(ptr++) = MIN&0x7F;
    *(ptr++) = HR&0x3F;
    *(ptr++) = DAY&0x3F;
    *(ptr++) = MTH&0x1F;
    *(ptr++) = YR;
    *(ptr++) = DOW&0x07;         //week

    RTCCON &= ~0x80;             //RTCRD位清零，否则时间日历寄存器将一直保持上次读取的值
}


////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: ReadRTCProc
** 函数描述: 读RTC时间，值储存在数组：_UINT8_RTC 中。
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
void ReadRTCProc(void)
{
    GetRealTime();
//    if(gbLowPWRFlag == 0)
//    {
//        g_PwrDnHour = _UINT8_RTC[2];
//        g_PwrDnYear = _UINT8_RTC[5];
//    }
}


////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: RTC_ClockThread
** 函数描述:
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
void RTC_ClockThread(void)
{
	unsigned char data i;

	do
	{
		RTC_ReadClock(&g_Buffer[0]);
        _nop_();
		_nop_();
        RTC_ReadClock(&g_Buffer[7]);
	}
	while (memcmp(&g_Buffer[0], &g_Buffer[7], 7) != 0);

	if (g_Buffer[0] != g_Clk.Sec)
    {
        g_Flag.Clk |= C_SEC;
    }
	if (g_Buffer[1] != g_Clk.Min)
	{
		g_Flag.Clk |= C_MIN;
	}
	if (g_Buffer[2] != g_Clk.Hour)
    {
        g_Flag.Clk |= C_HOUR;
    }
	if (g_Buffer[3] != g_Clk.Day)
    {
        g_Flag.Clk |= C_DAY;
    }
    if(g_Buffer[5] != g_Clk.Year)
    {
        g_Flag.Clk |= C_YEAR;
    }

	if((g_Flag.Run & F_CLR) == F_CLR)
    {
        g_Flag.Run &= ~F_CLR;
        g_Flag.Clk &= ~(C_HOUR | C_YEAR | C_MIN | C_DAY);
    }

	for (i=0; i<7; i++)
	{
		*(&g_Clk.Sec+i) = g_Buffer[i];  //将读取到的时间载入buffer中
	}
}



////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: SaveHistorys
** 函数描述:
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
// void SaveHistorys(void)
// {
//     ECRunKwh();
// //    ECRunQKwh();
// //    ECRunQKwh_RQKwh();
//     VER_WRbytes(LMON_EC, &KWH_PN[0],4,1);       //数据转存
// //    VER_RDbytes(LMON_EC, &KWH_LMON[0],4);   	//历史数据刷新
// }



////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: PwrDn_Bill
** 函数描述:
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
/*
void PwrDn_Bill(void)
{
    unsigned int xdata CHour,JHour;

    if (g_PwrDnHour != _UINT8_RTC[2])
    {
        //Hour Flg
        Change32KToPLL();
        CHour = BCD2HEX(_UINT8_RTC[3])*24 + BCD2HEX(_UINT8_RTC[2]);
        EE_to_RAM(EE_SAM_ENDDATE, &g_Buffer[0],2);
        JHour = BCD2HEX( g_Buffer[0] )*24 + BCD2HEX( g_Buffer[1] );
        if (CHour == JHour)
        {
            VER_RDbytes(PwrDnBill, &g_Buffer[0], 1);
            if (g_Buffer[0] != 0xA5)
            {
                g_Buffer[0] = 0xA5;
                VER_WRbytes(PwrDnBill, &g_Buffer[0], 1, 1);
            }
        }
        ChangePLLTo32K();
    }
    if(g_PwrDnYear != _UINT8_RTC[5])
    {
        Change32KToPLL();
		g_Buffer[0] = 0x00;
        if((g_Flag.Function & 0x0001) == 0x0001)
        {
            g_Buffer[0] = 0xA5;
            VER_WRbytes(DemandBillFlg, (unsigned char *)&g_Buffer[0], 1, 1);
        }
        ChangePLLTo32K();
    }
    g_PwrDnHour = _UINT8_RTC[2];
    g_PwrDnYear = _UINT8_RTC[5];
}
*/


////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: AP_Bill
** 函数描述:
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
/*
void AP_Bill(unsigned char Mode)
{
    unsigned int data CHour,JHour;

    switch(Mode)
    {
        case PwrUpBill:
        {
            VER_RDbytes(PwrDnBill, &g_Buffer[0], 1);
            if(g_Buffer[0] == 0xA5)
            {
                SaveHistorys();     							//当月转存到上一月，再将上一月数据更新到LCD中
                //AP_ClearEvent();
                Demand_Bill();
                g_Buffer[0] = 0x00;
                VER_WRbytes(PwrDnBill,&g_Buffer[0],1,1);
                VER_WRbytes(CMon_Time,&_UINT8_RTC[0],6,1);
            }
            break;
        }

        case NorBill:
        {
            CHour = BCD2HEX(_UINT8_RTC[3])*24 + BCD2HEX(_UINT8_RTC[2]);
            VER_RDbytes(EE_SAM_ENDDATE, &g_Buffer[0],2);
            JHour = BCD2HEX( g_Buffer[0] )*24 + BCD2HEX( g_Buffer[1] );

            if(CHour == JHour)
            {
                SaveHistorys();		                  			//电能结算
                //AP_ClearEvent();								//事件标志复位
                Demand_Bill();	                				//需量结算
                VER_WRbytes(CMon_Time,&_UINT8_RTC[0],6,1);    	//结算时间写入
            }
            break;
        }

        case ManuBill:
        {
            SaveHistorys();	            						//电能结算
            //AP_ClearEvent();            					//事件标志复位
            Demand_Bill();	            						//需量结算
            VER_WRbytes(CMon_Time,&_UINT8_RTC[0],6,1);          //结算时间写入
            break;
        }

        default:
        {
            break;
        }
    }
}
*/


////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: CreepThread
** 函数描述:
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
void CreepThread()							//潜动处理
{
    if (++gbPCNTCount > C_StartCreepT)    	//潜动时间	 //6818s        113.63min    0x1AA2
	{
		gbPCNTCount=0;

		EADR=0x00;
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		EDTAH=0x00;
		EDTAM=0x00;
		EDTAL=0x00;
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		EADR=PCNT|0x80;
	}
}


////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: AP_ClearEvent
** 函数描述: 事件清零
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
// void AP_ClearEvent(void)
// {
//     g_Flag.ALARM[0] = 0x00;
//     g_Flag.EventFlg = 0x00;

//     VER_WRbytes(EventFlag, (unsigned char *)&g_Flag.EventFlg, 1, 1);

//     g_EventDly.ByPassDly = 0;
//     g_EventDly.RevDly = 0;
// }


////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: AP_ClearEnergy
** 函数描述: 电能清零
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
//void AP_ClearEnergy(unsigned char ClearKey)
//{
//    unsigned char xdata Buffer[6];

//    if(ClearKey != D_ClearEnergyPSW)
//    {
//        return;
//    }

//    //RAM
//    //pluse clear
//    KWHP  = 0;
//    RKWHP = 0;
//    //decimal clear
//    KWH_PN[0] = 0;
//    KWH_PN[1] = 0;
//    KWH_PN[2] = 0;
//    KWH_PN[3] = 0;
//    //decimal backup clear
//    KWHD1 =0;
//    KWHD2 =0;
//    KWHD3 =0;
//    RKWHD1=0;
//    RKWHD2=0;
//    RKWHD3=0;
//    //forward decimal clear
//    KWHD = 0;
//    kwh_value.integer[0] = 0;
//    kwh_value.integer[1] = 0;
//    kwh_value.integer[2] = 0;
//    //reverse decimal clear
//    RKWHD = 0;
//    RKWH[0] = 0;
//    RKWH[1] = 0;
//    RKWH[2] = 0;

//    //pluse clear
//    QKWHP  = 0;
//    RQKWHP = 0;
//    //decimal clear
//    QKWH_PN[0] = 0;
//    QKWH_PN[1] = 0;
//    QKWH_PN[2] = 0;
//    QKWH_PN[3] = 0;
//    //decimal backup clear
//    QKWHD1 =0;
//    QKWHD2 =0;
//    QKWHD3 =0;
//    RQKWHD1=0;
//    RQKWHD2=0;
//    RQKWHD3=0;
//    //forward decimal clear
//    QKWHD = 0;
//    QKWH[0] = 0;
//    QKWH[1] = 0;
//    QKWH[2] = 0;
//    //reverse decimal clear
//    RQKWHD = 0;
//    RQKWH[0] = 0;
//    RQKWH[1] = 0;
//    RQKWH[2] = 0;

//	//EEPROM
//    MemInitSet(&Buffer[0], 0x00, 6);

//    //pluse clear
//    if(ClearKey == D_ClearEnergyPSW)
//    {
//        //SEQ_write(EE_KWHP, &Buffer[0],4);
//        VER_WRbytes(EE_KWHP, &Buffer[0],2,1);//正向有功脉冲小数电量
//        VER_WRbytes(EE_RKWHP, &Buffer[0],2,1);//反向有功脉冲小数电量

//        VER_WRbytes(EE_QKWHP, &Buffer[0],2,1);//正向无功脉冲小数电量
//        VER_WRbytes(EE_RQKWHP, &Buffer[0],2,1);//反向无功脉冲小数电量
//    }

//    //forward energy clear
//    if(ClearKey == D_ClearEnergyPSW)
//    {
//        VER_WRbytes(EE_KWH0, &Buffer[0],3,1);
//    }
//    //reverse energy clear
//    /* if(ClearKey == D_ClearEnergyPSW)
//    {
//        VER_WRbytes(EE_RKWH0, &Buffer[0], 3, 1);
//    }
// */
//    //forward energy clear
//   /*  if(ClearKey == D_ClearEnergyPSW)
//    {
//        VER_WRbytes(EE_QKWH0, &Buffer[0],3,1);
//    } */
//    //reverse energy clear
//  /*   if(ClearKey == D_ClearEnergyPSW)
//    {
//        VER_WRbytes(EE_RQKWH0, &Buffer[0], 3, 1);
//    } */

//    //Set Bill time by current time
//    VER_WRbytes(CMon_Time, &_UINT8_RTC[0], 6,1);
//	SaveHistorys();
//}


////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: AP_ClearAllData
** 函数描述: 总清零
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
//void AP_ClearAllData(unsigned char Cmd)
//{
////    MDRst(3);
//    AP_ClearEvent();
//    AP_ClearEnergy(Cmd);

////    MemInitSet(&g_Buffer[0], 0x00, 6);
////    VER_WRbytes(PwrDnBill, &g_Buffer[0], 1, 1);
////    VER_WRbytes(DemandBillFlg, (unsigned char *)&g_Buffer[0], 1, 1);
//}


////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: JumpMeterClsZeroThread
** 函数描述: 跳线电量清零线程处理
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
/* void JumpMeterClsZeroThread(void)		 //跳线电量清零线程处理
{
	//CP当清零键有效是，
	if(g_Cal.ClsKey == K_Cal)
	{
	    if(g_Cal.ClsKeyFlag  == D_InitStart)
	    {
	        g_Cal.ClsKeyFlag = D_Initing;
	    }
	    else if(g_Cal.ClsKeyFlag  == D_Initing)
	    {
	        g_Cal.ClsKeyFlag = D_InitAdjust;
			if(((gBClsKeyEnable == 0xA5) && (gBClsKeyEnable1 == 0x55)) && ((KEY_CLS() == 0)) && (g_Cal.ClsKeyFlag == D_InitAdjust))
			{
	        	//AP_ClearAllData(D_ClearEnergyPSW);
			    gBClsKeyCount = 0;
			    gBClsKeyEnable = 0x00;
			    gBClsKeyEnable1 = 0x00;

				g_Cal.ClsKey = 0x00;
				g_Cal.ClsKeyFlag = 0x00;
				gbFgClsKeyProg = 0;
			}
	    }
	    else if(g_Cal.ClsKeyFlag  == D_InitEnd)				//结束校表程序
	    {
		    gBClsKeyCount = 0;
		    gBClsKeyEnable = 0x00;
		    gBClsKeyEnable1 = 0x00;

			g_Cal.ClsKey = 0x00;
			g_Cal.ClsKeyFlag = 0x00;
			gbFgClsKeyProg = 0;
	    }
		else
		{
		    gBClsKeyCount = 0;
		    gBClsKeyEnable = 0x00;
		    gBClsKeyEnable1 = 0x00;

			g_Cal.ClsKey = 0x00;
			g_Cal.ClsKeyFlag = 0x00;
			gbFgClsKeyProg = 0;
		}
	}
	else
	{
	    if((LPDCON & Bin(01000000)) == Bin(01000000)) 		//VIN > 1.2V,it is ic AC MODE
	    {
		    if(1 == JudgeVoltugeRange())//检测VRMS输入电压是否超过偏离值
			{

                if(g_Cal.ClsKeyStep != 0x01)
                {
                    gBClsKeyEnable = 0xA5;
                    gBClsKeyEnable1 = 0x55;
                    g_Cal.ClsKeyStep = 0x01;		//若短接一直短接，置一次有效标志
                    gbFgClsKeyProg = 0xF002;
                }

			}

				if(((gBClsKeyEnable == 0xA5) && (gBClsKeyEnable1 == 0x55))
				{
					if(InFactoryClsMode())        	  			//工厂未使能
					{
					    g_Cal.ClsKeyFlag = D_InitStart;		  	//启动校表初始化程序
					    g_Cal.ClsKey = K_Cal;
					}
					else
					{
					    g_Cal.ClsKey = 0x00;
					    g_Cal.ClsKeyFlag = 0x00;
					    gbFgClsKeyProg = 0;
					}
				}
				else
				{
				    g_Cal.ClsKey = 0x00;
				    g_Cal.ClsKeyFlag = 0x00;
					gbFgClsKeyProg = 0;
				}
			}
		}
	}
} */


////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: AP_RSTCNTINC
** 函数描述:
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
// void AP_RSTCNTINC(void)
// {
//     unsigned char xdata Tmp;

//     if (0 == VER_RDbytes(CNT_RST, &Tmp, 1))
//     {
//         Tmp = 0;
//     }

//     Tmp = BCD2HEX(Tmp);
//     if (Tmp < 100)
//         Tmp++;
//     Tmp = HEX2BCD(Tmp);

//     VER_WRbytes(CNT_RST, &Tmp, 1, 1);
// }


////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: BatteryVolThread
** 函数描述:
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
#if BatCheckEn == ENABLE
void BatteryVolThread(void)
{
	if (GetVbat() > 0xA8C)    				  //0xA8C = 2.7V,测电池电压
	{
	    gbbattery=0;
	}
	else
	{
	    gbbattery=1;
	}
}
#endif




////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: TimeTaskThread
** 函数描述: 每秒钟执行一次
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
void TimeTaskThread()
{
    //unsigned char xdata day_shift;
//    unsigned char xdata DayBuff[4];
    //unsigned char xdata LDayBuff[4];
//    unsigned char temp1;
//    unsigned char temp2;
    if(g_ClkSec == 0)
	{
		return;
	}
	g_ClkSec = 0;

//RTC时钟标志线程
    RTC_ClockThread();

//秒到线程处理
    if (g_Flag.Clk & C_SEC)
	{
		g_Flag.Clk &= ~C_SEC;

        //继电器相关进程
        if(DelayFlag > 1)//响应继电器断电5s命令
        {
            DelayFlag++;
			//RealyCtrl(1);//断电
            if(DelayFlag == 6)
            {
                RealyCtrl(0);//通电
            }
        }
        //判断安装模块
        if(g_Tran.AutoReportCount != 0)
        {
            //判断安装的模块
            if((NB_LORA[0] == 0x00)&&(NB_LORA[1] == 0x00))
            {
                g_Tran.AutoReportCount --;
                if( g_Tran.AutoReportCount == 1)
                {
                    NB_LORA_PANDUAN(&NB_LORA[0]);
									NB_Error();
                    VER_WRbytes(EE_NB_LORA, &NB_LORA[0], 2, 1);
//					if((NB_LORA[0] == 0xBB)&&(NB_LORA[1] == 0xBB))
//						NB_Error();
                }
            }
        }
         //如果安装NB模块，启动NB初始化
        if((NB_LORA[0] == 0xBB)&&(NB_LORA[1] == 0xBB))
        {
            if(g_NB.ReInitTime != 0)
            {
                g_NB.ReInitTime --;
                if((g_NB.ReInitTime  == 0x00))//超过初始化时间，重新进行初始化
                {
                    if((g_NB.InitState[0] == NB_Init_Reset) && (g_NB.InitState[1] == NB_Init_Reset))
                    {
                        UART0_SendString("AT+QRST=1\r\n",11);
                        g_NB.InitStep = 1;
                        g_NB.ReInitTime = 10;
                        g_NB.InitState[0] = 0x00;
                        g_NB.InitState[1] = 0x00;
                        g_NB.ReInitTime = 0;
                    }
                    else
                        NB_Init();
                }
            }
        }

         //显示相关进程
        if (g_Disp.PollingDisplayCount != 0)//显示相关，轮显倒计时
        {
            g_Disp.PollingDisplayCount--;
        }

        if (g_AllDisDly != 0)   //显示相关，全屏显示倒计时
        {
            g_AllDisDly--;
        }
        g_Flag.Run |= F_DISP;//显示相关，置显示标志

        ECRunKwh();     				//电能显示刷新，计算组合有功

        CreepThread();				  	 //潜动处理线程

		PowerAdjThread();				 //校表线程处理

	}

//分钟到线程处理
    if(g_Flag.Clk & C_MIN)
    {
        g_Flag.Clk &= ~C_MIN;

        //自动上报进程
        if(g_Tran.AutoReportCount != 0)
        {
            //如果安装了NB模块，等待初始化结束启动上报进程
            if((NB_LORA[0] == 0xBB)&&(NB_LORA[1] == 0xBB))
            {
                if((g_NB.InitState[0] == NB_Init_OK)&&(g_NB.InitState[1] == NB_Init_OK))
                {
                    //检查NB模块硬件状态
                    // NB_LORA_PANDUAN(&NB_LORA[0]);
                     //通讯相关进程，超过3次后台无ACK回应，显示错误

                     g_Tran.AutoReportCount --;
                }
            }
            //安装LORA模块，为透传模式，直接发送
            else if((NB_LORA[0] == 0xAA)&&(NB_LORA[1] == 0xAA))
            {
                g_Tran.AutoReportCount --;
            }
            //判断ACK状态
            if(g_Tran.AutoReportCount == 0)
            {
                 if(g_Tran.AutoReportTime >3)
                    {
                        UART0_SendString("AT+CFUN=0\r\n",11);//关闭NB模块
                        g_NB.InitCount = 0x00;
                        g_NB.ReInitTime = NB_RETRY_TIME;
                        g_Tran.AutoReportTime = 0;
                        g_NB.InitStep = 1;
                        g_NB.InitState[0] = NB_Init_Reset;
                        g_NB.InitState[1] = NB_Init_Reset;
                        g_Flag.ALARM[0] = 0x0a;
                        g_Flag.ALARM[1] = 0x00;
                        g_Tran.AutoReportFlag = NO_ACK_RD;
                    }
                if(g_Tran.AutoReportFlag == WAITING_ACK)
                {
                    g_NB.InitStep = 1;
                    g_NB.ReInitTime = 2;
                    g_NB.InitState[0] = 0x00;
                    g_NB.InitState[1] = 0x00;

                    g_Tran.AutoReportFlag = COUNT_DOWN;
                }
                else if(g_Tran.AutoReportFlag == COUNT_DOWN)
                {
                    g_Tran.AutoReportFlag = REPORT_TIME;
                }
            }
        }

        //检查模块安装状态
//       NB_LORA_PANDUAN(&NB_LORA[0]);
//	    SEQ_write(EE_NB_LORA, &NB_LORA[0], 2);
				//VER_RDbytes(EE_NB_LORA, &NB_LORA[0], 2);
        //继电器状态
        VER_RDbytes(RELAY_STATUS, &DelayStatus[0], 2);
        //NB初始化状态
        VER_RDbytes(EE_NB_STATE, &g_NB.InitState[0], 2);
        if((DelayStatus[0] != 0xA5)&&(DelayStatus[1] != 0xA5))
        {
            DelayFlag = 1;//继电器断电
        }
        else
        {
            DelayFlag = 0;//继电器通电
        }

        VER_RDbytes(EE_Meter_address, &param_data.meter_address[0], 6);//通讯地址
        EE_to_RAM(EE_Meter_Factory, &param_data.meter_factory[0],1);   //厂商代码
        VER_RDbytes((unsigned int)EE_KHH_address , &kwh_value.integer[0], 5);         //有功正向

        KWH_RKWH_dot();                            //小数位校验
        ECRunKwh();                                //60秒刷新组合有功

        if(ConstPara_Chk() != g_ConstPara.Chk)		//脉冲常数校验RAM
        {
            meter_const = DefaultThreshold;
            ConstPara_Init(meter_const, 0x00);
        }
        gbFgShowItemNum = D_ShowItemNum;		//显示项目次数校验RAM
    }

//小时到线程处理
    if(g_Flag.Clk & C_HOUR)
    {
        g_Flag.Clk &= ~C_HOUR;
//        if((NB_LORA[0] == 0xBB)&&(NB_LORA[1] == 0xBB))
//        {
//         if((g_NB.InitState[0] == NB_Init_OK)&&(g_NB.InitState[1] == NB_Init_OK))
//            NB_Error();
//        }

//        AP_Bill(NorBill);							//电量结算

        EMU_Check();								//检查EMU中校表参数是否与EEPROM中相符
        VER_CHK((unsigned int)EE_KHH_address, 5);                		//查有功正向电量备份
        VER_CHK(EE_Meter_address,6);                //检查通信地址备份
        // VER_CHK(EE_RKWH0, 3);               		//查有功反向电量备份

        // VER_CHK(EE_QKWH0, 3);                		//查无功正向电量备份
        // VER_CHK(EE_RQKWH0, 3);               		//查无功反向电量备份
    }

//天到线程处理
    if (g_Flag.Clk & C_DAY)
    {
        g_Flag.Clk &= ~C_DAY;

    }

//年到线程处理
    if(g_Flag.Clk & C_YEAR)
    {
        g_Flag.Clk &= ~C_YEAR;
    }
}

