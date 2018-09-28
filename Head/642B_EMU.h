/*==============================================================================
            Copyright(C) 1997-2019.  Sinowealth Tech. Co., Ltd.
--------------------------------------------------------------------------------
Project No.   : XXX
File Name     : _642B_EMU..H
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
#ifndef  _642B_EMU_H
#define  _642B_EMU_H


#ifdef _642B_EMU_C
   #define  _642B_EMU_EXT
#else
   #define  _642B_EMU_EXT   extern
#endif
//------------------------------------------------------------------------------

//--------------


/*****����������ַ******/
#define		I1DTA       		0x00   //����ͨ��1ADC���ֵ
#define		I2DTA       		0x01   //����ͨ��2ADC���ֵ
#define		VDTA        		0x02   //��ѹͨ��ADC���ֵ
#define		APWR1       		0x03   //ͨ��1˲ʱ�й�����
#define		RPWR1   			0x04   //ͨ��1˲ʱ�޹�����
#define		APWR2   			0x05   //ͨ��2˲ʱ�й�����
#define		RPWR2   			0x06   //ͨ��2˲ʱ�޹�����
#define		AERY   				0x07   //�й�����ֵ����������ģʽ��
#define		RERY  	    		0x08   //�޹�����ֵ����������ģʽ��
#define		FREQ  				0x09   //��ѹƵ��ֵ
#define		I1RMS  				0x0A   //ͨ��1������Чֵ
#define		I2RMS  				0x0B   //ͨ��2������Чֵ
#define		VRMS   				0x0C   //��ѹ��Чֵ
#define		WPA    				0x0D   //�й����������ۼ�ֵ����������ģʽ��
#define		VARPA  				0x0E   //�޹����������ۼ�ֵ����������ģʽ��
#define		APWRA1   			0x0F   //ͨ��1�й�����ֵƽ��ֵ���趨���ڣ�
#define		RPWRA1   			0x10   //ͨ��1�޹�����ֵƽ��ֵ���趨���ڣ�
#define		APWRA2   			0x11   //ͨ��2�й�����ֵƽ��ֵ���趨���ڣ�
#define		RPWRA2   			0x12   //ͨ��2�޹�����ֵƽ��ֵ���趨���ڣ�
#define		AERYL  				0x13   //�й����ܵ�λֵ����������ģʽ��
#define		RERYL  				0x14   //�޹����ܵ�λֵ����������ģʽ��
#define		VDTAMAX  			0x15   //��ѹ�ŵ������ADC����ֵ���趨���ڣ�
#define		AERY_CONSTH   		0x16   //�й�����ֵ������ģʽ��
#define		RERY_CONSTH  	    0x17   //�޹�����ֵ������ģʽ��
#define		AERY_CONSTL   		0x18   //�й����ܵ�λֵ������ģʽ��
#define		RERY_CONSTL  	    0x19   //�޹����ܵ�λֵ������ģʽ��
#define		WPA_CONST    		0x1A   //�й����������ۼ�ֵ������ģʽ��
#define		VARPA_CONST  		0x1B   //�޹����������ۼ�ֵ������ģʽ��


