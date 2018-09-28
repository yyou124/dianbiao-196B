/*==============================================================================
                Copyright(C) 1997-2019.  Sinowealth Tech. Co., Ltd.
--------------------------------------------------------------------------------
Project Name  : ADJUSTMETERPROC.c
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
#ifndef  ADJUSTMETERPROC_C
	#define  ADJUSTMETERPROC_C
#endif
//-------------------------------------------------------------------------------

//---------------


#include "config.h"
#include "Headconfig.h"




////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: AdjustMeterInit
** 功能描述: 通信校表初始化处理程序
** 入口参数: 无
** 出口参数: 无
//校表初始化
//10FF
*******************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////
void AdjustMeterInit(void)
{
    __U32_Def val;
		//float adjust_temp;
		unsigned long temp;
	  val.val = 0;
    //Factory Mode && Calibration Flag && 功率稳定
    if((0xF001 == gbFgKeyProg) && (g_Cal.Flag  == D_Initing) && (1 == AverStablePowerJudge()))
    {
        switch(g_Cal.Step)
        {
            case S_DataInit:		                 //Step 1
            {
                VER_RDbytes(ADJ_ADDR,&adjust_data.gain,sizeof(adjust_data));          //Read Calibration Data

                //!For CH1 电流通道1未使用
                // ClrEMU_REG(W1GAIN);
                // ClrEMU_REG(P1CAL);
                // ClrEMU_REG(WATT1OS);

                // adjust_data.w1gain = 0;
                // adjust_data.p1cal = 0;
                // adjust_data.w1os = 0;

                //!For CH2
                ClrEMU_REG(W2GAIN);
                ClrEMU_REG(P2CAL);
                ClrEMU_REG(WATT2OS);

                adjust_data.w2gain = 0;
                adjust_data.p2cal = 0;
                adjust_data.w2os = 0;

                //!For COMMON
                ClrEMU_REG(I2GAIN);


                //adjust_data.i2gain = 0;
				//将ICONT写入寄存器
                adjust_data.icont = g_ConstPara.Icont;
                val.val = adjust_data.icont;
                WriteEMU_REG(ICONT,val);

				adjust_data.vgain = 0x0000;
				adjust_data.irms2gain = 0x0000;
				// adjust_data.power2gain = 0x0000;
                adjust_data_f.power2gain.y = 0.0; //换成浮点数
                //将用电量清零
                MemInitSet(&kwh_value.integer[0], 0x00, 5);
                VER_WRbytes(EE_KWH0,&kwh_value.integer[0],5,1);
				//带校验向EEPROM写入校表
				VER_WRbytes_limit(ADJ_ADDR,&adjust_data.gain,sizeof(adjust_data), 1);
                VER_WRbytes(ADJ_ADDR_POWER2GAIN, &adjust_data_f.power2gain.Buffer[0], 4, 1);
                MemInitSet(&g_CalTmp.Buffer[0], 0x00, sizeof(g_CalTmp));
                g_Cal.Step++;
                break;
            }

            case S_ADCShort:                 //Step 2
            {
                //Init Offset Register
                ClrEMU_REG(IRMS1OS);
                ClrEMU_REG(IRMS2OS);
                ClrEMU_REG(ADCOSU);
				//ADC nomal
               val.val= 0x00C6;
            	WriteEMU_REG(EMUCFG0, val);             //ADC输入不短接
                g_Cal.Step++;
                break;
            }
			case S_Wait:				                //Step 3
            {
                //Caculate Register Init
                //g_CalTmp.Offset.I1Buf = 0;
                g_CalTmp.Offset.I2Buf = 0;
                g_CalTmp.Offset.VBuf = 0;
				g_CalTmp.Offset.P2Buf = 0;
				g_Cal.Step++;
				break;
            }
			//读取4次电压，电流值
            case S_GetOfst1:
            case S_GetOfst2:
            case S_GetOfst3:
            case S_GetOfst4:
            {
                //Step 4
				//电流通道1未使用
                //Read I1
                // val = ReadEMU_REG(I1RMS);
                // if (val.val >= 0x800000)
                // {
                //     val.val = 0x00;
                // }
                // g_CalTmp.Offset.I1Buf += val.val;

                //Read I2
                val = ReadEMU_REG(I2RMS);
                if (val.val >= 0x800000)
                {
                    val.val = 0x00;
                }
                g_CalTmp.Offset.I2Buf += val.val;

                //Read V
                val = ReadEMU_REG(VRMS);

//                if (val.val>=0x800000)         //ADC输出值是有可能为负的，不需要处理
//                {
//                    val.val = 0x00;
//                }
                g_CalTmp.Offset.VBuf += val.val;

				//Read Power
				val = ReadEMU_REG(APWRA2);
				temp = val.val;
				if((temp & 0x800000)== 0x800000)	//电流反向
				{
					temp = ~temp + 1;
					temp &= 0x00ffffff;
				}
				//Add
				g_CalTmp.Offset.P2Buf += temp;

                g_Cal.Step++;
                break;

            }

            case S_Calculate:
            {
                //Step 5
                //Average Offset Result
				g_CalTmp.Offset.I1Buf = 0;
                //g_CalTmp.Offset.I1Buf = g_CalTmp.Offset.I1Buf >> 2;		  // 取四次平均值
                g_CalTmp.Offset.I2Buf = g_CalTmp.Offset.I2Buf >> 2;
                g_CalTmp.Offset.VBuf = g_CalTmp.Offset.VBuf >> 2;
				g_CalTmp.Offset.P2Buf = g_CalTmp.Offset.P2Buf >>2;

                //Check Offset Result
                if(g_CalTmp.Offset.I1Buf >= 0x800000)
				{
                    g_CalTmp.Offset.I1Buf = 0;
				}
                if(g_CalTmp.Offset.I2Buf >= 0x800000)
				{
                    g_CalTmp.Offset.I2Buf = 0;
				}


                //Calculate the calibration parameter
				//求电压换算系数
				//adjust_temp = (float)g_CalTmp.Offset.VBuf;
				//adjust_temp = adjust_temp/220;
                adjust_data.vgain = g_CalTmp.Offset.VBuf / 220;
                //求电流换算系数
				//adjust_temp = (float)g_CalTmp.Offset.I2Buf/5;
                adjust_data.irms2gain = g_CalTmp.Offset.I2Buf / 5; 
				//求功率换算系数
				adjust_data_f.power2gain.y = g_CalTmp.Offset.P2Buf / 1100.0;
                //Store the parameter into adjust_data struct
                /* adjust_data.irms1os = g_CalTmp.Offset.I1Buf;
                val.val = adjust_data.irms1os;
                WriteEMU_REG(IRMS1OS,val);

                adjust_data.irms2os = g_CalTmp.Offset.I2Buf;
                val.val = adjust_data.irms2os;
                WriteEMU_REG(IRMS2OS,val);

                adjust_data.adcos = (unsigned int)g_CalTmp.Offset.VBuf;
                val.val = adjust_data.adcos;
                WriteEMU_REG(ADCOSU,val); */

                //Enable ADC input
                val.val= 0x00C6;
            	WriteEMU_REG(EMUCFG0, val);             //ADC输入不短接

                //Write all the calibration parameter into EEPROM
                VER_WRbytes_limit(ADJ_ADDR,&adjust_data.gain,sizeof(adjust_data),1);
                VER_WRbytes(ADJ_ADDR_POWER2GAIN, &adjust_data_f.power2gain.Buffer[0], 4, 1);//浮点数写入
                //g_Cal.Key = 0x00;
                //g_Cal.Step = 0x00;

				gbAverTimes = 0;
				gbAdjShowOk2S = 0;
				gbAdjStepFlag = 0;
				gbAdjustShowID = 0;

			    g_Cal.Flag = D_InitAdjust;
                break;
            }
            default:
			{
				g_Cal.Step++;
				break;
			}
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: AdjustMeterProc
** 功能描述: 通信校表处理程序
** 入口参数: 无
** 出口参数: 无
*******************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////
// unsigned char AdjustMeterProc(unsigned int ID,unsigned char *Src)
// {
//     unsigned char rlt = 0;
// 	unsigned int tmp = 0;
// 	unsigned long ultmp = 0;
//     __U32_Def val;

