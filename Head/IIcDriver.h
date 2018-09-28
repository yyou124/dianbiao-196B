/*==============================================================================
            Copyright(C) 1997-2019.  Sinowealth Tech. Co., Ltd.
--------------------------------------------------------------------------------
Project No.   : XXX
File Name     : IIVDRIVER..H
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
#ifndef	IIVDRIVER_H
#define	IIVDRIVER_H

#ifndef IIVDRIVER_C
	#define IIVDRIVER_EXT	extern
#else
	#define IIVDRIVER_EXT
#endif
//------------------------------------------------------------------------------

//----------------


//iic WP是否有效定义
#define IIC_WP 			 DISABLE//ENABLE////

//口线定义
#if IIC_WP == ENABLE
sbit    WP 		 		 = 	P3^2;				//EEPROM write protected
#define TRIS_WP_HI()     P3CR |= Bin(00000100)	//WP在第2位
#endif

sbit    P_SCL 		 	 =	P2^1;				//IIC SCK 时钟口
#define SCL        		 P_SCL
#define P_SCL_BIT		 Bin(00000010)          //SCL在第1位
#define TRIS_SCL_LO()    P2CR &= ~P_SCL_BIT   	//SCL方向口设置为输入
#define TRIS_SCL_HI()    P2CR |= P_SCL_BIT    	//SCL方向口设置为输出

sbit    P_SDA 		 	 =	P2^2;				//IIC SDA 数据口
#define SDA        		 P_SDA
#define P_SDA_BIT		 Bin(00000100)          //SDA在第2位
#define TRIS_SDA_LO()    P2CR &= ~P_SDA_BIT   	//SDA方向口设置为输入
#define TRIS_SDA_HI()    P2CR |= P_SDA_BIT  	//SDA方向口设置为输出



IIVDRIVER_EXT void Delay1ms(unsigned char Count);
IIVDRIVER_EXT void I2C_delay(void);
IIVDRIVER_EXT void I2C_start(void);
IIVDRIVER_EXT void I2C_stop(void);
IIVDRIVER_EXT bit I2C_WR1byte(unsigned char Data);
IIVDRIVER_EXT unsigned char I2C_RD1byte(bit ACK);


#endif