/*==============================================================================
            Copyright(C) 1997-2019.  Sinowealth Tech. Co., Ltd.
--------------------------------------------------------------------------------
Project No.   : XXX
File Name     : DISPLAYPROC..H
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





//Display中的显示标志T1~T9对应真值表的名称.
#define	LCD_T1   	(Bin(00000001))           //低位
#define	LCD_T2   	(Bin(00000001))           //
#define	LCD_T3   	(Bin(00000001))           //
#define	LCD_T4   	(Bin(00000001))           //
#define	LCD_T5   	(Bin(00000001))           //
#define	LCD_T6   	(Bin(00000001))           //
#define	LCD_T7   	(Bin(00000001))           //
#define	LCD_T8   	(Bin(00000001))           //
#define	LCD_T9   	(Bin(00000001))           //


//small digital zone
#define	LCD_P4   	(Bin(00001000))           //
#define	LCD_P3   	(Bin(00001000))           //
#define	LCD_P2   	(Bin(00001000))           //
#define	LCD_P1   	(Bin(00001000))           //

#define SEG_A		(Bin(00001000))			  //低位
#define SEG_B		(Bin(00000100))
#define SEG_C		(Bin(00000010))
#define SEG_D		(Bin(00000001))

#define SEG_F		(Bin(01000000))			  //高位
#define SEG_G		(Bin(00100000))
#define SEG_E		(Bin(00010000))

#define SEG_P1		(Bin(00001000))
#define SEG_V       (Bin(00001000))
#define SEG_I       (Bin(00001000))
#define SEG_P       (Bin(00001000))
#define SEG_KWH     (Bin(00001000))

#define CHAR_0		(SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F)
#define CHAR_1		(SEG_B | SEG_C)
#define CHAR_2		(SEG_A | SEG_B | SEG_D | SEG_E | SEG_G)
#define CHAR_3		(SEG_A | SEG_B | SEG_C | SEG_D | SEG_G)
#define CHAR_4		(SEG_B | SEG_C | SEG_F | SEG_G)
#define CHAR_5		(SEG_A | SEG_C | SEG_D | SEG_F | SEG_G)
#define CHAR_6		(SEG_A | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G)
#define CHAR_7		(SEG_A | SEG_B | SEG_C)
#define CHAR_8		(SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G)
#define CHAR_9		(SEG_A | SEG_B | SEG_C | SEG_D | SEG_F | SEG_G)

#define CHAR_A		(SEG_A | SEG_B | SEG_C | SEG_E | SEG_F | SEG_G)
#define CHAR_B		(SEG_C | SEG_D | SEG_E | SEG_F | SEG_G)
#define CHAR_C		(SEG_A | SEG_D | SEG_E | SEG_F)
#define CHAR_D		(SEG_B | SEG_C | SEG_D | SEG_E | SEG_G)
#define CHAR_E		(SEG_A | SEG_D | SEG_E | SEG_F | SEG_G)
#define CHAR_F		(SEG_A | SEG_E | SEG_F | SEG_G)
#define CHAR_G		(SEG_A | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G)
#define CHAR_H		(SEG_B | SEG_C | SEG_E | SEG_F | SEG_G)
#define CHAR_I      0x00
#define CHAR_J		(SEG_B | SEG_C | SEG_D | SEG_E)
#define CHAR_K      (SEG_F | SEG_E | SEG_G)//0x00
#define CHAR_L		(SEG_D | SEG_E | SEG_F)
#define CHAR_M      0x00
#define CHAR_N		(SEG_A | SEG_B | SEG_C | SEG_E | SEG_F)
#define CHAR_O		(SEG_C | SEG_D | SEG_E | SEG_G)
#define CHAR_P		(SEG_A | SEG_B | SEG_E | SEG_F | SEG_G)
#define CHAR_Q      0x00
#define CHAR_R		(SEG_E | SEG_F | SEG_G)
#define CHAR_S		(SEG_A | SEG_C | SEG_D | SEG_F | SEG_G)
#define CHAR_T		(SEG_D | SEG_E | SEG_F | SEG_G)
#define CHAR_U		(SEG_B | SEG_C | SEG_D | SEG_E | SEG_F)
#define CHAR_V		(SEG_B | SEG_F | SEG_G)
#define CHAR_W      0x00
#define CHAR_X      0x00
#define CHAR_Y      0x00
#define CHAR_Z      0x00
#define CHAR_HENGXIAN     (SEG_G)			 //-


//big digital zone
/*
#define DS_BDSEG_A		0x0400
#define DS_BDSEG_B		0x0004
#define DS_BDSEG_C		0x0010
#define DS_BDSEG_D		0x2000
#define DS_BDSEG_E		0x1000
#define DS_BDSEG_F		0x0800
#define DS_BDSEG_G		0x0008

#define DS_BDCHAR_0		(DS_BDSEG_A | DS_BDSEG_B | DS_BDSEG_C | DS_BDSEG_D | DS_BDSEG_E | DS_BDSEG_F)
#define DS_BDCHAR_1		(DS_BDSEG_B | DS_BDSEG_C)
#define DS_BDCHAR_2		(DS_BDSEG_A | DS_BDSEG_B | DS_BDSEG_D | DS_BDSEG_E | DS_BDSEG_G)
#define DS_BDCHAR_3		(DS_BDSEG_A | DS_BDSEG_B | DS_BDSEG_C | DS_BDSEG_D | DS_BDSEG_G)
#define DS_BDCHAR_4		(DS_BDSEG_B | DS_BDSEG_C | DS_BDSEG_F | DS_BDSEG_G)
#define DS_BDCHAR_5		(DS_BDSEG_A | DS_BDSEG_C | DS_BDSEG_D | DS_BDSEG_F | DS_BDSEG_G)
#define DS_BDCHAR_6		(DS_BDSEG_A | DS_BDSEG_C | DS_BDSEG_D | DS_BDSEG_E | DS_BDSEG_F | DS_BDSEG_G)
#define DS_BDCHAR_7		(DS_BDSEG_A | DS_BDSEG_B | DS_BDSEG_C)
#define DS_BDCHAR_8		(DS_BDSEG_A | DS_BDSEG_B | DS_BDSEG_C | DS_BDSEG_D | DS_BDSEG_E | DS_BDSEG_F | DS_BDSEG_G)
#define DS_BDCHAR_9		(DS_BDSEG_A | DS_BDSEG_B | DS_BDSEG_C | DS_BDSEG_D | DS_BDSEG_F | DS_BDSEG_G)
#define DS_BDCHAR_A		(DS_BDSEG_A | DS_BDSEG_B | DS_BDSEG_C | DS_BDSEG_E | DS_BDSEG_F | DS_BDSEG_G)
#define DS_BDCHAR_B		(DS_BDSEG_C | DS_BDSEG_D | DS_BDSEG_E | DS_BDSEG_F | DS_BDSEG_G)
#define DS_BDCHAR_C		(DS_BDSEG_A | DS_BDSEG_D | DS_BDSEG_E | DS_BDSEG_F)
#define DS_BDCHAR_D		(DS_BDSEG_B | DS_BDSEG_C | DS_BDSEG_D | DS_BDSEG_E | DS_BDSEG_G)
#define DS_BDCHAR_E		(DS_BDSEG_A | DS_BDSEG_D | DS_BDSEG_E | DS_BDSEG_F | DS_BDSEG_G)
#define DS_BDCHAR_F		(DS_BDSEG_A | DS_BDSEG_E | DS_BDSEG_F | DS_BDSEG_G)
#define DS_BDCHAR_G		(DS_BDSEG_A | DS_BDSEG_C | DS_BDSEG_D | DS_BDSEG_E | DS_BDSEG_F | DS_BDSEG_G)
#define DS_BDCHAR_H		(DS_BDSEG_B | DS_BDSEG_C | DS_BDSEG_E | DS_BDSEG_F | DS_BDSEG_G)
#define DS_BDCHAR_J		(DS_BDSEG_B | DS_BDSEG_C | DS_BDSEG_D | DS_BDSEG_E)
#define DS_BDCHAR_L		(DS_BDSEG_D | DS_BDSEG_E | DS_BDSEG_F)
#define DS_BDCHAR_N		(DS_BDSEG_A | DS_BDSEG_B | DS_BDSEG_C | DS_BDSEG_E | DS_BDSEG_F)
#define DS_BDCHAR_O		(DS_BDSEG_C | DS_BDSEG_D | DS_BDSEG_E | DS_BDSEG_G)
#define DS_BDCHAR_P		(DS_BDSEG_A | DS_BDSEG_B | DS_BDSEG_E | DS_BDSEG_F | DS_BDSEG_G)
#define DS_BDCHAR_R		(DS_BDSEG_E | DS_BDSEG_G)
#define DS_BDCHAR_S		(DS_BDSEG_A | DS_BDSEG_C | DS_BDSEG_D | DS_BDSEG_F | DS_BDSEG_G)
#define DS_BDCHAR_T		(DS_BDSEG_D | DS_BDSEG_E | DS_BDSEG_F | DS_BDSEG_G)
#define DS_BDCHAR_U		(DS_BDSEG_B | DS_BDSEG_C | DS_BDSEG_D | DS_BDSEG_E | DS_BDSEG_F)
#define DS_BDCHAR_V		(DS_BDSEG_B | DS_BDSEG_F | DS_BDSEG_G)
#define DS_BDCHAR_NEG   (DS_BDSEG_G)

//small digital zone
#define DS_SDSEG_A		0x01000000
#define DS_SDSEG_B		0x00010000
#define DS_SDSEG_C		0x00000100
#define DS_SDSEG_D		0x00000001
#define DS_SDSEG_E		0x00000200
#define DS_SDSEG_F		0x02000000
#define DS_SDSEG_G		0x00020000

#define DS_SDCHAR_0		(DS_SDSEG_A | DS_SDSEG_B | DS_SDSEG_C | DS_SDSEG_D | DS_SDSEG_E | DS_SDSEG_F)
#define DS_SDCHAR_1		(DS_SDSEG_B | DS_SDSEG_C)
#define DS_SDCHAR_2		(DS_SDSEG_A | DS_SDSEG_B | DS_SDSEG_D | DS_SDSEG_E | DS_SDSEG_G)
#define DS_SDCHAR_3		(DS_SDSEG_A | DS_SDSEG_B | DS_SDSEG_C | DS_SDSEG_D | DS_SDSEG_G)
#define DS_SDCHAR_4		(DS_SDSEG_B | DS_SDSEG_C | DS_SDSEG_F | DS_SDSEG_G)
#define DS_SDCHAR_5		(DS_SDSEG_A | DS_SDSEG_C | DS_SDSEG_D | DS_SDSEG_F | DS_SDSEG_G)
#define DS_SDCHAR_6		(DS_SDSEG_A | DS_SDSEG_C | DS_SDSEG_D | DS_SDSEG_E | DS_SDSEG_F | DS_SDSEG_G)
#define DS_SDCHAR_7		(DS_SDSEG_A | DS_SDSEG_B | DS_SDSEG_C)
#define DS_SDCHAR_8		(DS_SDSEG_A | DS_SDSEG_B | DS_SDSEG_C | DS_SDSEG_D | DS_SDSEG_E | DS_SDSEG_F | DS_SDSEG_G)
#define DS_SDCHAR_9		(DS_SDSEG_A | DS_SDSEG_B | DS_SDSEG_C | DS_SDSEG_D | DS_SDSEG_F | DS_SDSEG_G)
#define DS_SDCHAR_A		(DS_SDSEG_A | DS_SDSEG_B | DS_SDSEG_C | DS_SDSEG_E | DS_SDSEG_F | DS_SDSEG_G)
#define DS_SDCHAR_B		(DS_SDSEG_C | DS_SDSEG_D | DS_SDSEG_E | DS_SDSEG_F | DS_SDSEG_G)
#define DS_SDCHAR_C		(DS_SDSEG_A | DS_SDSEG_D | DS_SDSEG_E | DS_SDSEG_F)
#define DS_SDCHAR_D		(DS_SDSEG_B | DS_SDSEG_C | DS_SDSEG_D | DS_SDSEG_E | DS_SDSEG_G)
#define DS_SDCHAR_E		(DS_SDSEG_A | DS_SDSEG_D | DS_SDSEG_E | DS_SDSEG_F | DS_SDSEG_G)
#define DS_SDCHAR_F		(DS_SDSEG_A | DS_SDSEG_E | DS_SDSEG_F | DS_SDSEG_G)
#define DS_SDCHAR_G		(DS_SDSEG_A | DS_SDSEG_C | DS_SDSEG_D | DS_SDSEG_E | DS_SDSEG_F | DS_SDSEG_G)
#define DS_SDCHAR_H		(DS_SDSEG_B | DS_SDSEG_C | DS_SDSEG_E | DS_SDSEG_F | DS_SDSEG_G)
#define DS_SDCHAR_J		(DS_SDSEG_B | DS_SDSEG_C | DS_SDSEG_D | DS_SDSEG_E)
#define DS_SDCHAR_L		(DS_SDSEG_D | DS_SDSEG_E | DS_SDSEG_F)
#define DS_SDCHAR_N		(DS_SDSEG_A | DS_SDSEG_B | DS_SDSEG_C | DS_SDSEG_E | DS_SDSEG_F)
#define DS_SDCHAR_O		(DS_SDSEG_C | DS_SDSEG_D | DS_SDSEG_E | DS_SDSEG_G)
#define DS_SDCHAR_P		(DS_SDSEG_A | DS_SDSEG_B | DS_SDSEG_E | DS_SDSEG_F | DS_SDSEG_G)
#define DS_SDCHAR_R		(DS_SDSEG_E | DS_SDSEG_F | DS_SDSEG_G)
#define DS_SDCHAR_S		(DS_SDSEG_A | DS_SDSEG_C | DS_SDSEG_D | DS_SDSEG_F | DS_SDSEG_G)
#define DS_SDCHAR_T		(DS_SDSEG_D | DS_SDSEG_E | DS_SDSEG_F | DS_SDSEG_G)
#define DS_SDCHAR_U		(DS_SDSEG_B | DS_SDSEG_C | DS_SDSEG_D | DS_SDSEG_E | DS_SDSEG_F)
#define DS_SDCHAR_V		(DS_SDSEG_B | DS_SDSEG_F | DS_SDSEG_G)

//other flags
#define DS_FGCHAR_CoverEnable	0x02 //未发生开端盖事件，SEG2
#define DS_FGCHAR_CoverOpen		0x01 //发生开端盖事件，SEG2
#define DS_FGCHAR_Bypass_L		0x04 //发生旁路状态，SEG2
#define DS_FGCHAR_Bypass_R		0x08 //发生旁路状态，SEG1
#define DS_FGCHAR_Revs			0x08 //发生反向状态，SEG2
#define DS_FGCHAR_Comu			0x01 //通讯符号，SEG1
#define DS_FGCHAR_BAT			0x02 //电池欠压，SEG1
#define DS_FGCHAR_Unit_kW		0x0F //单位kW，SEG0
#define DS_FGCHAR_Unit_V		0x02 //单位V，SEG0
#define DS_FGCHAR_Unit_A		0x18 //单位A，SEG0
#define DS_FGCHAR_Unit_h		0x20 //单位h，SEG15
#define DS_FGCHAR_Dec1			0x20 //数据区从右到左第1个小数点,SEG5
#define DS_FGCHAR_Dec2			0x20 //数据区从右到左第2个小数点,SEG7
#define DS_FGCHAR_Time_LeftUp	0x20 //显示时间的左边上方小数点,SEG13
#define DS_FGCHAR_Time_LeftDn	0x20 //显示时间的左边下方小数点,SEG11
#define DS_FGCHAR_Time_RightUp	0x20 //显示时间的右边上方小数点,SEG0
#define DS_FGCHAR_Time_RightDn	0x20 //显示时间的右边下方小数点,SEG7
#define DS_FGCHAR_OBIS_Dec1		0x02 //OBIS区从右到左第1个小数点,SEG7
#define DS_FGCHAR_OBIS_Dec2		0x02 //OBIS区从右到左第2个小数点,SEG11
#define DS_FGCHAR_OBIS_Dec3		0x02 //OBIS区从右到左第3个小数点,SEG15
*/



