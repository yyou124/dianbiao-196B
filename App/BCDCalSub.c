/*==============================================================================
                Copyright(C) 1997-2019.  Sinowealth Tech. Co., Ltd.
--------------------------------------------------------------------------------
Project Name  : BCDCALSUB.c
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
#ifndef  BCDCALSUB_C
	#define  BCDCALSUB_C
#endif
//-------------------------------------------------------------------------------

//---------------


#include "config.h"
#include "Headconfig.h"


////////////////////////////////////////////////////////////////////////////////////////////
//******************************************************************************************
// turn byte data to BCD data
//******************************************************************************************
char Byte_BCD(char Data) small
{
   if ( Data > 99 ) return 0x00;
   return( Data/10*0x10 + Data%10 );
}

////////////////////////////////////////////////////////////////////////////////////////////
//******************************************************************************************
// turn byte data to BCD data
//******************************************************************************************
// unsigned int Byte_BCDH(unsigned char Data) small
// {
//     unsigned char tmp;
//     tmp = Data%100;
//     return( Data/100*0x0100 + tmp/10*0x10 + tmp%10 );
// }

////////////////////////////////////////////////////////////////////////////////////////////
//******************************************************************************************
// turn BCD data to byte data
//******************************************************************************************
// char BCD_Byte(char Data) small
// {
//    return(((Data>>4)&0x0f)*10+(Data&0x0f));
// }

////////////////////////////////////////////////////////////////////////////////////////////
//******************************************************************************************
// turn word data to BCD data
//******************************************************************************************
void Word_BCD2( char *Ptr, unsigned int Data ) small
{
   if ( Data > 9999 )
   {
      *(Ptr+1) = 0x00;
      *Ptr = 0x00;
      return;
   }

   *(Ptr+0) = Byte_BCD( (char)(Data/100) );
   *(Ptr+1) = Byte_BCD( (char)(Data%100) );
}

////////////////////////////////////////////////////////////////////////////////////////////
//******************************************************************************************
// turn byte data to BCD data
//******************************************************************************************
// unsigned int BCD2_Word(unsigned char *Ptr) small
// {
//     return ((int)BCD_Byte(*Ptr)*100 +
//             (int)BCD_Byte(*(Ptr+1)) );
// }
// unsigned long BCD3_Long(char *Ptr2BCD3) small
// {
//     return ((long)BCD_Byte(*(Ptr2BCD3+0))*10000 +
//             (long)BCD_Byte(*(Ptr2BCD3+1))*100 +
//             (long)BCD_Byte(*(Ptr2BCD3+2)) );
// }

////////////////////////////////////////////////////////////////////////////////////////////
//******************************************************************************************
// turn long data to BCD4 data
//******************************************************************************************
// unsigned long BCD4_Long(char *Ptr2BCD4) small
// {
//     return ((long)BCD_Byte(*(Ptr2BCD4+0))*1000000 +
//             (long)BCD_Byte(*(Ptr2BCD4+1))*10000 +
//             (long)BCD_Byte(*(Ptr2BCD4+2))*100 +
//             (long)BCD_Byte(*(Ptr2BCD4+3)) );
// }

////////////////////////////////////////////////////////////////////////////////////////////
//******************************************************************************************
// turn byte data to BCD data
//******************************************************************************************
void Long_BCD4(char *Ptr, unsigned long Data) small
{
    if ( Data > 99999999 )
    {
        *(Ptr+3) = 0x00;
        *(Ptr+2) = 0x00;
        *(Ptr+1) = 0x00;
        *Ptr = 0x00;
        return;
    }
    Word_BCD2(Ptr+0, Data/10000);
    Word_BCD2(Ptr+2, Data%10000);
}

////////////////////////////////////////////////////////////////////////////////////////////
//******************************************************************************************
// turn byte data to BCD data
//******************************************************************************************
void Long_BCD3(char *Ptr,unsigned long Data)  small
{
    if (Data>999999)
    {
        *(Ptr+2)=0x00;
        *(Ptr+1)=0x00;
        *Ptr=0x00;
        return;
    }
    *(Ptr+0)=Byte_BCD(Data/10000);
    Word_BCD2(Ptr+1, Data%10000);
}

////////////////////////////////////////////////////////////////////////////////////////////
//******************************************************************************************
// turn byte data to BCD data
//******************************************************************************************
// unsigned char  AtoB(unsigned char Asc ) small
// {
//     unsigned char Tmp=0x00;

//     if ( Asc >= 0x30 && Asc <= 0x39 )
//         Tmp = Asc - 0x30;
//     else if ( Asc >= 0x41 && Asc <= 0x46 )
//         Tmp = Asc - 0x37;
//     return (Tmp);
// }

////////////////////////////////////////////////////////////////////////////////////////////
//******************************************************************************************
// turn byte data to BCD data
//******************************************************************************************
// unsigned char  BtoA(unsigned char BCD ) small
// {
//     unsigned char data Tmp=0x00;

//     if ( BCD < 10 )
//         Tmp = BCD + 0x30;
//     else if ( BCD >= 10 )
//         Tmp = BCD + 0x37;
//     return (Tmp);
// }

////////////////////////////////////////////////////////////////////////////////////////////
//******************************************************************************************
//   Turn ASCII to BCD data Lenth是Asc数据的长度 固定为BCD的两倍
//******************************************************************************************
// void Asc_BCD(unsigned char xdata *BCD, unsigned char xdata *Asc, unsigned char Lenth )  small
// {
//     unsigned char data i;
//     unsigned char xdata * data Ptr;

//     for ( i=(Lenth>>1); i>0; i-- )
//     {
//         Ptr = BCD + i - 1;
//         *Ptr = AtoB( *Asc++ ) << 4;
//         *Ptr += AtoB( *Asc++ );
//     }
// }

////////////////////////////////////////////////////////////////////////////////////////////
//******************************************************************************************
// turn byte data to BCD data
//******************************************************************************************
// void Asc_BCDA( unsigned char xdata *BCD, unsigned char xdata *Asc, unsigned char Lenth )  small
// {
//     unsigned char data i;
//     unsigned char xdata * data Ptr;

//     Lenth >>= 1;
//     for ( i=0; i<Lenth; i++ )
//     {
//         Ptr = BCD + i ;
//         *Ptr = AtoB( *Asc++ ) << 4;
//         *Ptr += AtoB( *Asc++ );
//     }
// }

////////////////////////////////////////////////////////////////////////////////////////////
//******************************************************************************************
// Turn BCD to ASCII data
//******************************************************************************************
// void BCD_Asc(unsigned char xdata *Asc, unsigned char xdata *BCD ,unsigned char Lenth ) small
// {
//    unsigned char data i;
//    unsigned char xdata * data Ptr;

//     for ( i=Lenth; i>0; i-- )
//     {
//         Ptr = BCD + i - 1;
//         *Asc++ = BtoA( (*Ptr>>4) & 0x0F );
//         *Asc++ = BtoA( *Ptr & 0x0F );
//     }
// }

////////////////////////////////////////////////////////////////////////////////////////////
//******************************************************************************************
// turn byte data to BCD data
//******************************************************************************************
// void BCD_AscA(unsigned char xdata *Asc, unsigned char *BCD ,unsigned char Lenth )  small
// {
//    unsigned char data i;
//    unsigned char * data Ptr;

//     for ( i=0; i<Lenth; i++ )
//     {
//         Ptr = BCD + i;
//         *Asc++ = BtoA( (*Ptr>>4) & 0x0F );
//         *Asc++ = BtoA( *Ptr & 0x0F );
//     }
// }
////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: HexToASCII
** 函数描述: 将一个字节HEX转化为ASCII码
** 输入参数: data_hex待转换数据
** 输出参数: char输出的ASCII码
*******************************************************************************************/
char  HexToASCII(unsigned char  data_hex)
{
 char  ASCII_Data;
 ASCII_Data=data_hex & 0x0F;
 if(ASCII_Data<10)
  ASCII_Data=ASCII_Data+0x30; //‘0--9’
 else
  ASCII_Data=ASCII_Data+0x37;       //‘A--F’
 return ASCII_Data;
}
////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: HexGroupToString
** 函数描述: 将HEX数组转化为字符串
** 输入参数: *OutHexBuffer输出字符串，*InStrBuffer待转换数组，strLength转换长度
** 输出参数: 无
*******************************************************************************************/
void HexGroupToString(char *OutStrBuffer, unsigned char *InHexBuffer, unsigned int HexLength)
{
 unsigned int i, k=0;
 for(i=0;i<HexLength;i++)
 {
  OutStrBuffer[k++]=HexToASCII((InHexBuffer[i]>>4)&0x0F);
  OutStrBuffer[k++]=HexToASCII(InHexBuffer[i]&0x0F);
 }
 OutStrBuffer[k]='\0';
}
////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: StringToHexGroup
** 函数描述: 将字符串转化为HEX数组
** 输入参数: *OutHexBuffer输出数组，*InStrBuffer待转换字符串，strLength转换长度
** 输出参数: 0- 转换错误 1-转换成功
*******************************************************************************************/

