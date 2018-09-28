/*==============================================================================
            Copyright(C) 1997-2019.  Sinowealth Tech. Co., Ltd.
--------------------------------------------------------------------------------
Project No.   : XXX
File Name     : _LCD..H	
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
/*
#ifndef  _LCD_H
#define  _LCD_H


#ifdef _LCD_C
   #define  _LCD_EXT     
#else 
   #define  _LCD_EXT   extern
#endif
*/
//------------------------------------------------------------------------------

#ifndef		_LCD_H
#define		_LCD_H

//Dis_RAM[8]中的显示标志//对应真值表的名称.
#define     LCD_MD                    0x10         //
#define     LCD_PREV                  0x20			//
#define     LCD_1                     0x40			//
#define     LCD_2                     0x80			//
#define     LCD_Imp                   0x01			//
#define     LCD_B                     0x02			//
#define     LCD_4                     0x04			//
#define     LCD_3                     0x08 			//


//Dis_RAM[9]中的显示标志//对应真值表的名称.
#define     LCD_S5                     0x10         //
#define     LCD_S6                     0x20			//
#define     LCD_S7                     0x40			//
#define     LCD_S8                     0x80			//
#define     LCD_L1                     0x01			//
#define     LCD_L2                     0x02			//
#define     LCD_L3                     0x04			//
#define     LCD_S0                     0x08 		//

//Dis_RAM[10]中的显示标志
#define     LCD_S4                     0x10         //
#define     LCD_S3                     0x20			//
#define     LCD_S2                     0x40			//
#define     LCD_S1                     0x80			//


//Dis_RAM[11]中的显示标志//先逻辑对应，然后显示时相与为真则将其SEG或上08.
#define     LCD_S9                     0x80
#define     LCD_C7                     0x40
#define     LCD_C6                     0x20
#define     LCD_C3                     0x10
#define     LCD_C2                     0x08
#define     LCD_C1                     0x04
#define     LCD_L4                     0x02
#define     LCD_C5                     0x01





extern void Init_LCD(void);
extern void ACInit_LCD(void);
extern void PWONInit_LCD(void);
extern void PWOFFInit_LCD(void);   


#endif