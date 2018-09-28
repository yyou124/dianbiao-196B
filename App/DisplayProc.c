/*==============================================================================
                Copyright(C) 1997-2019.  Sinowealth Tech. Co., Ltd.
--------------------------------------------------------------------------------
Project Name  : DISPLAYPROC.c
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
#ifndef  DISPLAYPROC_C
	#define  DISPLAYPROC_C
#endif
//-------------------------------------------------------------------------------

//---------------


#include "config.h"
#include "Headconfig.h"
#include "LCDDriver.h"


const DISPSETTINGTABLE code DispSettingTable[]=
{
//显示项ID号码--显示数据地址---------------显示数据序号---------显示数据类型,高4位是MEM类型，低4位是数据长度
//DispPageID----DispDataAddr----------------DispDataIndex-------DispDataType
//int-----------int-------------------------char----------------char
	0x9010,     &g_InsBCD.Voltage[0],       DIS_V,            	MEMTYPE_RAM|3,		//当前电压
	0x9080,     &g_InsBCD.Current[0],       DIS_I,           	MEMTYPE_RAM|3,		//当前电流
	0x9070,     &g_InsBCD.Power[0],       	DIS_P,          	MEMTYPE_RAM|4,		//当前有功功率
	0x9090,     &KWH_PN[0],               	DIS_kWh,        	MEMTYPE_RAM|4		//有功电量
	//0xE002,	&g_Flag.ALARM[1],			DIS_NB_ERROR,		MEMTYPE_RAM|1,	//NB初始化失败
/*
	0xC420,     &g_InsBCD.Power[0],         DIS_kW,             MEMTYPE_RAM|3,		//有功功率
    0xA010,     &g_Demand.CurDemand[0],     DIS_CURMD,          MEMTYPE_RAM|2,		//当月需量
    0xA090,     &g_Demand.PrevDemand[0],    DIS_RREVMD,         MEMTYPE_RAM|2,		//上个月的需量
    0xC400,     &g_InsBCD.Voltage[0],       DIS_V,         		MEMTYPE_RAM|3,
	0xC415,     &g_InsBCD.Current[0],       DIS_I,              MEMTYPE_RAM|3,
	0xC410,     &g_InsBCD.Current_l[0],     DIS_L1_I,           MEMTYPE_RAM|3,
	0xC411,     &g_InsBCD.Current_n[0],     DIS_L2_I,           MEMTYPE_RAM|3,
	0xC195,     &_UINT8_RTC[3],             DIS_Date,           MEMTYPE_RAM|3,
	0xC180,     &_UINT8_RTC[0],             DIS_Time,           MEMTYPE_RAM|3,
	0xC004,     &MeterNum[2],               DIS_ADDRASC,        MEMTYPE_RAM|4,
    0xE001,     &g_Flag.ALARM[0],           DIS_UNIT_EVENT,     MEMTYPE_RAM|1
*/
};



//数据区字符对应段码
const unsigned char code NumSeg[37] =
{
	CHAR_0,CHAR_1,CHAR_2,CHAR_3,CHAR_4,
	CHAR_5,CHAR_6,CHAR_7,CHAR_8,CHAR_9,
	CHAR_A,CHAR_B,CHAR_C,CHAR_D,CHAR_E,
	CHAR_F,CHAR_G,CHAR_H,CHAR_I,CHAR_J,
	CHAR_K,CHAR_L,CHAR_M,CHAR_N,CHAR_O,
	CHAR_P,CHAR_Q,CHAR_R,CHAR_S,CHAR_T,
	CHAR_U,CHAR_V,CHAR_W,CHAR_X,CHAR_Y,
	CHAR_Z,CHAR_HENGXIAN
};
////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: BCDweishu
** 功能描述:判断输入BCD码的位数
** 入口参数: *bcd要判断的BCD码	len长度
** 出口参数: 无
*******************************************************************************************/
unsigned char BCDweishu(unsigned char *bcd, unsigned char len)
{
    unsigned char i, temp,weishu;
    weishu = len +len;
    for (i = len; i > 0; i--)
    {
		temp = bcd[i-1];
        if((temp>>4) == 0x00)
            weishu--;
        else
            break;
        if(temp == 0x00)
             weishu--;
        else
            break;
    }
        return weishu;
}
////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: BCDshiftRight
** 功能描述: 将BCD码右移一位
** 入口参数: *inbcd输入BCD码	*outbcd输出BCD码	len BCD码长度
** 出口参数: 无
*******************************************************************************************/
void BCDshiftRight(unsigned char *outbcd, unsigned char *inbcd, unsigned char len)
{
    unsigned char temp, i;
    for (i = 0; i < len;i++)
        outbcd[i] = 0x00;
    for (i = 1; i < len + 1; i++)
    {
        temp = (inbcd[len - i] >> 4) & 0x0F;
        if (i < len)
            temp += ((inbcd[len - i - 1] << 4) & 0xF0);
        outbcd[len - i] = temp;
    }
}