// 	rlt = 0;
//     val.val = 0;
// 	if (0xF001 != gbFgKeyProg)
// 	{
// 		rlt = 0xff;
// 		return rlt;
// 	}

//     switch(ID)
//     {
//         case Adj1_VRMS_GAIN://0x10C0
// 		    //VRMS GAIN
//             VER_RDbytes(ADJ_ADDR,&adjust_data.gain,sizeof(adjust_data));
// 			tmp = ((*Src) << 8) + *(Src+1);
// 			if ((tmp < D_vgain_Min) || (tmp > D_vgain_Max))
// 			{
// 				tmp = D_vgain;
// 				rlt = 0xff;
// 			}
//             adjust_data.vgain = tmp;
//             VER_WRbytes(ADJ_ADDR,&adjust_data.gain,sizeof(adjust_data),1);
//         break;


//         case Adj2_I1RMS_GAIN://0x10C1
//             //I1RMS GAIN
// 			VER_RDbytes(ADJ_ADDR,&adjust_data.gain,sizeof(adjust_data));
// 			tmp = ((*Src) << 8) + *(Src+1);
// 			if ((tmp < D_irms1gain_Min) || (tmp > D_irms1gain_Max))
// 			{
// 				tmp = D_irms1gain;
// 				rlt = 0xff;
// 			}
//             adjust_data.irms1gain = tmp;
//             VER_WRbytes(ADJ_ADDR,&adjust_data.gain,sizeof(adjust_data),1);
//         break;

