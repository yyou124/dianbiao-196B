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
** ��������: BatModeThreadProc
** ��������: ���������е�������,һ����ִ��һ��
** �������: ��
** �������: ��
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
** ��������: CheckRTCBCD
** ��������: RTCʮ����תBCD���顣��ȷ����0�����󷵻�1
** �������: ��
** �������: ��
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
** ��������: ClkIsBad
** ��������: RTC�Ĵ�����ֵ��顣��ȷ����0.���󷵻�1
** �������: ��
** �������: ��
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
** ��������: CheckRTC
** ��������:
** �������: ��
** �������: ��
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
** ��������: RTC_ReadClock
** ��������:
** �������: ��
** �������: ��
*******************************************************************************************/
void RTC_ReadClock(unsigned char xdata * ptr)
{
    RTCCON |= 0x80;              //��λRTCRDλ��������ʱ���������µ���Ӧ�Ĵ�������ֹͣ����
    Delay_us(50);                //RTCRDд1�󲻻�������Ϊ1���в�����32us����ʱ

    *(ptr++) = SEC&0x7F;
    *(ptr++) = MIN&0x7F;
    *(ptr++) = HR&0x3F;
    *(ptr++) = DAY&0x3F;
    *(ptr++) = MTH&0x1F;
    *(ptr++) = YR;
    *(ptr++) = DOW&0x07;         //week

    RTCCON &= ~0x80;             //RTCRDλ���㣬����ʱ�������Ĵ�����һֱ�����ϴζ�ȡ��ֵ
}


////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** ��������: ReadRTCProc
** ��������: ��RTCʱ�䣬ֵ���������飺_UINT8_RTC �С�
** �������: ��
** �������: ��
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
** ��������: RTC_ClockThread
** ��������:
** �������: ��
** �������: ��
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
		*(&g_Clk.Sec+i) = g_Buffer[i];  //����ȡ����ʱ������buffer��
	}
}



////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** ��������: SaveHistorys
** ��������:
** �������: ��
** �������: ��
*******************************************************************************************/
// void SaveHistorys(void)
// {
//     ECRunKwh();
// //    ECRunQKwh();
// //    ECRunQKwh_RQKwh();
//     VER_WRbytes(LMON_EC, &KWH_PN[0],4,1);       //����ת��
// //    VER_RDbytes(LMON_EC, &KWH_LMON[0],4);   	//��ʷ����ˢ��
// }



