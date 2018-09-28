/*==============================================================================
            Copyright(C) 1997-2019.  Sinowealth Tech. Co., Ltd.
--------------------------------------------------------------------------------
Project No.   : XXX
File Name     : POWERADJUSTMETER..H
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
#ifndef	POWERADJUSTMETER_H
#define	POWERADJUSTMETER_H

#ifndef POWERADJUSTMETER_C
	#define POWERADJUSTMETER_EXT	extern
#else
	#define POWERADJUSTMETER_EXT
#endif
//------------------------------------------------------------------------------

//----------------


//默认低EMU参数，调整到基准的0.5200到1.4800   	 +/-0.48
#define D_MUL_Err_0_48  0.48

#define D_vgain 		0x4000		   //default
#define D_vgain_Min     (unsigned int)((float)D_vgain*(1 - D_MUL_Err_0_48))
#define D_vgain_Max     (unsigned int)((float)D_vgain*(1 + D_MUL_Err_0_48))

#define D_nmgain  		0x4000		   //default
#define D_nmgain_Min    (unsigned int)((float)D_nmgain*(1 - D_MUL_Err_0_48))
#define D_nmgain_Max    (unsigned int)((float)D_nmgain*(1 + D_MUL_Err_0_48))

#define D_irms1gain 	0x4000		   //default
#define D_irms1gain_Min (unsigned int)((float)D_irms1gain*(1 - D_MUL_Err_0_48))
#define D_irms1gain_Max (unsigned int)((float)D_irms1gain*(1 + D_MUL_Err_0_48))

#define D_irms2gain 	0x4000		   //default
#define D_irms2gain_Min (unsigned int)((float)D_irms2gain*(1 - D_MUL_Err_0_48))
#define D_irms2gain_Max (unsigned int)((float)D_irms2gain*(1 + D_MUL_Err_0_48))

//默认低EMU参数，功率校正范围为基准的0.8200到基准的1.1800	   +/-0.18
#define D_MUL_Err_0_18  0.18

#define D_w1gain 		0x4000		   //default
#define D_w1gain_Min    (unsigned int)((float)D_w1gain*(1 - D_MUL_Err_0_18))
#define D_w1gain_Max    (unsigned int)((float)D_w1gain*(1 + D_MUL_Err_0_18))
#define D_w2gain 		0xCD3B//0x4000		   //default
#define D_w2gain_Min    (unsigned int)((float)D_w2gain*(1 - D_MUL_Err_0_18))
#define D_w2gain_Max    (unsigned int)((float)D_w2gain*(1 + D_MUL_Err_0_18))

#define D_p1cal 		0x4000		   //default
#define D_p1cal_Min     (unsigned int)((float)D_p1cal*(1 - D_MUL_Err_0_18))
#define D_p1cal_Max     (unsigned int)((float)D_p1cal*(1 + D_MUL_Err_0_18))
#define D_p2cal 		0x4000		   //default
#define D_p2cal_Min     (unsigned int)((float)D_p2cal*(1 - D_MUL_Err_0_18))
#define D_p2cal_Max     (unsigned int)((float)D_p2cal*(1 + D_MUL_Err_0_18))

#define D_w1os 		    0x4000		   //default
#define D_w1os_Min      (unsigned int)((float)D_w1os*(1 - D_MUL_Err_0_18))
#define D_w1os_Max      (unsigned int)((float)D_w1os*(1 + D_MUL_Err_0_18))
#define D_w2os 		    0x4000		   //default
#define D_w2os_Min      (unsigned int)((float)D_w2os*(1 - D_MUL_Err_0_18))
#define D_w2os_Max      (unsigned int)((float)D_w2os*(1 + D_MUL_Err_0_18))


#define D_adcos			0x0000
#define D_adcos_Min     (unsigned int)((float)D_adcos*(1 - D_MUL_Err_0_18))
#define D_adcos_Max     (unsigned int)((float)D_adcos*(1 + D_MUL_Err_0_18))

#define D_irms1os		0x0000
#define D_irms1os_Min   (unsigned int)((float)D_irms1os*(1 - D_MUL_Err_0_18))
#define D_irms1os_Max   (unsigned int)((float)D_irms1os*(1 + D_MUL_Err_0_18))

#define D_irms2os		0x0000
#define D_irms2os_Min   (unsigned int)((float)D_irms2os*(1 - D_MUL_Err_0_18))
#define D_irms2os_Max   (unsigned int)((float)D_irms2os*(1 + D_MUL_Err_0_18))

//////////////////////////////////////////////////////////////////////
#define D_SHUNT		    1500.0
#define	Transformer		3.2/2000

#define D_ICON			0X41			  		//icon
#define D_L_GAIN	    16.0			  		//锰铜L,8倍增益
#define D_N_GAIN	    1.0			  			//互感器N,1倍增益
#define D_V_GAIN	    1.0			  			//电压,1倍增益
#define D_MT_VALUE	    0.4		  		        //锰铜阻值是

#define D_UN_R1			1200.0					//1.2M
#define D_UN_R2			1.0						//1K

#define D_REF_V			1.4						//1.414V
#define D_REF_AD        8388608.0
#define D_UN		    220.0 					//220v
#define D_IB		    5.0					    //5A
#define D_Pulse		    0x001200				//PLUSE IS 1200

#define D_UN_PROPROTION	(float)((D_REF_V*(D_UN_R1+D_UN_R2)*D_V_GAIN)/(D_REF_AD*D_UN_R2))	//220V =VRMS*D_UN_PROPROTION
#define D_UN_Value 		(float)(D_UN/D_UN_PROPROTION)//+(float)(D_UN/D_UN_PROPROTION)*0.047    //1339980.0	 //220v
#define D_UN_Min	    (unsigned long)(D_UN_Value*0.6)
#define D_UN_Max	    (unsigned long)(D_UN_Value*1.4)

#define D_IL_10A		D_L_GAIN*1.414*D_IB*D_SHUNT*0.000001		//10A,锰铜采样L电流
#define D_IL_10A_Value  (D_IL_10A*2545362)/0.2						//0.2=200MV

#define D_IL_05A		D_IL_10A									//10A*5%,锰铜采样L电流
#define D_IL_05A_Value  D_IL_10A*0.5 								//0.2=200MV

#define D_IL_01A		D_IL_10A									//10A*10%,锰铜采样L电流
#define D_IL_01A_Value  D_IL_10A*0.1 								//0.2=200MV

#define D_IN_10A		D_N_GAIN*1.414*D_IB*Transformer				//10A,互感器采样L电流
#define D_IN_10A_Value  (D_IN_10A*2545362)/0.2 						//0.2=200MV

#define D_IN_05A		D_IN_10A									//10A*5%,互感器采样L电流
#define D_IN_05A_Value  D_IN_10A*0.5		 						//0.2=200MV

#define D_IN_01A		D_IN_10A									//10A*10%,互感器采样L电流
#define D_IN_01A_Value  D_IN_10A*0.1 								//0.2=200MV


#define D_Channel_L   	0x01
#define D_Channel_N   	0x02

#define D_Adj_L_10IB   	0x01
#define D_Adj_L_05IB  	0x02
#define D_Adj_L_01IB    0x03

#define D_Adj_N_10IB   	0x04
#define D_Adj_N_05IB  	0x05
#define D_Adj_N_01IB    0x06



#define  ERR  0
#define  OK  1


#define D_PowerErr 0.03//0.05


POWERADJUSTMETER_EXT uint32 xdata AdjTemp0;
POWERADJUSTMETER_EXT uint32 xdata AdjTemp[8];
POWERADJUSTMETER_EXT uint32 xdata AverPower;
POWERADJUSTMETER_EXT uint32 xdata AdjTempValueEverySec;
POWERADJUSTMETER_EXT unsigned char xdata gbAverTimes;
POWERADJUSTMETER_EXT bit JudgeVoltugeRange(void);
POWERADJUSTMETER_EXT bit AverStablePowerJudge(void);
POWERADJUSTMETER_EXT unsigned int GetAdjMeterTepe(void);
POWERADJUSTMETER_EXT unsigned char xdata gBajust_type;

POWERADJUSTMETER_EXT bit gbAdjSecFlag; 							//1S 功率校表时的秒标志

POWERADJUSTMETER_EXT unsigned int xdata  gBAdjKeyCount;
POWERADJUSTMETER_EXT unsigned char xdata gBAdjKeyNew;
POWERADJUSTMETER_EXT unsigned char xdata gBAdjKeyOld;
POWERADJUSTMETER_EXT unsigned char xdata gBAdjKeyStatus;
POWERADJUSTMETER_EXT unsigned char xdata gBAdjKeyEnable; 		//meter sum cls enable command word
POWERADJUSTMETER_EXT unsigned char xdata gBAdjKeyEnable1;		//meter sum cls enable command word
POWERADJUSTMETER_EXT unsigned int xdata gbAdjustShowID;
POWERADJUSTMETER_EXT unsigned char xdata gbAdjShowOk2S;
POWERADJUSTMETER_EXT unsigned char xdata gbAdjStepFlag;
POWERADJUSTMETER_EXT unsigned char xdata gbAdjCurrentStep;

POWERADJUSTMETER_EXT unsigned int xdata  vol_ib;	  //基本电压
POWERADJUSTMETER_EXT unsigned int xdata  cur_ib;    //基本电流
POWERADJUSTMETER_EXT unsigned char xdata mc[3];	  //脉冲常数



POWERADJUSTMETER_EXT __U32_Def AdjustTypeProc(uint8 AdjustType,uint32 PowerConstant,uint8 ch);
POWERADJUSTMETER_EXT uint8 JudgeAdjustType(uint8 ch);
POWERADJUSTMETER_EXT uint32 GetPowerConstant(void);
POWERADJUSTMETER_EXT void JumpPowerAutoAdjust(void);		    //scan adjust key enable is keep 6s
POWERADJUSTMETER_EXT void PowerAdjThread(void);                 //功率校表线程


/*******************************************************************************************
** 函数名称: WriteEMU
** 函数描述: 对EMU中间接寄存器写操作
** 输入参数: sdar:要写入的寄存器的地址
             *ptr:写入数据的首地址
** 输出参数: 无
*******************************************************************************************/
POWERADJUSTMETER_EXT void WriteEMU_REG(uint8 sadr,__U32_U08 val);