//数据存储类型
#define MEMTYPE_RAM    			(0<<4)
#define MEMTYPE_CODE   			(1<<4)
#define MEMTYPE_FRAM   			(2<<4)
#define MEMTYPE_EEPROM 			(3<<4)
#define MEMTYPE_FLASH  			(5<<4)




//数据单位编号index
#define DIS_V		 	    1
#define DIS_I          		2
#define DIS_P				3
#define DIS_kWh 	        4

#define DIS_ERROR           0
/*
#define DIS_kW			    	5
#define DIS_CURMD    	   		6
#define DIS_RREVMD   	   		7
#define DIS_V		   		    8
#define DIS_I       	   		9
#define DIS_L1_I    	  		10
#define DIS_L2_I    			11
#define DIS_Time			    12
#define DIS_Date			    13
#define DIS_ADDRASC   		    14

#define DIS_UNIT_kvarh      	15
#define DIS_UNIT_kVAh       	16
#define DIS_UNIT_kvar       	17
#define DIS_UNIT_kVA        	18
#define DIS_UNIT_Hz         	19
#define DIS_UNIT_DateTime   	20
#define	DIS_UNIT_FULLDISP   	21
#define	DIS_UNIT_EVENT      	22
#define DIS_UNIT_CURMDTIME  	23
#define DIS_UNIT_RREVMDTIME 	24
*/

