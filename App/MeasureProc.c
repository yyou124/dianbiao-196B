/*==============================================================================
                Copyright(C) 1997-2019.  Sinowealth Tech. Co., Ltd.
--------------------------------------------------------------------------------
Project Name  : MEASUREPROC.c
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
#ifndef  MEASUREPROC_C
	#define  MEASUREPROC_C
#endif
//-------------------------------------------------------------------------------

//---------------



#include "config.h"
#include "Headconfig.h"




//extern M_NBMODE g_NB;

//extern NB_PROTOCOL_NORMAL protocol_nb;
unsigned int xdata UpGradeFlg _at_ 0x0A00;

#if MeterType == MT_1P2L
	const unsigned char code SoftwareVersionNew[20] = {"xxxxxxxxxxxxxxxxxxxx"};
 	const unsigned char code SoftwareVersionTest[30] = {"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"};
#else
 	const unsigned char code SoftwareVersionNew[20] = {"xxxxxxxxxxxxxxxxxxxx"};
 	const unsigned char code SoftwareVersionTest[30] = {"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"};
#endif



////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: EE_KWH_shift
** 函数描述: 根据电量计算EEPROM中的存储地址
** 输入参数: 无
** 输出参数:
*******************************************************************************************/
void EE_KWH_shift(void)
{
    unsigned char buff[2];
    unsigned int hex_data[2];
	unsigned char temp;

	EE_to_RAM(EE_KWH_ADDRESS,(unsigned char *)&temp,1);	//读出用电量地址
	temp = (temp-EE_KWH0)/EE_KWH_SHIFT;	//计算偏移数量
	hex_data[0] = temp * EE_KWH_MAX;	//计算当前偏移地址的用电量

    ReverseCpy(buff, &KWH[2], 2);//从万位提取用电量
    hex_data[1] = BCD2toINT(buff);	//计算实际用电量

	if(hex_data[1]-hex_data[0] >= EE_KWH_MAX) //更新用电量存储地址
	{
		hex_data[1] = hex_data[1] / EE_KWH_MAX;
		EE_KHH_address = hex_data[1] * EE_KWH_SHIFT + EE_KWH0;
		if(EE_KHH_address>0xFE)
			EE_KHH_address = 0x00;
    SEQ_write(EE_KWH_ADDRESS,(unsigned char *)&EE_KHH_address,1);
	}

}
////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: ALARMLED
** 函数描述:
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
/*
void ALARMLED(void)
{
    if (g_AllDisDly == 0)
    {
        if((g_Flag.EventFlg & E_Cover) == E_Cover)       				   //发生开端盖事件，LED灯闪烁，高优先级
        {
            FAULT_LED = (FAULT_LED^FAULT_LED_ON);
        }
        else
        {
            #if MeterType == MT_1P2L
            	if ((g_Flag.ALARM[0] & (E_IReverse + E_Bypass)) != 0)     	//单相两线表发生反向、旁路状态，LED灯闪烁
	            {
	                FAULT_LED &= ~FAULT_LED_ON;							  	//0

	                REV_LED &= ~REV_LED_ON;                 		        //电流正反向LED	 OFF
	            }
	            else
	            {
	                FAULT_LED |= FAULT_LED_ON;							  	//1

	                REV_LED |= REV_LED_ON;                 		        	//电流正反向LED	 ON
	            }
            #else
            	if ((g_Flag.ALARM[0] & E_IReverse) != 0)                  	//单相三线表发生反向状态，LED灯闪烁
	            {
	                FAULT_LED &= ~FAULT_LED_ON;							  	//0

	                REV_LED &= ~REV_LED_ON;                 		        //电流正反向LED	 OFF
	            }
	            else
	            {
	                FAULT_LED |= FAULT_LED_ON;							  	//1

	                REV_LED |= REV_LED_ON;                 		        	//电流正反向LED	 ON
	            }
            #endif
        }
    }
    else
    {
        FAULT_LED |= FAULT_LED_ON;										//1
    }
}
*/



