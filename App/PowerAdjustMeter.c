/*==============================================================================
                Copyright(C) 1997-2019.  Sinowealth Tech. Co., Ltd.
--------------------------------------------------------------------------------
Project Name  : POWERADJUSTMETER.c
Description   :
Notice        :
			 1:
			 2:
Author        : James.wang (email: James.wang@sinowealth.com)
Start Date    : 2015/11/08
Approve Date  :
Version       : V0.0
Function List :
             1:
             2:
RevisionHistory:
Rev#  CheckSum    Date     Author     Comments(Function+Date)
-----+--------+----------+---------+------------------------------------------
0.0     XXXX   2015/11/08 james.wang Just build the file
==============================================================================*/

//-------------------------------------------------------------------------------
#ifndef  POWERADJUSTMETER_C
	#define  POWERADJUSTMETER_C
#endif
//-------------------------------------------------------------------------------

//---------------


#include "config.h"
#include "Headconfig.h"




	uint32 adjust_wcval1;

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** ��������: PowerAdjThread
** ��������: ���߹���У���̴߳���
** �������: ��
** �������: ��
*******************************************************************************************/
void PowerAdjThread(void)                					//����У���߳�
{

	if((gBAdjKeyEnable == 0xA5) && (gBAdjKeyEnable1 == 0x55))
	//if(gbFgKeyProg == 0xF001)	//У��FLAG��Ч,У���ʼ��
	{
		if((LPDCON & Bin(00001000)) == Bin(00001000)) 		//�� VDD > 2.7V,it is ic AC MODE
		{
			if(g_Cal.Flag  == D_InitStart)
			{
				g_Cal.Flag = D_Initing;
				g_Cal.Step = S_DataInit;
			}
			else if(g_Cal.Flag  == D_Initing)
			{
				AdjustMeterInit();	//У�������ʼ��		 					//����У���ʼ������
				gbAdjStepFlag = 0;
				gbAdjCurrentStep = 0;
			}
			else if(g_Cal.Flag  == D_InitAdjust)
			{
				JumpPowerAutoAdjust();		 					//����У�����
				gbAdjShowOk2S = 0;
			}
			else if(g_Cal.Flag  == D_InitOk)				    //У�����OK��ʾ����	1S
			{
				gbAdjStepFlag = 0;
				gbAdjCurrentStep = 0;

				gbAdjShowOk2S++;
				if(gbAdjShowOk2S > 2)
				{
					gbAdjShowOk2S = 0;
					g_Cal.Flag  = D_InitEnd;
				}
			}
			else if(g_Cal.Flag  == D_InitEnd)				    //����У�����
			{
				//У���������У�������FLAGд��EEPROM��
				VER_WRbytes_limit(ADJ_ADDR,&adjust_data.gain,sizeof(adjust_data), 1);
				//д�����FLAG
				MemInitSet(&kwh_value.integer[0],0x00,5);
				VER_WRbytes(EE_KWH0,&kwh_value.integer[0],5, 1);
				//д��У�������־
				MemInitSet(&g_Buffer[0], 0xA5, 4);
				VER_WRbytes(EE_FirstProg_FLAG,&g_Buffer[0],4, 1);
				//д��̵���������־
				MemInitSet(&g_Buffer[0], 0xA5, 2);
				VER_WRbytes(RELAY_STATUS,&DelayStatus[0],2, 1);
				gBAdjKeyCount = 0;
				gBAdjKeyEnable = 0x00;
				gBAdjKeyEnable1 = 0x00;

				//g_Cal.Key = 0x00;
				g_Cal.Flag = 0x00;
				gbFgKeyProg = 0;
			}
		}
	}
	else
	{
		if((LPDCON & Bin(00001000)) == Bin(00001000))
		{
			if((1 == JudgeVoltugeRange())& gbFgKeyProg == 0xF001)

			{
				if(gBAdjKeyStatus == 0)
				{
					gBAdjKeyCount++;
					if(gBAdjKeyCount >= 6)		//6s=1S*6
					{
						gBAdjKeyCount = 0;
						gBAdjKeyEnable = 0xA5;
						gBAdjKeyEnable1 = 0x55;
						gbFgKeyProg = 0xF001;
						if (InFactoryMode())        	  		//����δʹ��
						{
							g_Cal.Flag = D_InitStart;		  	//����У���ʼ������
						}
						gBAdjKeyStatus = 1;		//��У���һ����Ч��־
					}
				}
			}
		}
	}

}



