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
** 函数名称: ClrEMU_REG
** 函数描述: 对EMU中间接寄存器,复位EMU寄存器为0
** 输入参数: sdar:要写入的寄存器的地址
             *ptr:写入数据的首地址
** 输出参数: 无
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
** 函数名称: WriteEMU_REG
** 函数描述: 对EMU中间接寄存器写操作
** 输入参数: sdar:要写入的寄存器的地址
             *ptr:写入数据的首地址
** 输出参数: 无
*******************************************************************************************/
void WriteEMU_REG(uint8 sadr,__U32_Def val)			 //20120315
{
	EA = 0;
    EADR=0x00;
	_nop_();	            //一个NOP
	_nop_();	            //一个NOP
	_nop_();	            //一个NOP
	_nop_();	            //一个NOP
	_nop_();	            //一个NOP
	_nop_();	            //一个NOP
	EDTAH=val.TBuf.buf2;
	EDTAM=val.TBuf.buf1;
	EDTAL=val.TBuf.buf0;
	_nop_();	            //一个NOP
	_nop_();	            //一个NOP
	_nop_();	            //一个NOP
	_nop_();	            //一个NOP
	_nop_();	            //一个NOP
	_nop_();	            //一个NOP
	EADR=sadr|0x80;
    _nop_();	            //一个NOP
	_nop_();	            //一个NOP
	_nop_();	            //一个NOP
	_nop_();	            //一个NOP
	_nop_();	            //一个NOP
	_nop_();	            //一个NOP
    EA = 1;
}

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: ReadEMU_REG
** 函数描述: 对EMU中间接寄存器读操作
** 输入参数: sdar:要读入的寄存器的地址
			    num :读入数组的字节数
				 datatype:数据类型，0：无符号型;1:有符合型
** 输出参数: rdata:返回寄存器的值
*******************************************************************************************/
__U32_Def ReadEMU_REG(uint8 sadr)
{
	__U32_Def val;

    EA = 0;
	val.val=0x00;

	EADR=sadr&0x7F;
	_nop_();	            //一个NOP
	_nop_();	            //一个NOP
	_nop_();	            //一个NOP
	_nop_();	            //一个NOP
	_nop_();	            //一个NOP
	_nop_();	            //一个NOP
	_nop_();	            //一个NOP
	_nop_();	            //一个NOP
	_nop_();	            //一个NOP
	_nop_();	            //一个NOP
	val.TBuf.buf2=EDTAH;
	val.TBuf.buf1=EDTAM;
	val.TBuf.buf0=EDTAL;
    EA = 1;
	return(val);
}

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: cal_L_mode
** 功能描述: 使用L线计量
** 入口参数: 无
** 出口参数: 无
*******************************************************************************************/
// void cal_L_mode(void)
// {
// 	__U32_Def val;

// 	val.val = 0x2042C1;
// 	WriteEMU_REG(EMUCFG1, val);
// }

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: cal_N_mode
** 功能描述: 使用N线计量
** 入口参数: 无
** 出口参数: 无
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
** 函数名称: cal_1P3L_mode
** 功能描述: 使用1P3L计量
** 入口参数: 无
** 出口参数: 无
*******************************************************************************************/
// void cal_1P3L_mode(void)
// {
// 	__U32_Def val;