/*******************************************************************************************
** 函数名称: ReadEMU
** 函数描述: 对EMU中间接寄存器读操作
** 输入参数: sdar:要读入的寄存器的地址
			    num :读入数组的字节数
				 datatype:数据类型，0：无符号型;1:有符合型
** 输出参数: rdata:返回寄存器的值
*******************************************************************************************/
POWERADJUSTMETER_EXT __U32_U08 ReadEMU_REG(uint8 sadr);

/*********************************************************************************************************
** 函数名称: ReadOriginalPower
** 功能描述: 读取原始功率值
** 入口参数: avr_times-平均次数
** 出口参数: 无
********************************************************************************************************/
POWERADJUSTMETER_EXT uint32 ReadOriginalPower(uint8 avr_times,uint8 ch);

/*********************************************************************************************************
** 函数名称: Cal_RealPower
** 功能描述: 计算理论功率值
** 入口参数:  kw_val:校表台输出功率: XXXXX.XXX(W)  3位小数
** 出口参数: 返回计算的理论功率值
********************************************************************************************************/
POWERADJUSTMETER_EXT uint32 Cal_RealPower(uint32 kw_val);

/*********************************************************************************************************
** 函数名称: SoftAdjust_IB_Process
** 功能描述: 校准大功率
** 入口参数: jzval：理论功率值：脉冲常数/3600*表台输出功率/1000*ICONT*8388608 如：1200/3600/220*10/1000*96*8388608=576716
**           channel: 电流通道 0-通道1 1-通道2
** 出口参数: 无
********************************************************************************************************/
POWERADJUSTMETER_EXT __U32_Def SoftAdjust_IB_Process(uint32 jzval,uint8 channel);

