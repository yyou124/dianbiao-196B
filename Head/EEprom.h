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

//----------------

// EEPROM ADDRESS
/*-------------------------------EEPROM��Ϣ(������EEPROM��ַ��������ҳ)-----------------------*/
#define D_EEP_BUF_LEN       50          			//eeprom �Ĳ�����BUFF�ĳ���
#define EE_SIZE             8                		//8bitһҳ  24c04
#define EE_PAGE             512	                    //2Kbit EEPROM�ռ䣬һ��256ҳ
#define MAX_EE_SIZE         16                      //EEPROMһ�������д���ַ����
#define	FRAM_EEPROM			0xA0                    //EEPROM��ͨѶ��ַ
//EEPROM PAGE ADDRESS: 0X000 ~ 0X1FF
#define Meter_Factory_Broad 0x99
#define Meter_Factory_Def   0x00

/*-------------------------------�ڲ���EEPROMʹ�����----------------------------------------*/
//����С��λ��ַ
#define EE_PowerDown        0x0500                  //����FLAG
#define	EE_KWHD 			0x0531u    			    //��ǰ�������й�����������
#define	EE_KWHP 			EE_KWHD+1  			    //��ǰ�������й�����С��
#define EE_KWHCHK           EE_KWHP+1               //��ǰ�������й��������塢С��У��

/*-------------------------------�ⲿEEPROMʹ�����----------------------------------------*/
#define EE_BAKADDR        	0x0100U                 //EEPROM���ݱ��ݵ�ַ
//���õ�����ַ  5+2�ֽ�                     0x0001~0x0007
#define EE_KWH_SHIFT        8              //�����洢ƫ�Ƶ�ַ
#define EE_KWH_MAX          90             //�����õ�������EEPROM��ַ����λ ��ȣ�
#define EE_KWH_NUM          (ADJ_ADDR+1)/8  //���ݵ�ǰ����ռ�õ����洢�ռ����


#define	EE_KWH0				0x0001					//��ǰ���й�����������λ
#define	EE_KWH1				EE_KWH0+1 				//5+2
#define	EE_KWH2				EE_KWH1+1
#define	EE_KWH3				EE_KWH2+1
#define	EE_KWH4				EE_KWH3+1
#define	EE_KWHSUM			EE_KWH4+1               //SUMУ����
#define	EE_KWHXOR			EE_KWHSUM+1				//XORУ����

//У�����(������������) 13+2�ֽ�           0x00A0~0x00AC
#define ADJ_ADDR    		0x00A0u        		    //У�����
#define	ADJ_ADDR_SUM		ADJ_ADDR+13             //У����
#define	ADJ_ADDR_XOR		ADJ_ADDR+13			    //XORУ����

//��̱�־  2+2�ֽ�                        0x00AC~0x00AF
#define EE_PROG_FLAG        0x00BCu
//У�����(������)--���� 4+2�ֽ�            0x00C0~0x00C5
#define ADJ_ADDR_POWER2GAIN 0x00C0u
//��һ���ϵ��־ 4+2�ֽ�                    0x00C6~0x00CB
#define	EE_FirstProg_FLAG	0x00C6u
//�̵���ͨ�ϵ��־ 2+2�ֽ�                  0x00CC~0x00CF
#define RELAY_STATUS        0x00CC                  //��ǰ�豸�̵���״̬
//ͨѶģ�鰲װ��־ 2+2�ֽ�                  0x00D1~0x00D4
#define EE_NB_LORA          0x00D1                  // LORA=>AA NB=> BB
//NB��ʼ��״̬ 2+2�ֽ�                      0x00D7~0x00DA
#define EE_NB_STATE         0x00D7
//ͨ�ŵ�ַ  6+2�ֽ�                         0x00E0~0x00E7
#define EE_Meter_address	0x00E0
//���ұ��  1�ֽ�
#define EE_Meter_Factory    0x00E9
//����汾��    1�ֽ�
#define EE_soft_code        0x00EB
//Ӳ���汾��    1�ֽ�
#define EE_hard_code        0x00ED
//�������õ����洢��ַ
#define EE_KWH_ADDRESS      0x00F0
//�¼�FLAG 2�ֽ�
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