//     val.val = 0x2082F1;						// 120%Un	1秒出26个脉冲，22.5MS,0x0042F1;
// 	WriteEMU_REG(EMUCFG1, val);
// }

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: Init_EMU
** 功能描述: 初始化EMU
** 入口参数: 无
** 出口参数: 无
*******************************************************************************************/
void Init_EMU(void)
{
	__U32_Def val;

	EMUIE = Bin(00000000);                  //关闭EMU所有中断，防止有功脉冲输出
    EMUIF = 0x00;

    EMUSR = Bin(00000000);					//bit7关闭计量模块
											//bit6,bit5=00计量时钟源设置为PLL/2
  											//bit6,bit5=01计量时钟源设置为PLL/4
											//bit6,bit5=10计量时钟源设置为PLL/8
											//bit6,bit5=11计量时钟源设置为32.768HZ

    LDOCON = Bin(11000000);                 //bit7,LDO bandgap电路开关
											//bit6,bit5=00,0V,LDO电压输出设置无输出
											//bit6,bit5=01,2.8V,LDO电压输出设置2.8V输出
											//bit6,bit5=1X,LDO电压输出设置为VOUT输出

    val.val = 0x0000E8;
    WriteEMU_REG(EMUCFG3, val);             //打开VERF,配置EMU计量寄存器3
	Delay_ms(1);							//delay 1 ms

    val.val = 0x9FF333;
    WriteEMU_REG(EMUCFG2, val);             //PGA的CHOP时钟源选择为32K,低功耗设置0x400888


    val.val= 0x0000;
    //WriteEMU_REG(I2GAIN, val);              //这个值为0，不使用硬件切换通道，而使用软件比较方式
    WriteEMU_REG(VRMSOS, val);
    WriteEMU_REG(ADCOSI1, val);
    WriteEMU_REG(ADCOSI2, val);

    val.val= 0x0028;						//0.004 P
    WriteEMU_REG(SPTS, val);                //用时间变量做判断，功率启动阀值设置。待定

    val.val = 4000;
    WriteEMU_REG(SUMSAMPS, val);            //设置为4000，刚好1秒发生一次周期累加中断
    val.val = 0x0A00;
    WriteEMU_REG(SAGTHR,val);               //掉电阀值设置为70V，即70V对应的电压通道ADC采样值高16位
    val.val = 0xC8;
    WriteEMU_REG(SAGCNT,val);               //SAG检测时间大于2个周波（取50ms），1LSB=0.25ms
    //录入校表参数
	//val.val=adjust_data.icont;
	val.val=adjust_data.icont;
	WriteEMU_REG(ICONT,val);                //粗调

	// val.val=adjust_data.w1gain;
	// WriteEMU_REG(W1GAIN,val);               //细调

//	val.val=0X0000;//adjust_data.w2gain;
	val.val=adjust_data.w2gain;
	WriteEMU_REG(W2GAIN,val);               //细调

	// val.val=adjust_data.p1cal;
	// WriteEMU_REG(P1CAL,val);                //相位补偿
//	val.val=0X0000;//adjust_data.p2cal;
	val.val=adjust_data.p2cal;
	WriteEMU_REG(P2CAL,val);

	// val.val=adjust_data.w1os;
	// WriteEMU_REG(WATT1OS,val);              //功率offset修正
	val.val=adjust_data.w2os;
	WriteEMU_REG(WATT2OS,val);

	// val.val=adjust_data.irms1os;            //电流offset修正
	// WriteEMU_REG(IRMS1OS,val);
	val.val=0x0000;//adjust_data.irms2os;
	WriteEMU_REG(IRMS2OS,val);
    // val.val = adjust_data.adcos;
    // WriteEMU_REG(ADCOSU,val);

    //校表参数录入完成
	val.val=0x00;
	WriteEMU_REG(PCNT,val);

    val.val= 0x0000C7;
	WriteEMU_REG(EMUCFG0, val);             //电压通道为1倍增益，电流通道1为16倍增益，电流通道2为1倍增益，打开3路ADC


    val.val = 0x0000E8;
    WriteEMU_REG(EMUCFG3, val);             //打开VERF
	Delay_ms(60);							//delay 60 ms


    //val.val= 0x00C5;                      	//00 11 00 0 1 0 1
    val.val= 0x00C6;                        // 00 11 00 0 1 1 0
	WriteEMU_REG(EMUCFG0, val);             //电压通道为1倍增益，电流通道2为16倍增，关闭电流通道1的ADC
    //使能计量模块
    EMUSR = Bin(10000000);                  //打开计量模块，计量时钟源设置为PLL/2

    Delay_ms(250);					        //打开计量模块到使能计量模块要延时2秒钟//防保护程序，特性1
    Delay_ms(250);
    Delay_ms(250);
    Delay_ms(250);							//delay 1s

    val.val = 0x000AD1;						//0000 0000 0000 1010 1101 0001
	//val.val = 0x204AD1;						//0010 0000   0100 1010   1101 0001
    //PF输出高电平有效，掉零线模式关闭，电压通道/电流通道1/电流通道2打开
    //输出脉冲宽度90ms，小信号脉冲加速1倍数，有功/无功电能绝对值累加
    //使能有功/无功电能累加，选择通道2为计量通道
	WriteEMU_REG(EMUCFG1, val);         	//脉冲宽度90ms，有功功率绝对值累加，单相单线表选择通道2为计量通道


    EMUSR = Bin(10000000);                  //清除计量状态标志
	EMUIF = Bin(00000000);        		    //清计量模块的所有中断标志
	IEN1 |= Bin(00000001);        		    //使能计量模块中断。
}


