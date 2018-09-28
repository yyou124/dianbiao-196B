/*==============================================================================
                Copyright(C) 1997-2019.  Sinowealth Tech. Co., Ltd.
--------------------------------------------------------------------------------
Project Name  : MEASUREMDPROC.c
Description   :
Notice        :
			 1:
			 2:
Author        : James.wang (email: James.wang@sinowealth.com)
Start Date    : 2015/06/26
Approve Date  :
Version       : V0.0
Function List :
             1:
             2:
RevisionHistory:
Rev#  CheckSum    Date     Author     Comments(Function+Date)
-----+--------+----------+---------+------------------------------------------
0.0     XXXX   2015/06/26 james.wang Just build the file
==============================================================================*/

//-------------------------------------------------------------------------------
#ifndef  MEASUREMDPROC_C
	#define  MEASUREMDPROC_C
#endif
//-------------------------------------------------------------------------------

//---------------


#include "config.h"	
#include "Headconfig.h"


////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: DemandMeasThread
** 函数描述: 需量线程处理
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
#if DemandEn == ENABLE  
void DemandMeasThread(void)
{
    unsigned long data OldMD = 0,NewMD = 0;
    unsigned char data MdInv = 0,Tmp = 0;

    VER_RDbytes(MDINV , &Tmp, 1);
    MdInv = BCD2HEX(Tmp); 
    if((0 == MdInv)||(MdInv > 60)) MdInv = 30;

    Tmp = BCD2HEX(_UINT8_RTC[1]); 

    if(0 == (Tmp%MdInv)) 
    {
        do
        {
            NewMD = g_Demand.Max0;
        }while(NewMD != g_Demand.Max0);
		
        do
        {
            g_Demand.Max0 = 0;
        }while(g_Demand.Max0 != 0);
		
        if((g_ConstPara.Constant == 1000) || (g_ConstPara.Constant == 2000) || (g_ConstPara.Constant == 1600) || (g_ConstPara.Constant == 3200))   //哥伦比亚要求脉冲常数为3200
        {
        }
        else
        {
            if((0==VER_RDbytes(EE_PULSECONST,&meter_const,1)) || meter_const==0)  
            {
                meter_const = DefaultThreshold;	
            }
            ConstPara_Init(meter_const, 0x00);
        }
        
        OldMD = (unsigned long)(NewMD *(60.0*1000/g_ConstPara.Constant)/MdInv/10);// XX.XXKW
        Write_ArrayRes((unsigned char *)&NewMD,OldMD,4);

        OldMD = 0x00000000;
        VER_RDbytes(CM_MD, (unsigned char *)&OldMD+2, 2);
        if( NewMD > OldMD )
        {
			VER_WRbytes(CM_MD, (unsigned char *)&NewMD+2, 2, 1);
        }
    }
    Demand_DataToRam();
}
#endif 


////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: MDRst
** 函数描述: 需量复位处理
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
/*
void MDRst(unsigned char Mode)
{
    unsigned char xdata Buffer[6];
    unsigned int data OldMD = 0,NewMD = 0;
    
    g_Demand.Max0 = 0;
    
    if(Mode == 1)										//Demand Reset
    {
        VER_RDbytes(CM_MD, (unsigned char *)&OldMD, 2);
        VER_RDbytes(CM_MANUAL_MD, (unsigned char *)&NewMD, 2);

        if(OldMD > NewMD)
        {
            VER_WRbytes(CM_MANUAL_MD, (unsigned char *)&OldMD, 2, 1);
        }
        MemInitSet(&Buffer[0],0x00,2);
        VER_WRbytes(CM_MD, (unsigned char *)&Buffer[0], 2, 1);
    }
    else if(Mode == 2)									//Demand bill
    {
        MemInitSet(&Buffer[0],0x00,2);
        VER_WRbytes(CM_MD, (unsigned char *)&Buffer[0], 2, 1);
        VER_WRbytes(CM_MANUAL_MD, (unsigned char *)&Buffer[0], 2, 1);
    }
    else if(Mode == 3)									//Demand all clear
    {
        MemInitSet(&Buffer[0],0x00,2);
        VER_WRbytes(CM_MD, (unsigned char *)&Buffer[0], 2, 1);
        VER_WRbytes(CM_MANUAL_MD, (unsigned char *)&Buffer[0], 2, 1);
        VER_WRbytes(LMON_MD, (unsigned char *)&Buffer[0], 2, 1);
    }
    Demand_DataToRam();
}
*/


////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: Demand_Bill
** 函数描述: 需量账单处理
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
void Demand_Bill(void)
{
    unsigned char data Tmp = 0;

    if((g_Flag.Function & 0x0001) == 0x0001)         //最低位为1时，需量年复位
    {
        VER_RDbytes(DemandBillFlg, (unsigned char *)&Tmp, 1);
        if(Tmp == 0xA5)
        {
            Demand_Save();
            Tmp = 0x00;
            VER_WRbytes(DemandBillFlg, (unsigned char *)&Tmp, 1, 1);
        }
    }
    else
    {
        Demand_Save();
    }
    Demand_DataToRam();  
}


////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: Demand_Save
** 函数描述: 需量存储处理
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
void Demand_Save(void)  
{
    unsigned char xdata Buffer[10];
    unsigned int data OldMD = 0,NewMD = 0;
    
    VER_RDbytes(CM_MD, (unsigned char *)&OldMD, 2);
    VER_RDbytes(CM_MANUAL_MD, (unsigned char *)&NewMD, 2);
    
    if(OldMD > NewMD)
    {
        VER_RDbytes(CM_MD, (unsigned char *)&Buffer[0], 2);
    }
    else
    {
        VER_RDbytes(CM_MANUAL_MD, (unsigned char *)&Buffer[0], 2);
    }
    VER_WRbytes(LMON_MD, (unsigned char *)&Buffer[0], 2, 1);

	MDRst(2);
    Demand_DataToRam();
}


////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: Demand_DataToRam
** 函数描述: 需量TO RAM处理
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
void Demand_DataToRam(void)
{
    VER_RDbytes(CM_MD, (unsigned char *)&g_Demand.CurDemand[0], 2);
    VER_RDbytes(LMON_MD, (unsigned char *)&g_Demand.PrevDemand[0], 2);
}