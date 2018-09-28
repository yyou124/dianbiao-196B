/*==============================================================================
            Copyright(C) 1997-2019.  Sinowealth Tech. Co., Ltd.
--------------------------------------------------------------------------------
Project No.   : XXX
File Name     : EEPROM..H
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
#ifndef	EEPROM_H
#define	EEPROM_H

#ifndef EEPROM_C
	#define EEPROM_EXT	extern
#else
	#define EEPROM_EXT
#endif
//------------------------------------------------------------------------------

//EEPROM ADDRESS
#define EE_ADDRESS_WRITE    0xA0
#define EE_ADDRESS_READ     0xA1
//EEPROM信息(包括其EEPROM地址定义区，页)
#define D_EEP_BUF_LEN       64          			//eeprom 的操作的BUFF的长度
#define EE_SIZE             64                		//64
#define EE_PAGE             512	                    //2Kb EEPROM空间，一共8页

#define EE_BAKADDR        	0x4000U		          	//EEPROM数据备份地址
//#define EE_SAM_ENDDATE      0x0000           ///2+2B 结算日期,即自动抄表日2

//---------制造信息(包括软件版本号+硬件版本号+厂家编号等)    PAGE 186
//#define EE_hard_code		0x2E80			//32+2B 硬件版本号
//#define EE_soft_code      0x0000          //软件版本号

//---------通讯地址      PAGE 186
// #define EE_Meter_address	0x0120					//通信地址
// #define EE_MASUM        	EE_Meter_address+12  	//12+2
// #define EE_MAXOR            EE_MASUM+1
// #define	EE_PROG_FLAG				0x01A0u			//编程标志	 ,2字节
// #define	EE_FirstProg_FLAG			0x01ACu		    //第一次上电标志,4字节
//---------正向有功电量电量数据地址 PAGE 181
#define	EE_KWH0				0x2D40u					//当前总有功电量整数低位
#define	EE_KWH2				EE_KWH0+1
#define	EE_KWH4				EE_KWH2+1
#define	EE_KWHSUM			EE_KWH4+1               //SUM校验码
#define	EE_KWHXOR			EE_KWHSUM+1				//XOR校验码

#define EE_KWH0_R1          EE_KWHXOR+1             //费率1电量
#define	EE_KWH2_R1			EE_KWH0_R1+1
#define	EE_KWH4_R1			EE_KWH2_R1+1
#define	EE_KWHSUM_R1		EE_KWH4_R1+1
#define	EE_KWHXOR_R1		EE_KWHSUM_R1+1

#define EE_KWH0_R2          EE_KWHXOR_R1+1           //费率2电量
#define	EE_KWH2_R2			EE_KWH0_R2+1
#define	EE_KWH4_R2			EE_KWH2_R2+1
#define	EE_KWHSUM_R2		EE_KWH4_R2+1
#define	EE_KWHXOR_R2		EE_KWHSUM_R2+1

#define EE_KWH0_R3          EE_KWHXOR_R2+1           //费率3电量
#define	EE_KWH2_R3			EE_KWH0_R3+1
#define	EE_KWH4_R3			EE_KWH2_R3+1
#define	EE_KWHSUM_R3		EE_KWH4_R3+1
#define	EE_KWHXOR_R3		EE_KWHSUM_R3+1

#define EE_KWH0_R4          EE_KWHXOR_R3+1           //费率4电量
#define	EE_KWH2_R4			EE_KWH0_R4+1
#define	EE_KWH4_R4			EE_KWH2_R4+1
#define	EE_KWHSUM_R4		EE_KWH4_R4+1
#define	EE_KWHXOR_R4		EE_KWHSUM_R4+1

//---------当前正向有功小数点电量地址 PAGE 182
#define	EE_KWHP 			0x2D80u    			    //当前正向总有功电量脉冲数
#define	EE_KWHD 			EE_KWHP+1  			    //当前正向总有功电量小数
#define EE_KWHCHK           EE_KWHD+1               //当前正向总有功电量脉冲、小数校验

#define	EE_KWHP_R1 			E_KWHCHK+1    		    //费率1
#define	EE_KWHD_R1 			EE_KWHP_R1+1
#define EE_KWHCHK_R1        EE_KWHD_R1+1

#define	EE_KWHP_R2 			EE_KWHCHK_R1+1    	    //费率2
#define	EE_KWHD_R2 			EE_KWHP_R2+1
#define EE_KWHCHK_R2        EE_KWHD_R2+1

#define	EE_KWHP_R3 			EE_KWHCHK_R2+1    	    //费率3
#define	EE_KWHD_R3 			EE_KWHP_R3+1
#define EE_KWHCHK_R3        EE_KWHD_R3+1

#define	EE_KWHP_R4 			EE_KWHCHK_R3+1    		//费率4
#define	EE_KWHD_R4 			EE_KWHP_R4+1
#define EE_KWHCHK_R4        EE_KWHD_R4+1

//---------第一套费率时段  PAGE 183
#define EE_0A_TIME1         0x2DC0u                  //第一时段
#define EE_0A_TIME2         EE_0A_TIME1+1
#define EE_0A_TIME3         EE_0A_TIME2+1
#define EE_0A_TIME4         EE_0A_TIME3+1
#define EE_0A_TIME5         EE_0A_TIME4+1
#define EE_0A_TIME6         EE_0A_TIME5+1
#define EE_0A_TIME7         EE_0A_TIME6+1
#define EE_0A_TIME8         EE_0A_TIME7+1
#define EE_0A_TIME9         EE_0A_TIME8+1
#define EE_0A_TIME10        EE_0A_TIME9+1
#define EE_0A_TIME11        EE_0A_TIME10+1
#define EE_0A_TIME12        EE_0A_TIME11+1
#define EE_0A_TIME13        EE_0A_TIME12+1
#define EE_0A_TIME14        EE_0A_TIME13+1
#define EE_0A_SUM           EE_0A_TIME14+1
#define EE_0A_XOR           EE_0A_SUM+1

//---------第二套费率时段  PAGE 184
#define EE_AA_TIME1         0x2E00u                  //第一时段
#define EE_AA_TIME2         EE_AA_TIME1+1
#define EE_AA_TIME3         EE_AA_TIME2+1
#define EE_AA_TIME4         EE_AA_TIME3+1
#define EE_AA_TIME5         EE_AA_TIME4+1
#define EE_AA_TIME6         EE_AA_TIME5+1
#define EE_AA_TIME7         EE_AA_TIME6+1
#define EE_AA_TIME8         EE_AA_TIME7+1
#define EE_AA_TIME9         EE_AA_TIME8+1
#define EE_AA_TIME10        EE_AA_TIME9+1
#define EE_AA_TIME11        EE_AA_TIME10+1
#define EE_AA_TIME12        EE_AA_TIME11+1
#define EE_AA_TIME13        EE_AA_TIME12+1
#define EE_AA_TIME14        EE_AA_TIME13+1
#define EE_AA_SUM           EE_AA_TIME14+1
#define EE_AA_XOR           EE_AA_SUM+1

//---------校表参数  PAGE 185
#define ADJ_ADDR    			0x2E40u        		//校表参数37个字节+2个校验码
#define	ADJ_ADDR_SUM			ADJ_ADDR+1              //校验码
#define	ADJ_ADDR_XOR			ADJ_ADDR+1			//XOR校验码

//---------掉电储存     PAGE 187    0x2EC0


//---------上一小时总电量数据储存  PAGE 188  0x2F00
#define LAST_HOUR_MIN       0x2F00u                 //储存小时数
#define LAST_HOUR_HOUR      LAST_HOUR_MIN+1         //储存分钟
#define LAST_HOUR_DAY       LAST_HOUR_HOUR+1        //储存日期
#define LAST_HOUR_F1I1      LAST_HOUR_DAY+1         //费率1电量整数部分1
#define LAST_HOUR_F1I2      LAST_HOUR_F1I1+1        //费率1电量整数部分2
#define LAST_HOUR_F1I3      LAST_HOUR_F1I2+1        //费率1电量整数部分3
#define LAST_HOUR_F1D       LAST_HOUR_F1I3+1        //费率1电量小数部分

#define LAST_HOUR_F2I1      LAST_HOUR_F1D+1
#define LAST_HOUR_F2I2      LAST_HOUR_F2I1+1
#define LAST_HOUR_F2I3      LAST_HOUR_F2I2+1
#define LAST_HOUR_F2D       LAST_HOUR_F2I3+1

#define LAST_HOUR_F3I1      LAST_HOUR_F2D+1
#define LAST_HOUR_F3I2      LAST_HOUR_F3I1+1
#define LAST_HOUR_F3I3      LAST_HOUR_F3I2+1
#define LAST_HOUR_F3D       LAST_HOUR_F3I3+1

#define LAST_HOUR_F4I1      LAST_HOUR_F3D+1
#define LAST_HOUR_F4I2      LAST_HOUR_F4I1+1
#define LAST_HOUR_F4I3      LAST_HOUR_F4I2+1
#define LAST_HOUR_F4D       LAST_HOUR_F4I3+1

#define LAST_HOUR_SUM       LAST_HOUR_F4D+1
#define LAST_HOUR_XOR       LAST_HOUR_SUM+1


//---------费率切换时间 PAGE 189      0x2F40
#define EE_SWITCH1_MOUTH    0x2F40u         //第一时间节点，月
#define EE_SWITCH1_DAY      0x2F41u         //第一时间节点，日
#define EE_SWITCH1_HOUR     0x2F42u         //第一时间节点，时
#define EE_SWITCH1_MIN      0x2F43u         //第一时间节点，时
#define EE_SWITCH1_FL       0x2F44u         //第一时间节点，费率
#define EE_SWITCH1_SUM      0x2F45u
#define EE_SWITCH1_XOR      0x2F46u

#define EE_SWITCH2_MOUTH    0x2F47u         //第二时间节点，月
#define EE_SWITCH2_DAY      0x2F48u         //第二时间节点，日
#define EE_SWITCH2_HOUR     0x2F49u         //第二时间节点，时
#define EE_SWITCH2_MIN      0x2F4Au         //第二时间节点，时
#define EE_SWITCH2_FL       0x2F4Bu         //第二时间节点，费率
#define EE_SWITCH2_SUM      0x2F4Cu
#define EE_SWITCH2_XOR      0x2F4Du

//--------------30天每小时电量储存
#define D1_H01_MIN          EE_INIT_ADDRESS
#define D1_H01_HOUR         D1_H01_MIN+1
#define D1_H01_DAY          D1_H01_HOUR+1
#define D1_H01_F1I          D1_H01_DAY+1
#define D1_H01_F1D          D1_H01_F1I+1
#define D1_H01_F2I          D1_H01_F1D+1
#define D1_H01_F2D          D1_H01_F2I+1
#define D1_H01_F3I          D1_H01_F2D+1
#define D1_H01_F3D          D1_H01_F3I+1
#define D1_H01_F4I          D1_H01_F3D+1
#define D1_H01_F4D          D1_H01_F4I+1
#define D1_H01_SUM          D1_H01_F4D+1
#define D1_H01_XOR          D1_H01_SUM+1

/*
单页储存4小时电量数据，需储存30*24小时电量数据，共计180页
每小时数据地址分配：
01~04点： 起始地址+(天数-1)*0180H+(小时数-1)*000DH+0000H
05~08点： 起始地址+(天数-1)*0180H+(小时数-5)*000DH+0040H
09~12点： 起始地址+(天数-1)*0180H+(小时数-9)*000DH+0080H
13~16点： 起始地址+(天数-1)*0180H+(小时数-13)*000DH+00C0H
17~20点： 起始地址+(天数-1)*0180H+(小时数-17)*000DH+0100H
21~24点： 起始地址+(天数-1)*0180H+(小时数-21)*000DH+0140H

*/
#define EE_INIT_ADDRESS     0x0040u     //每小时电量储存起始地址
#define EE_DAY_SWITCH1      0x0010u     //天数偏移地址1
#define EE_DAY_SWITCH2      0x0011u     //天数偏移地址2
#define EE_DAY_SWITCH3      0x0012u     //天数偏移地址3


