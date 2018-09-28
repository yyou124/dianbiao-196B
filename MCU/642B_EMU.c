/*==============================================================================
                Copyright(C) 1997-2019.  Sinowealth Tech. Co., Ltd.
--------------------------------------------------------------------------------
Project Name  : _642B_EMU.c
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
#ifndef  _642B_EMU_C
	#define  _642B_EMU_C
#endif
//-------------------------------------------------------------------------------

//---------------



#include "config.h"
#include "Headconfig.h"



////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** ��������: ClrEMU_REG
** ��������: ��EMU�м�ӼĴ���,��λEMU�Ĵ���Ϊ0
** �������: sdar:Ҫд��ļĴ����ĵ�ַ
             *ptr:д�����ݵ��׵�ַ
** �������: ��
*******************************************************************************************/
void ClrEMU_REG(unsigned char sadr)
{
    EA = 0;
    EADR = 0;
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    EDTAH = 0;
    EDTAM = 0;
    EDTAL = 0;
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    EADR = sadr|0x80;
    EA = 1;
}

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** ��������: WriteEMU_REG
** ��������: ��EMU�м�ӼĴ���д����
** �������: sdar:Ҫд��ļĴ����ĵ�ַ
             *ptr:д�����ݵ��׵�ַ
** �������: ��
*******************************************************************************************/
void WriteEMU_REG(uint8 sadr,__U32_Def val)			 //20120315
{
	EA = 0;
    EADR=0x00;
	_nop_();	            //һ��NOP
	_nop_();	            //һ��NOP
	_nop_();	            //һ��NOP
	_nop_();	            //һ��NOP
	_nop_();	            //һ��NOP
	_nop_();	            //һ��NOP
	EDTAH=val.TBuf.buf2;
	EDTAM=val.TBuf.buf1;
	EDTAL=val.TBuf.buf0;
	_nop_();	            //һ��NOP
	_nop_();	            //һ��NOP
	_nop_();	            //һ��NOP
	_nop_();	            //һ��NOP
	_nop_();	            //һ��NOP
	_nop_();	            //һ��NOP
	EADR=sadr|0x80;
    _nop_();	            //һ��NOP
	_nop_();	            //һ��NOP
	_nop_();	            //һ��NOP
	_nop_();	            //һ��NOP
	_nop_();	            //һ��NOP
	_nop_();	            //һ��NOP
    EA = 1;
}

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** ��������: ReadEMU_REG
** ��������: ��EMU�м�ӼĴ���������
** �������: sdar:Ҫ����ļĴ����ĵ�ַ
			    num :����������ֽ���
				 datatype:�������ͣ�0���޷�����;1:�з�����
** �������: rdata:���ؼĴ�����ֵ
*******************************************************************************************/
__U32_Def ReadEMU_REG(uint8 sadr)
{
	__U32_Def val;

    EA = 0;
	val.val=0x00;

	EADR=sadr&0x7F;
	_nop_();	            //һ��NOP
	_nop_();	            //һ��NOP
	_nop_();	            //һ��NOP
	_nop_();	            //һ��NOP
	_nop_();	            //һ��NOP
	_nop_();	            //һ��NOP
	_nop_();	            //һ��NOP
	_nop_();	            //һ��NOP
	_nop_();	            //һ��NOP
	_nop_();	            //һ��NOP
	val.TBuf.buf2=EDTAH;
	val.TBuf.buf1=EDTAM;
	val.TBuf.buf0=EDTAL;
    EA = 1;
	return(val);
}

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** ��������: cal_L_mode
** ��������: ʹ��L�߼���
** ��ڲ���: ��
** ���ڲ���: ��
*******************************************************************************************/
// void cal_L_mode(void)
// {
// 	__U32_Def val;

// 	val.val = 0x2042C1;
// 	WriteEMU_REG(EMUCFG1, val);
// }

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** ��������: cal_N_mode
** ��������: ʹ��N�߼���
** ��ڲ���: ��
** ���ڲ���: ��
********************************************************************************************/
void cal_N_mode(void)
{
	__U32_Def val;

	val.val = 0x000AD1;
    //val.val = 0x204AD1;
	WriteEMU_REG(EMUCFG1, val);
}

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** ��������: cal_1P3L_mode
** ��������: ʹ��1P3L����
** ��ڲ���: ��
** ���ڲ���: ��
*******************************************************************************************/
// void cal_1P3L_mode(void)
// {
// 	__U32_Def val;