////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** ��������: JumpPowerAutoAdjust
** ��������: ���߹���У�����
** �������: ��
** �������: ��
*******************************************************************************************/
void JumpPowerAutoAdjust(void)
{
    __U32_Def xdata val;
    float xdata L_temp = 0;
    float xdata N_temp = 0;
    float xdata L_N_AdjTemp = 0;
    unsigned char xdata Jump_CmdH = 0;
    unsigned char xdata Jump_CmdL = 0;

	uint8 ajust_type;
	uint32 jzwcval;
	float ajust_temp;

	//unsigned int ID;
	val.val = 0;
	if((gBAdjKeyEnable == 0xA5) && (gBAdjKeyEnable1 == 0x55))
	//if(gbFgKeyProg == 0xF001)	//У��FLAG��Ч,У���ʼ��
	{

		if(1 == JudgeVoltugeRange())//�ڵ�ѹ��Χ��
		{
			if(gbFgKeyProg == 0xF001)
			{
				switch (ADJ_ID)
				{												//1.0
					case Adj5_CHB_Power_GAIN://0x10C4
						if((gbAdjStepFlag&Bin(00000001)) == Bin(00000000))
						{
							if(gbUartAdjust)//UARTͨѶУ��
							{
								if(Sign_Bit)//����
								{
									ajust_temp = (float)Adj_Error * 0.0001 * pow(2, 16) / (1 + (float)Adj_Error * 0.0001);
									jzwcval = (unsigned long) ajust_temp;
								}
								else//����
								{
									ajust_temp =pow(2, 16) - (pow(2, 16) * (float)Adj_Error * 0.0001 / (1 + (float)Adj_Error * 0.0001));
									jzwcval = (unsigned long) ajust_temp;
								}
								val.TBuf.buf0 =	 jzwcval & 0xFF;
								val.TBuf.buf1 =  (jzwcval>>8)&0xFF;
							}
							else//����У��
							{
								ajust_type = gBajust_type;
								jzwcval = GetPowerConstant();//��ȡ���۹���ֵ
								//����EMU�Ĵ���ֵ
								val = AdjustTypeProc(ajust_type,jzwcval,D_Channel_N);
							}
							_nop_();
							WriteEMU_REG(W2GAIN,val);
							adjust_data.w2gain = (unsigned int)val.val;
							gbAverTimes = 0;
							gbAdjStepFlag |= Bin(00000001);			  //OK
							gbAdjCurrentStep = 1;
						}
					break;
					case Adj7_CHB_Phase://0x10C6			//0.5L
						if((gbAdjStepFlag&Bin(00000010)) == Bin(00000000))
						{
							if(gbUartAdjust)//UARTͨѶУ��
							{
								if(Sign_Bit)//����
								{
									ajust_temp = 1.1411 * (float)Adj_Error * 0.0001 * pow(2, 15) / (0.07846 - 0.5697 * (float)Adj_Error * 0.0001);
									jzwcval = (unsigned long) ajust_temp;
								}
								else//����
								{
									ajust_temp = 65535 - 1.1667 * (float)Adj_Error * 0.0001 * pow(2,15) / (0.07846 + (0.5825 * 0.0001 * (float)Adj_Error));
									jzwcval = (unsigned long) ajust_temp;
								}
								val.TBuf.buf0 =	 jzwcval & 0xFF;
								val.TBuf.buf1 =  (jzwcval>>8)&0xFF;
							}
							else
							{
								ajust_type = gBajust_type;
								jzwcval = GetPowerConstant();

								val = AdjustTypeProc(ajust_type,jzwcval,D_Channel_N);
							}
							_nop_();
							WriteEMU_REG(P2CAL,val);
							adjust_data.p2cal = (unsigned int)val.val;
							gbAverTimes = 0;
							gbAdjStepFlag |= Bin(00000010);		   //OK
							gbAdjCurrentStep = 2;
							//ֻУ���С�źŹ������棬����
							//g_Cal.Flag  = D_InitOk;
						}
					break;
					case Adj9_CHB_Offset://0x10C8				//0.1
						if((gbAdjStepFlag&Bin(00000100)) == Bin(00000000))
						{
							if(gbUartAdjust)//UARTͨѶУ��
							{
								//һ���Լ��������ֳ�����
								ajust_temp = (DefaultConstant * DefaultIcont* (float)Adj_Error * 0.0001 * D_UN * D_IB * (float)Sign_Bit * 0.01 * pow(2, 23) / (8000  * 1000)) ;
								ajust_temp = pow(2,16)-ajust_temp/3600;
								jzwcval = (unsigned long) ajust_temp;

								val.TBuf.buf0 =	 jzwcval & 0xFF;
								val.TBuf.buf1 =  (jzwcval>>8)&0xFF;
							}
							else
							{

							}
							_nop_();

							WriteEMU_REG(WATT2OS,val);
							adjust_data.w2os = (unsigned int)val.val;

							gbAverTimes = 0;
							gbAdjStepFlag |= Bin(00000100);			//OK
							gbAdjCurrentStep = 3;
							//g_Cal.Flag  = D_InitEnd;
							//g_Cal.Flag  = D_InitOk;
						}
					break;

					default:
					break;
				}
		    }
			if(!gbUartAdjust)
			{
				//���ݹ��ʻ�ȡУ������
				//1100W ��������
				//550W	��λ�ǲ�

				ADJ_ID = GetAdjMeterTepe();
			}
		 }
		 else
		 {
		     return;		      				//err return
		 }
	 }
}