////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: 正向有功小数点电能值
** 函数描述: 保存断电数据
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
void check_dotKWHD(void)
{
	unsigned char  xdata  ECEpBuff[2];
	unsigned char temp0;

	//EE_to_RAM(EE_KWHP,&ECEpBuff[0],2);
//    if (0 == VER_RDbytes(EE_KWHP, &ECEpBuff[0], 2)) //若校验出错，则直接恢复默认值
//    {
//        ECEpBuff[0] =  g_ConstPara.Threshold/2;
//        ECEpBuff[1] = 0x50;
//        VER_WRbytes(EE_KWHP, &ECEpBuff[0],2,1);//正向有功脉冲小数电量
//    }
	temp0 = EEPromByteRead(0x05,0x01);//掉电数据
	if(temp0 == 0xaa)
	{
		//EE_to_RAM(0x531, &ECEpBuff[0], 2);	//将有功脉冲数读进ECEpBuff[0]中，小数读进ECEpBuff[1]中
		ECEpBuff[0] = EEPromByteRead(0x05,0x31);
		ECEpBuff[1] = EEPromByteRead(0x05,0x32);
		EEPromSectorErase(0x04);// 写之前，扇区擦除
		EEPromByteProgram(0x04,0x31,ECEpBuff[0]);
		EEPromByteProgram(0x04,0x32,ECEpBuff[1]);
		EEPromSectorErase(0x05);// 写之前，扇区擦除
	}
	else
	{
		//EE_to_RAM(0x431, &ECEpBuff[0], 2);	//将有功脉冲数读进ECEpBuff[0]中，小数读进ECEpBuff[1]中
		ECEpBuff[0] = EEPromByteRead(0x04,0x31);
		ECEpBuff[1] = EEPromByteRead(0x04,0x32);
	}
	KWHP = ECEpBuff[0];//脉冲数
	KWHD = ECEpBuff[1];//小数点电能
	//小数点正向电能储存进三个区域，相互校验
	KWHD1 = KWHD;
	KWHD2 = KWHD;
	KWHD3 = KWHD;
}


////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: 反向有功小数点电能值
** 函数描述: 保存断电数据
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
/* void check_dotRKWHD(void)
{
	unsigned char  xdata  ECEpBuff[2];

	//EE_to_RAM(EE_RKWHP,&ECEpBuff[0],2);
    VER_RDbytes(EE_RKWHP, &ECEpBuff[0], 2);

	if (ECEpBuff[0] > (2*g_ConstPara.Threshold))
	{
		ECEpBuff[0] =  g_ConstPara.Threshold/2;
	}
	if (((ECEpBuff[1] & 0x0f) > 0x09) ||  ((ECEpBuff[1] & 0xf0) > 0x90))
	{
		ECEpBuff[1] = 0x50;
	}

	RKWHP= ECEpBuff[0];
	RKWHD= ECEpBuff[1];

	RKWHD1 = RKWHD;
	RKWHD2 = RKWHD;
	RKWHD3 = RKWHD;
} */


////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: 正向反向有功小数点电能值，小数点电能相互校验
** 函数描述: 保存断电数据
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
void KWH_RKWH_dot(void)
{
//正向有功
	if (KWHD1 == KWHD2)
	{
		KWHD3 = KWHD1;
	}
	else if (KWHD1 == KWHD3)
	{
		KWHD2 = KWHD1;
	}
	else if (KWHD2 == KWHD3)
	{
		KWHD1 = KWHD2;
	}
	else
	{
		check_dotKWHD();
	}
	if (KWHD != KWHD1)
	{
		KWHD = KWHD1;
	}

//反向有功
	/* if (RKWHD1 == RKWHD2)
	{
		RKWHD3 = RKWHD1;
	}
	else if (RKWHD1 == RKWHD3)
	{
		RKWHD2 = RKWHD1;
	}
	else if (RKWHD2 == RKWHD3)
	{
		RKWHD1 = RKWHD2;
	}
	else
	{
		check_dotRKWHD();
	}
	if (RKWHD != RKWHD1)
	{
		RKWHD = RKWHD1;
	} */
}



////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
*函数说明：RAM中正向有功CHECK
*输入参数：无
*输出参数：EE对应地址不同数据时刷新,正确返回0，不正确，返回0xff
*返    回：无
*条    件：
*******************************************************************************************/
// unsigned char KWH_Check(void)
// {
//     unsigned char xdata rlt = 0;

//     //检查RAM中是否有正确的小数电量
//     if (KWHD1 == KWHD2)
// 	{
//         KWHD3 = KWHD1;
// 	}
// 	else if (KWHD1 == KWHD3)
// 	{
// 		KWHD2 = KWHD1;
// 	}
// 	else if (KWHD2 == KWHD3)
// 	{
// 		KWHD1 = KWHD2;
// 	}
// 	else
// 	{
// 		rlt = 0xff;
//         return rlt;
// 	}

//     if (DotBCD_Check(KWHD1))
//     {
//         rlt = 0xff;
//     }
//     return rlt;
// }



////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
*函数说明：RAM中反向有功CHECK
*输入参数：无
*输出参数：EE对应地址不同数据时刷新
*返    回：无
*条    件：
*******************************************************************************************/
/* unsigned char RKWH_Check(void)
{
    unsigned char xdata rlt = 0;

    //检查RAM中是否有正确的小数电量
    if (RKWHD1 == RKWHD2)
	{
		RKWHD3 = RKWHD1;
	}
	else if (RKWHD1 == RKWHD3)
	{
		RKWHD2 = RKWHD1;
	}
	else if (RKWHD2 == RKWHD3)
	{
		RKWHD1 = RKWHD2;
	}
	else
	{
		rlt = 0xff;
        return rlt;
	}

    if (DotBCD_Check(RKWHD1))
    {
        rlt = 0xff;
    }
    return rlt;
}
 */




////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: 正向无功小数点电能值
** 函数描述: 保存断电数据
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
//void check_dotQKWHD(void)
//{
//	unsigned char  xdata  ECEqBuff[2];