//     val.val = 0x2082F1;						// 120%Un	1���26�����壬22.5MS,0x0042F1;
// 	WriteEMU_REG(EMUCFG1, val);
// }

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** ��������: Init_EMU
** ��������: ��ʼ��EMU
** ��ڲ���: ��
** ���ڲ���: ��
*******************************************************************************************/
void Init_EMU(void)
{
	__U32_Def val;

	EMUIE = Bin(00000000);                  //�ر�EMU�����жϣ���ֹ�й��������
    EMUIF = 0x00;

    EMUSR = Bin(00000000);					//bit7�رռ���ģ��
											//bit6,bit5=00����ʱ��Դ����ΪPLL/2
  											//bit6,bit5=01����ʱ��Դ����ΪPLL/4
											//bit6,bit5=10����ʱ��Դ����ΪPLL/8
											//bit6,bit5=11����ʱ��Դ����Ϊ32.768HZ

    LDOCON = Bin(11000000);                 //bit7,LDO bandgap��·����
											//bit6,bit5=00,0V,LDO��ѹ������������
											//bit6,bit5=01,2.8V,LDO��ѹ�������2.8V���
											//bit6,bit5=1X,LDO��ѹ�������ΪVOUT���

    val.val = 0x0000E8;
    WriteEMU_REG(EMUCFG3, val);             //��VERF,����EMU�����Ĵ���3
	Delay_ms(1);							//delay 1 ms

    val.val = 0x9FF333;
    WriteEMU_REG(EMUCFG2, val);             //PGA��CHOPʱ��Դѡ��Ϊ32K,�͹�������0x400888


    val.val= 0x0000;
    //WriteEMU_REG(I2GAIN, val);              //���ֵΪ0����ʹ��Ӳ���л�ͨ������ʹ������ȽϷ�ʽ
    WriteEMU_REG(VRMSOS, val);
    WriteEMU_REG(ADCOSI1, val);
    WriteEMU_REG(ADCOSI2, val);

    val.val= 0x0028;						//0.004 P
    WriteEMU_REG(SPTS, val);                //��ʱ��������жϣ�����������ֵ���á�����

    val.val = 4000;
    WriteEMU_REG(SUMSAMPS, val);            //����Ϊ4000���պ�1�뷢��һ�������ۼ��ж�
    val.val = 0x0A00;
    WriteEMU_REG(SAGTHR,val);               //���緧ֵ����Ϊ70V����70V��Ӧ�ĵ�ѹͨ��ADC����ֵ��16λ
    val.val = 0xC8;
    WriteEMU_REG(SAGCNT,val);               //SAG���ʱ�����2���ܲ���ȡ50ms����1LSB=0.25ms
    //¼��У�����
	//val.val=adjust_data.icont;
	val.val=adjust_data.icont;
	WriteEMU_REG(ICONT,val);                //�ֵ�

	// val.val=adjust_data.w1gain;
	// WriteEMU_REG(W1GAIN,val);               //ϸ��

//	val.val=0X0000;//adjust_data.w2gain;
	val.val=adjust_data.w2gain;
	WriteEMU_REG(W2GAIN,val);               //ϸ��

	// val.val=adjust_data.p1cal;
	// WriteEMU_REG(P1CAL,val);                //��λ����
//	val.val=0X0000;//adjust_data.p2cal;
	val.val=adjust_data.p2cal;
	WriteEMU_REG(P2CAL,val);

	// val.val=adjust_data.w1os;
	// WriteEMU_REG(WATT1OS,val);              //����offset����
	val.val=adjust_data.w2os;
	WriteEMU_REG(WATT2OS,val);

	// val.val=adjust_data.irms1os;            //����offset����
	// WriteEMU_REG(IRMS1OS,val);
	val.val=0x0000;//adjust_data.irms2os;
	WriteEMU_REG(IRMS2OS,val);
    // val.val = adjust_data.adcos;
    // WriteEMU_REG(ADCOSU,val);

    //У�����¼�����
	val.val=0x00;
	WriteEMU_REG(PCNT,val);

    val.val= 0x0000C7;
	WriteEMU_REG(EMUCFG0, val);             //��ѹͨ��Ϊ1�����棬����ͨ��1Ϊ16�����棬����ͨ��2Ϊ1�����棬��3·ADC


    val.val = 0x0000E8;
    WriteEMU_REG(EMUCFG3, val);             //��VERF
	Delay_ms(60);							//delay 60 ms


    //val.val= 0x00C5;                      	//00 11 00 0 1 0 1
    val.val= 0x00C6;                        // 00 11 00 0 1 1 0
	WriteEMU_REG(EMUCFG0, val);             //��ѹͨ��Ϊ1�����棬����ͨ��2Ϊ16�������رյ���ͨ��1��ADC
    //ʹ�ܼ���ģ��
    EMUSR = Bin(10000000);                  //�򿪼���ģ�飬����ʱ��Դ����ΪPLL/2

    Delay_ms(250);					        //�򿪼���ģ�鵽ʹ�ܼ���ģ��Ҫ��ʱ2����//��������������1
    Delay_ms(250);
    Delay_ms(250);
    Delay_ms(250);							//delay 1s

    val.val = 0x000AD1;						//0000 0000 0000 1010 1101 0001
	//val.val = 0x204AD1;						//0010 0000   0100 1010   1101 0001
    //PF����ߵ�ƽ��Ч��������ģʽ�رգ���ѹͨ��/����ͨ��1/����ͨ��2��
    //���������90ms��С�ź��������1�������й�/�޹����ܾ���ֵ�ۼ�
    //ʹ���й�/�޹������ۼӣ�ѡ��ͨ��2Ϊ����ͨ��
	WriteEMU_REG(EMUCFG1, val);         	//������90ms���й����ʾ���ֵ�ۼӣ����൥�߱�ѡ��ͨ��2Ϊ����ͨ��


    EMUSR = Bin(10000000);                  //�������״̬��־
	EMUIF = Bin(00000000);        		    //�����ģ��������жϱ�־
	IEN1 |= Bin(00000001);        		    //ʹ�ܼ���ģ���жϡ�
}


