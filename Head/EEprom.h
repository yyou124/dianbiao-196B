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

//----------------

// EEPROM ADDRESS
/*-------------------------------EEPROM信息(包括其EEPROM地址定义区，页)-----------------------*/
#define D_EEP_BUF_LEN       50          			//eeprom 的操作的BUFF的长度
#define EE_SIZE             8                		//8bit一页  24c04
#define EE_PAGE             512	                    //2Kbit EEPROM空间，一共256页
#define MAX_EE_SIZE         16                      //EEPROM一次性最大写入地址长度
#define	FRAM_EEPROM			0xA0                    //EEPROM的通讯地址
//EEPROM PAGE ADDRESS: 0X000 ~ 0X1FF
#define Meter_Factory_Broad 0x99
#define Meter_Factory_Def   0x00

/*-------------------------------内部类EEPROM使用情况----------------------------------------*/
//电量小数位地址
#define EE_PowerDown        0x0500                  //掉电FLAG
#define	EE_KWHD 			0x0531u    			    //当前正向总有功电量脉冲数
#define	EE_KWHP 			EE_KWHD+1  			    //当前正向总有功电量小数
#define EE_KWHCHK           EE_KWHP+1               //当前正向总有功电量脉冲、小数校验

/*-------------------------------外部EEPROM使用情况----------------------------------------*/
#define EE_BAKADDR        	0x0100U                 //EEPROM数据备份地址
//总用电量地址  5+2字节                     0x0001~0x0007
#define EE_KWH_SHIFT        8              //电量存储偏移地址
#define EE_KWH_MAX          90             //多少用电量更换EEPROM地址（单位 万度）
#define EE_KWH_NUM          (ADJ_ADDR+1)/8  //根据当前所能占用的最大存储空间计算


#define	EE_KWH0				0x0001					//当前总有功电量整数低位
#define	EE_KWH1				EE_KWH0+1 				//5+2
#define	EE_KWH2				EE_KWH1+1
#define	EE_KWH3				EE_KWH2+1
#define	EE_KWH4				EE_KWH3+1
#define	EE_KWHSUM			EE_KWH4+1               //SUM校验码
#define	EE_KWHXOR			EE_KWHSUM+1				//XOR校验码

//校表参数(不包括浮点数) 13+2字节           0x00A0~0x00AC
#define ADJ_ADDR    		0x00A0u        		    //校表参数
#define	ADJ_ADDR_SUM		ADJ_ADDR+13             //校验码
#define	ADJ_ADDR_XOR		ADJ_ADDR+13			    //XOR校验码

//编程标志  2+2字节                        0x00AC~0x00AF
#define EE_PROG_FLAG        0x00BCu
//校表参数(浮点型)--功率 4+2字节            0x00C0~0x00C5
#define ADJ_ADDR_POWER2GAIN 0x00C0u
//第一次上电标志 4+2字节                    0x00C6~0x00CB
#define	EE_FirstProg_FLAG	0x00C6u
//继电器通断电标志 2+2字节                  0x00CC~0x00CF
#define RELAY_STATUS        0x00CC                  //当前设备继电器状态
//通讯模块安装标志 2+2字节                  0x00D1~0x00D4
#define EE_NB_LORA          0x00D1                  // LORA=>AA NB=> BB
//NB初始化状态 2+2字节                      0x00D7~0x00DA
#define EE_NB_STATE         0x00D7
//通信地址  6+2字节                         0x00E0~0x00E7
#define EE_Meter_address	0x00E0
//厂家编号  1字节
#define EE_Meter_Factory    0x00E9
//软件版本号    1字节
#define EE_soft_code        0x00EB
//硬件版本号    1字节
#define EE_hard_code        0x00ED
//计算后的用电量存储地址
#define EE_KWH_ADDRESS      0x00F0
//事件FLAG 2字节
//#define     EventFlag       0x00F0

#define     CNT_BLANK           (0x00F2 + 4 + 2)
#define     CNT_RST             (CNT_BLANK)



EEPROM_EXT unsigned char EEPromByteRead(unsigned char EE_addrH, unsigned char EE_addrL);
EEPROM_EXT void EEPromByteProgram(unsigned char nAddrH,unsigned char nAddrL, unsigned char nData);
EEPROM_EXT void EEPromSectorErase(unsigned char nAddrH);

EEPROM_EXT void VER_CHK(unsigned int EE_addr, unsigned char N);
EEPROM_EXT bit SEQ_write(unsigned int EE_addr, unsigned char *EE_buff, unsigned char N);
EEPROM_EXT bit SEQ_write_limit(unsigned int EE_addr, unsigned char *EE_buff, unsigned char N);
EEPROM_EXT bit EE_to_RAM(unsigned int EE_addr, unsigned char *EE_buff, unsigned char N);
EEPROM_EXT unsigned char VER_RDbytes(unsigned int EE_addr, unsigned char *EE_buff, unsigned char N);
EEPROM_EXT void VER_WRbytes(unsigned int EE_addr, unsigned char *EE_buff, unsigned char N, unsigned char firstOK);
EEPROM_EXT void VER_WRbytes_limit(unsigned int EE_addr, unsigned char *EE_buff, unsigned char N, unsigned char firstOK);
EEPROM_EXT void SUM_xorb(unsigned char *RAM,unsigned char N);
EEPROM_EXT void MemInitSet(char *dst, unsigned char value, unsigned int count);


//special function for interup
EEPROM_EXT void EEPromByteProgram_INT(unsigned char nAddrH,unsigned char nAddrL, unsigned char nData);



#endif