////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称:  ReadEMUFromTOEeprom
** 函数描述:  在初始化RAM中使用
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
void ReadEMUFromTOEeprom(void)
{
    //if(EE_to_RAM(ADJ_ADDR,&adjust_data.gain,sizeof(adjust_data))==0)
    if(VER_RDbytes(ADJ_ADDR, &adjust_data.gain, sizeof(adjust_data))==0)
    {
        //最大电流为60A，脉冲常数为1200，因此将脉冲宽度调整为45ms/90 待定
        adjust_data.gain = 0x43;    //不知道有什么用....
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

        //VER_WRbytes(ADJ_ADDR, &adjust_data.gain, sizeof(adjust_data), 1);	  //是否在这里要保存默认值，还是放到校表里将所有值初始化，现在校表是校一个就写一个值。
    }
    //加载校表参数（浮点型）
    if (VER_RDbytes(ADJ_ADDR_POWER2GAIN, &adjust_data_f.power2gain.Buffer[0], 4) == 0)
    {
        adjust_data_f.power2gain.y = 22.0;
    }
    //以下代码有什么作用?
//调整到基准的0.7340到1.4800
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

//功率校正范围为基准的0.8125到基准的1.1800
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
** 函数名称: EMU_Check
** 功能描述:    检查EMU寄存器中校表参数是否与EEPROM储存的相等，在RTC小时线程中出现
** 入口参数: 无
** 出口参数: 无
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
			WriteEMU_REG(ICONT,val);                 //脉冲常数计算对应值
		}

//        val = ReadEMU_REG(ADCOSU);
//		if(val.val != adjust_data.adcos)
//		{
//			val.val = adjust_data.adcos;
//			WriteEMU_REG(ADCOSU,val);                //ADC偏置
//		}
		val = ReadEMU_REG(W2GAIN);
		if(val.val != adjust_data.w2gain)
		{
			val.val = adjust_data.w2gain;
			WriteEMU_REG(W2GAIN,val);                  //通道2的功率修正
		}

		val = ReadEMU_REG(P2CAL);
		if(val.val != adjust_data.p2cal)
		{
			val.val = adjust_data.p2cal;
			WriteEMU_REG(P2CAL,val);                  //通道2的相位修正
		}

		val = ReadEMU_REG(WATT2OS);
		if(val.val != adjust_data.w2os)
		{
			val.val = adjust_data.w2os;
			WriteEMU_REG(WATT2OS,val);                 //通道2的5%
		}

//		val = ReadEMU_REG(IRMS2OS);
//		if(val.val != adjust_data.irms2os)
//		{
//			val.val = adjust_data.irms2os;
//			WriteEMU_REG(IRMS2OS,val);                  //通道2的电流OFFSET
//		}
	}

