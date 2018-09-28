/*==============================================================================
            Copyright(C) 1997-2019.  Sinowealth Tech. Co., Ltd.
--------------------------------------------------------------------------------
Project No.   : XXX
File Name     : _642B_EMU..H
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
#ifndef  _642B_EMU_H
#define  _642B_EMU_H


#ifdef _642B_EMU_C
   #define  _642B_EMU_EXT
#else
   #define  _642B_EMU_EXT   extern
#endif
//------------------------------------------------------------------------------

//--------------


/*****计量参数地址******/
#define		I1DTA       		0x00   //电流通道1ADC输出值
#define		I2DTA       		0x01   //电流通道2ADC输出值
#define		VDTA        		0x02   //电压通道ADC输出值
#define		APWR1       		0x03   //通道1瞬时有功功率
#define		RPWR1   			0x04   //通道1瞬时无功功率
#define		APWR2   			0x05   //通道2瞬时有功功率
#define		RPWR2   			0x06   //通道2瞬时无功功率
#define		AERY   				0x07   //有功电能值（正常计量模式）
#define		RERY  	    		0x08   //无功电能值（正常计量模式）
#define		FREQ  				0x09   //电压频率值
#define		I1RMS  				0x0A   //通道1电流有效值
#define		I2RMS  				0x0B   //通道2电流有效值
#define		VRMS   				0x0C   //电压有效值
#define		WPA    				0x0D   //有功电能脉冲累计值（正常计量模式）
#define		VARPA  				0x0E   //无功电能脉冲累计值（正常计量模式）
#define		APWRA1   			0x0F   //通道1有功功率值平均值（设定周期）
#define		RPWRA1   			0x10   //通道1无功功率值平均值（设定周期）
#define		APWRA2   			0x11   //通道2有功功率值平均值（设定周期）
#define		RPWRA2   			0x12   //通道2无功功率值平均值（设定周期）
#define		AERYL  				0x13   //有功电能低位值（正常计量模式）
#define		RERYL  				0x14   //无功电能低位值（正常计量模式）
#define		VDTAMAX  			0x15   //电压信道最大波形ADC采样值（设定周期）
#define		AERY_CONSTH   		0x16   //有功电能值（常数模式）
#define		RERY_CONSTH  	    0x17   //无功电能值（常数模式）
#define		AERY_CONSTL   		0x18   //有功电能低位值（常数模式）
#define		RERY_CONSTL  	    0x19   //无功电能低位值（常数模式）
#define		WPA_CONST    		0x1A   //有功电能脉冲累计值（常数模式）
#define		VARPA_CONST  		0x1B   //无功电能脉冲累计值（常数模式）


/*****校表参数地址******/
#define		EMUCFG0  			0x30   //EMU计量配置寄存器0
#define		EMUCFG1  			0x31   //EMU计量配置寄存器1
#define		EMUCFG2  			0x32   //EMU计量配置寄存器2
#define		EMUCFG3  			0x33   //EMU计量配置寄存器3
#define		W1GAIN  			0x34   //通道1功率增益寄存器(有功无功使用同一组)
#define		P1CAL  	    		0x35   //通道1电压电流相位补偿寄存器
#define		W2GAIN    			0x36   //通道2功率增益寄存器(有功无功使用同一组)
#define		P2CAL   			0x37   //通道2电压电流相位补偿寄存器
#define		I2GAIN   			0x38   //通道2电流增益设置寄存器
#define		WATT1OS   			0x39   //通道1有功功率偏置
#define		WATT2OS    			0x3A   //通道2有功功率偏置
#define		VAR1OS    			0x3B   //通道1无功功率偏置
#define		VAR2OS    			0x3C   //通道2无功功率偏置
#define		IRMS1OS    			0x3D   //通道1电流有效值偏置
#define		IRMS2OS    			0x3E   //通道2电流有效值偏置
#define		VRMSOS    			0x3F   //电压有效值偏置
#define		ADCOSI1    			0x40   //电流通道1 ADC偏置设置
#define		ADCOSI2   			0x41   //电流通道2 ADC偏置设置
#define		ADCOSU    			0x42   //电压通道ADC偏置设置
#define		SPTS    			0x43   //功率启动设置寄存器
#define		VCONST    			0x44   //掉零线电压固定值
#define		SAGTHR    			0x45   //失压门限设置
#define		SAGCNT    			0x46   //失压采样计数
#define		ICONT    			0x47   //输出脉冲频率设置
#define		PCNT      			0x48   //快速有功脉冲计数（正常计量模式）
#define		QCNT    			0x49   //快速无功脉冲计数（正常计量模式）
#define		SUMSAMPS   			0x4A   //计算周期设置寄存器
#define		APCONST    			0x4B   //有功功率常数寄存器
#define		RPCONST   			0x4C   //无功功率常数寄存器
#define		PCNT_CONST   		0x4D   //快速有功脉冲计数（常数模式）
#define		VQCNT_CONST  		0x4E   //快速无功脉冲计数（常数模式）



#define     D_ClearEnergyPSW       	0xA5 	//清电量的命令字

#define     SAG_CHECK()          	(0)		//((EMUSR & 0x10) == 0x10)

#define     DefaultConstant      	1200	//imp
#define     DefaultThreshold     	0x0C
#define     DefaultIcont        	0x30
//#define     DefaultIcont        	0x41

#define     DefaultIStart        	20		    				//mA  //1级表的启动电流为千分之4的Ib
#define     C_StartCurrentT      	DefaultIStart/2		        //mA  //启动实验时电流通道切换的判断依据，取启动电流的一半

