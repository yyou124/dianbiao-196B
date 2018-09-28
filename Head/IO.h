/*==============================================================================
            Copyright(C) 1997-2019.  Sinowealth Tech. Co., Ltd.
--------------------------------------------------------------------------------
Project No.   : XXX
File Name     : IO..H	
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

#ifndef  IO_H
#define  IO_H


#ifdef IO_C
   #define  IO_EXT     
#else 
   #define  IO_EXT   extern
#endif

//------------------------------------------------------------------------------



#define	    FSYSCLK			    (32768*125)				//系统频率4.096M

//sub return value
#define 	TRUE 	1
#define 	FALSE 	0

#define 	READPOWERCLSFF			0x56				//any num
#define 	READOPENTOPFF 			0x89				//any num


#define     InFactoryMode()         (gbFgKeyProg == 0xF001)
#define     InFactoryClsMode()      (gbFgClsKeyProg == 0xF002)

#define     KEY_BC               0//P5_4		            //编程输入

#define     KEY_DISP_SET()       {(P2CR &= Bin(10111111));(P2PCR |= Bin(01000000));}	  //p2.6,int3
#define     KEY_DISP()           (Bin(01000000) == (P2 & Bin(01000000))) 					//显示按键

#define     KEY_PRO_SET()        {(P3CR &= Bin(11110111));(P3PCR |= Bin(00001000));}	  //p3.3
#define 	KEY_PRO()            (Bin(00001000) == (P3 & Bin(00001000)))					//编程开关	  //P3.3

#define     KEY_ADJ_SET()        {(P3CR &= Bin(11110111));(P3PCR |= Bin(00001000));}	  //p3.3
#define 	KEY_ADJ()            (Bin(00001000) == (P3 & Bin(00001000)))					//校表开关	  //P3.3

#define     KEY_CLS_SET()        {(P3CR &= Bin(11101111));(P3PCR |= Bin(00010000));}	  //3.4
#define 	KEY_CLS()            (Bin(00010000) == (P3 & Bin(00010000)))					//清零开关	  //P3.4


#define     FAULT_LED            P5				        //窃电LED	  EARTH LED
#define     FAULT_LED_ON         Bin(00010000)			//窃电LED	  P5_4

#define     ALARM_LED            P5				        //报警LED
#define     ALARM_LED_ON         Bin(00100000)			//报警LED	  P5_5

#define     REV_LED              P2  				    //电流正反向LED
#define     REV_LED_ON           Bin(00100000)		    //电流正反向LED	 P2_5


#define     KEY_DG               P3_6			        					//端盖开关
#define     KEY_BG               (Bin(10000000) == (P5 & Bin(10000000)))	//表盖开关
#define 	KEY_CLR()            (Bin(00100000) == (P5 & Bin(00100000)))	//清零开关


#define     BG_LED               P2_1						//背光
#define		BLACK_LED_OPEN()	 do{P2CR |= BIT1;BG_LED = 1;} while(0)
#define 	BLACK_LED_CLOSE()	 do{P2CR |= BIT1;BG_LED = 0;} while(0)






#define		MIN_UART_DATA_LEN		12					//0x68,A0,A1,A2,A3,A4,A5,0x68,C,L,CS,0x16
//#define		MAX_UART_DATA_LEN		75	

//uart receive and transmit command
#define		DLT645_WAKEUP_CODE		0xFE
#define		DLT645_HEAD_CODE		0x68
#define		DLT645_BRD_CODE			0xAA
#define		DLT645_END_CODE			0x16

#define 	MODIFY485BAUDRATEORDER  0xA5				//modify RS485 baudrate order


//LCD 
#define 	D_LCD_SHOW_HEX		  0			     		//LCD需要显示的数据的属性是HEX还是BCD
#define 	D_LCD_SHOW_BCD		  1			    

//共显示9个8，共18个SEGMENT从左向右开始显示，两个SEGMENT共同显示一个8字
#define 	D_LCD_SHOW_PLACE_1	  0              		//两个SEGMNET共同显示一个8字
#define 	D_LCD_SHOW_PLACE_2	  2              		//两个SEGMNET共同显示一个8字
#define 	D_LCD_SHOW_PLACE_3	  4              		//两个SEGMNET共同显示一个8字
#define 	D_LCD_SHOW_PLACE_4	  6              		//两个SEGMNET共同显示一个8字
#define 	D_LCD_SHOW_PLACE_5	  8              		//两个SEGMNET共同显示一个8字
#define 	D_LCD_SHOW_PLACE_6	  10             		//两个SEGMNET共同显示一个8字
#define 	D_LCD_SHOW_PLACE_7	  12             		//两个SEGMNET共同显示一个8字
#define 	D_LCD_SHOW_PLACE_8	  14             		//两个SEGMNET共同显示一个8字
#define 	D_LCD_SHOW_PLACE_9	  16             		//两个SEGMNET共同显示一个8字

//07规约的密码
#define 	PassWordLevel_2			0x02				//2 level password
#define 	PassWordLevel_4			0x04				//4 level password
#define 	PassWordLevel_6		    0x06				//6 level password 

//DLT645 Error Uint
#define		DLT_ERR_OTHER			0x01
#define		DLT_ERR_NODATA			0x02
#define		DLT_ERR_PASSWORD		0x04
#define		DLT_ERR_COMMUNICATION	0x08
#define		DLT_ERR_YEAR			0x10
#define		DLT_ERR_DAY				0x20
#define		DLT_ERR_RATE			0x40
#define		DLT_ERR_POWERDOWN		0x80





#endif
