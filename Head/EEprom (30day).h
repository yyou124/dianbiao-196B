/*==============================================================================
            Copyright(C) 1997-2019.  Sinowealth Tech. Co., Ltd.
--------------------------------------------------------------------------------
Project No.   : XXX
File Name     : EEPROM..H
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
//EEPROM��Ϣ(������EEPROM��ַ��������ҳ)
#define D_EEP_BUF_LEN       64          			//eeprom �Ĳ�����BUFF�ĳ���
#define EE_SIZE             64                		//64
#define EE_PAGE             512	                    //2Kb EEPROM�ռ䣬һ��8ҳ

#define EE_BAKADDR        	0x4000U		          	//EEPROM���ݱ��ݵ�ַ
//#define EE_SAM_ENDDATE      0x0000           ///2+2B ��������,���Զ�������2

//---------������Ϣ(��������汾��+Ӳ���汾��+���ұ�ŵ�)    PAGE 186
//#define EE_hard_code		0x2E80			//32+2B Ӳ���汾��
//#define EE_soft_code      0x0000          //����汾��

//---------ͨѶ��ַ      PAGE 186
// #define EE_Meter_address	0x0120					//ͨ�ŵ�ַ
// #define EE_MASUM        	EE_Meter_address+12  	//12+2
// #define EE_MAXOR            EE_MASUM+1
// #define	EE_PROG_FLAG				0x01A0u			//��̱�־	 ,2�ֽ�
// #define	EE_FirstProg_FLAG			0x01ACu		    //��һ���ϵ��־,4�ֽ�
//---------�����й������������ݵ�ַ PAGE 181
#define	EE_KWH0				0x2D40u					//��ǰ���й�����������λ
#define	EE_KWH2				EE_KWH0+1
#define	EE_KWH4				EE_KWH2+1
#define	EE_KWHSUM			EE_KWH4+1               //SUMУ����
#define	EE_KWHXOR			EE_KWHSUM+1				//XORУ����

#define EE_KWH0_R1          EE_KWHXOR+1             //����1����
#define	EE_KWH2_R1			EE_KWH0_R1+1
#define	EE_KWH4_R1			EE_KWH2_R1+1
#define	EE_KWHSUM_R1		EE_KWH4_R1+1
#define	EE_KWHXOR_R1		EE_KWHSUM_R1+1

#define EE_KWH0_R2          EE_KWHXOR_R1+1           //����2����
#define	EE_KWH2_R2			EE_KWH0_R2+1
#define	EE_KWH4_R2			EE_KWH2_R2+1
#define	EE_KWHSUM_R2		EE_KWH4_R2+1
#define	EE_KWHXOR_R2		EE_KWHSUM_R2+1

#define EE_KWH0_R3          EE_KWHXOR_R2+1           //����3����
#define	EE_KWH2_R3			EE_KWH0_R3+1
#define	EE_KWH4_R3			EE_KWH2_R3+1
#define	EE_KWHSUM_R3		EE_KWH4_R3+1
#define	EE_KWHXOR_R3		EE_KWHSUM_R3+1

#define EE_KWH0_R4          EE_KWHXOR_R3+1           //����4����
#define	EE_KWH2_R4			EE_KWH0_R4+1
#define	EE_KWH4_R4			EE_KWH2_R4+1
#define	EE_KWHSUM_R4		EE_KWH4_R4+1
#define	EE_KWHXOR_R4		EE_KWHSUM_R4+1

//---------��ǰ�����й�С���������ַ PAGE 182
#define	EE_KWHP 			0x2D80u    			    //��ǰ�������й�����������
#define	EE_KWHD 			EE_KWHP+1  			    //��ǰ�������й�����С��
#define EE_KWHCHK           EE_KWHD+1               //��ǰ�������й��������塢С��У��

#define	EE_KWHP_R1 			E_KWHCHK+1    		    //����1
#define	EE_KWHD_R1 			EE_KWHP_R1+1
#define EE_KWHCHK_R1        EE_KWHD_R1+1

#define	EE_KWHP_R2 			EE_KWHCHK_R1+1    	    //����2
#define	EE_KWHD_R2 			EE_KWHP_R2+1
#define EE_KWHCHK_R2        EE_KWHD_R2+1

#define	EE_KWHP_R3 			EE_KWHCHK_R2+1    	    //����3
#define	EE_KWHD_R3 			EE_KWHP_R3+1
#define EE_KWHCHK_R3        EE_KWHD_R3+1

#define	EE_KWHP_R4 			EE_KWHCHK_R3+1    		//����4
#define	EE_KWHD_R4 			EE_KWHP_R4+1
#define EE_KWHCHK_R4        EE_KWHD_R4+1

//---------��һ�׷���ʱ��  PAGE 183
#define EE_0A_TIME1         0x2DC0u                  //��һʱ��
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

//---------�ڶ��׷���ʱ��  PAGE 184
#define EE_AA_TIME1         0x2E00u                  //��һʱ��
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

//---------У�����  PAGE 185
#define ADJ_ADDR    			0x2E40u        		//У�����37���ֽ�+2��У����
#define	ADJ_ADDR_SUM			ADJ_ADDR+1              //У����
#define	ADJ_ADDR_XOR			ADJ_ADDR+1			//XORУ����

//---------���索��     PAGE 187    0x2EC0


//---------��һСʱ�ܵ������ݴ���  PAGE 188  0x2F00
#define LAST_HOUR_MIN       0x2F00u                 //����Сʱ��
#define LAST_HOUR_HOUR      LAST_HOUR_MIN+1         //�������
#define LAST_HOUR_DAY       LAST_HOUR_HOUR+1        //��������
#define LAST_HOUR_F1I1      LAST_HOUR_DAY+1         //����1������������1
#define LAST_HOUR_F1I2      LAST_HOUR_F1I1+1        //����1������������2
#define LAST_HOUR_F1I3      LAST_HOUR_F1I2+1        //����1������������3
#define LAST_HOUR_F1D       LAST_HOUR_F1I3+1        //����1����С������

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


//---------�����л�ʱ�� PAGE 189      0x2F40
#define EE_SWITCH1_MOUTH    0x2F40u         //��һʱ��ڵ㣬��
#define EE_SWITCH1_DAY      0x2F41u         //��һʱ��ڵ㣬��
#define EE_SWITCH1_HOUR     0x2F42u         //��һʱ��ڵ㣬ʱ
#define EE_SWITCH1_MIN      0x2F43u         //��һʱ��ڵ㣬ʱ
#define EE_SWITCH1_FL       0x2F44u         //��һʱ��ڵ㣬����
#define EE_SWITCH1_SUM      0x2F45u
#define EE_SWITCH1_XOR      0x2F46u

#define EE_SWITCH2_MOUTH    0x2F47u         //�ڶ�ʱ��ڵ㣬��
#define EE_SWITCH2_DAY      0x2F48u         //�ڶ�ʱ��ڵ㣬��
#define EE_SWITCH2_HOUR     0x2F49u         //�ڶ�ʱ��ڵ㣬ʱ
#define EE_SWITCH2_MIN      0x2F4Au         //�ڶ�ʱ��ڵ㣬ʱ
#define EE_SWITCH2_FL       0x2F4Bu         //�ڶ�ʱ��ڵ㣬����
#define EE_SWITCH2_SUM      0x2F4Cu
#define EE_SWITCH2_XOR      0x2F4Du

//--------------30��ÿСʱ��������
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
��ҳ����4Сʱ�������ݣ��财��30*24Сʱ�������ݣ�����180ҳ
ÿСʱ���ݵ�ַ���䣺
01~04�㣺 ��ʼ��ַ+(����-1)*0180H+(Сʱ��-1)*000DH+0000H
05~08�㣺 ��ʼ��ַ+(����-1)*0180H+(Сʱ��-5)*000DH+0040H
09~12�㣺 ��ʼ��ַ+(����-1)*0180H+(Сʱ��-9)*000DH+0080H
13~16�㣺 ��ʼ��ַ+(����-1)*0180H+(Сʱ��-13)*000DH+00C0H
17~20�㣺 ��ʼ��ַ+(����-1)*0180H+(Сʱ��-17)*000DH+0100H
21~24�㣺 ��ʼ��ַ+(����-1)*0180H+(Сʱ��-21)*000DH+0140H

*/
#define EE_INIT_ADDRESS     0x0040u     //ÿСʱ����������ʼ��ַ
#define EE_DAY_SWITCH1      0x0010u     //����ƫ�Ƶ�ַ1
#define EE_DAY_SWITCH2      0x0011u     //����ƫ�Ƶ�ַ2
#define EE_DAY_SWITCH3      0x0012u     //����ƫ�Ƶ�ַ3


#define     CMon_Time           0x0444            //6+2
#define		mUserASCII8			0x0450 		         	//10
#define		mUserASCII8_2		0x0460 			        //10
#define		mMeterInfo		    0x0470		            //8
#define 	FuncSW	        	0x047B		        	//2 ���ܿ���2+2CRC
#define		SysSW				0x0480			    	//4 ϵͳ����4+2CRC

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