//	//cal_N_mode();
}

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: EMUTampProc
** 功能描述:
** 入口参数: 无
** 出口参数: 无
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

	if (gbSecFlag)					     //1秒读一次参数
	{
		gbSecFlag=0;

        //读取当前电压
        val = ReadEMU_REG(VRMS);
        lngTmpVA = (val.val / adjust_data.vgain) * 10;
        //lngTmpVA = val.val * 0.01917;
        //lngTmpVA = val.val * (float)((1.4*1200.0/8388608.0)*100.0);   //分压网络是1200kΩ和1kΩ，PGA放大倍数为1
        //lngTmpVA = val.val * V_Coefficient;								//(float)0.0426;

        //lngTmpVA = lngTmpVA * adjust_data.vgain;
        //lngTmpVA = lngTmpVA >> 14;

	    temp = 700; 		  //如果当前电压与上次电压差值大于70V,则更新滤波器缓冲区


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
		Long_BCD3(&g_InsBCD.Voltage[0], lngTmpVA);       //放大10倍的电压值，单位为V

//通道2的电流处理
		val = ReadEMU_REG(I2RMS);
		tmpval = val.val;
        //if (tmpval >= 0x800000)
		if (tmpval >= 0x680000)   //0X680000为负数，处理为零
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
        Long_BCD3(&g_InsBCD.Current[0], tmpval);      //放大1000倍的电流值，单位为A
//通道2有功率处理
        if(curn >= C_IStartForFilter)
        {
            val = ReadEMU_REG(APWRA2);
            tmpval = val.val;
    		if ((tmpval & 0x800000) == 0x800000)     //B路电流反向
    		{
                if (curn >= DefaultIStart*g_EventDly.RevT)
                {
                    ReverseI = 0xAA;
                }
                tmpval = ~tmpval + 1;//取绝对值
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
        //Long_BCD3(&g_InsBCD.Power[0], tmpval);       //放大100倍有功率值，单位为W，单相两线表，取计量回路的功率
        Long_BCD4(&g_InsBCD.Power[0], powern);
        Long_BCD4(&g_InsBCD.Power1[0], powern1);

//通道2无功率处理
        if(curn >= C_IStartForFilter)
        {
            val = ReadEMU_REG(RPWRA2);
            tmpval = val.val;
    		if ((tmpval & 0x800000) == 0x800000)     //B路电流反向
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


            //rpowern = (tmpval / adjust_data_f.power2gain.y) * 100; //获得实际无功功率，用于计算视在功率和功率因数
            rpowern = tmpval * (float)(QP1_Coefficient / g_ConstPara.Icont / g_ConstPara.Constant);
        }
        else
        {
            rpowern = 0;
        }
        Long_BCD4(&g_InsBCD.RPower[0], rpowern);       //放大100倍的功率值，单位为W，单相两线表，取计量回路的功率

//频率处理
        val = ReadEMU_REG(FREQ);    //读FREQ寄存器的值
		tmpval = val.val;
        freq = tmpval; //计算频率，放大100倍
        Word_BCD2(&g_InsBCD.Frequence[0], freq);       // 转成BCD格式，放大100倍的频率，单位为Hz

//视在功率计算    rpowern 无功功率， powern 有功功率
        apowern = sqrt(pow(powern,2)+pow(rpowern,2));
        tmpval = apowern;
        Long_BCD4(&g_InsBCD.APower[0], tmpval);         //放大100倍的视在功率 单位VA

//功率因数计算
        factorp = ((float)powern)/((float)apowern);
		//factorp = ((float)1)/((float)2);
        tmpval = factorp*1000;
        Word_BCD2(&g_InsBCD.PowerFactor[0], tmpval);    //放大1000倍的功率因数
	}

    //test
// g_InsBCD.Voltage[0] = 0x00;//电压
// g_InsBCD.Voltage[1] = 0x11;
// g_InsBCD.Voltage[2] = 0x22;
// g_InsBCD.Voltage[3] = 0x33;
// g_InsBCD.Current[0] = 0x26;//电流
// g_InsBCD.Current[1] = 0x26;
// g_InsBCD.Current[2] = 0x26;
// g_InsBCD.Current[3] = 0x26;
// g_InsBCD.Power[0] = 0x27;//功率
// g_InsBCD.Power[1] = 0x27;
// g_InsBCD.Power[2] = 0x27;
// g_InsBCD.Power[3] = 0x27;
// g_InsBCD.Frequence[0] = 0x99;
// g_InsBCD.Frequence[1] = 0x49;
// g_InsBCD.Frequence[2] = 0x00;
// g_InsBCD.Frequence[3] = 0x00;
// g_InsBCD.RPower[0] = 0x28;//无功
// g_InsBCD.RPower[1] = 0x28;
// g_InsBCD.RPower[2] = 0x28;
// g_InsBCD.RPower[3] = 0x28;
// g_InsBCD.APower[0] = 0x29;//视在
// g_InsBCD.APower[1] = 0x29;
// g_InsBCD.APower[2] = 0x29;
// g_InsBCD.APower[3] = 0x29;
// g_InsBCD.PowerFactor[0] = 0x30;//功率因数
// g_InsBCD.PowerFactor[1] = 0x30;
// g_InsBCD.PowerFactor[2] = 0x30;
// g_InsBCD.PowerFactor[3] = 0x30;
}