////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** ��������: PwrDn_Bill
** ��������:
** �������: ��
** �������: ��
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
** ��������: AP_Bill
** ��������:
** �������: ��
** �������: ��
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
                SaveHistorys();     							//����ת�浽��һ�£��ٽ���һ�����ݸ��µ�LCD��
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
                SaveHistorys();		                  			//���ܽ���
                //AP_ClearEvent();								//�¼���־��λ
                Demand_Bill();	                				//��������
                VER_WRbytes(CMon_Time,&_UINT8_RTC[0],6,1);    	//����ʱ��д��
            }
            break;
        }

        case ManuBill:
        {
            SaveHistorys();	            						//���ܽ���
            //AP_ClearEvent();            					//�¼���־��λ
            Demand_Bill();	            						//��������
            VER_WRbytes(CMon_Time,&_UINT8_RTC[0],6,1);          //����ʱ��д��
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
** ��������: CreepThread
** ��������:
** �������: ��
** �������: ��
*******************************************************************************************/
void CreepThread()							//Ǳ������
{
    if (++gbPCNTCount > C_StartCreepT)    	//Ǳ��ʱ��	 //6818s        113.63min    0x1AA2
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
** ��������: AP_ClearEvent
** ��������: �¼�����
** �������: ��
** �������: ��
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
** ��������: AP_ClearEnergy
** ��������: ��������
** �������: ��
** �������: ��
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
//        VER_WRbytes(EE_KWHP, &Buffer[0],2,1);//�����й�����С������
//        VER_WRbytes(EE_RKWHP, &Buffer[0],2,1);//�����й�����С������

//        VER_WRbytes(EE_QKWHP, &Buffer[0],2,1);//�����޹�����С������
//        VER_WRbytes(EE_RQKWHP, &Buffer[0],2,1);//�����޹�����С������
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
** ��������: AP_ClearAllData
** ��������: ������
** �������: ��
** �������: ��
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
** ��������: JumpMeterClsZeroThread
** ��������: ���ߵ��������̴߳���
** �������: ��
** �������: ��
*******************************************************************************************/
/* void JumpMeterClsZeroThread(void)		 //���ߵ��������̴߳���
{
	//CP���������Ч�ǣ�
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
	    else if(g_Cal.ClsKeyFlag  == D_InitEnd)				//����У�����
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
		    if(1 == JudgeVoltugeRange())//���VRMS�����ѹ�Ƿ񳬹�ƫ��ֵ
			{

                if(g_Cal.ClsKeyStep != 0x01)
                {
                    gBClsKeyEnable = 0xA5;
                    gBClsKeyEnable1 = 0x55;
                    g_Cal.ClsKeyStep = 0x01;		//���̽�һֱ�̽ӣ���һ����Ч��־
                    gbFgClsKeyProg = 0xF002;
                }

			}

				if(((gBClsKeyEnable == 0xA5) && (gBClsKeyEnable1 == 0x55))
				{
					if(InFactoryClsMode())        	  			//����δʹ��
					{
					    g_Cal.ClsKeyFlag = D_InitStart;		  	//����У���ʼ������
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
** ��������: AP_RSTCNTINC
** ��������:
** �������: ��
** �������: ��
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
** ��������: BatteryVolThread
** ��������:
** �������: ��
** �������: ��
*******************************************************************************************/
#if BatCheckEn == ENABLE
void BatteryVolThread(void)
{
	if (GetVbat() > 0xA8C)    				  //0xA8C = 2.7V,���ص�ѹ
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
** ��������: TimeTaskThread
** ��������: ÿ����ִ��һ��
** �������: ��
** �������: ��
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

//RTCʱ�ӱ�־�߳�
    RTC_ClockThread();

//�뵽�̴߳���
    if (g_Flag.Clk & C_SEC)
	{
		g_Flag.Clk &= ~C_SEC;

        //�̵�����ؽ���
        if(DelayFlag > 1)//��Ӧ�̵����ϵ�5s����
        {
            DelayFlag++;
			//RealyCtrl(1);//�ϵ�
            if(DelayFlag == 6)
            {
                RealyCtrl(0);//ͨ��
            }
        }
        //�жϰ�װģ��
        if(g_Tran.AutoReportCount != 0)
        {
            //�жϰ�װ��ģ��
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
         //�����װNBģ�飬����NB��ʼ��
        if((NB_LORA[0] == 0xBB)&&(NB_LORA[1] == 0xBB))
        {
            if(g_NB.ReInitTime != 0)
            {
                g_NB.ReInitTime --;
                if((g_NB.ReInitTime  == 0x00))//������ʼ��ʱ�䣬���½��г�ʼ��
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

         //��ʾ��ؽ���
        if (g_Disp.PollingDisplayCount != 0)//��ʾ��أ����Ե���ʱ
        {
            g_Disp.PollingDisplayCount--;
        }

        if (g_AllDisDly != 0)   //��ʾ��أ�ȫ����ʾ����ʱ
        {
            g_AllDisDly--;
        }
        g_Flag.Run |= F_DISP;//��ʾ��أ�����ʾ��־

        ECRunKwh();     				//������ʾˢ�£���������й�

        CreepThread();				  	 //Ǳ�������߳�

		PowerAdjThread();				 //У���̴߳���

	}

//���ӵ��̴߳���
    if(g_Flag.Clk & C_MIN)
    {
        g_Flag.Clk &= ~C_MIN;

        //�Զ��ϱ�����
        if(g_Tran.AutoReportCount != 0)
        {
            //�����װ��NBģ�飬�ȴ���ʼ�����������ϱ�����
            if((NB_LORA[0] == 0xBB)&&(NB_LORA[1] == 0xBB))
            {
                if((g_NB.InitState[0] == NB_Init_OK)&&(g_NB.InitState[1] == NB_Init_OK))
                {
                    //���NBģ��Ӳ��״̬
                    // NB_LORA_PANDUAN(&NB_LORA[0]);
                     //ͨѶ��ؽ��̣�����3�κ�̨��ACK��Ӧ����ʾ����

                     g_Tran.AutoReportCount --;
                }
            }
            //��װLORAģ�飬Ϊ͸��ģʽ��ֱ�ӷ���
            else if((NB_LORA[0] == 0xAA)&&(NB_LORA[1] == 0xAA))
            {
                g_Tran.AutoReportCount --;
            }
            //�ж�ACK״̬
            if(g_Tran.AutoReportCount == 0)
            {
                 if(g_Tran.AutoReportTime >3)
                    {
                        UART0_SendString("AT+CFUN=0\r\n",11);//�ر�NBģ��
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

        //���ģ�鰲װ״̬
//       NB_LORA_PANDUAN(&NB_LORA[0]);
//	    SEQ_write(EE_NB_LORA, &NB_LORA[0], 2);
				//VER_RDbytes(EE_NB_LORA, &NB_LORA[0], 2);
        //�̵���״̬
        VER_RDbytes(RELAY_STATUS, &DelayStatus[0], 2);
        //NB��ʼ��״̬
        VER_RDbytes(EE_NB_STATE, &g_NB.InitState[0], 2);
        if((DelayStatus[0] != 0xA5)&&(DelayStatus[1] != 0xA5))
        {
            DelayFlag = 1;//�̵����ϵ�
        }
        else
        {
            DelayFlag = 0;//�̵���ͨ��
        }

        VER_RDbytes(EE_Meter_address, &param_data.meter_address[0], 6);//ͨѶ��ַ
        EE_to_RAM(EE_Meter_Factory, &param_data.meter_factory[0],1);   //���̴���
        VER_RDbytes((unsigned int)EE_KHH_address , &kwh_value.integer[0], 5);         //�й�����

        KWH_RKWH_dot();                            //С��λУ��
        ECRunKwh();                                //60��ˢ������й�

        if(ConstPara_Chk() != g_ConstPara.Chk)		//���峣��У��RAM
        {
            meter_const = DefaultThreshold;
            ConstPara_Init(meter_const, 0x00);
        }
        gbFgShowItemNum = D_ShowItemNum;		//��ʾ��Ŀ����У��RAM
    }

//Сʱ���̴߳���
    if(g_Flag.Clk & C_HOUR)
    {
        g_Flag.Clk &= ~C_HOUR;
//        if((NB_LORA[0] == 0xBB)&&(NB_LORA[1] == 0xBB))
//        {
//         if((g_NB.InitState[0] == NB_Init_OK)&&(g_NB.InitState[1] == NB_Init_OK))
//            NB_Error();
//        }

//        AP_Bill(NorBill);							//��������

        EMU_Check();								//���EMU��У������Ƿ���EEPROM�����
        VER_CHK((unsigned int)EE_KHH_address, 5);                		//���й������������
        VER_CHK(EE_Meter_address,6);                //���ͨ�ŵ�ַ����
        // VER_CHK(EE_RKWH0, 3);               		//���й������������

        // VER_CHK(EE_QKWH0, 3);                		//���޹������������
        // VER_CHK(EE_RQKWH0, 3);               		//���޹������������
    }

//�쵽�̴߳���
    if (g_Flag.Clk & C_DAY)
    {
        g_Flag.Clk &= ~C_DAY;

    }

//�굽�̴߳���
    if(g_Flag.Clk & C_YEAR)
    {
        g_Flag.Clk &= ~C_YEAR;
    }
}