/*****У�������ַ******/
#define		EMUCFG0  			0x30   //EMU�������üĴ���0
#define		EMUCFG1  			0x31   //EMU�������üĴ���1
#define		EMUCFG2  			0x32   //EMU�������üĴ���2
#define		EMUCFG3  			0x33   //EMU�������üĴ���3
#define		W1GAIN  			0x34   //ͨ��1��������Ĵ���(�й��޹�ʹ��ͬһ��)
#define		P1CAL  	    		0x35   //ͨ��1��ѹ������λ�����Ĵ���
#define		W2GAIN    			0x36   //ͨ��2��������Ĵ���(�й��޹�ʹ��ͬһ��)
#define		P2CAL   			0x37   //ͨ��2��ѹ������λ�����Ĵ���
#define		I2GAIN   			0x38   //ͨ��2�����������üĴ���
#define		WATT1OS   			0x39   //ͨ��1�й�����ƫ��
#define		WATT2OS    			0x3A   //ͨ��2�й�����ƫ��
#define		VAR1OS    			0x3B   //ͨ��1�޹�����ƫ��
#define		VAR2OS    			0x3C   //ͨ��2�޹�����ƫ��
#define		IRMS1OS    			0x3D   //ͨ��1������Чֵƫ��
#define		IRMS2OS    			0x3E   //ͨ��2������Чֵƫ��
#define		VRMSOS    			0x3F   //��ѹ��Чֵƫ��
#define		ADCOSI1    			0x40   //����ͨ��1 ADCƫ������
#define		ADCOSI2   			0x41   //����ͨ��2 ADCƫ������
#define		ADCOSU    			0x42   //��ѹͨ��ADCƫ������
#define		SPTS    			0x43   //�����������üĴ���
#define		VCONST    			0x44   //�����ߵ�ѹ�̶�ֵ
#define		SAGTHR    			0x45   //ʧѹ��������
#define		SAGCNT    			0x46   //ʧѹ��������
#define		ICONT    			0x47   //�������Ƶ������
#define		PCNT      			0x48   //�����й������������������ģʽ��
#define		QCNT    			0x49   //�����޹������������������ģʽ��
#define		SUMSAMPS   			0x4A   //�����������üĴ���
#define		APCONST    			0x4B   //�й����ʳ����Ĵ���
#define		RPCONST   			0x4C   //�޹����ʳ����Ĵ���
#define		PCNT_CONST   		0x4D   //�����й��������������ģʽ��
#define		VQCNT_CONST  		0x4E   //�����޹��������������ģʽ��



#define     D_ClearEnergyPSW       	0xA5 	//�������������

#define     SAG_CHECK()          	(0)		//((EMUSR & 0x10) == 0x10)

#define     DefaultConstant      	1200	//imp
#define     DefaultThreshold     	0x0C
#define     DefaultIcont        	0x30
//#define     DefaultIcont        	0x41

#define     DefaultIStart        	20		    				//mA  //1�������������Ϊǧ��֮4��Ib
#define     C_StartCurrentT      	DefaultIStart/2		        //mA  //����ʵ��ʱ����ͨ���л����ж����ݣ�ȡ����������һ��

#define     DefaultUn            	220.0		//V
#define     DefaultIb            	5.0		//A
#define     DefaultPStartPer       	0.004	    //1�������������Ϊǧ��֮4��P,2�������������Ϊǧ��֮5��P

//1000w*60min*60S
//---------------=S	Ǳ��ʱ��							   //234s,0x00EA
//imp*Un*Ib*0.004
#define     D_StartCreepT			(unsigned int)((600*1000000)/(DefaultConstant*DefaultUn*DefaultIb))
#define     C_StartCreepT        	(unsigned int)(0x1AA2)      //Ǳ��ʱ�� 220V 5A 1200imp  113.63����=6818��


#define     C_DefaultCurExTT        5        				    //�ϵ��ʼ��Ϊ5
#define     C_CurExToShuntDlyMIN    0
#define     C_CurExToShuntDlyT      2
#define     C_CurExToCTDlyT         8
#define     C_CurExToCTDlyMAX       10

#define     C_IStartForFilter       20       				//ʵ���������� 5A*0.4%=20.0mA



#define 	V_MUL      		100.0					   		//A=MA*1000,����ǽ���ѹϵ���Ŵ�100����
#define 	EMU_ADC_REF    	1.4                             //EMU�Ĳο���ѹ����λV
#define 	EMU_ADC_Value  	8388608.0                       //2^23
#define 	V_R1       		1200.0					   		//��ѹ�˷�ѹ���裬��λK��
#define 	V_R2       		100.0                           //��ѹ�˷�ѹ���裬��λK��
#define 	V_PGA      		1.0                             //��ѹ����PGA�Ŵ�����Ϊ1

