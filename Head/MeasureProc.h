/*==============================================================================
            Copyright(C) 1997-2019.  Sinowealth Tech. Co., Ltd.
--------------------------------------------------------------------------------
Project No.   : XXX
File Name     : MEASUREPROC..H
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
#ifndef  MEASUREPROC_H
#define  MEASUREPROC_H


#ifdef MEASUREPROC_C
   #define  MEASUREPROC_EXT
#else
   #define  MEASUREPROC_EXT   extern
#endif
//------------------------------------------------------------------------------

//-----------------

#define sptsvalue   			0x0000					 		//用时间变量做判断，不在用启动阀值做了。

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



//在这里使用绝对值累加，不区分正向，反向
#define KWHP        kwh_value.pluse         		//总正向有功
#define KWHD        kwh_value.dec
#define KWH         kwh_value.integer


// #define RKWHP       rkwh_value.pluse        		//总反向有功
// #define RKWHD       rkwh_value.dec
// #define RKWH        rkwh_value.integer


// #define QKWHP       qkwh_value.pluse         		//总正向无功
// #define QKWHD       qkwh_value.dec
// #define QKWH        qkwh_value.integer


// #define RQKWHP      rqkwh_value.pluse        		//总反向无功
// #define RQKWHD      rqkwh_value.dec
// #define RQKWH       rqkwh_value.integer

typedef union {
    float y;
    unsigned char Buffer[4];
} _Fdata;//浮点型联合体定义

typedef struct{
                unsigned char  gain;	  //初始增益
				unsigned int   icont;	  //初始调试分频系数

                unsigned int   vgain;     //瞬时电压增益
                //unsigned int   irms1gain; //通道1瞬时电流增益
                unsigned int irms2gain;  //通道2瞬时电流增益

				//unsigned int   w1gain;	  //通道1的功率修正	 //看下正偏多还是负偏多,0xa6/0xffff=0.0025 ,(0xffff-0xf6ea)/0xffff=0.03
                unsigned int   w2gain;	  //通道2的功率修正

				//unsigned int   p1cal;	  //通道1的相位修正
                unsigned int   p2cal;	  //通道2的相位修正

                //unsigned int   w1os;	  //通道1的偏置OFFSET修正
				unsigned int   w2os;	  //通道2的偏置OFFSET修正

                //unsigned int   adcos;     //ADCOS偏置修正
				//unsigned long  irms1os;	  //通道1的电流OFFSET修正
				//unsigned long  irms2os;	  //通道2的电流OFFSET修正

				//unsigned int   i2gain;	  //通道2的电流增益
				//unsigned int   spts;	  //启动功率设置

				//unsigned long  vconstant; //单相单线时电压修正
                //unsigned int   nmgain;    //单相单线时功率修正

//				unsigned int   vol_ib;	  //基本电压
//				unsigned int   cur_ib;    //基本电流
//				unsigned char  mc[3];	  //脉冲常数

}__ADJUST_DATA;											//24

typedef struct
{
                // unsigned int power2gain; //瞬时功率增益
                _Fdata power2gain; //瞬时功率增益值较小，使用浮点数提高精度
}__ADJUST_DATA_F;   //存储浮点型的交表参数

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
    unsigned long Max0;		            // 缓存寄存器
    unsigned char CurDemand[2];
    unsigned char PrevDemand[2];
}MAXDEMAND;

typedef struct
{
    unsigned int Constant;      //脉冲常数
    unsigned char Threshold;    //脉冲当量
    unsigned char Icont;        //发脉冲寄存器
    unsigned int Chk;           //校验
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
  	unsigned char	PollingDisplayCount;	// 轮显时间间隔计数器
	unsigned char	PollingDisplayCountSet;	// 轮显时间间隔计数器设置初值,显示区0,1,2 循环 TEST ALT,
	unsigned char	PollingDisplayPtr;  	// 轮显表指针
	unsigned int 	PollingDisplayID;		// 轮显项ID标识
	unsigned char	KeyDisplaySet;			// 键显请求标志
    unsigned int    ShutDisplayCount;		// 关显示时间计数器
    unsigned int    ShutDisplayCountSet;	// 关显示时间
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



//结构体
typedef struct                      				//电量
{
    unsigned char 	pluse;          				//当前电量脉冲数
    unsigned char 	dec;	    					//当前电量小数
    unsigned char 	integer[5];	    				//当前整数电量
}ENERGY_group;
MEASUREPROC_EXT	ENERGY_group;


typedef struct{
				 unsigned char  meter_address[6];			   		//通讯地址
                 unsigned char  meter_address_def[6];           //默认表号
                 unsigned char  meter_factory[2];
				 //unsigned char  password_0[3];	   		//0级密码
				 //unsigned char  password_2[3];	   		//2级密码
                 unsigned char address_input_flag[8];
}__PARAM_DATA;
MEASUREPROC_EXT __PARAM_DATA xdata param_data;



MEASUREPROC_EXT bit gbSecFlag;                    		//延时1s标志
MEASUREPROC_EXT bit gbLowPWRFlag;

MEASUREPROC_EXT unsigned char xdata EE_KHH_address;         //计算后的用电量EEPROM存储地址
//MEASUREPROC_EXT unsigned char xdata keytime[1];	  		//开盖键
MEASUREPROC_EXT unsigned int xdata gbPCNTCount;
//MEASUREPROC_EXT unsigned char xdata g_TCoverDly;



MEASUREPROC_EXT unsigned char data g_ClkSec;
MEASUREPROC_EXT unsigned char xdata g_PwrDnHour;
MEASUREPROC_EXT unsigned char xdata g_PwrDnYear;
MEASUREPROC_EXT unsigned char data PwrCnt;



MEASUREPROC_EXT FLAG xdata g_Flag;
//MEASUREPROC_EXT unsigned char g_Clearn;
//MEASUREPROC_EXT unsigned char g_CoverEnableDly;
//MEASUREPROC_EXT unsigned char g_EcDecDot_Config;	    //电量小数位设置


MEASUREPROC_EXT unsigned char xdata g_AllDisDly;
MEASUREPROC_EXT M_DISKEY xdata g_Disp;

MEASUREPROC_EXT MAXDEMAND xdata g_Demand;
MEASUREPROC_EXT EVENTDLY xdata g_EventDly;

MEASUREPROC_EXT uint8 meter_const;				   		//电表常数
MEASUREPROC_EXT CONSTPARA xdata g_ConstPara;
MEASUREPROC_EXT __ADJUST_DATA_F xdata adjust_data_f;    //浮点型校表参数
MEASUREPROC_EXT __ADJUST_DATA xdata adjust_data;	 	//校表参数

MEASUREPROC_EXT M_CLK xdata g_Clk;
MEASUREPROC_EXT M_INSBCD xdata g_InsBCD;

MEASUREPROC_EXT unsigned char xdata g_Buffer[MAX_BUF+1];
MEASUREPROC_EXT const unsigned char code SoftwareVersionNew[20];
MEASUREPROC_EXT const unsigned char code SoftwareVersionTest[30];

MEASUREPROC_EXT unsigned int xdata UpGradeFlg;

MEASUREPROC_EXT unsigned char xdata MeterNum[6];
MEASUREPROC_EXT unsigned char xdata g_MeterNumAsc[12];




//MEASUREPROC_EXT unsigned long xdata curl;				//火线电流
MEASUREPROC_EXT unsigned long xdata curn;				//零线电流
MEASUREPROC_EXT float xdata curn_temp;
//MEASUREPROC_EXT unsigned long xdata powerl;		    //火线有功功率
MEASUREPROC_EXT unsigned long xdata powern;		    	//零线有功功率
//MEASUREPROC_EXT unsigned long xdata rpowerl;			//火线无功功率
MEASUREPROC_EXT unsigned long xdata rpowern;		    //零线无功功率
MEASUREPROC_EXT unsigned long xdata freq;		        //电压频率
MEASUREPROC_EXT unsigned long xdata apowern;		    //视在功率
MEASUREPROC_EXT float xdata factorp;		    //功率因数

MEASUREPROC_EXT unsigned char xdata CurChExDly;


MEASUREPROC_EXT  uint8 xdata KWHD1;
MEASUREPROC_EXT  uint8 xdata KWHD2;
MEASUREPROC_EXT  uint8 idata KWHD3;						//将xdata改成idata，因为其汇编会多个中间量
MEASUREPROC_EXT  uint8 xdata RKWHD1;
MEASUREPROC_EXT  uint8 xdata RKWHD2;
MEASUREPROC_EXT  uint8 idata RKWHD3;					//将xdata改成idata，因为其汇编会多个中间量

MEASUREPROC_EXT unsigned char xdata	KWH_PN[6];	        //当月组合有功
//MEASUREPROC_EXT unsigned char xdata KWH_LMON[4];		//上个月组合有功
//MEASUREPROC_EXT unsigned char xdata KWH_DAY[4];		    //当日总有功电量
//MEASUREPROC_EXT unsigned char xdata KWH_LDAY[4];		//前一日总有功电量
//MEASUREPROC_EXT unsigned char xdata KWH_DAY1[4];		//当日有功电量

MEASUREPROC_EXT uint8 xdata QKWHD1;
MEASUREPROC_EXT uint8 xdata QKWHD2;
MEASUREPROC_EXT uint8 idata QKWHD3;						//将xdata改成idata，因为其汇编会多个中间量
MEASUREPROC_EXT uint8 xdata RQKWHD1;
MEASUREPROC_EXT uint8 xdata RQKWHD2;
MEASUREPROC_EXT uint8 idata RQKWHD3;					//将xdata改成idata，因为其汇编会多个中间量

// MEASUREPROC_EXT unsigned char xdata QKWH_PN[4];	        //当月组无有功
// MEASUREPROC_EXT unsigned char xdata QKWH_LMON[4];		//上个月组无有功

// MEASUREPROC_EXT unsigned char xdata QKWH_Inductive[4];	//当月无功电感量
// MEASUREPROC_EXT unsigned char xdata RQKWH_Capactive[4];	//当月无功电容量

MEASUREPROC_EXT ENERGY_group xdata	kwh_value;		 	//正向有功值
// MEASUREPROC_EXT ENERGY_group xdata	rkwh_value;		    //反向有功值

// MEASUREPROC_EXT ENERGY_group xdata	qkwh_value;		    //正向无功值
// MEASUREPROC_EXT ENERGY_group xdata	rqkwh_value;	    //反向无功值


MEASUREPROC_EXT unsigned char NB_LORA[2];
MEASUREPROC_EXT void Init_RAM(void);//RAM初始化，已修改
//MEASUREPROC_EXT void BatteryInitMode(void);

//MEASUREPROC_EXT void ALARMLED(void);

MEASUREPROC_EXT void ConstPara_Init(unsigned char meter_const, unsigned char Cmd); //脉冲常数初始化，已修改
MEASUREPROC_EXT unsigned int ConstPara_Chk(void);

MEASUREPROC_EXT void EE_KWH_shift(void);        //用电量的EEPROM地址计算
MEASUREPROC_EXT void check_dotKWHD(void);  			 	//check 正向有功小数部分,已修改
//MEASUREPROC_EXT void check_dotRKWHD(void); 			 	//check 反向有功小数部分
MEASUREPROC_EXT void KWH_RKWH_dot(void);			 	//check 正向，反向有功小数部分，已修改，去掉了反向部分
MEASUREPROC_EXT void ECRunKwh (void);                	//计算组合有功
MEASUREPROC_EXT void ReadMeterFromTOEeprom(void);

MEASUREPROC_EXT void check_dotQKWHD(void);  		 	//check 正向无功小数部分
//MEASUREPROC_EXT void check_dotRQKWHD(void); 		 	//check 反向无功小数部分
MEASUREPROC_EXT void QKWH_RQKWH_dot(void);			 	//check 正向，反向无功小数部分
//MEASUREPROC_EXT void ECRunQKwh (void);               	//计算组合无功
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