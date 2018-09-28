/*==============================================================================
                Copyright(C) 1997-2019.  ChipNorth Tech. Co., Ltd.
--------------------------------------------------------------------------------
Project Name  : RELAYDrive.h
Description   :
Notice        :
			 1:
			 2:
Author        : YouDeqiang
Start Date    : 2018/05/28
Approve Date  :
Version       : V0.0
Function List :
             1:
             2:
RevisionHistory:
Rev#  CheckSum    Date     Author     Comments(Function+Date)
-----+--------+----------+---------+------------------------------------------
0.0     XXXX   2018/05/28   You        Just build the file
==============================================================================*/
#ifndef	DELAYDRIVER_H
#define	DELAYDRIVER_H

#ifndef DELAYDRIVER_C
	#define DELAYDRIVER_EXT	extern
#else
	#define DELAYDRIVER_EXT
#endif


/*
P5.4 INA
P5.5 INB

*/
#define DELAY_INA			Bin(01000000)
#define DELAY_INB			Bin(10000000)

#define DELAY_INA_H()		P5 |= DELAY_INA		//INA置高
#define DELAY_INA_L()		P5 &= ~DELAY_INA	//INA置低

#define DELAY_INB_H()		P5 |= DELAY_INB		//INB置高
#define DELAY_INB_L()		P5 &= ~DELAY_INB	//INB置低


DELAYDRIVER_EXT unsigned char DelayStatus[2];	  //NB数据UART接收完成FLAG
DELAYDRIVER_EXT unsigned char DelayFlag;
//NBDRIVER_EXT unsigned char idata Data_buffer[40];//NB数据缓冲器


DELAYDRIVER_EXT void RealyCtrl(unsigned char On_Offn);
DELAYDRIVER_EXT void GetRelayStatus(void);

#endif
//------------------------------------------------------------------------------

//----------------