#define     CMon_Time           0x0444            //6+2
#define		mUserASCII8			0x0450 		         	//10
#define		mUserASCII8_2		0x0460 			        //10
#define		mMeterInfo		    0x0470		            //8
#define 	FuncSW	        	0x047B		        	//2 功能开关2+2CRC
#define		SysSW				0x0480			    	//4 系统开关4+2CRC

#define     MDINV               0x0490
#define     CM_MD               0x0493
#define     CM_MANUAL_MD        0x0497
#define     LMON_MD             0x049B

#define     EventFlag           0x04B1
#define     DemandBillFlg       0x04B4
#define     DISINV              0x04B7

#define     LMON_EC             0x04C0
#define     CODECHECK           0x04F0
#define     EN_TCHK             0x04F4
#define     ByPassThres         0x04F8
#define     ReverseThres        0x04FC
#define     PwrDnBill           0x0500
#define     PwrDnDD             0x0508
#define     FacNUM				0x050C


#define     CNT_BLANK           (0x0520 + 4 + 2)
#define     CNT_RST             (CNT_BLANK)



EEPROM_EXT unsigned char EEPromByteRead(unsigned char EE_addrH, unsigned char EE_addrL);
EEPROM_EXT void EEPromByteProgram(unsigned char nAddrH,unsigned char nAddrL, unsigned char nData);
EEPROM_EXT void EEPromSectorErase(unsigned char nAddrH);

