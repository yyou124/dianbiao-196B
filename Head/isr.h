/*==============================================================================
            Copyright(C) 1997-2019.  Sinowealth Tech. Co., Ltd.
--------------------------------------------------------------------------------
Project No.   : XXX
File Name     : ISR..H	
Description   : 
              ；    
              ；   
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

#ifndef  ISR_H
#define  ISR_H


#ifdef ISR_C
   #define  ISR_EXT     
#else 
   #define  ISR_EXT   extern
#endif
//------------------------------------------------------------------------------

//-----------------

					//中断优先级位预设置
#define                 EX0_PRIORITY                0x01
#define                 T0_PRIORITY                 0x01<<1
#define                 EX1_PRIORITY                0x01<<2
#define                 T1_PRIORITY                 0x01<<3
#define                 UART0_PRIORITY              0x01<<4
#define                 T2_PRIORITY                 0x01<<5
#define                 ADC_PRIORITY                0x01<<6

#define                 EX2_PRIORITY                0x01<<1
#define                 EX3_PRIORITY                0x01<<2
#define                 BT_PRIORITY                 0x01<<3
#define                 TWI_PRIORITY                0x01<<6
#define                 FCOF_PRIORITY               0x01<<7

#endif