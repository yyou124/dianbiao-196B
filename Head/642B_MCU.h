/*==============================================================================
            Copyright(C) 1997-2019.  Sinowealth Tech. Co., Ltd.
--------------------------------------------------------------------------------
Project No.   : XXX
File Name     : _642B_MCU..H	
Description   : 
              £»    
              £»   
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

#ifndef  _642B_MCU_H
#define  _642B_MCU_H


#ifdef _642B_MCU_C
   #define  _642B_MCU_EXT     
#else 
   #define  _642B_MCU_EXT   extern
#endif

//------------------------------------------------------------------------------


#define D_CheckVIN  1			 //=1,check power on/off use VIN  NO DEFINE,check power on/off use adc
#define ADC_VBAT        4
#define ADC_REF		    6








_642B_MCU_EXT unsigned char  xdata BlinkFlg;							  //AUTO UPDATA AND SSP
_642B_MCU_EXT unsigned char  xdata UPFirmDely;
_642B_MCU_EXT unsigned char  xdata Ssp_Flag1;


_642B_MCU_EXT unsigned int GetVbat(void);

_642B_MCU_EXT unsigned char Reset_power(void);

_642B_MCU_EXT void CheckPowerOn(void);
//_642B_MCU_EXT void CheckPowerOff(void);
_642B_MCU_EXT void ChangePLLTo32K(void);
_642B_MCU_EXT void Change32KToPLL(void);

_642B_MCU_EXT void Init_IO(void);
_642B_MCU_EXT void Init_PWM(void);
_642B_MCU_EXT void Init_TPS(void);
_642B_MCU_EXT void Init_Foc(void);

_642B_MCU_EXT void ACInit_IO(void);
_642B_MCU_EXT void ACInit_RTC(void);
_642B_MCU_EXT void ACInit_ADC(void);
_642B_MCU_EXT void ACInit_LCD(void);
_642B_MCU_EXT void ACInit_Timer(void);
_642B_MCU_EXT void ACInit_EMUPluse(void);
_642B_MCU_EXT void ACSet_Uart0Baud(void);
_642B_MCU_EXT void ACModeWDTProc(void);

_642B_MCU_EXT void BatSupplySwitch (void);
_642B_MCU_EXT void PwrDownInit_LCD(void); 
_642B_MCU_EXT void PDModeWDTProc(void);


#endif


