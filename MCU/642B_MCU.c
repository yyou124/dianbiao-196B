/*==============================================================================
                Copyright(C) 1997-2019.  Sinowealth Tech. Co., Ltd.
--------------------------------------------------------------------------------
Project Name  : _642B_MCU.c
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
#ifndef  _642B_MCU_C
	#define  _642B_MCU_C
#endif
//-------------------------------------------------------------------------------

//---------------



#include "config.h"
#include "Headconfig.h"







////////////////////////////////////////////////////////////////////////////////////////////
//RTC SFR
unsigned char xdata   SBSC     _at_  0xFFA0;
unsigned char xdata   SEC      _at_  0xFFA1;
unsigned char xdata   MIN      _at_  0xFFA2;
unsigned char xdata   HR       _at_  0xFFA3;
unsigned char xdata   DAY      _at_  0xFFA4;
unsigned char xdata   MTH      _at_  0xFFA5;
unsigned char xdata   YR       _at_  0xFFA6;
unsigned char xdata   DOW      _at_  0xFFA7;
unsigned char xdata   RTCDATH  _at_  0xFFA8;   //RTC校准值改为2字节
unsigned char xdata   RTCDATL  _at_  0xFFA9;
unsigned char xdata   RTCALM   _at_  0xFFAA;
unsigned char xdata   A0SEC    _at_  0xFFAB;
unsigned char xdata   A0MIN    _at_  0xFFAC;
unsigned char xdata   A0HR     _at_  0xFFAD;
unsigned char xdata   A0DAY    _at_  0xFFAE;
unsigned char xdata   A0DOW    _at_  0xFFAF;
unsigned char xdata   A1SEC    _at_  0xFFB0;
unsigned char xdata   A1MIN    _at_  0xFFB1;
unsigned char xdata   A1HR     _at_  0xFFB2;
unsigned char xdata   RTCCON   _at_  0xFFB3;
unsigned char xdata   RTCWR    _at_  0xFFB4;
unsigned char xdata   RTCPSW   _at_  0xFFB5;
unsigned char xdata   RTCIE    _at_  0xFFB6;
unsigned char xdata   RTCIF    _at_  0xFFB7;
unsigned char xdata   RTCECL   _at_  0xFFB8;
unsigned char xdata   RTCECH   _at_  0xFFB9;
unsigned char xdata   RTCTMR   _at_  0xFFBA;

//温度测量SFR
unsigned char xdata   TPCON    _at_  0xFFE8;
unsigned char xdata   TEMPH    _at_  0xFFE9;
unsigned char xdata   TEMPL    _at_  0xFFEA;
unsigned char xdata   TEMPKH   _at_  0xFFEB;
unsigned char xdata   TEMPKL   _at_  0xFFEC;
unsigned char xdata   TEMPBH   _at_  0xFFED;
unsigned char xdata   TEMPBL   _at_  0xFFEE;
unsigned char xdata   TPMOD0   _at_  0xFFE0;
unsigned char xdata   TPMOD1   _at_  0xFFE1;







////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: Reset_power
** 函数描述: 初始化power模块
** 输入参数: 无
** 输出参数: 无
** 返回: 0表示复位上电
** 返回: 1表示其他复位上电
*******************************************************************************************/
/* unsigned char Reset_power(void)
{
	if((RSTSTAT&0x20) == 0x20)	      //POWER ON RESET
	{
		return(0);
	}
	else
	{
		return(1);
	}
} */

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: Init_PWM
** 函数描述: 初始化PWM模块
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
/* void Init_PWM(void)
{
    PWM0CON = 0x00;   //关闭PWMx模块，关闭PWMx中断使能，清除PWMx中断标志，禁止PWMx输出
    PWM1CON = 0x00;   //P1.4和P5.6不作为PWMx输出口
} */

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: Init_TPS
** 函数描述: 初始化TPS模块
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
/* void Init_TPS(void)
{
    TPCON = 0x00;    					//关闭TPS模块，关闭TPS中断使能，清除TPS中断标志
} */

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: Init_LCD
** 函数描述: 初始化LCD模块
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
/* void Init_LCD(void)
{
    LCDCON  = Bin(00000111);          	//4COM，1/3偏置，驱动电压0.8*VOUT
    LCDCON1 = Bin(00000010);		    //传统模式，3*20K偏压电阻
    MemInitSet(&LCDBuf, 0x00, 32);		//清屏
//    MemInitSet(&LCDBuf, 0xFF, 32);		//清屏
    LCDCON |= Bin(10000000);			//开启LCD
} */

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: Init_LCD
** 函数描述: 初始化LCD模块
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
/* void ACInit_LCD(void)
{
    if (LCDCON != Bin(10000100))     //6COM，1/3偏置，驱动电压0.9*VOUT			  //2014/11/29
    {
		LCDCON =  Bin(1000010);													//2014//11/29
    }
    if (LCDCON1!= Bin(00000010))     //传统模式，3*20K偏压电阻
    {
        LCDCON1= Bin(00000010);
    }
} */

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: PWONInit_LCD
** 函数描述: 初始化LCD模块
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
void PWONInit_LCD(void)
{
	unsigned char data i;
	LCDCON = Bin(00000111);          	//4COM，1/3偏置，驱动电压0.8*VOUT
	LCDCON1= Bin(00000010);		     	//传统模式，3*20K偏压电阻
	LCDCON |= Bin(10000000);		 	//开启LCD
	for (i=0; i<LCDFRAME; i++)
	{
		ShowLCDBuf[i]=0xFF;
	}
    LcdDataToICBufShow(&ShowLCDBuf[0]);
}

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: PWOFFInit_LCD
** 函数描述: 初始化LCD模块
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
/* void PWOFFInit_LCD(void)
{
	unsigned char data i;

	for (i=0; i<LCDFRAME; i++)
    {
	    ShowLCDBuf[i]=0x00;
	}
    LcdDataToICBufShow(&ShowLCDBuf[0]);

    LCDCON &= Bin(01111111);   //关闭LCD
    LCDCON1= Bin(00000000);    //降低lcd功耗
} */

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: Change32KToPLL
** 函数描述: 初始化系统时钟
** 输入参数: 无
** 输出参数: 无
 ******************************************************************************************/