////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** ��������:  ReadEMUFromTOEeprom
** ��������:  �ڳ�ʼ��RAM��ʹ��
** �������: ��
** �������: ��
*******************************************************************************************/
void ReadEMUFromTOEeprom(void)
{
    //if(EE_to_RAM(ADJ_ADDR,&adjust_data.gain,sizeof(adjust_data))==0)
    if(VER_RDbytes(ADJ_ADDR, &adjust_data.gain, sizeof(adjust_data))==0)
    {
        //������Ϊ60A�����峣��Ϊ1200����˽������ȵ���Ϊ45ms/90 ����
        adjust_data.gain = 0x43;    //��֪����ʲô��....
        adjust_data.icont = g_ConstPara.Icont;

        adjust_data.vgain = 22.0;
        //adjust_data.nmgain = 0x4000;
        //adjust_data.irms1gain = 0x4000;
        adjust_data.irms2gain = 0x4000;

        //adjust_data.w1gain = 0x00;
        adjust_data.w2gain = 0x00;
        //adjust_data.p1cal = 0x00;
        adjust_data.p2cal = 0x00;
        //adjust_data.w1os = 0x00;
        adjust_data.w2os = 0x00;

        //adjust_data.adcos = 0x00;
        //adjust_data.irms1os = 0x00;
        //adjust_data.irms2os = 0x00;

        //adjust_data.i2gain = 0x00;
        //adjust_data.spts = 0x00;

        //VER_WRbytes(ADJ_ADDR, &adjust_data.gain, sizeof(adjust_data), 1);	  //�Ƿ�������Ҫ����Ĭ��ֵ�����Ƿŵ�У���ｫ����ֵ��ʼ��������У����Уһ����дһ��ֵ��
    }
    //����У������������ͣ�
    if (VER_RDbytes(ADJ_ADDR_POWER2GAIN, &adjust_data_f.power2gain.Buffer[0], 4) == 0)
    {
        adjust_data_f.power2gain.y = 22.0;
    }
    //���´�����ʲô����?
//��������׼��0.7340��1.4800
    /* if ((adjust_data.vgain < D_vgain_Min) || (adjust_data.vgain > D_vgain_Max))
    {
        adjust_data.vgain = D_vgain;		//0x4000;
    }

    if ((adjust_data.nmgain < D_nmgain_Min) || (adjust_data.nmgain > D_nmgain_Max))
    {
        adjust_data.nmgain = D_nmgain;		//0x4000;
    }

    if ((adjust_data.irms1gain < D_irms1gain_Min) || (adjust_data.irms1gain > D_irms1gain_Max))
    {
        adjust_data.irms1gain = D_irms1gain;		//0x4000;
    }

    if ((adjust_data.irms2gain < D_irms2gain_Min) || (adjust_data.irms2gain > D_irms2gain_Max))
    {
        adjust_data.irms2gain = D_irms2gain;		//0x4000;
    }
 */

//����У����ΧΪ��׼��0.8125����׼��1.1800
   /*  if ((adjust_data.w1gain > D_w1gain_Min) && (adjust_data.w1gain < D_w1gain_Max))
    {
        adjust_data.w1gain = D_w1gain;
    }

    if ((adjust_data.p1cal > D_p1cal_Min) && (adjust_data.p1cal < D_p1cal_Max))
    {
        adjust_data.p1cal = D_p1cal;
    }

    if ((adjust_data.w1os > D_w1os_Min) && (adjust_data.w1os < D_w1os_Max))
    {
        adjust_data.w1os = D_w1os;
    }
 */
/*
    if ((adjust_data.w2gain > D_w2gain_Min) && (adjust_data.w2gain < D_w2gain_Max))
    {
        adjust_data.w2gain = D_w2gain;
    }

    if ((adjust_data.p2cal > D_p2cal_Min) && (adjust_data.p2cal < D_p2cal_Max))
    {
        adjust_data.p2cal = D_p2cal;
    }

    if ((adjust_data.w2os > D_w2os_Min) && (adjust_data.w2os < D_w2os_Max))
    {
        adjust_data.w2os = D_w2os;
    }
*/
}

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** ��������: EMU_Check
** ��������:    ���EMU�Ĵ�����У������Ƿ���EEPROM�������ȣ���RTCСʱ�߳��г���
** ��ڲ���: ��
** ���ڲ���: ��
*******************************************************************************************/
void EMU_Check(void)
{
	__U32_Def xdata val;

	if(VER_RDbytes(ADJ_ADDR, &adjust_data.gain, sizeof(adjust_data)) != 0)
	{
		val = ReadEMU_REG(ICONT);
		if(val.val != adjust_data.icont)
		{
			val.val = adjust_data.icont;
			WriteEMU_REG(ICONT,val);                 //���峣�������Ӧֵ
		}

//        val = ReadEMU_REG(ADCOSU);
//		if(val.val != adjust_data.adcos)
//		{
//			val.val = adjust_data.adcos;
//			WriteEMU_REG(ADCOSU,val);                //ADCƫ��
//		}
		val = ReadEMU_REG(W2GAIN);
		if(val.val != adjust_data.w2gain)
		{
			val.val = adjust_data.w2gain;
			WriteEMU_REG(W2GAIN,val);                  //ͨ��2�Ĺ�������
		}

		val = ReadEMU_REG(P2CAL);
		if(val.val != adjust_data.p2cal)
		{
			val.val = adjust_data.p2cal;
			WriteEMU_REG(P2CAL,val);                  //ͨ��2����λ����
		}

		val = ReadEMU_REG(WATT2OS);
		if(val.val != adjust_data.w2os)
		{
			val.val = adjust_data.w2os;
			WriteEMU_REG(WATT2OS,val);                 //ͨ��2��5%
		}

//		val = ReadEMU_REG(IRMS2OS);
//		if(val.val != adjust_data.irms2os)
//		{
//			val.val = adjust_data.irms2os;
//			WriteEMU_REG(IRMS2OS,val);                  //ͨ��2�ĵ���OFFSET
//		}
	}

//	//cal_N_mode();
}

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** ��������: EMUTampProc
** ��������:
** ��ڲ���: ��
** ���ڲ���: ��
*******************************************************************************************/
void EMUTampProc(void)
{
	__U32_Def val;
    unsigned long xdata lngTmpVA = 0;
    unsigned long temp;
	uint32 tmpval;
	uint32 chn=0;
    unsigned char data Fptr;
    unsigned char ReverseI = 0x00;

	if (gbSecFlag)					     //1���һ�β���
	{
		gbSecFlag=0;

        //��ȡ��ǰ��ѹ
        val = ReadEMU_REG(VRMS);
        lngTmpVA = (val.val / adjust_data.vgain) * 10;
        //lngTmpVA = val.val * 0.01917;
        //lngTmpVA = val.val * (float)((1.4*1200.0/8388608.0)*100.0);   //��ѹ������1200k����1k����PGA�Ŵ���Ϊ1
        //lngTmpVA = val.val * V_Coefficient;								//(float)0.0426;

        //lngTmpVA = lngTmpVA * adjust_data.vgain;
        //lngTmpVA = lngTmpVA >> 14;

	    temp = 700; 		  //�����ǰ��ѹ���ϴε�ѹ��ֵ����70V,������˲���������


        if(((lngTmpVA >= g_FilterV.LastVal) && ((lngTmpVA - g_FilterV.LastVal) > temp))||((lngTmpVA < g_FilterV.LastVal) && ((g_FilterV.LastVal - lngTmpVA) > temp)))
        {
            for(Fptr = 0; Fptr < 4; Fptr++)
            {
                g_FilterV.Buf[Fptr] = lngTmpVA;
            }
            g_FilterV.FPtr = 0;
        }
        else
        {
            g_FilterV.FPtr = g_FilterV.FPtr % 4;
            g_FilterV.Buf[g_FilterV.FPtr] = lngTmpVA;
            g_FilterV.FPtr++;
        }
        g_FilterV.LastVal = lngTmpVA;

        g_FilterV.Buf[4] = 0;
        for(Fptr = 0; Fptr < 4; Fptr++)
        {
            g_FilterV.Buf[4] += g_FilterV.Buf[Fptr];
        }

        g_FilterV.Buf[4] = g_FilterV.Buf[4] >> 2;
        lngTmpVA = g_FilterV.Buf[4];

		//lngTmpVA = lngTmpVA *0.1;
		Long_BCD3(&g_InsBCD.Voltage[0], lngTmpVA);       //�Ŵ�10���ĵ�ѹֵ����λΪV

//ͨ��2�ĵ�������
		val = ReadEMU_REG(I2RMS);
		tmpval = val.val;
        //if (tmpval >= 0x800000)
		if (tmpval >= 0x680000)   //0X680000Ϊ����������Ϊ��
		{
			tmpval = 0;
		}
        //tmpval = tmpval * (float)((1.4/0.00018/16.0/8388608.0)*1000.0);
        //tmpval = tmpval * I2_Coefficient;				//(float)0.058;
        curn_temp = ((float)tmpval / adjust_data.irms2gain);
        tmpval = curn_temp * 1000;
		curn = tmpval;//>> 14;

		tmpval = curn;
        if(tmpval < C_IStartForFilter)
        {
            tmpval = 0;
        }
		//tmpval = tmpval*10;
        Long_BCD3(&g_InsBCD.Current[0], tmpval);      //�Ŵ�1000���ĵ���ֵ����λΪA
//ͨ��2�й��ʴ���
        if(curn >= C_IStartForFilter)
        {
            val = ReadEMU_REG(APWRA2);
            tmpval = val.val;
    		if ((tmpval & 0x800000) == 0x800000)     //B·��������
    		{
                if (curn >= DefaultIStart*g_EventDly.RevT)
                {
                    ReverseI = 0xAA;
                }
                tmpval = ~tmpval + 1;//ȡ����ֵ
								tmpval &= 0x00ffffff;
    		}
			else
			{
                if (curn >= DefaultIStart*g_EventDly.RevT)
                {
                    ReverseI = 0x00;
                }
			}
            //powern = tmpval * (float)((3600.0*8000.0/g_ConstPara.Icont/g_ConstPara.Constant/8388608.0)*1000.0*16384.0);
            //powern = tmpval * (float)(P1_Coefficient/g_ConstPara.Icont/g_ConstPara.Constant);  //56250000.0
			//powern = tmpval * 0.0629;
             powern = (tmpval / adjust_data_f.power2gain.y) * 100;
            powern1 = tmpval * (float)(P1_Coefficient / g_ConstPara.Icont / g_ConstPara.Constant);

        }
        else
        {
            powern = 0;
        }
        // tmpval = powern;
		// tmpval = tmpval*100;
        // powern = tmpval;
        //Long_BCD3(&g_InsBCD.Power[0], tmpval);       //�Ŵ�100���й���ֵ����λΪW���������߱�ȡ������·�Ĺ���
        Long_BCD4(&g_InsBCD.Power[0], powern);
        Long_BCD4(&g_InsBCD.Power1[0], powern1);

//ͨ��2�޹��ʴ���
        if(curn >= C_IStartForFilter)
        {
            val = ReadEMU_REG(RPWRA2);
            tmpval = val.val;
    		if ((tmpval & 0x800000) == 0x800000)     //B·��������
    		{
                if (curn >= DefaultIStart*g_EventDly.RevT)
                {
//                    ReverseI = 0xAA;
                }
                tmpval = ~tmpval + 1;
								tmpval &= 0x00ffffff;
    		}
            //powern = tmpval * (float)((3600.0*8000.0/g_ConstPara.Icont/g_ConstPara.Constant/8388608.0)*1000.0*16384.0);
            //rpowern = tmpval * (float)(QP1_Coefficient/g_ConstPara.Icont/g_ConstPara.Constant);         	//56250000.0
			//rpowern = tmpval * 0.0629;


            //rpowern = (tmpval / adjust_data_f.power2gain.y) * 100; //���ʵ���޹����ʣ����ڼ������ڹ��ʺ͹�������
            rpowern = tmpval * (float)(QP1_Coefficient / g_ConstPara.Icont / g_ConstPara.Constant);
        }
        else
        {
            rpowern = 0;
        }
        Long_BCD4(&g_InsBCD.RPower[0], rpowern);       //�Ŵ�100���Ĺ���ֵ����λΪW���������߱�ȡ������·�Ĺ���

//Ƶ�ʴ���
        val = ReadEMU_REG(FREQ);    //��FREQ�Ĵ�����ֵ
		tmpval = val.val;
        freq = tmpval; //����Ƶ�ʣ��Ŵ�100��
        Word_BCD2(&g_InsBCD.Frequence[0], freq);       // ת��BCD��ʽ���Ŵ�100����Ƶ�ʣ���λΪHz

//���ڹ��ʼ���    rpowern �޹����ʣ� powern �й�����
        apowern = sqrt(pow(powern,2)+pow(rpowern,2));
        tmpval = apowern;
        Long_BCD4(&g_InsBCD.APower[0], tmpval);         //�Ŵ�100�������ڹ��� ��λVA

//������������
        factorp = ((float)powern)/((float)apowern);
		//factorp = ((float)1)/((float)2);
        tmpval = factorp*1000;
        Word_BCD2(&g_InsBCD.PowerFactor[0], tmpval);    //�Ŵ�1000���Ĺ�������
	}

    //test
// g_InsBCD.Voltage[0] = 0x00;//��ѹ
// g_InsBCD.Voltage[1] = 0x11;
// g_InsBCD.Voltage[2] = 0x22;
// g_InsBCD.Voltage[3] = 0x33;
// g_InsBCD.Current[0] = 0x26;//����
// g_InsBCD.Current[1] = 0x26;
// g_InsBCD.Current[2] = 0x26;
// g_InsBCD.Current[3] = 0x26;
// g_InsBCD.Power[0] = 0x27;//����
// g_InsBCD.Power[1] = 0x27;
// g_InsBCD.Power[2] = 0x27;
// g_InsBCD.Power[3] = 0x27;
// g_InsBCD.Frequence[0] = 0x99;
// g_InsBCD.Frequence[1] = 0x49;
// g_InsBCD.Frequence[2] = 0x00;
// g_InsBCD.Frequence[3] = 0x00;
// g_InsBCD.RPower[0] = 0x28;//�޹�
// g_InsBCD.RPower[1] = 0x28;
// g_InsBCD.RPower[2] = 0x28;
// g_InsBCD.RPower[3] = 0x28;
// g_InsBCD.APower[0] = 0x29;//����
// g_InsBCD.APower[1] = 0x29;
// g_InsBCD.APower[2] = 0x29;
// g_InsBCD.APower[3] = 0x29;
// g_InsBCD.PowerFactor[0] = 0x30;//��������
// g_InsBCD.PowerFactor[1] = 0x30;
// g_InsBCD.PowerFactor[2] = 0x30;
// g_InsBCD.PowerFactor[3] = 0x30;
}