////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
函数说明：
    LCD数据区显示，液晶数据区有8个数码管，从数据区的最低位开始显示数据，
	一个数据区数字要跨2个SEG脚。
参数：
    *SegPtr：对应SEG脚位置
	*DataAddr：指向要显示的数据区内容
	DataAttri: Data Attribute,0 = hex D_LCD_SHOW_HEX == 0；1=BCD,D_LCD_SHOW_BCD == 1,默认是HEX的格式
	Lenth：数据长度
返回：
    无
*******************************************************************************************/
void DspNum(unsigned char xdata *SegPtr, unsigned char xdata *DataAddr, unsigned char DataAttri,unsigned char Lenth)
{
	unsigned char tmp;


	if (Lenth > 9)     										//数据最多9字节
	{
        return;
	}
	//判断要显示数据的位数

    if(DataAttri == D_LCD_SHOW_HEX)
	{
	    while (Lenth != 0)
	    {
	        tmp = NumSeg[(*DataAddr)];             			// 取数字HEX的显示字段库
			*SegPtr |= (unsigned char)((tmp>>0)&0x0F);    	// 再显示低位数字
			SegPtr++;
	        *SegPtr |= (unsigned char)((tmp>>4)&0x0F);		// 先显示高位数字
			SegPtr++;
	        DataAddr++;
	        Lenth--;
	    }
	}
	else if(DataAttri == D_LCD_SHOW_BCD)
	{
	    while (Lenth != 0)
	    {
	        tmp = NumSeg[(((*DataAddr)>>0)&0x0F)];          // 取数字BCD的低四位的显示字段库
			*SegPtr |= (unsigned char)((tmp>>0)&0x0F);    	// 先显示低位数字
			SegPtr++;
	        *SegPtr |= (unsigned char)((tmp>>4)&0x0F);		// 再显示高位数字
			SegPtr++;

	        tmp = NumSeg[(((*DataAddr)>>4)&0x0F)];          // 取数字BCD的高四位的显示字段库
			*SegPtr |= (unsigned char)((tmp>>0)&0x0F);    	// 先显示低位数字
			SegPtr++;
	        *SegPtr |= (unsigned char)((tmp>>4)&0x0F);		// 再显示高位数字
			SegPtr++;

	        DataAddr++;
	        Lenth--;
	    }
	}
	else
	{
	    while (Lenth != 0)
	    {
	        tmp = NumSeg[(*DataAddr)];             			// 取数字HEX的显示字段库
			*SegPtr |= (unsigned char)((tmp>>4)&0x0F);    	// 再显示高位数字
			SegPtr++;
	        *SegPtr |= (unsigned char)((tmp>>0)&0x0F);		// 先显示低位数字
			SegPtr++;
	        DataAddr++;
	        Lenth--;
	    }
	}

}


////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: Display_HighZeroEn
** 函数描述: LCD高位显示处理
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
// void Display_HighZeroEn(unsigned char *LCDTmp, unsigned char num)  		//取消高位0显示
// {
// 	unsigned char i;

// 	if ((g_Flag.Function & 0x0080) == 0x0080)    						//BIT7用作是否取消高位0显示，1：是，0：否
// 	{
// 		for (i=0; i<num; i++)
// 		{
// 			if ((LCDTmp[17 - i*2] == (NumSeg[0] & 0x00FF)) && (LCDTmp[18 - i*2] == ((NumSeg[0]>>8) & 0xFF)))
// 			{
// 				LCDTmp[17 - i*2] = 0x00;
//                 LCDTmp[18 - i*2] = 0x00;
// 			}
// 		}
// 	}
// }