void Change32KToPLL(void)
{
	unsigned char data i,temp;

    temp = CLKCON;
    temp &= Bin(00101100);
    if(Bin(00101100) != temp)
    {
       CLKCON = Bin(00100000);
       CLKCON |=Bin(00001000);	       	//打开PLL
       for(i=0;i<100;i++)
       {
          _nop_();				  		//DELAY 5MS
       }
       CLKCON |=Bin(00100100);			//2分频，4.096MHZ
    }
}

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: ChangePLLTo32K
** 函数描述: 初始化系统时钟
** 输入参数: 无
** 输出参数: 无
 ******************************************************************************************/
/* void ChangePLLTo32K(void)
{
	unsigned char data temp;

	temp = CLKCON;
	temp &= Bin(00001100);
	if(Bin(00000000) != temp)
	{
	    CLKCON = Bin(00000000);
		CLKCON &= Bin(11111011);      //选择32.768KHZ 为OSCSCLK
		_nop_();
		CLKCON &= Bin(11110111);      //降低功耗
	}
} */

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: Init_Timer
** 功能描述: 初始化TIMER
** 入口参数: 无
** 出口参数: 无
*******************************************************************************************/
 void Init_Timer(void)
{
    TR0=0;							      //关闭TIMER0
    TF0=0;                                //清TIMER0溢出标志
    TR1=0;							      //关闭TIMER1
    TF1=0;			                      //清TIMER1溢出标志
    TR2=0;							      //关闭TIMER2
    TF2=0;	                              //清TIMER2溢出标志

    TF1		=	0;
	TMOD	=	Bin(00010001);				//设置Timer1为定时模式，工作方式一(16bit，用于)
	TL1		=	(2^16-3412)&0x00FF;
	TH1		=	((2^16-3412)>>8)&0xFF;



    TR1		=	1;							//start timer1
    ET1     =   1;                          //使能Timer1中断
}

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: ACInit_Timer
** 功能描述: 用于刷新Timer的SFR
** 入口参数: 无
** 出口参数: 无
*******************************************************************************************/
 void ACInit_Timer(void)
{
    if(TCON1!= Bin(00000000))        //单相两线表的T0定时器采用SysClk/12作为时基
	{
        TCON1= Bin(00000000);
	}
    if(TMOD != Bin(00010001))        //TIMER0和TIMER1为16位模式
	{
        TMOD = Bin(00010001);
	}
    if(ET1 != 1)
	{
        ET1 = 1;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: Init_IO
** 函数描述: 初始化IO
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
void Init_IO(void)
{
	PXMOD = Bin(00000011);  //P2.4不作为RTC脉冲输出口，P3.4不作为IOMUX，P2.1和P2.2为CMOS推挽输出
    //LCD
    P0SS = D_P0SS;          //P0.2-P0.5作为LCD端口（SEG7 - SEG10）
	P1SS = D_P1SS;          //P1.4-P1.7作为LCD端口（SEG1 - SEG4）
	P3SS = D_P3SS;          //P3.3-P3.6作为I/O
	P5SS = D_P5SS;          //P5.0-P5.3作为LCD端口（SEG21 - SEG24）
    //IO
    P0   = D_P0;
    P0CR = D_P0CR;
    P0PCR= D_P0PCR;

    P1   = D_P1;
    P1CR = D_P1CR;
    P1PCR= D_P1PCR;


    P2   = D_P2;
    P2CR = D_P2CR;
    P2PCR= D_P2PCR;
    P2DRV= 0x00;

    P3   = D_P3;
    P3CR = D_P3CR;
    P3PCR= D_P3PCR;
    P3DRV= 0x00;

    P5   = D_P5;
    P5CR = D_P5CR;
    P5PCR= D_P5PCR;
}

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: Init_IO
** 函数描述: 初始化IO
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
 void ACInit_IO(void)
{
    if (PXMOD != Bin(00000011))  				//P2.4不作为RTC脉冲输出口，P3.4不作为IOMUX，P2.1和P2.2为CMOS推挽输出
	{
        PXMOD = Bin(00000011);
	}


    if(P0SS != D_P0SS) P0SS = D_P0SS;          //P0.2-P0.5作为LCD端口（SEG7 - SEG10）
    if(P1SS != D_P1SS) P1SS = D_P1SS;          //P1.4-P1.7作为LCD端口（SEG1 - SEG4）
    if(P3SS != D_P3SS) P3SS = D_P3SS;          //P3.3-P3.6作为I/O
    if(P5SS != D_P5SS) P5SS = D_P5SS;          ///P5.0-P5.3作为LCD端口（SEG21 - SEG24）


   if(P2PCR!= D_P2PCR) P2PCR= D_P2PCR;
   if(P3PCR!= D_P3PCR) P3PCR= D_P3PCR;

   if(P5CR != D_P5CR)  P5CR = D_P5CR;
   if(P5PCR!= D_P5PCR) P5PCR= D_P5PCR;
}

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: ACInit_EMUPluse
** 函数描述: AC下EMU中断刷新
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
 void ACInit_EMUPluse(void)
{
    //允许PF脉冲输出，累加周期中断      关闭RTC时钟补偿
    if(((EMUIE & 0x48) != 0x48) && (0x00 == mPWRDN_PTM)) //&& (g_AllDisDly == 0))
    {
        EMUIF = 0x00;
        EMUIE |= 0x48;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: Init_ADC
** 函数描述: 初始化ADC
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
/* void Init_ADC(void)
{
   ADT  =Bin(00000000);
   ADCON=Bin(00000000);        //关闭ADC模块，关闭ADC中断使能，清除ADC中断标志
   ADCH =Bin(00000000);        //P2.3-P2.7不作为ADC输入
} */

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: BatSupplySwitch
** 函数描述: 开启电池供电自动切换功能，当LPD事件发生时，自动切换为电池供电
** 输入参数:
** 输出参数:
*******************************************************************************************/
/* void BatSupplySwitch (void)
{
    //Check the LPDEN & AUTOS
    if ((LPDCON & Bin(10000001)) != Bin(10000000))
    {
        _push_(IEN0);
        EA = 0;
        PASLO = 0x55;
        LPDCON &= Bin(11111110);
        PASLO = 0x55;
        LPDCON |= Bin(10000000);
        _pop_(IEN0);
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
    }

    if((IEN1 & Bin(10000000)) != Bin(10000000))
    {
        IEN1 |= Bin(10000000);
    }
 */
/*
    if ((LPDCON1 & Bin(11000000)) != Bin(00000000))   //关闭VOUT低电压检测
    {
        LPDCON1 =  0x00;
    }
*/
// }

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: CheckPowerOn
** 函数描述: 采样电源AD
** 输入参数:
** 输出参数: 无
*******************************************************************************************/
/* void CheckPowerOn(void)
{
    ADT  =Bin(00000000);				//Tadc=32Tsys 采样周期为15个Tadc
    ADCH =Bin(00000000);
    ADCON=Bin(00000000);
    ADCON=Bin(00000000);   */      		//启动

/*
    LPDCON1 = Bin(11000110);		     //<3.0v
    if ((LPDCON1 & Bin(01000000)) == Bin(00000000))   //>3.0v检测
    {
        gbLowPWRFlag=0; 	  		//=0 AC mode flag
    }
    else
    {
        gbLowPWRFlag=1;
//        BLACK_LED_CLOSE();
    }
*/

//    LPDCON = Bin(11000110);
/*     if((LPDCON & Bin(01000000)) == Bin(01000000)) //VIN > 1.2V
    {
        gbLowPWRFlag=0; 	  		//=0 AC mode flag
    }
    else
    {
        gbLowPWRFlag=1;
//        BLACK_LED_CLOSE();
    }
} */

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: CheckPowerOff
** 函数描述: 采样电源AD
** 输入参数:
** 输出参数: 无
*******************************************************************************************/
// void CheckPowerOff(void)
//{
//    ADT  =Bin(00000000);				//Tadc=32Tsys 采样周期为15个Tadc
//    ADCH =Bin(00000000);
//    ADCON=Bin(00000000);
//    ADCON=Bin(00000000);        		//启动 */
/*
    if(gbLowPWRFlag == 0)
    {
	    LPDCON1 = Bin(11000100);		     //<2.85v
        if ((LPDCON1 & Bin(01000000)) == Bin(01000000))   //<2.85v检测
        {
            Delay_ms(1);
            if ((LPDCON1 & Bin(01000000)) == Bin(01000000))   //<2.85v检测
            {
			    //REV_LED |= REV_LED_ON;			  		//REV LED	HIGH ENABLE IS OFF

                // g_Disp.ShutDisplayCount = 0;
                // g_Flag.ALARM[0] &=  ~(E_IReverse);

                PluseAndDecimalProc();					//脉冲数和小数电量
                EnergyProc();							//整数电量处理

                // BlinkFlg = 0x00;
                // gbLowPWRFlag=1;  						//=1,Battery mode flag
            }
        }
    }
*/


//    if(gbLowPWRFlag == 0)
//    {
//        //CHECK VIN < 1.2 || SAG CHECKED
//        if(((LPDCON & Bin(00001000)) == Bin(00000000)) || (SAG_CHECK())) //VDD < 2.7V
//        //if(((LPDCON & Bin(01000000)) == Bin(00000000)) || (SAG_CHECK())) //VIN < 1.2V
//        {
//            Delay_ms(1);
//            if(((LPDCON & Bin(00001000)) == Bin(00000000)) || (SAG_CHECK())) //VDD < 2.7V
//            //if(((LPDCON & Bin(01000000)) == Bin(00000000)) || (SAG_CHECK())) //VIN < 1.2V
//            {
//                //LED控制，不需要
//			    //REV_LED |= REV_LED_ON;			  		//REV LED	HIGH ENABLE IS OFF
//                //显示相关，不需要
//                //g_Disp.ShutDisplayCount = 0;
//                //警报LED相关，不需要
//                //g_Flag.ALARM[0] &=  ~(E_IReverse);

//                PluseAndDecimalProc();					//脉冲数和小数电量
//                EnergyProc();							//整数电量处理

//                //显示、电池模式，不需要
//                //BlinkFlg = 0x00;
//                //gbLowPWRFlag=1;  						//=1,Battery mode flag
//            }
//        }
//    }
//}

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: ACInit_RTC
** 函数描述:
** 输入参数:
** 输出参数:
*******************************************************************************************/
 void ACInit_RTC()
 {
     if(RTCIE  != 0x08)
     {
         RTCIE  = 0x08;
     }
 /*
     if (((RTCCON & 0x08) == 0x08) && (0xA5 != mPWRDN_PTM))	//只有mPWRDN_PTM为0xA5时，CALOUT引脚才输出秒脉冲
     {
         RTCCON = 0x00;
     }
 */
 }

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: BatteryInitMode
** 函数描述: 进入低功耗状态
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
/* void BatteryInitMode(void)
{
    __U32_Def val;

    EA =0;
    //*****关闭计量****************
    EMUIF=0x00;
    EMUIE=0x00;

	val.val= 0x0000;
	WriteEMU_REG(EMUCFG0, val);        //关闭3路ADC
	val.val= 0x0000;
	WriteEMU_REG(EMUCFG1, val);        //关闭累加模块

	val.val= 0x01E8;
	WriteEMU_REG(EMUCFG3, val);        //关闭VERF

    EMUSR=0x00;                        //关闭计量模块
    LDOCON = 0x00;                     //关闭LDO bandgap电路，LDO电压输出设置为0V(无输出)

    TR0 = 0;
    TR1 = 0;
    TR2 = 0;
    ET0 = 0;
    REN=0;
    REN1=0;
    REN2=0;

    //P2OS = 0x06;				 		//输出模式为COMS推挽输出
    PXMOD = Bin(00000011);  			//P2.4不作为RTC脉冲输出口，P3.4不作为IOMUX，P2.1和P2.2为CMOS推挽输出

    P2   = D_P2;
    P2CR = D_P2CR;
    P2PCR= D_P2PCR;
    P2DRV = 0x00;

    P3   = D_P3;
    P3CR = D_P3CR;
    P3PCR= D_P3PCR;
    P3DRV = 0x00;

    P5   = D_P5;
    P5CR = D_P5CR;
    P5PCR= D_P5PCR;

//    MemInitSet(&g_InsBCD.Voltage[0], 0x00, sizeof(g_InsBCD));
    g_Flag.ALARM[0] &= ~E_IReverse;
    LCDCON1= Bin(00000000);   				//降低LCD 功耗
    EA =1;
} */


////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: ACModeWDTProc
** 函数描述: 交流模式下，看门狗复位
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
 void ACModeWDTProc(void)
{
    if((gBCountFlag == 0xA5) && (gBTime0Flag == 0xA5))
    {
        gBCountFlag = 0;
        gBTime0Flag = 0;
        RSTSTAT &= Bin(11111000);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: PDModeWDTProc
** 函数描述: 电池模式下，看门狗复位
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
/* void PDModeWDTProc(void)
{
    if(gBCountFlag == 0xA5)
    {
        gBCountFlag = 0;
        gBTime0Flag = 0;
        RSTSTAT &= Bin(11111000);
    }
} */


////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: StartAdc
** 函数描述:
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
/* unsigned int StartAdc(unsigned char channel)
{
    unsigned char j;
    unsigned int i;
    unsigned int value,tmp;

    i = 5000;
	ADT = 0xFF;
    switch(channel)
    {
	    case ADC_REF:		//REF
		    ADCH = 0x00;
			ADCON = 0x80;
		    ADCON |= 0x0E;
	        break;

		case ADC_VBAT:		//VBAT
			ADCH = 0x00;
	        ADCON = 0x80;
		    ADCON |= 0x08;
	        break;

	    default:
	 	    ADCH = 0x00;
	    	ADCON = 0x00;
            break;
    }

    value = 0;
    for(j = 0; j < 4; j++)
    {
    	ADCON &= 0xBF;
    	ADCON |= 0x01;
	    DelayUs(3);
        while((ADCON&0x01) && (i > 0))
        {
            i--;
	        DelayUs(3);
        }

	    ADCON &= 0xBF;
	    i = 5000;
	    tmp = (unsigned int)ADDH;
	    tmp = tmp<<2;
	    tmp |= (unsigned int)(ADDL & 0x03);
	    value =value + tmp;
	    DelayUs(10);
    }
    ADCON = 0;
	value >>= 2;
    return value;
} */

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: GetVbat
** 函数描述:
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
/* unsigned int GetVbat(void)
{
    unsigned int xdata volage;
    unsigned int xdata volage1;
    unsigned int xdata volage2;

    volage = StartAdc(ADC_REF);
    volage1 = StartAdc(ADC_VBAT);
    volage2 = (unsigned int)((((unsigned long)1800*(unsigned long)volage1) / ((unsigned long)volage))*(unsigned long)2);
	return(volage2);
} */