////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** ��������: AdjustTypeProc
** ��������: IB,0.5L,0.05*IBУ����
** �������: AdjustType У����Ŀ PowerConstant ��ȡ�Ĺ���ֵ	ch ����ͨ��
** �������: ��
*******************************************************************************************/
__U32_Def  AdjustTypeProc(uint8 AdjustType,uint32 PowerConstant,uint8 ch)
{
      __U32_Def val;

	  if(AdjustType == D_Adj_N_10IB)					   	//IB 1.0;
	  {
		   	val = SoftAdjust_IB_Process(PowerConstant,ch);
	  }
	  else if(AdjustType == D_Adj_N_05IB)
	  {
	     	PowerConstant = (uint32)((float)PowerConstant*0.5);	//0.5L			   /2.0
		   	val = SoftAdjust_Phase_Process(PowerConstant,ch);
	  }
	  else if(AdjustType == D_Adj_N_01IB)
	  {                            			    //IB 0.5
	     	PowerConstant = (uint32)((float)PowerConstant*0.05);				   // /20.0
		   	val = SoftAdjust_IB5_Process(PowerConstant,ch);
	  }
	  else
	  {
			val.val = 0;						//err return
	  }
   	  return(val);
}



////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** ��������: GetPowerConstant
** ��������: ȡ��IB���ʳ���
** �������: ��
** �������: ��
*******************************************************************************************/
uint32 GetPowerConstant(void)
{
	uint32 jzwcval;
	uint32 kw_ck;

    mc[0]=(unsigned char)(D_Pulse>>0)&0xFF;	  	//0x00; //���峣��1200
    mc[1]=(unsigned char)(D_Pulse>>8)&0xFF;  	//0x12;
    mc[2]=(unsigned char)(D_Pulse>>16)&0xFF;	//0x00;
    vol_ib=(unsigned int)D_UN;					//220;  //������ѹ220V
    cur_ib=(unsigned int)D_IB;					//5;   //��������5A

	kw_ck=(uint32)vol_ib;
	kw_ck=kw_ck*(uint32)cur_ib;  				//KW
	kw_ck=kw_ck*1000;  							//W
	jzwcval = Cal_RealPower(kw_ck);
	return(jzwcval);
}



////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** ��������: JudgeAdjustType
** ��������: У�������жϳ���
** �������: ��
** �������: ��
*******************************************************************************************/
uint8 JudgeAdjustType(uint8 ch)
{
	uint8  ajust_type;
	uint32 IB_P_REAL;

	uint32 IB_P_1_0;
	uint32 IB_P_1_0_min;
	uint32 IB_P_1_0_max;

	uint32 IB_P_0_5_min;
	uint32 IB_P_0_5_max;

	uint32 IB_P_0_1_min;
	uint32 IB_P_0_1_max;


	/* if(ch == D_Channel_L)
	{
		cal_L_mode();    							//Ĭ�ϴӵ�һͨ����ʼУ׼
		IB_P_REAL = ReadOriginalPower(4,ch);		//��ȡʵ�ʹ���
	}
	else */ if(ch == D_Channel_N)
	{
		cal_N_mode();    							//�����һͨ��û�е���ֵ���л����ڶ�ͨ��
		IB_P_REAL = ReadOriginalPower(4,ch);	 	//��ȡʵ�ʹ���
	}
	else
	{
		ajust_type=0;
	    return(ajust_type);
	}


	IB_P_1_0 	 =  GetPowerConstant();
	IB_P_1_0_min = (uint32)(((float)IB_P_1_0)*0.75);
	IB_P_1_0_max = (uint32)(((float)IB_P_1_0)*1.25);

	IB_P_0_5_min = (uint32)(((float)IB_P_1_0)*0.5*0.75);
	IB_P_0_5_max = (uint32)(((float)IB_P_1_0)*0.5*1.25);

	IB_P_0_1_min = (uint32)(((float)IB_P_1_0)*0.05*0.15);
	IB_P_0_1_max = (uint32)(((float)IB_P_1_0)*0.05*1.25);


	if((IB_P_REAL > IB_P_1_0_min) && (IB_P_REAL < IB_P_1_0_max))
	{
		ajust_type = D_Adj_N_10IB;								//IB 1.0 ch0;
	}
	else if((IB_P_REAL > IB_P_0_5_min) && (IB_P_REAL < IB_P_0_5_max))
	{
		ajust_type = D_Adj_N_05IB;								//IB 0.5 ch0;
	}
	else if((IB_P_REAL > IB_P_0_1_min) && (IB_P_REAL < IB_P_0_1_max))
	{
		ajust_type = D_Adj_N_01IB;								//IB 0.1 ch0;
	}
	else
	{															//err return
		ajust_type=0;
	}
	return(ajust_type);
}