////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: GetDisPag
** 函数描述: 取显示页的内容处理
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
unsigned int GetDisPag(unsigned char AuxPagesNum, unsigned char Ptr)
{
	unsigned int data DisPagT = 0x0000;    		// 默认显示时间

	if ((AuxPagesNum == 1) && (Ptr<=AuxPagesNum))
	{
        DisPagT = 0xE001;
	}
	else if (Ptr>AuxPagesNum)    				// 显示指针大于告警条数，则从显示列表中查找其他显示项
	{
		if(gbFgShowItemNum)
        {
            Ptr -= AuxPagesNum;
 //         DisPagT = *(unsigned int *)&DispSettingTable[((Ptr-1) << 1)];  //取偶数项显示0,2,4,6...12
            DisPagT = *(unsigned int *)&DispSettingTable[((Ptr-1))];	   //取自然数显示0,1,2,3...12
        }
        else
        {
            DisPagT = 0x9010;
        }
	}
	else
	{
		DisPagT = 0x9010;
	}
	return (DisPagT);
}






////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: AddPointUnit
** 函数描述: 所有小数点单元的显示处理
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
// void AddPointUnit(unsigned char xdata *LCDTmp, unsigned char UnitID)
// {
// 	switch(UnitID)
// 	{
// 		case DIS_V:										//组合有功
// 			LCDTmp[8] |= LCD_P2;
// 		break;

// 		case DIS_I:										//正向无功
// 			LCDTmp[8] |= LCD_P2;
// 		break;

// 		case DIS_P:										//反向无功
// 			LCDTmp[8] |= LCD_P2;
// 		break;



// 		// case Adj5_CHB_Power_GAIN: 							//IB 1.0
// 		// case Adj7_CHB_Phase: 								//IB 0.5L
// 		// case Adj9_CHB_Offset: 								//IB 0.1
// 		//     LCDTmp[10] |= LCD_P3;
// 		// break;

// 		default:
// 		break;
// 	}
// }


////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: AddEventUnit
** 函数描述: 所有时间发生单元显示处理
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
// void AddEventUnit(unsigned char xdata *LCDTmp)
// {
//     if(gbLowPWRFlag == 0)
//     {
//         if((g_Flag.ALARM[0] & E_IReverse) == E_IReverse)
//         {
//             LCDTmp[0] |= LCD_P4;
// 			REV_LED &= ~REV_LED_ON;			  //REV LED	LOW ENABLE IS ON
//         }
// 		else
// 		{
//             LCDTmp[0] &= ~LCD_P4;
// 			REV_LED |= REV_LED_ON;			  //REV LED	HIGH ENABLE IS OFF
// 		}
//     }
// }



