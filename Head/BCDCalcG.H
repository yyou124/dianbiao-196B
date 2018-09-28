/*==============================================================================
            Copyright(C) 1997-2019.  Sinowealth Tech. Co., Ltd.
--------------------------------------------------------------------------------
Project No.   : XXX
File Name     : BCDCALCG..H
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
#ifndef  BCDCALCG_H
#define  BCDCALCG_H


#ifdef BCDCALCG_C
   #define  BCDCALCG_EXT
#else
   #define  BCDCALCG_EXT   extern
#endif
//------------------------------------------------------------------------------

//-----------------


//**********************************************************************
// turn byte data to BCD data
//**********************************************************************
BCDCALCG_EXT char         Byte_BCD(char Data) small;
//BCDCALCG_EXT unsigned int Byte_BCDH(unsigned char Data) small;


//**********************************************************************
// turn BCD data to byte data
//**********************************************************************
//BCDCALCG_EXT char BCD_Byte(char Data) small;


//**********************************************************************
// turn word data to BCD data
//**********************************************************************
BCDCALCG_EXT void     Word_BCD2( char *Ptr, unsigned int Data ) small;
//BCDCALCG_EXT unsigned int BCD2_Word(unsigned char *Ptr) small;
//BCDCALCG_EXT unsigned long BCD3_Long(char *Ptr2BCD3) small;


//**********************************************************************
// turn long data to BCD4 data
//**********************************************************************
//BCDCALCG_EXT unsigned long BCD4_Long(char *Ptr2BCD4) small;
BCDCALCG_EXT void     Long_BCD4(char *Ptr, unsigned long Data) small;
BCDCALCG_EXT void     Long_BCD3(char *Ptr,unsigned long Data)  small;

//**********************************************************************
//   Turn ASCII to BCD data Lenth是Asc数据的长度 固定为BCD的两倍
//**********************************************************************
//BCDCALCG_EXT unsigned char  AtoB(unsigned char Asc ) small;
//BCDCALCG_EXT unsigned char  BtoA(unsigned char BCD ) small;


//**********************************************************************
//   Turn ASCII to BCD data Lenth是Asc数据的长度 固定为BCD的两倍
//**********************************************************************
//BCDCALCG_EXT void Asc_BCD(unsigned char xdata *BCD, unsigned char xdata *Asc, unsigned char Lenth )  small;
//BCDCALCG_EXT void Asc_BCDA( unsigned char xdata *BCD, unsigned char xdata *Asc, unsigned char Lenth )  small;


//**********************************************************************
// Turn BCD to ASCII data
//**********************************************************************
//BCDCALCG_EXT void BCD_Asc(unsigned char xdata *Asc, unsigned char xdata *BCD ,unsigned char Lenth ) small;
//BCDCALCG_EXT void BCD_AscA(unsigned char xdata *Asc, unsigned char *BCD ,unsigned char Lenth )  small;

//**********************************************************************
// HEX and ASCII
//**********************************************************************
BCDCALCG_EXT char  HexToASCII(unsigned char  data_hex);
BCDCALCG_EXT void HexGroupToString(char *OutStrBuffer, unsigned char *InHexBuffer, unsigned int HexLength);
BCDCALCG_EXT unsigned char StringToHexGroup(unsigned char *OutHexBuffer, char *InStrBuffer, unsigned int strLength);
BCDCALCG_EXT unsigned int StrToInt(const char* Inbuf);
#endif