//         case Adj3_I2RMS_GAIN://0x10C2
//             //I2RMS GAIN
// 			VER_RDbytes(ADJ_ADDR,&adjust_data.gain,sizeof(adjust_data));
// 			tmp = ((*Src) << 8) + *(Src+1);
// 			if ((tmp < D_irms2gain_Min) || (tmp > D_irms2gain_Max))
// 			{
// 				tmp = D_irms2gain;
// 				rlt = 0xff;
// 			}
//             adjust_data.irms2gain = tmp;
//             VER_WRbytes(ADJ_ADDR,&adjust_data.gain,sizeof(adjust_data),1);
//         break;


//         case Adj5_CHB_Power_GAIN://0x10C4
// 			//CHB Power GAIN
// 			VER_RDbytes(ADJ_ADDR,&adjust_data.gain,sizeof(adjust_data));
// 			val.TBuf.buf0 =	 *Src;
// 			val.TBuf.buf1 =  *(Src+1);
// /*
// 			if ((val.val > D_w2gain_Min) && (val.val < D_w2gain_Max))
// 			{
// 				val.val = D_w2gain;							//0
// 				rlt = 0xff;
// 			}
// */

// _nop_();

// 			WriteEMU_REG(W2GAIN,val);
// 			adjust_data.w2gain = (unsigned int)val.val;
// _nop_();

// 			VER_WRbytes(ADJ_ADDR,&adjust_data.gain,sizeof(adjust_data),1);
// 		break;

//         case Adj7_CHB_Phase://0x10C6
// 			//CHB Phase
// 			VER_RDbytes(ADJ_ADDR,&adjust_data.gain,sizeof(adjust_data));
// 			val.TBuf.buf0 =	 *Src;
// 			val.TBuf.buf1 =  *(Src+1);
// /*
// 			if ((val.val > D_p2cal_Min) && (val.val < D_p2cal_Max))
// 			{
// 				val.val = D_p2cal;					  //0
// 				rlt = 0xff;
// 			}
// */
// 			WriteEMU_REG(P2CAL,val);
// 			adjust_data.p2cal = (unsigned int)val.val;
// 			VER_WRbytes(ADJ_ADDR,&adjust_data.gain,sizeof(adjust_data),1);
// 		break;

//         case Adj9_CHB_Offset://0x10C8
// 			//CHB Offset
// 			VER_RDbytes(ADJ_ADDR,&adjust_data.gain,sizeof(adjust_data));
// 			val.TBuf.buf0 =	 *Src;
// 			val.TBuf.buf1 =  *(Src+1);
// /*
// 			if ((val.val > D_w2os_Min) && (val.val < D_w2os_Max))
// 			{
// 				val.val = D_w2os;						//0
// 				rlt = 0xff;
// 			}
// */
// 			WriteEMU_REG(WATT2OS,val);
// 			adjust_data.w2os = (unsigned int)val.val;
// 			VER_WRbytes(ADJ_ADDR,&adjust_data.gain,sizeof(adjust_data),1);
// 		break;


//         case Adj4_CHA_Power_GAIN://0x10C3
//             //CHA Power GAIN
// 			VER_RDbytes(ADJ_ADDR,&adjust_data.gain,sizeof(adjust_data));
// 			val.TBuf.buf0 =	 *Src;
// 			val.TBuf.buf1 =  *(Src+1);
// 			if ((val.val > D_w1gain_Min) && (val.val < D_w1gain_Max))
// 			{
// 				val.val = D_w1gain;					//0
// 				rlt = 0xff;
// 			}
//             WriteEMU_REG(W1GAIN,val);
//             adjust_data.w1gain = (unsigned int)val.val;
//             VER_WRbytes(ADJ_ADDR,&adjust_data.gain,sizeof(adjust_data),1);
//         break;