////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: DisplayProc
** 函数描述: 显示处理
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
void DisplayProc(void)
{
	unsigned char data i,A_Cnt,Tmp,Rlt,weishu;
	unsigned int  data Addr = 0x0000;
	unsigned char data MemType,Index,Len,Decim = 0x00;
	unsigned char xdata DisData[6],Buf[4];

//TEST
//219.9V
//g_InsBCD.Voltage[3] = 0x00;
//g_InsBCD.Voltage[2] = 0x99;
//g_InsBCD.Voltage[1] = 0x21;
//g_InsBCD.Voltage[0] = 0x00;
////97.889A
//g_InsBCD.Current[3] = 0x89;
//g_InsBCD.Current[2] = 0x78;
//g_InsBCD.Current[1] = 0x09;
//g_InsBCD.Current[0] = 0x00;
////7654.32 W
//g_InsBCD.Power[3] = 0x32;
//g_InsBCD.Power[2] = 0x54;
//g_InsBCD.Power[1] = 0x76;
//g_InsBCD.Power[0] = 0x00;
////0.43Kwh
//KWH_PN[0] = 0x43;
//KWH_PN[1] = 0x00;
//KWH_PN[2] = 0x00;
//KWH_PN[3] = 0x00;

//TEST

//显示刷新时间
	if((g_Flag.Run & F_DISP) != F_DISP)			   //显示刷新时间
	{
		return;
	}
	g_Flag.Run &= ~F_DISP;

//全屏显示
//	if(g_AllDisDly != 0)						   //全屏显示
//	{
//		for (i=0; i<LCDFRAME; i++)
//        {
//		    ShowLCDBuf[i]=0x0F;
//		}
//        LcdDataToICBufShow(&ShowLCDBuf[0]);
//		g_Disp.PollingDisplayCount = 0;	//轮显时间
//		g_Disp.PollingDisplayPtr = 0;	//轮显项目
//        return;
//	}

//显示CLS---处理


//显示ADJ---校表处理
  if(gbFgKeyProg == 0xF001)
	{
	    if(g_Cal.Flag  == D_Initing)	   				//启动校表初始化程序显示ADJ---
	    {
	        MemInitSet(&ShowLCDBuf[0],0x00,LCDFRAME);
			DisData[5] = 0x0A;				//A
			DisData[4] = 0x0D;				//d
			DisData[3] = 0x13;				//j
			DisData[2] = 0x24;				//-
			DisData[1] = 0x24;				//-
			DisData[0] = 0x24;				//-
			DspNum(&ShowLCDBuf[D_LCD_SHOW_PLACE_1], &DisData[0], D_LCD_SHOW_HEX, 6);
	        LcdDataToICBufShow(&ShowLCDBuf[0]);
	        return;
	    }
		//Adjust
		if(g_Cal.Flag  == D_InitAdjust)
	    {
			switch (gbAdjustShowID)
			{
				case Adj5_CHB_Power_GAIN: 							//IB 1.0  //启动校表程序显示ADJ-1.0
				{
			        MemInitSet(&ShowLCDBuf[0],0x00,LCDFRAME);
					DisData[5] = 0x0A;				//A
					DisData[4] = 0x0D;				//d
					DisData[3] = 0x13;				//j
					DisData[2] = 0x24;				//-
					DisData[1] = 0x01;				//1
					DisData[0] = 0x00;				//0
					DspNum(&ShowLCDBuf[D_LCD_SHOW_PLACE_1], &DisData[0], D_LCD_SHOW_HEX, 6);
		            //AddPointUnit(&ShowLCDBuf[0], gbAdjustShowID);		    //所有小数点单元的显示处理
					ShowLCDBuf[1] |= SEG_P1;	//小数点
			        LcdDataToICBufShow(&ShowLCDBuf[0]);
				break;
				}
				case Adj7_CHB_Phase: 								//IB 0.5L  //启动校表程序显示ADJ-0.5
				{
			        MemInitSet(&ShowLCDBuf[0],0x00,LCDFRAME);
					DisData[5] = 0x0A;				//A
					DisData[4] = 0x0D;				//d
					DisData[3] = 0x13;				//j
					DisData[2] = 0x24;				//-
					DisData[1] = 0x00;				//0
					DisData[0] = 0x05;				//5
					DspNum(&ShowLCDBuf[D_LCD_SHOW_PLACE_1], &DisData[0], D_LCD_SHOW_HEX, 6);
		            //AddPointUnit(&ShowLCDBuf[0], gbAdjustShowID);		    //所有小数点单元的显示处理
					ShowLCDBuf[1] |= SEG_P1;	//小数点
			        LcdDataToICBufShow(&ShowLCDBuf[0]);
				break;
				}
				case Adj9_CHB_Offset: 								//IB 0.1  //启动校表程序显示ADJ-0.1
				{
			        MemInitSet(&ShowLCDBuf[0],0x00,LCDFRAME);
					DisData[5] = 0x0A;				//A
					DisData[4] = 0x0D;				//d
					DisData[3] = 0x13;				//j
					DisData[2] = 0x24;				//-
					DisData[1] = 0x00;				//0
					DisData[0] = 0x01;				//1
					DspNum(&ShowLCDBuf[D_LCD_SHOW_PLACE_1], &DisData[0], D_LCD_SHOW_HEX, 6);
		            //AddPointUnit(&ShowLCDBuf[0], gbAdjustShowID);		    //所有小数点单元的显示处理
			        ShowLCDBuf[1] |= SEG_P1;	//小数点
					LcdDataToICBufShow(&ShowLCDBuf[0]);
				break;
				}
					default:
				{
			        MemInitSet(&ShowLCDBuf[0],0x00,LCDFRAME);
					DisData[5] = 0x0A;				//A
					DisData[4] = 0x0D;				//d
					DisData[3] = 0x13;				//j
					DisData[2] = 0x24;				//-
					DisData[1] = 0x24;				//-
					DisData[0] = 0x24;				//-
					DspNum(&ShowLCDBuf[D_LCD_SHOW_PLACE_1], &DisData[0], D_LCD_SHOW_HEX, 6);
			        LcdDataToICBufShow(&ShowLCDBuf[0]);
				break;
				}
			}
				if((gbAdjCurrentStep == 1)&(gbAdjustShowID ==	Adj5_CHB_Power_GAIN))		  //1.0	  OK
				{
					MemInitSet(&ShowLCDBuf[0],0x00,LCDFRAME);
					DisData[5] = 0x18;				//O
					DisData[4] = 0x14;				//K
					DisData[3] = 0x24;				//-
					DisData[2] = 0x24;				//-
					DisData[1] = 0x01;				//1
					DisData[0] = 0x00;				//0
					DspNum(&ShowLCDBuf[D_LCD_SHOW_PLACE_1], &DisData[0], D_LCD_SHOW_HEX, 6);
					//AddPointUnit(&ShowLCDBuf[0], gbAdjustShowID);		    //所有小数点单元的显示处理
					ShowLCDBuf[1] |= SEG_P1;	//小数点
					LcdDataToICBufShow(&ShowLCDBuf[0]);
					return;
					
				}
				else if((gbAdjCurrentStep == 2)&(gbAdjustShowID ==	Adj7_CHB_Phase))			  //0.5L
				{
					 MemInitSet(&ShowLCDBuf[0],0x00,LCDFRAME);
					DisData[5] = 0x18;				//O
					DisData[4] = 0x14;				//K
					DisData[3] = 0x24;				//-
					DisData[2] = 0x24;				//-
					DisData[1] = 0x00;				//0
					DisData[0] = 0x05;				//5
					DspNum(&ShowLCDBuf[D_LCD_SHOW_PLACE_1], &DisData[0], D_LCD_SHOW_HEX, 6);
					//AddPointUnit(&ShowLCDBuf[0], gbAdjustShowID);		    //所有小数点单元的显示处理
					ShowLCDBuf[1] |= SEG_P1;	//小数点
					LcdDataToICBufShow(&ShowLCDBuf[0]);
					return;
				}
				else if((gbAdjCurrentStep == 3) &(gbAdjustShowID ==	Adj9_CHB_Offset))			  //0.1
				{
					MemInitSet(&ShowLCDBuf[0],0x00,LCDFRAME);
					DisData[5] = 0x18;				//O
					DisData[4] = 0x14;				//K
					DisData[3] = 0x24;				//-
					DisData[2] = 0x24;				//-
					DisData[1] = 0x00;				//0
					DisData[0] = 0x01;				//1
					DspNum(&ShowLCDBuf[D_LCD_SHOW_PLACE_1], &DisData[0], D_LCD_SHOW_HEX, 6);
					//AddPointUnit(&ShowLCDBuf[0], gbAdjustShowID);		    //所有小数点单元的显示处理
					ShowLCDBuf[1] |= SEG_P1;	//小数点
					LcdDataToICBufShow(&ShowLCDBuf[0]);
					return;
				}




			//if((gbAdjStepFlag & Bin(00000111)) == Bin(00000111))		   	//1.0,0.5L,0.1 IS OK
			if((gbAdjStepFlag & Bin(00000011)) == Bin(00000011))		   	//1.0,0.5L IS OK
			{
		        MemInitSet(&ShowLCDBuf[0],0x00,LCDFRAME);
				DisData[5] = 0x0A;				//A
				DisData[4] = 0x0D;				//d
				DisData[3] = 0x13;				//j
				DisData[2] = 0x24;				//-
				DisData[1] = 0x18;				//O
				DisData[0] = 0x14;				//K
				DspNum(&ShowLCDBuf[D_LCD_SHOW_PLACE_1], &DisData[0], D_LCD_SHOW_HEX, 6);
		        LcdDataToICBufShow(&ShowLCDBuf[0]);

                g_Cal.Flag  = D_InitOk;										//EXIT ADJUST
			}

	        return;
		}
		if(g_Cal.Flag  == D_InitOk)	   				//启动校表初始化程序显示ADJ-OK
	    {
	        MemInitSet(&ShowLCDBuf[0],0x00,LCDFRAME);
			DisData[5] = 0x0A;				//A
			DisData[4] = 0x0D;				//d
			DisData[3] = 0x13;				//j
			DisData[2] = 0x24;				//-
			DisData[1] = 0x18;				//O
			DisData[0] = 0x14;				//K
			DspNum(&ShowLCDBuf[D_LCD_SHOW_PLACE_1], &DisData[0], D_LCD_SHOW_HEX, 6);
	        LcdDataToICBufShow(&ShowLCDBuf[0]);
	        return;
	    }

	}

//轮显处理
	A_Cnt = 0;
    if(g_Flag.ALARM[0] != 0)
    {
        A_Cnt = 1;					 					    //A_Cnt = 0,默认的是第0屏
    }													    //A_Cnt = 1,对应的第N屏

	if (g_Disp.PollingDisplayCount == 0)                  	// 轮显时间到
	{
		Tmp = gbFgShowItemNum + A_Cnt;   					// 告警信息也要显示
	    g_Disp.PollingDisplayPtr++;
	    if (g_Disp.PollingDisplayPtr > Tmp)                	// 轮显到最后一页，则重头开始轮显
		{
			g_Disp.PollingDisplayPtr = 1;
		}
		if (g_Disp.PollingDisplayPtr < 1)                  	// 轮显指针乱，则从最后一页开始显示
		{
			g_Disp.PollingDisplayPtr = Tmp;
		}

	    g_Disp.PollingDisplayID = GetDisPag(A_Cnt, g_Disp.PollingDisplayPtr);	// 查找显示页，告警页或者显示配置表
		g_Disp.PollingDisplayCount = g_Disp.PollingDisplayCountSet;    			// 更新轮显周期计数器
	}


//查找对应的项的索引号
    Rlt = 0xff;
	for(i = 0; i < (sizeof(DispSettingTable)/sizeof(DISPSETTINGTABLE));i++)
	{
        if(DispSettingTable[i].DispPageID == g_Disp.PollingDisplayID)
        {
            Rlt = i;
            break;
        }
	}
    if(Rlt == 0xff)
    {
         Rlt = 0x00;
    }
    Addr = DispSettingTable[Rlt].DispDataAddr;
    Index = DispSettingTable[Rlt].DispDataIndex;
    MemType = DispSettingTable[Rlt].DispDataType & 0xF0;
    Len = DispSettingTable[Rlt].DispDataType & 0x0F;

	MemInitSet(&DisData, 0, 4);
	MemInitSet(&ShowLCDBuf[0],0x00,LCDFRAME);					//show lcd buff cls zero
//电压	XX X.X  2位
//电流  XX X.X XX 3位
//功率	XX XX XX .XX 4位
//电量	XX X X XX XX .XX 5位
//对应的索引显示处理
	switch(Index)
	{
		case DIS_V:
			memcpy(&Buf[0],(unsigned char xdata *)Addr,Len);
			//memcpy(&DisData[0],&Buf[0],4);
			ReverseCpy(&DisData[0],&Buf[0],4);
			DspNum(&ShowLCDBuf[D_LCD_SHOW_PLACE_1], &DisData[1], D_LCD_SHOW_BCD, 2);			 //4+2
			weishu  = BCDweishu(&DisData[1],2);	//判断位数
			//将超过位数的显示部分清零，小数点前一位保留
			for(i=weishu+weishu;i<LCDFRAME;i++)
			{
				if(i>3) ShowLCDBuf[i] = 0x00;
			}
			ShowLCDBuf[1] |= SEG_P1;	//小数点
			ShowLCDBuf[11] |= SEG_V;		//单位
			//Display_HighZeroEn(&ShowLCDBuf[0], 5);
		    break;


		case DIS_I:

			memcpy(&Buf[0],(unsigned char xdata *)Addr,Len);
			//memcpy(&DisData[0],&Buf[0],4);
			ReverseCpy(&DisData[0],&Buf[0],4);
			DspNum(&ShowLCDBuf[D_LCD_SHOW_PLACE_1], &DisData[2], D_LCD_SHOW_BCD, 2);			 //4+2
			weishu  = BCDweishu(&DisData[2],2);	//判断位数
			//将超过位数的显示部分清零，小数点前一位保留
			for(i=weishu+weishu;i<LCDFRAME;i++)
			{
				if(i>3) ShowLCDBuf[i] = 0x00;
			}
			ShowLCDBuf[1] |= SEG_P1;	//小数点
			ShowLCDBuf[9] |= SEG_I;		//单位
			//Display_HighZeroEn(&ShowLCDBuf[0], 5);
		    break;

		case DIS_P:

			memcpy(&Buf[0],(unsigned char xdata *)Addr,Len);
		 	BCDshiftRight(&DisData[0],&Buf[0],4);//右移一位
			memcpy(&Buf[0],&DisData[0],4);

			ReverseCpy(&DisData[0],&Buf[0],4);
			DspNum(&ShowLCDBuf[D_LCD_SHOW_PLACE_1], &DisData[0], D_LCD_SHOW_BCD, 3);			 //4+2
			weishu  = BCDweishu(&DisData[0],3);	//判断位数
			//将超过位数的显示部分清零，小数点前一位保留
			for(i=weishu+weishu;i<LCDFRAME;i++)
			{
				if(i>3) ShowLCDBuf[i] = 0x00;
			}
			ShowLCDBuf[1] |= SEG_P1;	//小数点
			ShowLCDBuf[7] |= SEG_P;		//单位
			//Display_HighZeroEn(&ShowLCDBuf[0], 5);
		    break;

		case DIS_kWh://电量显示之前是5字节输入

			memcpy(&Buf[0],(unsigned char xdata *)Addr,Len);
			ReverseCpy(&DisData[0],&Buf[0],4);
		 	BCDshiftRight(&Buf[0],&DisData[0],4);//右移一位
			ReverseCpy(&DisData[0],&Buf[0],4);
			// memcpy(&Buf[0],&DisData[0],4);
			// ReverseCpy(&DisData[0],&Buf[0],4);
			DspNum(&ShowLCDBuf[D_LCD_SHOW_PLACE_1], &DisData[0], D_LCD_SHOW_BCD, 3);			 //4+2
			weishu  = BCDweishu(&DisData[0],3);	//判断位数
			//将超过位数的显示部分清零，小数点前一位保留
			for(i=weishu+weishu;i<LCDFRAME;i++)
			{
				if(i>3) ShowLCDBuf[i] = 0x00;
			}
			ShowLCDBuf[1] |= SEG_P1;	//小数点
			ShowLCDBuf[5] |= SEG_KWH;		//单位
			//Display_HighZeroEn(&ShowLCDBuf[0], 5);
			break;
		default:
		    break;
	   }

//显示小数点处理
	//AddPointUnit(&ShowLCDBuf[0], Index);		    //所有小数点,其他显示项目单元的显示处理
	//AddEventUnit(&ShowLCDBuf[0]);				    //所有时间发生单元显示处理

//LCD闪屏处理
/*
    g_Flag.ALARM[1] ^= 0x01;
    if((0xF001 == gbFgKeyProg) && (BlinkFlg == 0x00) && ((g_Flag.ALARM[1] & 0x01) == 0x01))   //超级电容方案掉电S1判断不准，暂时去掉
    {
        for(i = 0; i < LCDFRAME+1; i++)
        {
            ShowLCDBuf[i] = 0x00;	  				//数据区清屏，1S FLASH LCD
        }
    }
*/
//ShowLCDBuf[0] = 0xFF;
//ShowLCDBuf[1] = 0xFF;
//ShowLCDBuf[2] = 0xFF;
//ShowLCDBuf[3] = 0xFF;
//ShowLCDBuf[4] = 0xFF;
//ShowLCDBuf[5] = 0xFF;
//ShowLCDBuf[6] = 0xFF;
//ShowLCDBuf[7] = 0xFF;
//ShowLCDBuf[8] = 0xFF;
//ShowLCDBuf[9] = 0xFF;
//ShowLCDBuf[10] = 0xFF;
//ShowLCDBuf[11] = 0xFF;

	LcdDataToICBufShow(&ShowLCDBuf[0]);			    //显示数据送到MCU的LCD BUFFER显示
}