////	  EE_to_RAM(EE_QKWHP,&ECEqBuff[0],2);
////    if (0 == VER_RDbytes(EE_QKWHP, &ECEqBuff[0], 2)) //若校验出错，则直接恢复默认值
////    {
////        ECEqBuff[0] =  g_ConstPara.Threshold/2;
////        ECEqBuff[1] = 0x50;
////        VER_WRbytes(EE_QKWHP, &ECEqBuff[0],2,1);//正向有功脉冲小数电量
////    }
//    VER_RDbytes(EE_QKWHP, &ECEqBuff[0], 2);

//	if (ECEqBuff[0] > (2*g_ConstPara.Threshold))
//		ECEqBuff[0] =  g_ConstPara.Threshold/2;

//	if (((ECEqBuff[1] & 0x0f) > 0x09) ||  ((ECEqBuff[1] & 0xf0) > 0x90))
//		ECEqBuff[1] = 0x50;

//	QKWHP = ECEqBuff[0];
//	QKWHD = ECEqBuff[1];

//	QKWHD1 = QKWHD;
//	QKWHD2 = QKWHD;
//	QKWHD3 = QKWHD;
//}


////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: 反向无功小数点电能值
** 函数描述: 保存断电数据
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
/* void check_dotRQKWHD(void)
{
	unsigned char  xdata  ECEqBuff[2];

	//EE_to_RAM(EE_RQKWHP,&ECEqBuff[0],2);
    VER_RDbytes(EE_RQKWHP, &ECEqBuff[0], 2);

	if (ECEqBuff[0] > (2*g_ConstPara.Threshold))
		ECEqBuff[0] =  g_ConstPara.Threshold/2;

	if (((ECEqBuff[1] & 0x0f) > 0x09) ||  ((ECEqBuff[1] & 0xf0) > 0x90))
		ECEqBuff[1] = 0x50;

	RQKWHP= ECEqBuff[0];
	RQKWHD= ECEqBuff[1];

	RQKWHD1 = RQKWHD;
	RQKWHD2 = RQKWHD;
	RQKWHD3 = RQKWHD;
}
 */

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: 正向反向无功小数点电能值
** 函数描述: 保存断电数据
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
//  void QKWH_RQKWH_dot(void)
// {
// //正向无功
// 	if (QKWHD1 == QKWHD2)
// 	{
// 		QKWHD3 = QKWHD1;
// 	}
// 	else if (QKWHD1 == QKWHD3)
// 	{
// 		QKWHD2 = QKWHD1;
// 	}
// 	else if (QKWHD2 == QKWHD3)
// 	{
// 		QKWHD1 = QKWHD2;
// 	}
// 	else
// 	{
// 		check_dotQKWHD();
// 	}
// 	if (QKWHD != QKWHD1)
// 	{
// 		QKWHD = QKWHD1;
// 	}

// //反向无功
// 	// if (RQKWHD1 == RQKWHD2)
// 	// {
// 	// 	RQKWHD3 = RQKWHD1;
// 	// }
// 	// else if (RQKWHD1 == RQKWHD3)
// 	// {
// 	// 	RQKWHD2 = RQKWHD1;
// 	// }
// 	// else if (RQKWHD2 == RQKWHD3)
// 	// {
// 	// 	RQKWHD1 = RQKWHD2;
// 	// }
// 	// else
// 	// {
// 	// 	check_dotRQKWHD();
// 	// }
// 	// if (RQKWHD != RQKWHD1)
// 	// {
// 	// 	RQKWHD = RQKWHD1;
// 	// }
// }



////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
*函数说明：RAM中正向无功CHECK
*输入参数：无
*输出参数：EE对应地址不同数据时刷新
*返    回：无
*条    件：
*******************************************************************************************/
// unsigned char QKWH_Check(void)
// {
//     unsigned char xdata rlt = 0;

//     //检查RAM中是否有正确的小数电量
//     if (QKWHD1 == QKWHD2)
// 	{
//         QKWHD3 = QKWHD1;
// 	}
// 	else if (QKWHD1 == QKWHD3)
// 	{
// 		QKWHD2 = QKWHD1;
// 	}
// 	else if (QKWHD2 == QKWHD3)
// 	{
// 		QKWHD1 = QKWHD2;
// 	}
// 	else
// 	{
// 		rlt = 0xff;
//         return rlt;
// 	}

//     if (DotBCD_Check(QKWHD1))
//     {
//         rlt = 0xff;
//     }
//     return rlt;
// }



////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
*函数说明：RAM中反向无功CHECK
*输入参数：无
*输出参数：EE对应地址不同数据时刷新
*返    回：无
*条    件：
*******************************************************************************************/
// unsigned char RQKWH_Check(void)
// {
//     unsigned char xdata rlt = 0;

