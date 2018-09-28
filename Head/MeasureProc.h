/*==============================================================================
            Copyright(C) 1997-2019.  Sinowealth Tech. Co., Ltd.
--------------------------------------------------------------------------------
Project No.   : XXX
File Name     : MEASUREPROC..H
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
#ifndef  MEASUREPROC_H
#define  MEASUREPROC_H


#ifdef MEASUREPROC_C
   #define  MEASUREPROC_EXT
#else
   #define  MEASUREPROC_EXT   extern
#endif
//------------------------------------------------------------------------------

//-----------------

#define sptsvalue   			0x0000					 		//��ʱ��������жϣ�������������ֵ���ˡ�

#define C_AllDispDly            3
#define	MAX_BUF                 180

//Clk
#define C_HALFS     			BIT0
#define C_SEC       			BIT1
#define C_5S        			BIT2
#define C_MIN       			BIT3
#define C_HOUR      			BIT4
#define C_DAY       			BIT5
#define C_SEC_C     			BIT6
#define C_YEAR                  BIT7

//RUN
#define	F_DISP					BIT0
#define F_KEY   				BIT1
#define F_ComOK 				BIT2
#define F_PBILL 				BIT3

//BILL
#define F_BILL  				BIT4
#define F_PWRUP 				BIT5
#define F_PCAL  				BIT6
#define F_CLR   				BIT7

//EVENT
#define E_IReverse 				BIT0
#define E_Bypass   				BIT1
#define E_Cover    				BIT2



//������ʹ�þ���ֵ�ۼӣ����������򣬷���
#define KWHP        kwh_value.pluse         		//�������й�
#define KWHD        kwh_value.dec
#define KWH         kwh_value.integer


// #define RKWHP       rkwh_value.pluse        		//�ܷ����й�
// #define RKWHD       rkwh_value.dec
// #define RKWH        rkwh_value.integer


// #define QKWHP       qkwh_value.pluse         		//�������޹�
// #define QKWHD       qkwh_value.dec
// #define QKWH        qkwh_value.integer


// #define RQKWHP      rqkwh_value.pluse        		//�ܷ����޹�
// #define RQKWHD      rqkwh_value.dec
// #define RQKWH       rqkwh_value.integer

typedef union {
    float y;
    unsigned char Buffer[4];
} _Fdata;//�����������嶨��

typedef struct{
                unsigned char  gain;	  //��ʼ����
				unsigned int   icont;	  //��ʼ���Է�Ƶϵ��

                unsigned int   vgain;     //˲ʱ��ѹ����
                //unsigned int   irms1gain; //ͨ��1˲ʱ��������
                unsigned int irms2gain;  //ͨ��2˲ʱ��������

				//unsigned int   w1gain;	  //ͨ��1�Ĺ�������	 //������ƫ�໹�Ǹ�ƫ��,0xa6/0xffff=0.0025 ,(0xffff-0xf6ea)/0xffff=0.03
                unsigned int   w2gain;	  //ͨ��2�Ĺ�������

				//unsigned int   p1cal;	  //ͨ��1����λ����
                unsigned int   p2cal;	  //ͨ��2����λ����

                //unsigned int   w1os;	  //ͨ��1��ƫ��OFFSET����
				unsigned int   w2os;	  //ͨ��2��ƫ��OFFSET����

                //unsigned int   adcos;     //ADCOSƫ������
				//unsigned long  irms1os;	  //ͨ��1�ĵ���OFFSET����
				//unsigned long  irms2os;	  //ͨ��2�ĵ���OFFSET����

				//unsigned int   i2gain;	  //ͨ��2�ĵ�������
				//unsigned int   spts;	  //������������

				//unsigned long  vconstant; //���൥��ʱ��ѹ����
                //unsigned int   nmgain;    //���൥��ʱ��������

//				unsigned int   vol_ib;	  //������ѹ
//				unsigned int   cur_ib;    //��������
//				unsigned char  mc[3];	  //���峣��

}__ADJUST_DATA;											//24

typedef struct
{
                // unsigned int power2gain; //˲ʱ��������
                _Fdata power2gain; //˲ʱ��������ֵ��С��ʹ�ø�������߾���
}__ADJUST_DATA_F;   //�洢�����͵Ľ������

typedef struct
{
   unsigned char 	Sec;
   unsigned char 	Min;
   unsigned char 	Hour;
   unsigned char 	Day;
   unsigned char 	Mon;
   unsigned char 	Year;
} YMDhms;

typedef struct
{
    unsigned long Max0;		            // ����Ĵ���
    unsigned char CurDemand[2];
    unsigned char PrevDemand[2];
}MAXDEMAND;

typedef struct
{
    unsigned int Constant;      //���峣��
    unsigned char Threshold;    //���嵱��
    unsigned char Icont;        //������Ĵ���
    unsigned int Chk;           //У��
}CONSTPARA;

typedef struct
{
	unsigned char	Run;
	unsigned char	EventFlg;
    unsigned char   Clk;
	unsigned char   ALARM[2];
    unsigned char   EnTChk;
	unsigned int    Function;
}FLAG;

typedef struct
{
    unsigned char   Sec;
    unsigned char   Min;
    unsigned char   Hour;
    unsigned char   Day;
    unsigned char   Mon;
    unsigned char   Year;
    unsigned char   Week;
} M_CLK;

typedef struct
{
  	unsigned char	PollingDisplayCount;	// ����ʱ����������
	unsigned char	PollingDisplayCountSet;	// ����ʱ�������������ó�ֵ,��ʾ��0,1,2 ѭ�� TEST ALT,
	unsigned char	PollingDisplayPtr;  	// ���Ա�ָ��
	unsigned int 	PollingDisplayID;		// ������ID��ʶ
	unsigned char	KeyDisplaySet;			// ���������־
    unsigned int    ShutDisplayCount;		// ����ʾʱ�������
    unsigned int    ShutDisplayCountSet;	// ����ʾʱ��
} M_DISKEY;

typedef struct
{
	unsigned char   ByPassDly;
    unsigned char   RevDly;
    unsigned char   NMDly;
    unsigned int    ByPassT;
    unsigned char   RevT;
} EVENTDLY;

typedef struct
{
    unsigned char Voltage[3];
	unsigned char Current[3];
    unsigned char Power[4];
    unsigned char RPower[4];
    unsigned char Frequence[2];
    unsigned char APower[4];
    unsigned char PowerFactor[2];
} M_INSBCD;



//�ṹ��
typedef struct                      				//����
{
    unsigned char 	pluse;          				//��ǰ����������
    unsigned char 	dec;	    					//��ǰ����С��
    unsigned char 	integer[5];	    				//��ǰ��������
}ENERGY_group;
MEASUREPROC_EXT	ENERGY_group;


typedef struct{
				 unsigned char  meter_address[6];			   		//ͨѶ��ַ
                 unsigned char  meter_address_def[6];           //Ĭ�ϱ��
                 unsigned char  meter_factory[2];
				 //unsigned char  password_0[3];	   		//0������
				 //unsigned char  password_2[3];	   		//2������
                 unsigned char address_input_flag[8];
}__PARAM_DATA;
MEASUREPROC_EXT __PARAM_DATA xdata param_data;



MEASUREPROC_EXT bit gbSecFlag;                    		//��ʱ1s��־
MEASUREPROC_EXT bit gbLowPWRFlag;

MEASUREPROC_EXT unsigned char xdata EE_KHH_address;         //�������õ���EEPROM�洢��ַ
//MEASUREPROC_EXT unsigned char xdata keytime[1];	  		//���Ǽ�
MEASUREPROC_EXT unsigned int xdata gbPCNTCount;
//MEASUREPROC_EXT unsigned char xdata g_TCoverDly;



MEASUREPROC_EXT unsigned char data g_ClkSec;
MEASUREPROC_EXT unsigned char xdata g_PwrDnHour;
MEASUREPROC_EXT unsigned char xdata g_PwrDnYear;
MEASUREPROC_EXT unsigned char data PwrCnt;



MEASUREPROC_EXT FLAG xdata g_Flag;
//MEASUREPROC_EXT unsigned char g_Clearn;
//MEASUREPROC_EXT unsigned char g_CoverEnableDly;
//MEASUREPROC_EXT unsigned char g_EcDecDot_Config;	    //����С��λ����


MEASUREPROC_EXT unsigned char xdata g_AllDisDly;
MEASUREPROC_EXT M_DISKEY xdata g_Disp;

MEASUREPROC_EXT MAXDEMAND xdata g_Demand;
MEASUREPROC_EXT EVENTDLY xdata g_EventDly;

MEASUREPROC_EXT uint8 meter_const;				   		//�����
MEASUREPROC_EXT CONSTPARA xdata g_ConstPara;
MEASUREPROC_EXT __ADJUST_DATA_F xdata adjust_data_f;    //������У�����
MEASUREPROC_EXT __ADJUST_DATA xdata adjust_data;	 	//У�����

MEASUREPROC_EXT M_CLK xdata g_Clk;
MEASUREPROC_EXT M_INSBCD xdata g_InsBCD;

MEASUREPROC_EXT unsigned char xdata g_Buffer[MAX_BUF+1];
MEASUREPROC_EXT const unsigned char code SoftwareVersionNew[20];
MEASUREPROC_EXT const unsigned char code SoftwareVersionTest[30];

MEASUREPROC_EXT unsigned int xdata UpGradeFlg;

MEASUREPROC_EXT unsigned char xdata MeterNum[6];
MEASUREPROC_EXT unsigned char xdata g_MeterNumAsc[12];




//MEASUREPROC_EXT unsigned long xdata curl;				//���ߵ���
MEASUREPROC_EXT unsigned long xdata curn;				//���ߵ���
MEASUREPROC_EXT float xdata curn_temp;
//MEASUREPROC_EXT unsigned long xdata powerl;		    //�����й�����
MEASUREPROC_EXT unsigned long xdata powern;		    	//�����й�����
//MEASUREPROC_EXT unsigned long xdata rpowerl;			//�����޹�����
MEASUREPROC_EXT unsigned long xdata rpowern;		    //�����޹�����
MEASUREPROC_EXT unsigned long xdata freq;		        //��ѹƵ��
MEASUREPROC_EXT unsigned long xdata apowern;		    //���ڹ���
MEASUREPROC_EXT float xdata factorp;		    //��������

MEASUREPROC_EXT unsigned char xdata CurChExDly;


MEASUREPROC_EXT  uint8 xdata KWHD1;
MEASUREPROC_EXT  uint8 xdata KWHD2;
MEASUREPROC_EXT  uint8 idata KWHD3;						//��xdata�ĳ�idata����Ϊ��������м���
MEASUREPROC_EXT  uint8 xdata RKWHD1;
MEASUREPROC_EXT  uint8 xdata RKWHD2;
MEASUREPROC_EXT  uint8 idata RKWHD3;					//��xdata�ĳ�idata����Ϊ��������м���

MEASUREPROC_EXT unsigned char xdata	KWH_PN[6];	        //��������й�
//MEASUREPROC_EXT unsigned char xdata KWH_LMON[4];		//�ϸ�������й�
//MEASUREPROC_EXT unsigned char xdata KWH_DAY[4];		    //�������й�����
//MEASUREPROC_EXT unsigned char xdata KWH_LDAY[4];		//ǰһ�����й�����
//MEASUREPROC_EXT unsigned char xdata KWH_DAY1[4];		//�����й�����

MEASUREPROC_EXT uint8 xdata QKWHD1;
MEASUREPROC_EXT uint8 xdata QKWHD2;
MEASUREPROC_EXT uint8 idata QKWHD3;						//��xdata�ĳ�idata����Ϊ��������м���
MEASUREPROC_EXT uint8 xdata RQKWHD1;
MEASUREPROC_EXT uint8 xdata RQKWHD2;
MEASUREPROC_EXT uint8 idata RQKWHD3;					//��xdata�ĳ�idata����Ϊ��������м���

// MEASUREPROC_EXT unsigned char xdata QKWH_PN[4];	        //���������й�
// MEASUREPROC_EXT unsigned char xdata QKWH_LMON[4];		//�ϸ��������й�

// MEASUREPROC_EXT unsigned char xdata QKWH_Inductive[4];	//�����޹������
// MEASUREPROC_EXT unsigned char xdata RQKWH_Capactive[4];	//�����޹�������

MEASUREPROC_EXT ENERGY_group xdata	kwh_value;		 	//�����й�ֵ
// MEASUREPROC_EXT ENERGY_group xdata	rkwh_value;		    //�����й�ֵ

// MEASUREPROC_EXT ENERGY_group xdata	qkwh_value;		    //�����޹�ֵ
// MEASUREPROC_EXT ENERGY_group xdata	rqkwh_value;	    //�����޹�ֵ


MEASUREPROC_EXT unsigned char NB_LORA[2];
MEASUREPROC_EXT void Init_RAM(void);//RAM��ʼ�������޸�
//MEASUREPROC_EXT void BatteryInitMode(void);

//MEASUREPROC_EXT void ALARMLED(void);

MEASUREPROC_EXT void ConstPara_Init(unsigned char meter_const, unsigned char Cmd); //���峣����ʼ�������޸�
MEASUREPROC_EXT unsigned int ConstPara_Chk(void);

MEASUREPROC_EXT void EE_KWH_shift(void);        //�õ�����EEPROM��ַ����
MEASUREPROC_EXT void check_dotKWHD(void);  			 	//check �����й�С������,���޸�
//MEASUREPROC_EXT void check_dotRKWHD(void); 			 	//check �����й�С������
MEASUREPROC_EXT void KWH_RKWH_dot(void);			 	//check ���򣬷����й�С�����֣����޸ģ�ȥ���˷��򲿷�
MEASUREPROC_EXT void ECRunKwh (void);                	//��������й�
MEASUREPROC_EXT void ReadMeterFromTOEeprom(void);

MEASUREPROC_EXT void check_dotQKWHD(void);  		 	//check �����޹�С������
//MEASUREPROC_EXT void check_dotRQKWHD(void); 		 	//check �����޹�С������
MEASUREPROC_EXT void QKWH_RQKWH_dot(void);			 	//check ���򣬷����޹�С������
//MEASUREPROC_EXT void ECRunQKwh (void);               	//��������޹�
//MEASUREPROC_EXT void ECRunQKwh_RQKwh(void);


//MEASUREPROC_EXT unsigned char DotBCD_Check(unsigned char Dot);
MEASUREPROC_EXT void Add1BCD(unsigned char *point, unsigned char len);
MEASUREPROC_EXT void BCD_ADD_N(unsigned char *dp_0, unsigned char *dp_1, unsigned char n);
//MEASUREPROC_EXT void BCD_MINUS_N(unsigned char *dp_0, unsigned char *dp_1, unsigned char n);

MEASUREPROC_EXT void EnergyProc(void);
//MEASUREPROC_EXT void PluseAndDecimalProc(void);
//MEASUREPROC_EXT void A_energy(unsigned char *s_addr, unsigned char *t_addr);	//unsigned char RAM)
MEASUREPROC_EXT void E_operating(unsigned int EE_addr, unsigned char *IRAM, unsigned char *DRAM, unsigned char len);
//MEASUREPROC_EXT void Q_operating(unsigned int EE_addr, unsigned char *IRAM, unsigned char *DRAM, unsigned char len);
MEASUREPROC_EXT    void NB_LORA_PANDUAN(unsigned char *flag);






#endif