#define     DefaultUn            	220.0		//V
#define     DefaultIb            	5.0		//A
#define     DefaultPStartPer       	0.004	    //1级表的启动电流为千分之4的P,2级表的启动电流为千分之5的P

//1000w*60min*60S
//---------------=S	潜动时间							   //234s,0x00EA
//imp*Un*Ib*0.004
#define     D_StartCreepT			(unsigned int)((600*1000000)/(DefaultConstant*DefaultUn*DefaultIb))
#define     C_StartCreepT        	(unsigned int)(0x1AA2)      //潜动时间 220V 5A 1200imp  113.63分钟=6818秒


#define     C_DefaultCurExTT        5        				    //上电初始化为5
#define     C_CurExToShuntDlyMIN    0
#define     C_CurExToShuntDlyT      2
#define     C_CurExToCTDlyT         8
#define     C_CurExToCTDlyMAX       10

#define     C_IStartForFilter       20       				//实际启动电流 5A*0.4%=20.0mA



#define 	V_MUL      		100.0					   		//A=MA*1000,这个是将电压系数放大100倍吗？
#define 	EMU_ADC_REF    	1.4                             //EMU的参考电压，单位V
#define 	EMU_ADC_Value  	8388608.0                       //2^23
#define 	V_R1       		1200.0					   		//高压端分压电阻，单位KΩ
#define 	V_R2       		100.0                           //低压端分压电阻，单位KΩ
#define 	V_PGA      		1.0                             //电压采样PGA放大倍数，为1

#define 	I_MUL      		1000.0					   		//A=MA*1000
#define 	I_R_MT	   		0.00040					   		//锰铜采样电阻，是150uΩ=0.00015KΩ
#define 	I_R_MT_PGA 		16.0						   	//PGA放大倍数为16
#define 	I_R_CT	   		50.0					   		//互感器是180uΩ
#define 	I_R_CT_PGA 		1.0						   		//PGA放大倍数为1


//VRMS*(float)((1.4*2551.0/8388608.0*1.0)*100.0);   		//分压网络是2550kΩ和1kΩ，PGA放大倍数为1
#define V_Coefficient	(float)(((EMU_ADC_REF*(V_R1+V_R2)*V_PGA)/(EMU_ADC_Value*V_R2))*V_MUL)		 //(float)0.0426

//I1RMS*(float)((1.4/(0.00018*16.0)/8388608.0)*1000.0);   	//锰铜是180uΩ，PGA放大倍数为16
#define I1_Coefficient	(float)((EMU_ADC_REF/((I_R_MT*I_R_MT_PGA)/EMU_ADC_Value)*I_MUL)) //(float)0.058;

//I2RMS*(float)((1.4/(0.00018*16.0)/8388608.0)*1000.0);   	//锰铜是180uΩ，PGA放大倍数为16
#define I2_Coefficient	(float)((EMU_ADC_REF/((I_R_CT*I_R_CT_PGA)/EMU_ADC_Value)*I_MUL)) //(float)0.058;

//APWRA1*(float)((3600.0*8000.0/g_ConstPara.Icont/g_ConstPara.Constant/8388608.0)*1000.0*16384.0);	  //放大16384倍的功率值，单位为W
#define P1_Coefficient	(float)((3600.0*8000.0*1000.0*16384.0)/EMU_ADC_Value)			//(float)(56250000.0

//APWRA2*(float)((3600.0*8000.0/g_ConstPara.Icont/g_ConstPara.Constant/8388608.0)*1000.0*16384.0);	  //放大16384倍的功率值，单位为W
#define P2_Coefficient	(float)((3600.0*8000.0*1000.0*16384.0)/EMU_ADC_Value)			//(float)(56250000.0

//RPWRA1*(float)((3600.0*8000.0/g_ConstPara.Icont/g_ConstPara.Constant/8388608.0)*1000.0*16384.0);	  //放大16384倍的功率值，单位为W
#define QP1_Coefficient	(float)((3600.0*8000.0*1000.0*16384.0)/EMU_ADC_Value)			//(float)(56250000.0

//RPWRA2*(float)((3600.0*8000.0/g_ConstPara.Icont/g_ConstPara.Constant/8388608.0)*1000.0*16384.0);	  //放大16384倍的功率值，单位为W
#define QP2_Coefficient	(float)((3600.0*8000.0*1000.0*16384.0)/EMU_ADC_Value)			//(float)(56250000.0



_642B_EMU_EXT void Init_EMU(void);//修改完成
_642B_EMU_EXT void EMU_Check(void);//EMU校准,待修改
_642B_EMU_EXT void ReadEMUFromTOEeprom(void);   //从EEPROM载入校表参数，待修改, 不知道有什么用
_642B_EMU_EXT void EMUTampProc(void);//ENU运行函数，待修改
//_642B_EMU_EXT void AdjustSub(void);


//EMU寄存器操作
_642B_EMU_EXT void WriteEMU_REG(uint8 sadr,__U32_Def val);//写间接寄存器
_642B_EMU_EXT __U32_Def ReadEMU_REG(uint8 sadr);//读间接寄存器
_642B_EMU_EXT void ClrEMU_REG(unsigned char sadr);//复位间接寄存器为0


//计量模式
//_642B_EMU_EXT void cal_L_mode(void);//使用L先计量
_642B_EMU_EXT void cal_N_mode(void);//使用N线计量
//_642B_EMU_EXT void cal_1P3L_mode(void);


#endif