//     //检查RAM中是否有正确的小数电量
//     if (RQKWHD1 == RQKWHD2)
// 	{
// 		RQKWHD3 = RQKWHD1;
// 	}
// 	else if (RQKWHD1 == RQKWHD3)
// 	{
// 		RQKWHD2 = RQKWHD1;
// 	}
// 	else if (RQKWHD2 == RQKWHD3)
// 	{
// 		RQKWHD1 = RQKWHD2;
// 	}
// 	else
// 	{
// 		rlt = 0xff;
//         return rlt;
// 	}

//     if (DotBCD_Check(RQKWHD1))
//     {
//         rlt = 0xff;
//     }
//     return rlt;
// }





////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
*函数说明：掉电保存小数电量和脉冲数
*输入参数：无
*输出参数：EE对应地址不同数据时刷新
*返    回：无
*条    件：
*******************************************************************************************/
// void PluseAndDecimalProc(void)
// {
// 	unsigned char temp1[2],temp2[2];

// //有功
// 	if ((KWHD==KWHD1) && (KWHD==KWHD2))
// 	{
// 		temp1[0] = KWHP;
// 		temp1[1] = KWHD;


// 		//EE_to_RAM(EE_KWHP,&temp2[0], 4);       //低频脉冲正反，正反功小数位.
//         VER_RDbytes_INT(EE_KWHP, &temp2[0], 2);      //读出脉冲数与小数电量

//         if ((temp1[0] != temp2[0]) || (temp1[1] != temp2[1]))
//         {
//             VER_WRbytes_INT(EE_KWHP, &temp1[0], 2,1);//正向有功脉冲小数电量
//         }
// 	}

//}