//         case Adj6_CHA_Phase://0x10C5
// 			//CHA Phase
// 			VER_RDbytes(ADJ_ADDR,&adjust_data.gain,sizeof(adjust_data));
// 			val.TBuf.buf0 =	 *Src;
// 			val.TBuf.buf1 =  *(Src+1);
// 			if ((val.val > D_p1cal_Min) && (val.val < D_p1cal_Max))
// 			{
// 				val.val = D_p1cal;					 //0
// 				rlt = 0xff;
// 			}
// 			WriteEMU_REG(P1CAL,val);
// 			adjust_data.p1cal = (unsigned int)val.val;
// 			VER_WRbytes(ADJ_ADDR,&adjust_data.gain,sizeof(adjust_data),1);
// 		break;

//         case Adj8_CHA_Offset://0x10C7
// 			//CHA Offset
// 			VER_RDbytes(ADJ_ADDR,&adjust_data.gain,sizeof(adjust_data));
// 			val.TBuf.buf0 =	 *Src;
// 			val.TBuf.buf1 =  *(Src+1);
// 			if ((val.val > D_w1os_Min) && (val.val < D_w1os_Max))
// 			{
// 				val.val = D_w1os;					 //0
// 				rlt = 0xff;
// 			}
// 			WriteEMU_REG(WATT1OS,val);
// 			adjust_data.w1os = (unsigned int)val.val;
// 			VER_WRbytes(ADJ_ADDR,&adjust_data.gain,sizeof(adjust_data),1);
// 		break;

//         case Adj11_adcos://0x10CA
// 			//adcos
// 			VER_RDbytes(ADJ_ADDR,&adjust_data.gain,sizeof(adjust_data));
// 			val.TBuf.buf0 =	 *Src;
// 			val.TBuf.buf1 =  *(Src+1);
// /*
// 			if ((val.val > D_adcos_Min) && (val.val < D_adcos_Max))
// 			{
// 				val.val = D_adcos;						//0
// 				rlt = 0xff;
// 			}
// */
// 			WriteEMU_REG(ADCOSU,val);
// 			adjust_data.adcos = (unsigned int)val.val;
// 			VER_WRbytes(ADJ_ADDR,&adjust_data.gain,sizeof(adjust_data),1);
// 		break;

//         case Adj12_irms1os://0x10CB
// 			//irms1os
// 			VER_RDbytes(ADJ_ADDR,&adjust_data.gain,sizeof(adjust_data));
// 			val.TBuf.buf0 =	 *Src;
// 			val.TBuf.buf1 =  *(Src+1);
// /*
// 			if ((val.val > D_irms1os_Min) && (val.val < D_irms1os_Max))
// 			{
// 				val.val = D_irms1os;						//0
// 				rlt = 0xff;
// 			}
// */
//             WriteEMU_REG(IRMS1OS,val);
// 			adjust_data.irms1os = (unsigned int)val.val;
// 			VER_WRbytes(ADJ_ADDR,&adjust_data.gain,sizeof(adjust_data),1);
// 		break;

//         case Adj13_irms2os://0x10CC
// 			//irms2os
// 			VER_RDbytes(ADJ_ADDR,&adjust_data.gain,sizeof(adjust_data));
// 			val.TBuf.buf0 =	 *Src;
// 			val.TBuf.buf1 =  *(Src+1);
// /*
// 			if ((val.val > D_irms2os_Min) && (val.val < D_irms2os_Max))
// 			{
// 				val.val = D_irms2os;						//0
// 				rlt = 0xff;
// 			}
// */
//             WriteEMU_REG(IRMS2OS,val);
// 			adjust_data.irms2os = (unsigned int)val.val;
// 			VER_WRbytes(ADJ_ADDR,&adjust_data.gain,sizeof(adjust_data),1);
// 		break;

// 		case Adj14_cal_L_mode://0x0F0A
// 			if ((*Src) == 0x01)
// 			{
// 				g_Cal.Flag |= D_Auto;
// 				cal_L_mode();
// 			}
// 			else
// 			{
// 				g_Cal.Flag &= ~D_Auto;
//                 #if MeterType == MT_1P3L
//                 cal_1P3L_mode();
//                 #endif
// 			}
// 		break;

// 		case Adj15_cal_N_mode://0x0F0B
// 			if ((*Src) == 0x01)
// 			{
// 				g_Cal.Flag |= D_Auto;
// 				cal_N_mode();
// 			}
// 			else
// 			{
// 				g_Cal.Flag &= ~D_Auto;
//                 #if MeterType == MT_1P3L
//                 cal_1P3L_mode();
//                 #endif
// 			}
// 		break;

//         default:
//         {
//             rlt = 0xff;
//             break;
//         }
//     }
//     return rlt;
// }