/*********************************************************************************************************
** 函数名称: SoftAdjust_IB5_Process
** 功能描述: 校准小功率
** 入口参数: jzval：理论功率值 脉冲常数/3600*表台输出功率/1000*ICONT*8388608 如：1200/3600/220*0.5/1000*96*8388608=28835
**           channle:电流通道号 0-通道1  1-通道2
** 出口参数: 无
********************************************************************************************************/
POWERADJUSTMETER_EXT __U32_Def SoftAdjust_IB5_Process(uint32 jzval,uint8 channel);

/*********************************************************************************************************
** 函数名称: SoftAdjust_Phase_Process
** 功能描述: 功率校准相位
** 入口参数: jzval：理论功率值 脉冲常数/3600*表台输出功率/1000*ICONT*8388608 如：1200/3600/220*5/1000*96*8388608=288358
**           channel：通道号
** 出口参数: 无
********************************************************************************************************/
POWERADJUSTMETER_EXT __U32_Def SoftAdjust_Phase_Process(uint32 jzval,uint8 channel);


/*********************************************************************************************************
** 函数名称: soft_AdjustSub_0
** 功能描述: 0电流校准
** 入口参数:
** 出口参数: 无
********************************************************************************************************/
//POWERADJUSTMETER_EXT void soft_AdjustSub_0();

POWERADJUSTMETER_EXT bit JudgeVoltugeRange(void);

#endif