////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: ConstPara_Chk
** 函数描述: 初始化RAM
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
unsigned int ConstPara_Chk(void)
{
	unsigned char i;
	unsigned int Chk = 0xA55A;
	unsigned char *p;
	p = (unsigned char *)&g_ConstPara.Constant;
	for (i = 0; i < (sizeof(CONSTPARA) - 2); i++)
	{
		Chk += *p;
		p++;
	}
	return Chk;

}
//1200-0x0C-0x20
//1600-0x10-0x1D
//1000-0x0A-0x2F
//2000-0x14-0x17
void ConstPara_Init(unsigned char meter_const, unsigned char Cmd)
{
	//__U32_Def val;

	if(meter_const == 0)
	{
	}

    //1200
	if(Cmd ==0xA5)
	{
		g_ConstPara.Constant  = 1200;
		g_ConstPara.Threshold = 0x0C;
		g_ConstPara.Icont 	  = DefaultIcont;
//		g_ConstPara.Icont 	  = 0x41;
		g_ConstPara.Chk       = ConstPara_Chk();//添加校验
	}


    /* if ((adjust_data.icont != g_ConstPara.Icont) && (Cmd != 0xA5))
    {
        //icont发生变化后需要对校表参数进行修改
        VER_RDbytes(ADJ_ADDR,&adjust_data.gain,sizeof(adjust_data));
        adjust_data.icont = g_ConstPara.Icont;
        val.val = 0x00;
        val.TBuf.buf0 = g_ConstPara.Icont;
        WriteEMU_REG(ICONT,val);
        VER_WRbytes(ADJ_ADDR,&adjust_data.gain,sizeof(adjust_data),1);

//        MDRst(0);
    } */
}
////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: ConstPara_Chk
** 函数描述: 初始化RAM
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
void NB_LORA_PANDUAN(unsigned char *flag)
{
	char p[5];
	char *dest;
	dest=&p;
	sprintf(dest,"AT\r\n");
	UART0_SendString((unsigned char*)dest,strlen(dest));
	Delay_ms(300);
	UART0_SendString((unsigned char*)dest,strlen(dest));
	Delay_ms(300);
	RSTSTAT &= Bin(11111000);	//清看门狗
	if(ATcmd_Transmit(dest,"OK",1000))//失败，不是NB模块
	{
		flag[0] = 0xAA;//判断为LORA模块
		flag[1] = 0xAA;
	}
	else				//判断为NB模块
	{
		flag[0] = 0xBB;
		flag[1] = 0xBB;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: Init_RAM
** 函数描述: 初始化RAM
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
void Init_RAM(void)
{
	//uint8 temp[6];
    //Note:
    //ReadEMUFromTOEeprom()需要在ConstPara_Init()之后，ConstPara_Init()不能写adjust_data，否则导致导致ReadEMUFromTOEeprom()的检查不起作用；
    //check_dot&check_dotRKWHD函数必须在ConstPara_Init（）之后，只有在g_ConstPara.Threshold正确后才能保证脉冲、小数电量初始化正常;
    meter_const = DefaultThreshold;					//脉冲常数为1200
	//加载脉冲常数，ICONT寄存器数值
    ConstPara_Init(meter_const, 0xA5);
	//校验校表参数正确与否，不正确恢复默认值
    ReadEMUFromTOEeprom();
    //从EEPROM中读取有功电能：脉冲数，小数点数据
	ReadMeterFromTOEeprom();
    //从EEPROM中读取有功电能：整数部分，5字节
	EE_to_RAM(EE_KWH_ADDRESS,(unsigned char *)&EE_KHH_address,1);	//读出用电量地址
	//加载用电量EEPROM存储地址
	VER_RDbytes((unsigned int)EE_KHH_address, &kwh_value.integer[0], 5);
	//计算组合有功：将整数部分与小数点部分组合到一起
	ECRunKwh();                                     //计算组合有功
	//读取表号
	//EE_to_RAM(EE_Meter_address, &param_data.meter_address[0], 6);
	VER_RDbytes(EE_Meter_address, &param_data.meter_address[0], 6);
	//表号不是BCD码，恢复默认
	if(!BCDCheck(&param_data.meter_address[0], 6))
	{
		MemInitSet(&param_data.meter_address[0],0x00, 6);
		VER_WRbytes(EE_Meter_address, &param_data.meter_address[0], 6, 1);
	}
	//加载默认表号
	MemInitSet(&param_data.meter_address_def[0],0x99, 6);
	//读取厂商代码
	EE_to_RAM(EE_Meter_Factory, &param_data.meter_factory[0],1);
	//厂商代码不是BCD码，恢复默认
//	if(!BCDCheck(&param_data.meter_factory[0], 1))
//	{
//		MemInitSet(&param_data.meter_factory[0],0x00, 1);
//		SEQ_write(EE_Meter_Factory, &param_data.meter_factory[0], 1);
//	}
    //检查EEPROM中冷起动次数是否正确，若错误则恢复为0，暂时不需要
    // if (0 == VER_RDbytes(CNT_RST, &temp[0], 1))
    // {
    //     temp[0] = 0x00;
    //     VER_WRbytes(CNT_RST, &temp[0], 1, 1);
    // }

    //检查EEPROM中读取结算时间数据是否正确，若错误则恢复默认
	//结算时间？自动抄表日期？？
/*
	if (0==VER_RDbytes(EE_SAM_ENDDATE,&temp[0],2))
	{
        temp[0]=0x01;
        temp[1]=0x00;
        VER_WRbytes(EE_SAM_ENDDATE,&temp[0],2,1);
	}
*/

    //检查EEPROM中首次上电标志，首次上电自动进入工厂模式,启动校表程序
	//如果EEPROM中的EE_FirstProg_FLAG第一次上电标志不为0xA5(4byte)
//	//则gbFgKeyProg = 0xF001，启动校表程序
// MemInitSet(&g_Buffer[0], 0x00, 4);
// VER_WRbytes(EE_FirstProg_FLAG,&g_Buffer[0],4, 1);
	EE_to_RAM(EE_FirstProg_FLAG, &g_Buffer[0], 4);
    if ((g_Buffer[0] != 0xA5) && (g_Buffer[1] != 0xA5) && (g_Buffer[2] != 0xA5) && (g_Buffer[3] != 0xA5))
    {
		gbFgKeyProg = 0xF001;
		SEQ_write(EE_PROG_FLAG, (uint8*)&gbFgKeyProg, 2);//启动校表程序Flag
		//此处在校表程序结束后，置FLGA
        //MemInitSet(&g_Buffer[0], 0x00, 4);
       //SEQ_write(EE_FirstProg_FLAG, &g_Buffer[0], 4);
    }
	else
	{
		gbFgKeyProg = 0x0000;
		SEQ_write(EE_PROG_FLAG, (uint8*)&gbFgKeyProg, 2);//启动校表程序Flag
	}
	EE_to_RAM(EE_PROG_FLAG, (uint8*)&gbFgKeyProg, 2);

    //检查EEPROM时钟修正参数，校验正确则修正，否则跳过
	Init_RTCAdjustProc();//不使用RTC时钟补偿

	//CurChExDly = C_DefaultCurExTT;                                          //Init I1&I2 exchange value
	//BlinkFlg = 0;                                                           //LCD fresh in factory mode enable flag
    //gbbattery=0;                                                            //Battery error flag

	gbPCNTCount=0; //潜动时间标志置0                                                          //Start & Creep timer
	MemInitSet((unsigned char xdata *)&g_FilterV.Buf[0], 0x00, sizeof(g_FilterV));		//Filter Buffer
	MemInitSet(&g_Cal.Step, 0x00, sizeof(g_Cal));		 								//Calibrate State
	MemInitSet(&g_Flag.Run, 0x00, sizeof(g_Flag));                                       //Init System Flag
	MemInitSet(&_UINT8_RTC[0],0x00,7);                                                   //Init RTC buffer

	//安装的模块FLAG初始化
	//NB_LORA_PANDUAN(&NB_LORA[0]);
	MemInitSet(&NB_LORA[0],0x00,2);
	VER_WRbytes(EE_NB_LORA, &NB_LORA[0], 2, 1);
	//继电器相关初始化
	//DelayStatus[0] = 0x11;
	//DelayStatus[1] = 0x11;
	VER_RDbytes(RELAY_STATUS, &DelayStatus[0], 2);
	if(gbFgKeyProg == 0x0000)//当前在非工厂模式，可以操作继电器
	{
	if((DelayStatus[0] != 0xA5)&&(DelayStatus[1] != 0xA5))
	{
		DelayFlag = 1;//继电器断电
	}
	else
	{
		DelayFlag = 0;//继电器通电
	}
	}

	//协议相关
	g_Tran.PwoerOn = 1; //上电握手
	g_Tran.AutoReportTime = 0;	//自动上报尝试次数
	g_Tran.AutoReportCount = NB_POWER_ON;//自动上报时间&通信时间初始化
	g_Tran.AutoReportFlag = 0;	//自动上报FLAG

	g_NB.UARTReceiveOK = 0;			//NB数据UART接收完成FLAG
	g_NB.DataReceiveOK = 0;		//
	//模块相关初始化
	g_NB.InitCount = 0;			//初始化次数
	g_NB.ReInitTime = 1;		//初始化，间隔时间
	g_NB.InitStep = 1;			//初始化，从第一步开始
	MemInitSet(&g_NB.InitState[0],NB_Init_NOT_OK,2);
	VER_WRbytes(EE_NB_STATE, &g_NB.InitState[0], 2, 1);

	//显示相关初始化
	g_Flag.ALARM[0] = 0X00; //报警置零
	gbFgShowItemNum = D_ShowItemNum;//显示项目次数,默认是第一项，当前总电量
    g_Flag.Run |= F_DISP;
	g_Disp.PollingDisplayPtr = 0;		  							//默认显示第一项，组合有功电能量
    //轮显时间间隔
    g_Disp.PollingDisplayCountSet = D_PollingDisplayTime;			//默认显示间隔为5s
    g_Disp.PollingDisplayCount = D_PollingDisplayTime;			//默认显示间隔为5s

	//校表变量初始化
	gBAdjKeyCount = 0;
	gBAdjKeyEnable = 0x00;
	gBAdjKeyEnable1 = 0x00;

	// gBAdjKeyNew = 0;
	// gBAdjKeyOld = 0;
	gBAdjKeyStatus = 0;						//校表跳线断开退出
	gbUartAdjust = 0;	//UART通讯校表启动标志
	ADJ_ID = 0;		//Uart校表步骤输入指示
	Sign_Bit = 0;	//Uart误差符号位
	Adj_Error = 0; //Uart误差XX.XX%


	//gBClsKeyCount = 0;
	//gBClsKeyEnable = 0x00;
	//gBClsKeyEnable1 = 0x00;
	//g_Cal.ClsKeyStep = 0;					//清一次有效标志
	//gbFgClsKeyProg = 0x0000;

	AverPower = 0;
	AdjTempValueEverySec = 0;
	gbAverTimes = 0;
	gbAdjShowOk2S = 0;
	gbAdjStepFlag = 0;
	gbAdjCurrentStep = 0;

	//中断优先级
    IPL0 = Bin(00000000);
    IPH0 = Bin(00000000);
    IPL1 = Bin(10000000);
    IPH1 = Bin(10000001);
}


////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称:  ReadMeterFromTOEeprom
** 函数描述:
** 输入参数: 无
** 输出参数: 无
** 读取有功,无功电能小数数据
*******************************************************************************************/
void ReadMeterFromTOEeprom(void)
{
    check_dotKWHD();		//加载有功脉冲数,小数点电能
    //check_dotRKWHD();		//反向，不需要
    //KWH_RKWH_dot();			//有功小数点电能校验

    //check_dotQKWHD();		//无功电量数据，不需要
    //check_dotRQKWHD();	//无功电量数据，不需要
    //QKWH_RQKWH_dot();		//无功电量数据，不需要
}





////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: ECRunKwh
** 函数描述: 初始化RAM
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
void ECRunKwh (void)                				//计算有功
{
	KWH_PN[0] = KWHD;               				//小数位
	KWH_PN[1] = kwh_value.integer[0];
	KWH_PN[2] = kwh_value.integer[1];
	KWH_PN[3] = kwh_value.integer[2];
	KWH_PN[4] = kwh_value.integer[3];
	KWH_PN[5] = kwh_value.integer[4];

	//A_energy(&KWH_PN[0], &RKWHD);	//将反向有功累加如组合有功中
}


////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: ECRunQKwh
** 函数描述: 初始化RAM
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
/* void ECRunQKwh (void)                				//计算当月无功
{
	QKWH_PN[0] = QKWHD;               				//小数位
	QKWH_PN[1] = QKWH[0];
	QKWH_PN[2] = QKWH[1];
	QKWH_PN[3] = QKWH[2];
	//A_energy(&QKWH_PN[0], &RQKWHD);
} */

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: ECRunQKwh_RQKwh
** 函数描述: 初始化RAM
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
/* void ECRunQKwh_RQKwh(void)
{
 	QKWH_Inductive[0] = 0x00;						//当月无功电感量
 	QKWH_Inductive[1] = 0x00;
 	QKWH_Inductive[2] = 0x00;
 	QKWH_Inductive[3] = 0x00;
	A_energy(&QKWH_Inductive[0], &QKWHD);

	RQKWH_Capactive[0] = 0x00;						//当月无功电容量
	RQKWH_Capactive[1] = 0x00;
	RQKWH_Capactive[2] = 0x00;
	RQKWH_Capactive[3] = 0x00;
	A_energy(&RQKWH_Capactive[0], &RQKWHD);
} */

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: EnergyProc
** 函数描述: 电量处理
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
void EnergyProc(void)
{

//正向有功累加
	if (KWHP>=g_ConstPara.Threshold)  					//正向总有功电量
	{
		if (KWHP >= g_ConstPara.Threshold*2)
		{
			KWHP = g_ConstPara.Threshold/2;
		}
		else
		{
            _push_(IEN0);
            EA = 0;
            KWHP -= g_ConstPara.Threshold;
            _pop_(IEN0);
			//有功电量累加,整数部分存入EEPROM中
			EE_KWH_shift();
			E_operating((unsigned int)EE_KHH_address, &kwh_value.integer[0], &KWHD, 6);   //电量处理
			KWHD1 = KWHD;
			KWHD2 = KWHD;
			KWHD3 = KWHD;
		}
	}
//采用绝对值累加，反向不需要
//反向有功累加
/*
	if (RKWHP>=g_ConstPara.Threshold) 					//反向总有功电量
	{
		if (RKWHP >= g_ConstPara.Threshold*2)
        {
            RKWHP = g_ConstPara.Threshold/2;
        }
		else
		{
            _push_(IEN0);
            EA = 0;
            RKWHP -= g_ConstPara.Threshold;
            _pop_(IEN0);

			E_operating(EE_RKWH0, &RKWH[0], &RKWHD, 3);  //电量处理

			RKWHD1 = RKWHD;
			RKWHD2 = RKWHD;
			RKWHD3 = RKWHD;
		}
	}
*/
//无功不需要
/*
//正向无功累加
	if (QKWHP>=g_ConstPara.Threshold)  					//正向总无功电量
	{
		if (QKWHP >= g_ConstPara.Threshold*2)
		{
			QKWHP = g_ConstPara.Threshold/2;
		}
		else
		{
            _push_(IEN0);
            EA = 0;
            QKWHP -= g_ConstPara.Threshold;
            _pop_(IEN0);

			Q_operating(EE_QKWH0, &QKWH[0], &QKWHD, 3);   //电量处理

			QKWHD1 = QKWHD;
			QKWHD2 = QKWHD;
			QKWHD3 = QKWHD;
		}
	}

//反向无功累加
	if (RQKWHP>=g_ConstPara.Threshold) 					//反向总无功电量
	{
		if (RQKWHP >= g_ConstPara.Threshold*2)
        {
            RQKWHP = g_ConstPara.Threshold/2;
        }
		else
		{
            _push_(IEN0);
            EA = 0;
            RQKWHP -= g_ConstPara.Threshold;
            _pop_(IEN0);

			Q_operating(EE_RQKWH0, &RQKWH[0], &RQKWHD, 3);  //电量处理

			RQKWHD1 = RQKWHD;
			RQKWHD2 = RQKWHD;
			RQKWHD3 = RQKWHD;
		}
	}
*/
}


////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
*函数说明:电量处理
*输入参数:EEPROM地址EE_addr,整数IRAM地址,小数DRAM,整数RAM数量len
*输出参数:LCD_b
*返    回:无
*条    件:对应首地址
*******************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////
void E_operating(unsigned int EE_addr ,unsigned char *IRAM, unsigned char *DRAM, unsigned char len)
{
	//unsigned char r;

	Add1BCD(DRAM, 1);
	//发生小数点进位，整数位加一
	if (!(*DRAM))								 //有可能有小数点的增加1度。1.25==>2.25
	{
		//从EEPROM中读出电量到IRAM中
		VER_RDbytes(EE_addr, IRAM, len);
		//r=VER_RDbytes(EE_addr, IRAM, len);

		Add1BCD(IRAM, 5);      					//整数电量累加
		_nop_();
		//将刷新后的电量存入EEPROM中
		VER_WRbytes(EE_addr, IRAM, len, 1);
		//VER_WRbytes(EE_addr, IRAM, len, r);
		ECRunKwh();            					//刷新组合有功
	}
}


////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
*函数说明:电量处理
*输入参数:EEPROM地址EE_addr,整数IRAM地址,小数DRAM,整数RAM数量len
*输出参数:LCD_b
*返    回:无
*条    件:对应首地址
*******************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////
// void Q_operating(unsigned int EE_addr, unsigned char *IRAM, unsigned char *DRAM, unsigned char len)
// {
// 	unsigned char r;

// 	Add1BCD(DRAM, 1);

// 	if (!(*DRAM))								//有可能有小数点的增加1度。1.25==>2.25
// 	{
// 		r=VER_RDbytes(EE_addr, IRAM, len);

// 		Add1BCD(IRAM, 3);      					//整数电量累加
// 		_nop_();
// 		VER_WRbytes(EE_addr, IRAM, len, r);
// //        ECRunQKwh(); 							//刷新组合无功
// //        ECRunQKwh_RQKwh();
// 	}
// }



////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
*函数说明：有功组合电量运算
*输入参数：源地址s_addr,目标地址t_addr,特征字RAM
*输出参数：结果在源地址s_addr
*返    回：无
*条    件：
*******************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////
// void A_energy(unsigned char *s_addr, unsigned char *t_addr)      //, unsigned char RAM)
// {
// 	BCD_ADD_N(s_addr, t_addr, 4);
// }

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
*函数说明：n个BCD码相加
*输入参数：源数据1地址dp_0, 源数据2地址dp_1，数据长度n
*输出参数：dp_0保存结果
*返    回：无
*条    件：n<=4,低字节在前
*******************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////
void BCD_ADD_N(unsigned char *dp_0, unsigned char *dp_1, unsigned char n)
{
	unsigned char i, temp;

	bit flag=0;

	for (i=0; i<n; i++)
	{
		//LSB
		temp=(*dp_0)&0x0f;
		temp=((*dp_1)&0x0f)+temp;
		if (flag)
		{
			flag=0;
			temp++;
		}
		if (temp>9)
		{
			temp-=10;
			flag=1;
		}
		(*dp_0)&=0xf0;
		(*dp_0)|=temp;
		//MSB
		temp=(((*dp_0)>>4)&0x0f);
		temp=(((*dp_1)>>4)&0x0f)+temp;
		if (flag)
		{
			flag=0;
			temp++;
		}
		if (temp>9)
		{
			temp-=10;
			flag=1;
		}
		(*dp_0)&=0x0f;
		(*dp_0)|=(temp<<4);
		dp_0++;
		dp_1++;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
*函数说明：n个BCD码相减
*输入参数：源数据1地址dp_0, 源数据2地址dp_1，数据长度n
*输出参数：dp_0保存结果
*返    回：无
*条    件：n<=4,低字节在前
*******************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////
// void BCD_MINUS_N(unsigned char *dp_0, unsigned char *dp_1, unsigned char n)
// {
// 	unsigned char i, temp0,temp1;

// 	bit flag=0;

// 	for (i=0; i<n; i++)
// 	{
// 		//LSB
// 		temp0=(*dp_0)&0x0f;//data1 lsb
// 		temp1=(*dp_1)&0x0f;//data2 lsb
// 		if(flag)//borrow flag
// 		{
// 			//被借位处理
// 			if(temp0)
// 			{
// 				flag = 0;
// 				temp0--;
// 			}
// 			else
// 			{
// 				temp0 += 9; //借位
// 				flag = 1;
// 			}
// 		}
// 		if(temp0<temp1)
// 		{
// 			flag = 1;
// 			temp0 +=10;	//借位
// 			temp0 -=temp1;
// 		}
// 		else
// 		{
// 			temp0 -=temp1;
// 		}
// 		(*dp_0)&=0xf0;
// 		(*dp_0)|=temp0;
// 		//MSB
// 		temp0=(((*dp_0)>>4)&0x0f);
// 		temp1=(((*dp_1)>>4)&0x0f);
// 		if(flag)
// 		{
// 			//被借位处理
// 			if(temp0)
// 			{
// 				flag = 0;
// 				temp0--;
// 			}
// 			else
// 			{
// 				temp0 += 9; //借位
// 				flag = 1;
// 			}
// 		}
// 		if(temp0<temp1)
// 		{
// 			flag = 1;
// 			temp0 +=10;	//借位
// 			temp0 -=temp1;
// 		}
// 		else
// 		{
// 			temp0 -=temp1;
// 		}
// 		(*dp_0)&=0x0f;
// 		(*dp_0)|=(temp0<<4);
// 		dp_0++;
// 		dp_1++;

// 	}
// }
////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
*函数说明：BCD码相加1
*输入参数：源地址s_addr,目标地址t_addr,特征字RAM
*输出参数：结果在源地址s_addr
*返    回：无
*条    件：
*******************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////
void Add1BCD(unsigned char *point, unsigned char len)
{
	unsigned char i;

	for (i=0; i<len; i++)
	{
		*(point+i)+=1;

		if ((*(point+i)&0x0f)>9)    *(point+i)+=6;

		if (*(point+i)>0x99)    *(point+i)=0;
		else    return;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
*函数说明：BCD码CHECK
*输入参数：源地址s_addr,目标地址t_addr,特征字RAM
*输出参数：结果在源地址s_addr
*返    回：无
*条    件：
*******************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////
// unsigned char DotBCD_Check(unsigned char Dot)
// {
//     return ( (unsigned char)(((Dot & 0x0f) > 0x09) || ((Dot & 0xf0) > 0x90)) );
// }