unsigned char StringToHexGroup(unsigned char *OutHexBuffer, char *InStrBuffer, unsigned int strLength)
{
 unsigned int i, k=0;
 unsigned char HByte,LByte;
 if(strLength%2 !=0)
  return 0;

 for(i=0; i<strLength; i=i+2)
 {
  if(InStrBuffer[i]>='0' && InStrBuffer[i]<='9')
  {
   HByte=InStrBuffer[i]-'0';
  }
  else if(InStrBuffer[i]>='A' && InStrBuffer[i]<='F')
  {
   HByte=InStrBuffer[i]-'A' +10;
  }
  else
  {
   HByte=InStrBuffer[i];
   return 0;
  }
  HByte=HByte <<4;
  HByte = HByte & 0xF0;
  if(InStrBuffer[i+1]>='0' && InStrBuffer[i+1]<='9')
  {
   LByte=InStrBuffer[i+1]-'0';
  }
  else if(InStrBuffer[i+1]>='A' && InStrBuffer[i+1]<='F')
  {
   LByte=InStrBuffer[i+1]-'A' +10;
  }
  else
  {
   LByte=InStrBuffer[i];
   return 0;
  }
  OutHexBuffer[k++]=HByte |LByte;
 }
 return 1;
}
////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: StrToInt
** 函数描述: 将字符串转化为INT类型
** 输入参数: *Inbuf待转换字符串
** 输出参数: int值
*******************************************************************************************/
unsigned int StrToInt(const char* Inbuf)
{
    unsigned int re = 0;
    unsigned char tmp = 0;
    while (1) {
        if (*Inbuf != 0x00) {
            tmp = tmp * 10 + *Inbuf - '0';
        } else {
            re = (re << 8) + tmp;
            if (*Inbuf == 0x00)
                break;
            tmp = 0;
        }
        Inbuf++;
    }
    return re;
}