////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** ��������: ReadOriginalPower
** ��������: ��ȡԭʼ����ֵ
** ��ڲ���: avr_times-ƽ������
** ���ڲ���: ��
** ���峣��/3600*��̨�������/1000*ICONT*8388608
** �磺1200/3600/220*0.5/1000*96*8388608=28835
** 2^16*(1-INT(ERR/(1+ERR))//2^16*(INT(ERR/(1+ERR))
** ��ʽ�ĳ���0.00004646*220*10*1200*(1+W1GAIN/2^16)
��*****************************************************************************************/
uint32 ReadOriginalPower(uint8 avr_times,uint8 ch)
{
	__U32_Def val;
	uint8  i;
	unsigned long temp;
	uint32 aery_old;
	uint32 aery_val;
	uint32 avr_aery;

	RSTSTAT=0x00;
	gbAdjSecFlag=0;
	while(!gbAdjSecFlag);              //��ʱ1�� �ȴ������ȶ�
	gbAdjSecFlag=0;
	RSTSTAT=0x00;
	gbAdjSecFlag=0;
	while(!gbAdjSecFlag);              //��ʱ1�� �ȴ������ȶ�
	gbAdjSecFlag=0;

	avr_aery=0;
	for (i=0;i<avr_times;i++)
	{
		RSTSTAT=0x00;
		while(!gbAdjSecFlag);
		gbAdjSecFlag=0;
		RSTSTAT=0x00;
		if(ch==D_Channel_L)
		{
		   	val=ReadEMU_REG(APWRA1);         //�ŵ�1�й�����ֵ�ۼ�ֵ
		}
		else if(ch==D_Channel_N)
		{
		   	val=ReadEMU_REG(APWRA2);         //�ŵ�2�й�����ֵ�ۼ�ֵ
		}
		else
		{
		   	val.val=0;                       //err return
	        return(0);
		}
		temp = val.val;
		if((temp & 0x800000)== 0x800000)	//��������
		{
			temp = ~temp + 1;
			temp &= 0x00ffffff;
		}

		aery_old=temp;
		avr_aery=avr_aery+aery_old;
	}
	aery_val=avr_aery/avr_times;
	RSTSTAT=0x00;
	_nop_();
	return(aery_val);
}


////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** ��������: Cal_RealPower
** ��������: �������۹���ֵ
** ��ڲ���: kw_val:У��̨�������: XXXXX.XXX(W)  3λС��
** ���ڲ���: ���ؼ�������۹���ֵ
*******************************************************************************************/
uint32 Cal_RealPower(uint32 kw_val)
{
	float mid_tmpval;
	uint32 const_val;

	const_val  = WriteToHex(&mc[0],3);	   					//0X001200=0X000640
	mid_tmpval = (float)kw_val;	 	  						//��̨�������:XXXXX.XXX(W)
	mid_tmpval = ((float)mid_tmpval)/((float)1000000); 		//ȥ��С���㣬������ʵ�Ч���㹫ʽ8388608*ICONT*Ec*Pi/3600/8000
	mid_tmpval = ((float)8388608)*((float)g_ConstPara.Icont)*((float)const_val)*mid_tmpval/((float)3600);
	mid_tmpval = mid_tmpval/((float)8000);       			//���۹���ֵ����Ƭ���ڲ����ܼĴ���Ϊ41λ��Ϊ�������㽫��10λȥ����������10λ
	return((uint32)mid_tmpval);
}


////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** ��������: SoftAdjust_IB_Process
** ��������: У׼����W1GAIN��W2GAIN
** ��ڲ���: jzval�����۹���ֵ�����峣��/3600*��̨�������/1000*ICONT*8388608
** �磺1200/3600/220*10/1000*96*8388608=576716
** channel: ����ͨ�� 0-ͨ��1 1-ͨ��2
** ���ڲ���: ��
*******************************************************************************************/
__U32_Def SoftAdjust_IB_Process(uint32 jzval,uint8 channel)
{
	__U32_Def val;
	float  adjustvaltemp,adjustvaltemp1;

	uint32 adjust_wcval;
	uint32 adjust_wcval1;

	uint8 temp;


	if(channel == D_Channel_L)
	{
	   val.val=0;
	   WriteEMU_REG(W1GAIN,val);  					//У����������
	}
	else if(channel == D_Channel_N)
	{
	   val.val=0;
	   WriteEMU_REG(W2GAIN,val);  					//У����������
	}
	else
	{
		val.val = 0;
		return(val);
	}


	temp = channel;

	adjust_wcval = AverPower;
	adjust_wcval1 = ReadOriginalPower(8,channel);	//6

	adjust_wcval = ReadOriginalPower(8,channel);	//6

	if(adjust_wcval >= jzval)         				//�����
	{
	   adjustvaltemp = adjust_wcval;
	   adjustvaltemp = adjustvaltemp-jzval;
	   adjustvaltemp = adjustvaltemp/jzval;
	   adjustvaltemp1 = adjustvaltemp;		   		//�����err
	   adjustvaltemp1 = (float)1+adjustvaltemp1;  	//1-INT(ERR/(1+ERR))-----------
	   adjustvaltemp = adjustvaltemp/adjustvaltemp1;
	   adjustvaltemp = (float)1-adjustvaltemp;
	}
	else			 									//�����
	{
	   adjustvaltemp = adjust_wcval;
	   adjustvaltemp = jzval-adjustvaltemp;
	   adjustvaltemp = adjustvaltemp/jzval;
	   adjustvaltemp1 = adjustvaltemp;		  		//�����err
	   adjustvaltemp1=(float)1-adjustvaltemp1;		//INT(ERR/(1+ERR))-----------
	   adjustvaltemp=adjustvaltemp/adjustvaltemp1;
	}
	adjustvaltemp = adjustvaltemp*(float)65536; 		//2^16*(1-INT(ERR/(1+ERR))//2^16*(INT(ERR/(1+ERR))-----------
	val.val = (uint32)adjustvaltemp;
	return(val);
}




////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** ��������: SoftAdjust_Phase_Process
** ��������: ����У׼��λP1CAL,P2CAL
** ��ڲ���: jzval�����۹���ֵ ���峣��/3600*��̨�������/1000*ICONT*8388608
** �磺1200/3600/220*5/1000*96*8388608=288358
** channel��ͨ����
** ���ڲ���: ��
*******************************************************************************************/
__U32_Def SoftAdjust_Phase_Process(uint32 jzval,uint8 channel)
{
	double  adjustvaltemp,adjustvaltemp1,adjustvaltemp2;
	__U32_Def val;
	bit Freq_type=0;   											//Ƶ������

	uint32 adjust_wcval;
	//uint32 adjust_wcval1;

	uint8 temp;

	val = ReadEMU_REG(FREQ); 									//2λС��
	if(val.val < 5500)
	{
		Freq_type = 0;  										//50HZ
	}
	else
	{
		Freq_type = 1;  										//60HZ
	}
	RSTSTAT = 0x00;



	if(channel == D_Channel_L)
	{
		val.val = 0;
		WriteEMU_REG(P1CAL,val);
	}
	else if(channel == D_Channel_N)
	{
		val.val = 0;
		WriteEMU_REG(P2CAL,val);
	}
	else
	{
		val.val = 0;
		return(val);
	}


	temp = channel;

	adjust_wcval = AverPower;
//	adjust_wcval1 = ReadOriginalPower(8,channel);	//6

	adjust_wcval = ReadOriginalPower(8,channel);	//6
	if(adjust_wcval >= jzval)  									//�����//1.1667*ERR*2^15/(0.07846+0.5828*ERR*100)
	{
		adjustvaltemp = adjust_wcval;
		adjustvaltemp = adjustvaltemp-jzval;
		adjustvaltemp = adjustvaltemp/jzval;
		if(Freq_type)    										//60HZ
		{
			adjustvaltemp1 =  adjustvaltemp*1.1659;
			adjustvaltemp2 =  adjustvaltemp*0.5817+0.09411;
		}
		else            										//50HZ
		{
			adjustvaltemp1 = adjustvaltemp*1.1667;
			adjustvaltemp2 = adjustvaltemp*0.5828+0.07846;
		}
		adjustvaltemp1 = adjustvaltemp1/adjustvaltemp2;
		adjustvaltemp1 = adjustvaltemp1*32768.0;
		adjustvaltemp1 = 65536.0-adjustvaltemp1;
	}
	else	   													//����� //1.1411*ERR*2^15/(0.07846-0.5697*ERR*100)
	{
		adjustvaltemp = adjust_wcval;
		adjustvaltemp = jzval-adjustvaltemp;
		adjustvaltemp = adjustvaltemp/jzval;
		if(Freq_type)     										//60HZ
		{
			adjustvaltemp1 = adjustvaltemp*1.1403;
			adjustvaltemp2 = 0.09411+adjustvaltemp*0.5689;
		}
		else              										//50HZ
		{
			adjustvaltemp1 = adjustvaltemp*1.1411;
			adjustvaltemp2 = 0.07846+adjustvaltemp*0.5697;
		}
		adjustvaltemp1 = adjustvaltemp1/adjustvaltemp2;
		adjustvaltemp1 = adjustvaltemp1*32768.0;
	}
	RSTSTAT = 0x00;
	val.val = (uint32)adjustvaltemp1;
	return(val);
}



////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** ��������: SoftAdjust_IB5_Process
** ��������: У׼С����WATT1OS��WATT2OS
** ��ڲ���: jzval�����۹���ֵ ���峣��/3600*��̨�������/1000*ICONT*8388608
** �磺1200/3600/220*0.5/1000*96*8388608=28835
** channle:����ͨ���� 0-ͨ��1  1-ͨ��2
** ���ڲ���: ��
*******************************************************************************************/
__U32_Def SoftAdjust_IB5_Process(uint32 jzval,uint8 channel)
{
   	float  adjustvaltemp,adjustvaltemp1;
   	__U32_Def val;
// 	 uint32 tmpval;
//   uint32 const_val;
   	uint32 adjust_wcval;
	//uint32 adjust_wcval1;

	uint8 temp;

   	bit Flag_fu = 0;   								//���ű�־



   	if(channel==D_Channel_L)
	{
   	    val.val=0;
	   	WriteEMU_REG(WATT1OS,val);
	}
	else if(channel==D_Channel_N)
	{
   	    val.val=0;
	   	WriteEMU_REG(WATT2OS,val);
	}
	else
	{
		val.val = 0;
		return(val);
	}


//-----ICONT*ERR*Un*Ib**0.01*K*Ec*2^23/(3600*8000*1000)-------------------
/*
	adjustvaltemp1 = adjust_data.icont;
	adjustvaltemp1 = adjustvaltemp1*(float)adjust_data.vol_ib;     //Un
	adjustvaltemp1 = adjustvaltemp1*(float)adjust_data.cur_ib;     //Ib
	adjustvaltemp1 = adjustvaltemp1*0.05;
	const_val = WriteToHex(&adjust_data.mc[0],3);
	adjustvaltemp1 = adjustvaltemp1*(float)const_val;   //Ec
	adjustvaltemp1 = adjustvaltemp1*(float)8388608/(float)3600/(float)8000/(float)1000;
	tmpval = (uint32)adjustvaltemp1;
*/

	temp = channel;

	adjust_wcval = AverPower;
//	adjust_wcval1 = ReadOriginalPower(8,channel);	//6

	adjust_wcval = ReadOriginalPower(8,channel);	//6
	if(adjust_wcval>=jzval)  								//�����
	{
		adjustvaltemp = adjust_wcval;
		adjustvaltemp = adjustvaltemp-jzval;
		adjustvaltemp = adjustvaltemp/jzval;
		Flag_fu=1;
	}
	else	 												//�����
	{
		adjustvaltemp = adjust_wcval;
		adjustvaltemp = jzval-adjustvaltemp;
		adjustvaltemp = adjustvaltemp/jzval;
		Flag_fu = 0;
	}
	adjustvaltemp = adjustvaltemp*adjustvaltemp1;
	if(Flag_fu)
	{
		adjustvaltemp = (float)65536-adjustvaltemp;
	}
	val.val = (uint32)adjustvaltemp;
	return(val);
}




////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** ��������: soft_AdjustSub_0
** ��������: У׼�ӳ���
** ��ڲ���:
** ���ڲ���: ��
*******************************************************************************************/
// void soft_AdjustSub_0()
// {
// 	__U32_U08 xdata val;
// 	int32 xdata tmpval[3];
// 	uint8 xdata i;

// 	val.val=0;
// 	WriteEMU_REG(ADCOSI1,val);
// 	val.val=0;
// 	WriteEMU_REG(ADCOSI2,val);
// 	val.val=0;
// 	WriteEMU_REG(ADCOSU,val);

// 	val.val=0;
// 	val.TBuf.buf0=Bin(00111111);    //bit3:1-ADC����̽�
// 	val.TBuf.buf1=Bin(00000000);
// 	WriteEMU_REG(EMUCFG0,val);

// 	val.val=0;
// 	WriteEMU_REG(IRMS1OS,val);

// 	val.val=0;
// 	WriteEMU_REG(IRMS2OS,val);

// 	RSTSTAT=0x00;
// 	Delay_ms(100);
// //------------У׼ADֵ-------------------------
// 	tmpval[0]=0;
// 	tmpval[1]=0;
// 	tmpval[2]=0;
// 	for(i=0;i<8;i++)
// 	{
// 		Delay_ms(300);
// 		val=ReadEMU_REG(I1DTA);
// 		if(val.val>=0x800000)
// 		{
// 			val.val=~val.val+1;
// 			val.val=val.val&0x00ffffff;
// 			tmpval[0]=tmpval[0]-(int32)val.val;
// 		}
// 		else
// 		{
// 			tmpval[0]=tmpval[0]+(val.val&0x00ffffff);
// 		}
// //      tmpval[0]=tmpval[0]+(val.val&0x00ffffff);
// 		val=ReadEMU_REG(I2DTA);
// 		if(val.val>=0x800000)
// 		{
// 			val.val=~val.val+1;
// 			val.val=val.val&0x00ffffff;
// 			tmpval[1]=tmpval[1]-(int32)val.val;
// 		}
// 		else
// 		{
// 			tmpval[1]=tmpval[1]+(val.val&0x00ffffff);
// 		}
// //      tmpval[1]=tmpval[1]+(val.val&0x00ffffff);
// 		val=ReadEMU_REG(VDTA);
// //      tmpval[2]=tmpval[2]+(val.val&0x00ffffff);
// 		if(val.val>=0x800000)
// 		{
// 			val.val=~val.val+1;
// 			val.val=val.val&0x00ffffff;
// 			tmpval[2]=tmpval[2]-(int32)val.val;
// 		}
// 		else
// 		{
// 			tmpval[2]=tmpval[2]+(val.val&0x00ffffff);
// 		}
// 		RSTSTAT=0x00;
// 	}

// 	for(i=0;i<3;i++)
// 	{
// 		tmpval[i]=tmpval[i]/8;
// 		tmpval[i]=0-tmpval[i];
// 	}
// 	val.val=(uint32)tmpval[0];
// 	WriteEMU_REG(ADCOSI1,val);
// 	val.val=(uint32)tmpval[1];
// 	WriteEMU_REG(ADCOSI2,val);
// 	val.val=(uint32)tmpval[2];
// 	WriteEMU_REG(ADCOSU,val);    //ȡADֵ�ķ���

// /*
// 	for(i=0;i<8;i++)
// 	{
// 		Delay_ms(300);
// 		val=ReadEMU_REG(I1DTA);
// 		if(val.val>=0x800000)
// 		{
// 			val.val=~val.val+1;
// 			val.val=val.val&0x00ffffff;
// 			tmpval[0]=tmpval[0]-(int32)val.val;
// 		}
// 		else
// 		{
// 			tmpval[0]=tmpval[0]+(val.val&0x00ffffff);
// 		}
// 		tmpval[0]=tmpval[0]+(val.val&0x00ffffff);
// 		val=ReadEMU_REG(I2DTA);
// 		if(val.val>=0x800000)
// 		{
// 			val.val=~val.val+1;
// 			val.val=val.val&0x00ffffff;
// 			tmpval[0]=tmpval[0]-(int32)val.val;
// 		}
// 		else
// 		{
// 			tmpval[0]=tmpval[0]+(val.val&0x00ffffff);
// 		}
// 		tmpval[1]=tmpval[1]+(val.val&0x00ffffff);
// 		val=ReadEMU_REG(VDTA);
// 		if(val.val>=0x800000)
// 		{
// 			val.val=~val.val+1;
// 			val.val=val.val&0x00ffffff;
// 			tmpval[0]=tmpval[0]-(int32)val.val;
// 		}
// 		else
// 		{
// 			tmpval[0]=tmpval[0]+(val.val&0x00ffffff);
// 		}
// 		tmpval[2]=tmpval[2]+(val.val&0x00ffffff);
// 		RSTSTAT=0x00;
// 	}
// 	for(i=0;i<3;i++)
// 	{
// 		tmpval[i]=tmpval[i]/8;
// 	}
// */
// 	val.val=0;
// 	val.TBuf.buf0=Bin(00110111);    //bit3:1-ADC����̽�
// 	val.TBuf.buf1=Bin(00000000);
// 	WriteEMU_REG(EMUCFG0,val);

// 	RSTSTAT=0x00;
// 	Delay_ms(100);
// 	//------------У׼��Чֵƫ��-------------------------
// 	tmpval[0]=0;
// 	tmpval[1]=0;
// 	tmpval[2]=0;
// 	for(i=0;i<8;i++)
// 	{
// 		Delay_ms(300);
// 		val=ReadEMU_REG(I1RMS);
// //      tmpval[0]=tmpval[0]+(val.val&0x00ffffff);
// 		if(val.val>=0x800000)
// 		{
// 			val.val=~val.val+1;
// 			val.val=val.val&0x00ffffff;
// 			tmpval[0]=tmpval[0]-(int32)val.val;
// 		}
// 		else
// 		{
// 			tmpval[0]=tmpval[0]+(val.val&0x00ffffff);
// 		}
// 		val=ReadEMU_REG(I2RMS);
// //      tmpval[1]=tmpval[1]+(val.val&0x00ffffff);
// 		if(val.val>=0x800000)
// 		{
// 			val.val=~val.val+1;
// 			val.val=val.val&0x00ffffff;
// 			tmpval[1]=tmpval[1]-(int32)val.val;
// 		}
// 		else
// 		{
// 			tmpval[1]=tmpval[1]+(val.val&0x00ffffff);
// 		}
// 		val=ReadEMU_REG(VRMS);
// //      tmpval[2]=tmpval[2]+(val.val&0x00ffffff);
// 		if(val.val>=0x800000)
// 		{
// 			val.val=~val.val+1;
// 			val.val=val.val&0x00ffffff;
// 			tmpval[2]=tmpval[2]-(int32)val.val;
// 		}
// 		else
// 		{
// 			tmpval[2]=tmpval[2]+(val.val&0x00ffffff);
// 		}
// 		RSTSTAT=0x00;
// 	}

// 	for(i=0;i<3;i++)
// 	{
// 		tmpval[i]=tmpval[i]/8;
// 		tmpval[i]=0-tmpval[i];
// 	}
// 	val.val=(uint32)tmpval[0];
// 	WriteEMU_REG(IRMS1OS,val);
// 	val.val=(uint32)tmpval[1];
// 	WriteEMU_REG(IRMS2OS,val);
// 	val.val=(uint32)tmpval[2];
// 	WriteEMU_REG(VRMSOS,val);    				//ȡֱ�Ӷ�ȡ��ֵ
// }



////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** ��������: JudgeVoltugeRange
** ��������:
** ��ڲ���:
** ���ڲ���: ��
*******************************************************************************************/
bit JudgeVoltugeRange(void)
{
	bit temp;
	__U32_Def val;

  	val = ReadEMU_REG(VRMS);	  					//�����ѹ	///read Line IB VOLTAGE//220v-0.2v-26d6d2h

	if(((float)val.val>(D_UN_Min)) &&  ((float)val.val<(D_UN_Max)))
	{
		temp = 1;
	}
	else
	{
		temp = 0;
	}

	return(temp);
}





////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** ��������: AverStablePowerJudge
** ��������:
** ��ڲ���:
** ���ڲ���: ��
*******************************************************************************************/
bit AverStablePowerJudge(void)
{
    bit tempflag;
	__U32_Def val;
	unsigned long temp;

	AdjTemp[0] = AdjTemp[1];
	AdjTemp[1] = AdjTemp[2];
	AdjTemp[2] = AdjTemp[3];
	AdjTemp[3] = AdjTemp[4];
	AdjTemp[4] = AdjTemp[5];

	val = ReadEMU_REG(APWRA2);         		//�ŵ�2�й�����ֵ�ۼ�ֵ
	temp = val.val;
	if((temp & 0x800000)== 0x800000)	//��������
	{
		temp = ~temp + 1;
		temp &= 0x00ffffff;
	}
	AdjTempValueEverySec = temp;

	AdjTemp[6] = AdjTemp[7];
	AdjTemp[7] = temp;
	AdjTemp[5] = (uint32)((float)(AdjTemp[6]+AdjTemp[7])*0.5);


	AdjTemp0 = 0;
	AdjTemp0 = AdjTemp[0]+AdjTemp[1]+AdjTemp[2]+AdjTemp[3]+AdjTemp[4]+AdjTemp[5];
	AdjTemp0 = (uint32)((float)AdjTemp0/6.0);					//6 times  average
    AverPower = AdjTemp0;
	tempflag = 0;

	gbAverTimes++;
	if(gbAverTimes > 6)
	{
		gbAverTimes = 6;
		if(AdjTemp0>=AdjTempValueEverySec)
		{
			if(((AdjTemp0-AdjTempValueEverySec)/AdjTemp0) < D_PowerErr)
			{
			   tempflag = 1;			//�����ȶ���־
			}
			else
			{
			   tempflag = 0;			//���ʲ����ı�־
			}
		}
		else
		{
			if(((AdjTempValueEverySec-AdjTemp0)/AdjTempValueEverySec) < D_PowerErr)
			{
			   tempflag = 1;			//�����ȶ���־
			}
			else
			{
			   tempflag = 0;			//���ʲ����ı�־
			}
		}
	}
	return(tempflag);
}

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** ��������: GetAdjMeterTepe
** ��������:
** ��ڲ���:
** ���ڲ���: ��
*******************************************************************************************/
unsigned int GetAdjMeterTepe(void)
{
	uint32 IB_P_REAL;

	uint32 IB_P_1_0;
	uint32 IB_P_1_0_min;
	uint32 IB_P_1_0_max;

	uint32 IB_P_0_5_min;
	uint32 IB_P_0_5_max;

	uint32 IB_P_0_1_min;
	uint32 IB_P_0_1_max;

	unsigned int ID;

    if(1 == AverStablePowerJudge())//�����ȶ���־
	{
		IB_P_REAL 	 =  AverPower;

		IB_P_1_0 	 =  GetPowerConstant();
		IB_P_1_0_min = (uint32)(((float)IB_P_1_0)*0.75);
		IB_P_1_0_max = (uint32)(((float)IB_P_1_0)*1.25);

		IB_P_0_5_min = (uint32)(((float)IB_P_1_0)*0.5*0.75);
		IB_P_0_5_max = (uint32)(((float)IB_P_1_0)*0.5*1.25);

		IB_P_0_1_min = (uint32)(((float)IB_P_1_0)*0.05*0.15);
		IB_P_0_1_max = (uint32)(((float)IB_P_1_0)*0.05*1.25);


		if((IB_P_REAL > IB_P_1_0_min) && (IB_P_REAL < IB_P_1_0_max))
		{
			gBajust_type = D_Adj_N_10IB;								//IB 1.0 ch0;
			ID = Adj5_CHB_Power_GAIN;
			gbAdjustShowID = Adj5_CHB_Power_GAIN;
		}
		else if((IB_P_REAL > IB_P_0_5_min) && (IB_P_REAL < IB_P_0_5_max))
		{
			gBajust_type = D_Adj_N_05IB;								//IB 0.5 ch0;
			ID = Adj7_CHB_Phase;
			gbAdjustShowID = Adj7_CHB_Phase;
		}
		else if((IB_P_REAL > IB_P_0_1_min) && (IB_P_REAL < IB_P_0_1_max))
		{
			gBajust_type = D_Adj_N_01IB;								//IB 0.1 ch0;
			ID = Adj9_CHB_Offset;
			gbAdjustShowID = Adj9_CHB_Offset;
		}
		else
		{
			ID = 0;																	//err return
			gBajust_type = 0;
			gbAdjustShowID = 0;
		}
	}
	else
	{
		ID = 0;																	//err return
		gBajust_type = 0;
		gbAdjustShowID = 0;
//		gbAverTimes = 0;
	}
	return(ID);
}