EEPROM_EXT void VER_CHK(unsigned int EE_addr, unsigned char N);
EEPROM_EXT bit SEQ_write(unsigned int EE_addr, unsigned char *EE_buff, unsigned char N);
EEPROM_EXT bit EE_to_RAM(unsigned int EE_addr, unsigned char *EE_buff, unsigned char N);
EEPROM_EXT unsigned char VER_RDbytes(unsigned int EE_addr, unsigned char *EE_buff, unsigned char N);
EEPROM_EXT void VER_WRbytes(unsigned int EE_addr, unsigned char *EE_buff, unsigned char N, unsigned char firstOK);
EEPROM_EXT void SUM_xorb(unsigned char *RAM,unsigned char N);
EEPROM_EXT void MemInitSet(char *dst, unsigned char value, unsigned int count);


//special function for interup
EEPROM_EXT unsigned char EEPromByteRead_INT(unsigned char EE_addrH, unsigned char EE_addrL);
EEPROM_EXT void EEPromByteProgram_INT(unsigned char nAddrH,unsigned char nAddrL, unsigned char nData);

EEPROM_EXT bit SEQ_write_INT(unsigned int EE_addr, unsigned char *EE_buff, unsigned char N);
EEPROM_EXT bit EE_to_RAM_INT(unsigned int EE_addr, unsigned char *EE_buff, unsigned char N);
EEPROM_EXT unsigned char VER_RDbytes_INT(unsigned int EE_addr, unsigned char *EE_buff, unsigned char N);
EEPROM_EXT void VER_WRbytes_INT(unsigned int EE_addr, unsigned char *EE_buff, unsigned char N, unsigned char firstOK);
EEPROM_EXT void SUM_xorb_INT(unsigned char *RAM,unsigned char N);


#endif