#define 	I_MUL      		1000.0					   		//A=MA*1000
#define 	I_R_MT	   		0.00040					   		//��ͭ�������裬��150u��=0.00015K��
#define 	I_R_MT_PGA 		16.0						   	//PGA�Ŵ���Ϊ16
#define 	I_R_CT	   		50.0					   		//��������180u��
#define 	I_R_CT_PGA 		1.0						   		//PGA�Ŵ���Ϊ1


//VRMS*(float)((1.4*2551.0/8388608.0*1.0)*100.0);   		//��ѹ������2550k����1k����PGA�Ŵ���Ϊ1
#define V_Coefficient	(float)(((EMU_ADC_REF*(V_R1+V_R2)*V_PGA)/(EMU_ADC_Value*V_R2))*V_MUL)		 //(float)0.0426

//I1RMS*(float)((1.4/(0.00018*16.0)/8388608.0)*1000.0);   	//��ͭ��180u����PGA�Ŵ���Ϊ16
#define I1_Coefficient	(float)((EMU_ADC_REF/((I_R_MT*I_R_MT_PGA)/EMU_ADC_Value)*I_MUL)) //(float)0.058;

//I2RMS*(float)((1.4/(0.00018*16.0)/8388608.0)*1000.0);   	//��ͭ��180u����PGA�Ŵ���Ϊ16
#define I2_Coefficient	(float)((EMU_ADC_REF/((I_R_CT*I_R_CT_PGA)/EMU_ADC_Value)*I_MUL)) //(float)0.058;

//APWRA1*(float)((3600.0*8000.0/g_ConstPara.Icont/g_ConstPara.Constant/8388608.0)*1000.0*16384.0);	  //�Ŵ�16384���Ĺ���ֵ����λΪW
#define P1_Coefficient	(float)((3600.0*8000.0*1000.0*16384.0)/EMU_ADC_Value)			//(float)(56250000.0

//APWRA2*(float)((3600.0*8000.0/g_ConstPara.Icont/g_ConstPara.Constant/8388608.0)*1000.0*16384.0);	  //�Ŵ�16384���Ĺ���ֵ����λΪW
#define P2_Coefficient	(float)((3600.0*8000.0*1000.0*16384.0)/EMU_ADC_Value)			//(float)(56250000.0

//RPWRA1*(float)((3600.0*8000.0/g_ConstPara.Icont/g_ConstPara.Constant/8388608.0)*1000.0*16384.0);	  //�Ŵ�16384���Ĺ���ֵ����λΪW
#define QP1_Coefficient	(float)((3600.0*8000.0*1000.0*16384.0)/EMU_ADC_Value)			//(float)(56250000.0

//RPWRA2*(float)((3600.0*8000.0/g_ConstPara.Icont/g_ConstPara.Constant/8388608.0)*1000.0*16384.0);	  //�Ŵ�16384���Ĺ���ֵ����λΪW
#define QP2_Coefficient	(float)((3600.0*8000.0*1000.0*16384.0)/EMU_ADC_Value)			//(float)(56250000.0



_642B_EMU_EXT void Init_EMU(void);//�޸����
_642B_EMU_EXT void EMU_Check(void);//EMUУ׼,���޸�
_642B_EMU_EXT void ReadEMUFromTOEeprom(void);   //��EEPROM����У����������޸�, ��֪����ʲô��
_642B_EMU_EXT void EMUTampProc(void);//ENU���к��������޸�
//_642B_EMU_EXT void AdjustSub(void);


//EMU�Ĵ�������
_642B_EMU_EXT void WriteEMU_REG(uint8 sadr,__U32_Def val);//д��ӼĴ���
_642B_EMU_EXT __U32_Def ReadEMU_REG(uint8 sadr);//����ӼĴ���
_642B_EMU_EXT void ClrEMU_REG(unsigned char sadr);//��λ��ӼĴ���Ϊ0


//����ģʽ
//_642B_EMU_EXT void cal_L_mode(void);//ʹ��L�ȼ���
_642B_EMU_EXT void cal_N_mode(void);//ʹ��N�߼���
//_642B_EMU_EXT void cal_1P3L_mode(void);


#endif