//OBIS区小数点位置
/*
#define SBMASK  0x07
#define SB1     0x01
#define SB2     0x02
#define SB3     0x04
#define SB12    (SB1|SB2)
#define SB23    (SB2|SB3)
#define SB13    (SB1|SB3)
#define SB123   (SB1|SB2|SB3)
*/

typedef struct
{
	unsigned int    DispPageID;        // 显示项ID号码
    unsigned int    DispDataAddr;      // 显示数据地址
    unsigned char   DispDataIndex;     // 显示数据序号
    unsigned char   DispDataType;      // 显示数据类型,高4位是MEM类型，低4位是数据长度
}DISPSETTINGTABLE;


//------------------------------------------------------------------------------
#ifndef DISPLAYPROC_H
#define DISPLAYPROC_H

#ifndef DISPLAYPROC_C
	#define DISPLAYPROC_EXT	extern
#else
	#define DISPLAYPROC_EXT
#endif
//------------------------------------------------------------------------------

//----------------




DISPLAYPROC_EXT bit gbFgLCDDisp;
DISPLAYPROC_EXT bit gbKeyOnceEn1;
DISPLAYPROC_EXT bit gbKeyOnceEn2;
DISPLAYPROC_EXT bit gbFgKey25ms;


DISPLAYPROC_EXT unsigned char xdata KeyNew;
DISPLAYPROC_EXT unsigned char xdata KeyOld;
DISPLAYPROC_EXT unsigned char xdata gbKeyDebounceCNT0;

DISPLAYPROC_EXT bit gbbattery;							//液晶显示电池是否充满电的情况。

//LCD
DISPLAYPROC_EXT unsigned char displaybutter[32];
DISPLAYPROC_EXT unsigned char xdata ShowLCDBuf[LCDFRAME];
DISPLAYPROC_EXT unsigned char xdata gbFgShowItemNum;


DISPLAYPROC_EXT unsigned char BCDweishu(unsigned char *bcd, unsigned char len);
DISPLAYPROC_EXT void BCDshiftRight(unsigned char *outbcd, unsigned char *inbcd, unsigned char len);

DISPLAYPROC_EXT void DspNum(unsigned char xdata *SegPtr, unsigned char xdata *DataAddr, unsigned char DataAttri,unsigned char Lenth);
//DISPLAYPROC_EXT void AddPointUnit(unsigned char xdata *LCDTmp, unsigned char UnitID);
//DISPLAYPROC_EXT void AddEventUnit(unsigned char xdata *LCDTmp);

DISPLAYPROC_EXT void DisplayProc(void);


#endif