/*==============================================================================
            Copyright(C) 1997-2019.  Sinowealth Tech. Co., Ltd.
--------------------------------------------------------------------------------
Project No.   : XXX
File Name     : IO..H	
Description   : 
              ��    
              ��   
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



#define	    FSYSCLK			    (32768*125)				//ϵͳƵ��4.096M

//sub return value
#define 	TRUE 	1
#define 	FALSE 	0

#define 	READPOWERCLSFF			0x56				//any num
#define 	READOPENTOPFF 			0x89				//any num


#define     InFactoryMode()         (gbFgKeyProg == 0xF001)
#define     InFactoryClsMode()      (gbFgClsKeyProg == 0xF002)

#define     KEY_BC               0//P5_4		            //�������

#define     KEY_DISP_SET()       {(P2CR &= Bin(10111111));(P2PCR |= Bin(01000000));}	  //p2.6,int3
#define     KEY_DISP()           (Bin(01000000) == (P2 & Bin(01000000))) 					//��ʾ����

#define     KEY_PRO_SET()        {(P3CR &= Bin(11110111));(P3PCR |= Bin(00001000));}	  //p3.3
#define 	KEY_PRO()            (Bin(00001000) == (P3 & Bin(00001000)))					//��̿���	  //P3.3

#define     KEY_ADJ_SET()        {(P3CR &= Bin(11110111));(P3PCR |= Bin(00001000));}	  //p3.3
#define 	KEY_ADJ()            (Bin(00001000) == (P3 & Bin(00001000)))					//У����	  //P3.3

#define     KEY_CLS_SET()        {(P3CR &= Bin(11101111));(P3PCR |= Bin(00010000));}	  //3.4
#define 	KEY_CLS()            (Bin(00010000) == (P3 & Bin(00010000)))					//���㿪��	  //P3.4


#define     FAULT_LED            P5				        //�Ե�LED	  EARTH LED
#define     FAULT_LED_ON         Bin(00010000)			//�Ե�LED	  P5_4

#define     ALARM_LED            P5				        //����LED
#define     ALARM_LED_ON         Bin(00100000)			//����LED	  P5_5

#define     REV_LED              P2  				    //����������LED
#define     REV_LED_ON           Bin(00100000)		    //����������LED	 P2_5


#define     KEY_DG               P3_6			        					//�˸ǿ���
#define     KEY_BG               (Bin(10000000) == (P5 & Bin(10000000)))	//��ǿ���
#define 	KEY_CLR()            (Bin(00100000) == (P5 & Bin(00100000)))	//���㿪��


#define     BG_LED               P2_1						//����
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
#define 	D_LCD_SHOW_HEX		  0			     		//LCD��Ҫ��ʾ�����ݵ�������HEX����BCD
#define 	D_LCD_SHOW_BCD		  1			    

//����ʾ9��8����18��SEGMENT�������ҿ�ʼ��ʾ������SEGMENT��ͬ��ʾһ��8��
#define 	D_LCD_SHOW_PLACE_1	  0              		//����SEGMNET��ͬ��ʾһ��8��
#define 	D_LCD_SHOW_PLACE_2	  2              		//����SEGMNET��ͬ��ʾһ��8��
#define 	D_LCD_SHOW_PLACE_3	  4              		//����SEGMNET��ͬ��ʾһ��8��
#define 	D_LCD_SHOW_PLACE_4	  6              		//����SEGMNET��ͬ��ʾһ��8��
#define 	D_LCD_SHOW_PLACE_5	  8              		//����SEGMNET��ͬ��ʾһ��8��
#define 	D_LCD_SHOW_PLACE_6	  10             		//����SEGMNET��ͬ��ʾһ��8��
#define 	D_LCD_SHOW_PLACE_7	  12             		//����SEGMNET��ͬ��ʾһ��8��
#define 	D_LCD_SHOW_PLACE_8	  14             		//����SEGMNET��ͬ��ʾһ��8��
#define 	D_LCD_SHOW_PLACE_9	  16             		//����SEGMNET��ͬ��ʾһ��8��

//07��Լ������
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
