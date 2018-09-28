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
** ��������: EE_KWH_shift
** ��������: ���ݵ�������EEPROM�еĴ洢��ַ
** �������: ��
** �������:
*******************************************************************************************/
void EE_KWH_shift(void)
{
    unsigned char buff[2];
    unsigned int hex_data[2];
	unsigned char temp;

	EE_to_RAM(EE_KWH_ADDRESS,(unsigned char *)&temp,1);	//�����õ�����ַ
	temp = (temp-EE_KWH0)/EE_KWH_SHIFT;	//����ƫ������
	hex_data[0] = temp * EE_KWH_MAX;	//���㵱ǰƫ�Ƶ�ַ���õ���

    ReverseCpy(buff, &KWH[2], 2);//����λ��ȡ�õ���
    hex_data[1] = BCD2toINT(buff);	//����ʵ���õ���

	if(hex_data[1]-hex_data[0] >= EE_KWH_MAX) //�����õ����洢��ַ
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
** ��������: ALARMLED
** ��������:
** �������: ��
** �������: ��
*******************************************************************************************/
/*
void ALARMLED(void)
{
    if (g_AllDisDly == 0)
    {
        if((g_Flag.EventFlg & E_Cover) == E_Cover)       				   //�������˸��¼���LED����˸�������ȼ�
        {
            FAULT_LED = (FAULT_LED^FAULT_LED_ON);
        }
        else
        {
            #if MeterType == MT_1P2L
            	if ((g_Flag.ALARM[0] & (E_IReverse + E_Bypass)) != 0)     	//�������߱���������·״̬��LED����˸
	            {
	                FAULT_LED &= ~FAULT_LED_ON;							  	//0

	                REV_LED &= ~REV_LED_ON;                 		        //����������LED	 OFF
	            }
	            else
	            {
	                FAULT_LED |= FAULT_LED_ON;							  	//1

	                REV_LED |= REV_LED_ON;                 		        	//����������LED	 ON
	            }
            #else
            	if ((g_Flag.ALARM[0] & E_IReverse) != 0)                  	//�������߱�������״̬��LED����˸
	            {
	                FAULT_LED &= ~FAULT_LED_ON;							  	//0

	                REV_LED &= ~REV_LED_ON;                 		        //����������LED	 OFF
	            }
	            else
	            {
	                FAULT_LED |= FAULT_LED_ON;							  	//1

	                REV_LED |= REV_LED_ON;                 		        	//����������LED	 ON
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
** ��������: �����й�С�������ֵ
** ��������: ����ϵ�����
** �������: ��
** �������: ��
*******************************************************************************************/
void check_dotKWHD(void)
{
	unsigned char  xdata  ECEpBuff[2];
	unsigned char temp0;

	//EE_to_RAM(EE_KWHP,&ECEpBuff[0],2);
//    if (0 == VER_RDbytes(EE_KWHP, &ECEpBuff[0], 2)) //��У�������ֱ�ӻָ�Ĭ��ֵ
//    {
//        ECEpBuff[0] =  g_ConstPara.Threshold/2;
//        ECEpBuff[1] = 0x50;
//        VER_WRbytes(EE_KWHP, &ECEpBuff[0],2,1);//�����й�����С������
//    }
	temp0 = EEPromByteRead(0x05,0x01);//��������
	if(temp0 == 0xaa)
	{
		//EE_to_RAM(0x531, &ECEpBuff[0], 2);	//���й�����������ECEpBuff[0]�У�С������ECEpBuff[1]��
		ECEpBuff[0] = EEPromByteRead(0x05,0x31);
		ECEpBuff[1] = EEPromByteRead(0x05,0x32);
		EEPromSectorErase(0x04);// д֮ǰ����������
		EEPromByteProgram(0x04,0x31,ECEpBuff[0]);
		EEPromByteProgram(0x04,0x32,ECEpBuff[1]);
		EEPromSectorErase(0x05);// д֮ǰ����������
	}
	else
	{
		//EE_to_RAM(0x431, &ECEpBuff[0], 2);	//���й�����������ECEpBuff[0]�У�С������ECEpBuff[1]��
		ECEpBuff[0] = EEPromByteRead(0x04,0x31);
		ECEpBuff[1] = EEPromByteRead(0x04,0x32);
	}
	KWHP = ECEpBuff[0];//������
	KWHD = ECEpBuff[1];//С�������
	//С����������ܴ�������������໥У��
	KWHD1 = KWHD;
	KWHD2 = KWHD;
	KWHD3 = KWHD;
}


////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** ��������: �����й�С�������ֵ
** ��������: ����ϵ�����
** �������: ��
** �������: ��
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
** ��������: �������й�С�������ֵ��С��������໥У��
** ��������: ����ϵ�����
** �������: ��
** �������: ��
*******************************************************************************************/
void KWH_RKWH_dot(void)
{
//�����й�
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

//�����й�
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
*����˵����RAM�������й�CHECK
*�����������
*���������EE��Ӧ��ַ��ͬ����ʱˢ��,��ȷ����0������ȷ������0xff
*��    �أ���
*��    ����
*******************************************************************************************/
// unsigned char KWH_Check(void)
// {
//     unsigned char xdata rlt = 0;

//     //���RAM���Ƿ�����ȷ��С������
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
*����˵����RAM�з����й�CHECK
*�����������
*���������EE��Ӧ��ַ��ͬ����ʱˢ��
*��    �أ���
*��    ����
*******************************************************************************************/
/* unsigned char RKWH_Check(void)
{
    unsigned char xdata rlt = 0;

    //���RAM���Ƿ�����ȷ��С������
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
** ��������: �����޹�С�������ֵ
** ��������: ����ϵ�����
** �������: ��
** �������: ��
*******************************************************************************************/
//void check_dotQKWHD(void)
//{
//	unsigned char  xdata  ECEqBuff[2];

////	  EE_to_RAM(EE_QKWHP,&ECEqBuff[0],2);
////    if (0 == VER_RDbytes(EE_QKWHP, &ECEqBuff[0], 2)) //��У�������ֱ�ӻָ�Ĭ��ֵ
////    {
////        ECEqBuff[0] =  g_ConstPara.Threshold/2;
////        ECEqBuff[1] = 0x50;
////        VER_WRbytes(EE_QKWHP, &ECEqBuff[0],2,1);//�����й�����С������
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
** ��������: �����޹�С�������ֵ
** ��������: ����ϵ�����
** �������: ��
** �������: ��
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
** ��������: �������޹�С�������ֵ
** ��������: ����ϵ�����
** �������: ��
** �������: ��
*******************************************************************************************/
//  void QKWH_RQKWH_dot(void)
// {
// //�����޹�
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

// //�����޹�
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
*����˵����RAM�������޹�CHECK
*�����������
*���������EE��Ӧ��ַ��ͬ����ʱˢ��
*��    �أ���
*��    ����
*******************************************************************************************/
// unsigned char QKWH_Check(void)
// {
//     unsigned char xdata rlt = 0;

//     //���RAM���Ƿ�����ȷ��С������
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
*����˵����RAM�з����޹�CHECK
*�����������
*���������EE��Ӧ��ַ��ͬ����ʱˢ��
*��    �أ���
*��    ����
*******************************************************************************************/
// unsigned char RQKWH_Check(void)
// {
//     unsigned char xdata rlt = 0;

//     //���RAM���Ƿ�����ȷ��С������
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
*����˵�������籣��С��������������
*�����������
*���������EE��Ӧ��ַ��ͬ����ʱˢ��
*��    �أ���
*��    ����
*******************************************************************************************/
// void PluseAndDecimalProc(void)
// {
// 	unsigned char temp1[2],temp2[2];

// //�й�
// 	if ((KWHD==KWHD1) && (KWHD==KWHD2))
// 	{
// 		temp1[0] = KWHP;
// 		temp1[1] = KWHD;


// 		//EE_to_RAM(EE_KWHP,&temp2[0], 4);       //��Ƶ����������������С��λ.
//         VER_RDbytes_INT(EE_KWHP, &temp2[0], 2);      //������������С������

//         if ((temp1[0] != temp2[0]) || (temp1[1] != temp2[1]))
//         {
//             VER_WRbytes_INT(EE_KWHP, &temp1[0], 2,1);//�����й�����С������
//         }
// 	}

//}


////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** ��������: ConstPara_Chk
** ��������: ��ʼ��RAM
** �������: ��
** �������: ��
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
		g_ConstPara.Chk       = ConstPara_Chk();//���У��
	}


    /* if ((adjust_data.icont != g_ConstPara.Icont) && (Cmd != 0xA5))
    {
        //icont�����仯����Ҫ��У����������޸�
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
** ��������: ConstPara_Chk
** ��������: ��ʼ��RAM
** �������: ��
** �������: ��
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
	RSTSTAT &= Bin(11111000);	//�忴�Ź�
	if(ATcmd_Transmit(dest,"OK",1000))//ʧ�ܣ�����NBģ��
	{
		flag[0] = 0xAA;//�ж�ΪLORAģ��
		flag[1] = 0xAA;
	}
	else				//�ж�ΪNBģ��
	{
		flag[0] = 0xBB;
		flag[1] = 0xBB;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** ��������: Init_RAM
** ��������: ��ʼ��RAM
** �������: ��
** �������: ��
*******************************************************************************************/
void Init_RAM(void)
{
	//uint8 temp[6];
    //Note:
    //ReadEMUFromTOEeprom()��Ҫ��ConstPara_Init()֮��ConstPara_Init()����дadjust_data�������µ���ReadEMUFromTOEeprom()�ļ�鲻�����ã�
    //check_dot&check_dotRKWHD����������ConstPara_Init����֮��ֻ����g_ConstPara.Threshold��ȷ����ܱ�֤���塢С��������ʼ������;
    meter_const = DefaultThreshold;					//���峣��Ϊ1200
	//�������峣����ICONT�Ĵ�����ֵ
    ConstPara_Init(meter_const, 0xA5);
	//У��У�������ȷ��񣬲���ȷ�ָ�Ĭ��ֵ
    ReadEMUFromTOEeprom();
    //��EEPROM�ж�ȡ�й����ܣ���������С��������
	ReadMeterFromTOEeprom();
    //��EEPROM�ж�ȡ�й����ܣ��������֣�5�ֽ�
	EE_to_RAM(EE_KWH_ADDRESS,(unsigned char *)&EE_KHH_address,1);	//�����õ�����ַ
	//�����õ���EEPROM�洢��ַ
	VER_RDbytes((unsigned int)EE_KHH_address, &kwh_value.integer[0], 5);
	//��������й���������������С���㲿����ϵ�һ��
	ECRunKwh();                                     //��������й�
	//��ȡ���
	//EE_to_RAM(EE_Meter_address, &param_data.meter_address[0], 6);
	VER_RDbytes(EE_Meter_address, &param_data.meter_address[0], 6);
	//��Ų���BCD�룬�ָ�Ĭ��
	if(!BCDCheck(&param_data.meter_address[0], 6))
	{
		MemInitSet(&param_data.meter_address[0],0x00, 6);
		VER_WRbytes(EE_Meter_address, &param_data.meter_address[0], 6, 1);
	}
	//����Ĭ�ϱ��
	MemInitSet(&param_data.meter_address_def[0],0x99, 6);
	//��ȡ���̴���
	EE_to_RAM(EE_Meter_Factory, &param_data.meter_factory[0],1);
	//���̴��벻��BCD�룬�ָ�Ĭ��
//	if(!BCDCheck(&param_data.meter_factory[0], 1))
//	{
//		MemInitSet(&param_data.meter_factory[0],0x00, 1);
//		SEQ_write(EE_Meter_Factory, &param_data.meter_factory[0], 1);
//	}
    //���EEPROM�����𶯴����Ƿ���ȷ����������ָ�Ϊ0����ʱ����Ҫ
    // if (0 == VER_RDbytes(CNT_RST, &temp[0], 1))
    // {
    //     temp[0] = 0x00;
    //     VER_WRbytes(CNT_RST, &temp[0], 1, 1);
    // }

    //���EEPROM�ж�ȡ����ʱ�������Ƿ���ȷ����������ָ�Ĭ��
	//����ʱ�䣿�Զ��������ڣ���
/*
	if (0==VER_RDbytes(EE_SAM_ENDDATE,&temp[0],2))
	{
        temp[0]=0x01;
        temp[1]=0x00;
        VER_WRbytes(EE_SAM_ENDDATE,&temp[0],2,1);
	}
*/

    //���EEPROM���״��ϵ��־���״��ϵ��Զ����빤��ģʽ,����У�����
	//���EEPROM�е�EE_FirstProg_FLAG��һ���ϵ��־��Ϊ0xA5(4byte)
//	//��gbFgKeyProg = 0xF001������У�����
// MemInitSet(&g_Buffer[0], 0x00, 4);
// VER_WRbytes(EE_FirstProg_FLAG,&g_Buffer[0],4, 1);
	EE_to_RAM(EE_FirstProg_FLAG, &g_Buffer[0], 4);
    if ((g_Buffer[0] != 0xA5) && (g_Buffer[1] != 0xA5) && (g_Buffer[2] != 0xA5) && (g_Buffer[3] != 0xA5))
    {
		gbFgKeyProg = 0xF001;
		SEQ_write(EE_PROG_FLAG, (uint8*)&gbFgKeyProg, 2);//����У�����Flag
		//�˴���У������������FLGA
        //MemInitSet(&g_Buffer[0], 0x00, 4);
       //SEQ_write(EE_FirstProg_FLAG, &g_Buffer[0], 4);
    }
	else
	{
		gbFgKeyProg = 0x0000;
		SEQ_write(EE_PROG_FLAG, (uint8*)&gbFgKeyProg, 2);//����У�����Flag
	}
	EE_to_RAM(EE_PROG_FLAG, (uint8*)&gbFgKeyProg, 2);

    //���EEPROMʱ������������У����ȷ����������������
	Init_RTCAdjustProc();//��ʹ��RTCʱ�Ӳ���

	//CurChExDly = C_DefaultCurExTT;                                          //Init I1&I2 exchange value
	//BlinkFlg = 0;                                                           //LCD fresh in factory mode enable flag
    //gbbattery=0;                                                            //Battery error flag

	gbPCNTCount=0; //Ǳ��ʱ���־��0                                                          //Start & Creep timer
	MemInitSet((unsigned char xdata *)&g_FilterV.Buf[0], 0x00, sizeof(g_FilterV));		//Filter Buffer
	MemInitSet(&g_Cal.Step, 0x00, sizeof(g_Cal));		 								//Calibrate State
	MemInitSet(&g_Flag.Run, 0x00, sizeof(g_Flag));                                       //Init System Flag
	MemInitSet(&_UINT8_RTC[0],0x00,7);                                                   //Init RTC buffer

	//��װ��ģ��FLAG��ʼ��
	//NB_LORA_PANDUAN(&NB_LORA[0]);
	MemInitSet(&NB_LORA[0],0x00,2);
	VER_WRbytes(EE_NB_LORA, &NB_LORA[0], 2, 1);
	//�̵�����س�ʼ��
	//DelayStatus[0] = 0x11;
	//DelayStatus[1] = 0x11;
	VER_RDbytes(RELAY_STATUS, &DelayStatus[0], 2);
	if(gbFgKeyProg == 0x0000)//��ǰ�ڷǹ���ģʽ�����Բ����̵���
	{
	if((DelayStatus[0] != 0xA5)&&(DelayStatus[1] != 0xA5))
	{
		DelayFlag = 1;//�̵����ϵ�
	}
	else
	{
		DelayFlag = 0;//�̵���ͨ��
	}
	}

	//Э�����
	g_Tran.PwoerOn = 1; //�ϵ�����
	g_Tran.AutoReportTime = 0;	//�Զ��ϱ����Դ���
	g_Tran.AutoReportCount = NB_POWER_ON;//�Զ��ϱ�ʱ��&ͨ��ʱ���ʼ��
	g_Tran.AutoReportFlag = 0;	//�Զ��ϱ�FLAG

	g_NB.UARTReceiveOK = 0;			//NB����UART�������FLAG
	g_NB.DataReceiveOK = 0;		//
	//ģ����س�ʼ��
	g_NB.InitCount = 0;			//��ʼ������
	g_NB.ReInitTime = 1;		//��ʼ�������ʱ��
	g_NB.InitStep = 1;			//��ʼ�����ӵ�һ����ʼ
	MemInitSet(&g_NB.InitState[0],NB_Init_NOT_OK,2);
	VER_WRbytes(EE_NB_STATE, &g_NB.InitState[0], 2, 1);

	//��ʾ��س�ʼ��
	g_Flag.ALARM[0] = 0X00; //��������
	gbFgShowItemNum = D_ShowItemNum;//��ʾ��Ŀ����,Ĭ���ǵ�һ���ǰ�ܵ���
    g_Flag.Run |= F_DISP;
	g_Disp.PollingDisplayPtr = 0;		  							//Ĭ����ʾ��һ�����й�������
    //����ʱ����
    g_Disp.PollingDisplayCountSet = D_PollingDisplayTime;			//Ĭ����ʾ���Ϊ5s
    g_Disp.PollingDisplayCount = D_PollingDisplayTime;			//Ĭ����ʾ���Ϊ5s

	//У�������ʼ��
	gBAdjKeyCount = 0;
	gBAdjKeyEnable = 0x00;
	gBAdjKeyEnable1 = 0x00;

	// gBAdjKeyNew = 0;
	// gBAdjKeyOld = 0;
	gBAdjKeyStatus = 0;						//У�����߶Ͽ��˳�
	gbUartAdjust = 0;	//UARTͨѶУ��������־
	ADJ_ID = 0;		//UartУ��������ָʾ
	Sign_Bit = 0;	//Uart������λ
	Adj_Error = 0; //Uart���XX.XX%


	//gBClsKeyCount = 0;
	//gBClsKeyEnable = 0x00;
	//gBClsKeyEnable1 = 0x00;
	//g_Cal.ClsKeyStep = 0;					//��һ����Ч��־
	//gbFgClsKeyProg = 0x0000;

	AverPower = 0;
	AdjTempValueEverySec = 0;
	gbAverTimes = 0;
	gbAdjShowOk2S = 0;
	gbAdjStepFlag = 0;
	gbAdjCurrentStep = 0;

	//�ж����ȼ�
    IPL0 = Bin(00000000);
    IPH0 = Bin(00000000);
    IPL1 = Bin(10000000);
    IPH1 = Bin(10000001);
}


////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** ��������:  ReadMeterFromTOEeprom
** ��������:
** �������: ��
** �������: ��
** ��ȡ�й�,�޹�����С������
*******************************************************************************************/
void ReadMeterFromTOEeprom(void)
{
    check_dotKWHD();		//�����й�������,С�������
    //check_dotRKWHD();		//���򣬲���Ҫ
    //KWH_RKWH_dot();			//�й�С�������У��

    //check_dotQKWHD();		//�޹��������ݣ�����Ҫ
    //check_dotRQKWHD();	//�޹��������ݣ�����Ҫ
    //QKWH_RQKWH_dot();		//�޹��������ݣ�����Ҫ
}





////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** ��������: ECRunKwh
** ��������: ��ʼ��RAM
** �������: ��
** �������: ��
*******************************************************************************************/
void ECRunKwh (void)                				//�����й�
{
	KWH_PN[0] = KWHD;               				//С��λ
	KWH_PN[1] = kwh_value.integer[0];
	KWH_PN[2] = kwh_value.integer[1];
	KWH_PN[3] = kwh_value.integer[2];
	KWH_PN[4] = kwh_value.integer[3];
	KWH_PN[5] = kwh_value.integer[4];

	//A_energy(&KWH_PN[0], &RKWHD);	//�������й��ۼ�������й���
}


////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** ��������: ECRunQKwh
** ��������: ��ʼ��RAM
** �������: ��
** �������: ��
*******************************************************************************************/
/* void ECRunQKwh (void)                				//���㵱���޹�
{
	QKWH_PN[0] = QKWHD;               				//С��λ
	QKWH_PN[1] = QKWH[0];
	QKWH_PN[2] = QKWH[1];
	QKWH_PN[3] = QKWH[2];
	//A_energy(&QKWH_PN[0], &RQKWHD);
} */

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** ��������: ECRunQKwh_RQKwh
** ��������: ��ʼ��RAM
** �������: ��
** �������: ��
*******************************************************************************************/
/* void ECRunQKwh_RQKwh(void)
{
 	QKWH_Inductive[0] = 0x00;						//�����޹������
 	QKWH_Inductive[1] = 0x00;
 	QKWH_Inductive[2] = 0x00;
 	QKWH_Inductive[3] = 0x00;
	A_energy(&QKWH_Inductive[0], &QKWHD);

	RQKWH_Capactive[0] = 0x00;						//�����޹�������
	RQKWH_Capactive[1] = 0x00;
	RQKWH_Capactive[2] = 0x00;
	RQKWH_Capactive[3] = 0x00;
	A_energy(&RQKWH_Capactive[0], &RQKWHD);
} */

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** ��������: EnergyProc
** ��������: ��������
** �������: ��
** �������: ��
*******************************************************************************************/
void EnergyProc(void)
{

//�����й��ۼ�
	if (KWHP>=g_ConstPara.Threshold)  					//�������й�����
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
			//�й������ۼ�,�������ִ���EEPROM��
			EE_KWH_shift();
			E_operating((unsigned int)EE_KHH_address, &kwh_value.integer[0], &KWHD, 6);   //��������
			KWHD1 = KWHD;
			KWHD2 = KWHD;
			KWHD3 = KWHD;
		}
	}
//���þ���ֵ�ۼӣ�������Ҫ
//�����й��ۼ�
/*
	if (RKWHP>=g_ConstPara.Threshold) 					//�������й�����
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

			E_operating(EE_RKWH0, &RKWH[0], &RKWHD, 3);  //��������

			RKWHD1 = RKWHD;
			RKWHD2 = RKWHD;
			RKWHD3 = RKWHD;
		}
	}
*/
//�޹�����Ҫ
/*
//�����޹��ۼ�
	if (QKWHP>=g_ConstPara.Threshold)  					//�������޹�����
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

			Q_operating(EE_QKWH0, &QKWH[0], &QKWHD, 3);   //��������

			QKWHD1 = QKWHD;
			QKWHD2 = QKWHD;
			QKWHD3 = QKWHD;
		}
	}

//�����޹��ۼ�
	if (RQKWHP>=g_ConstPara.Threshold) 					//�������޹�����
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

			Q_operating(EE_RQKWH0, &RQKWH[0], &RQKWHD, 3);  //��������

			RQKWHD1 = RQKWHD;
			RQKWHD2 = RQKWHD;
			RQKWHD3 = RQKWHD;
		}
	}
*/
}


////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
*����˵��:��������
*�������:EEPROM��ַEE_addr,����IRAM��ַ,С��DRAM,����RAM����len
*�������:LCD_b
*��    ��:��
*��    ��:��Ӧ�׵�ַ
*******************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////
void E_operating(unsigned int EE_addr ,unsigned char *IRAM, unsigned char *DRAM, unsigned char len)
{
	//unsigned char r;

	Add1BCD(DRAM, 1);
	//����С�����λ������λ��һ
	if (!(*DRAM))								 //�п�����С���������1�ȡ�1.25==>2.25
	{
		//��EEPROM�ж���������IRAM��
		VER_RDbytes(EE_addr, IRAM, len);
		//r=VER_RDbytes(EE_addr, IRAM, len);

		Add1BCD(IRAM, 5);      					//���������ۼ�
		_nop_();
		//��ˢ�º�ĵ�������EEPROM��
		VER_WRbytes(EE_addr, IRAM, len, 1);
		//VER_WRbytes(EE_addr, IRAM, len, r);
		ECRunKwh();            					//ˢ������й�
	}
}


////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
*����˵��:��������
*�������:EEPROM��ַEE_addr,����IRAM��ַ,С��DRAM,����RAM����len
*�������:LCD_b
*��    ��:��
*��    ��:��Ӧ�׵�ַ
*******************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////
// void Q_operating(unsigned int EE_addr, unsigned char *IRAM, unsigned char *DRAM, unsigned char len)
// {
// 	unsigned char r;

// 	Add1BCD(DRAM, 1);

// 	if (!(*DRAM))								//�п�����С���������1�ȡ�1.25==>2.25
// 	{
// 		r=VER_RDbytes(EE_addr, IRAM, len);

// 		Add1BCD(IRAM, 3);      					//���������ۼ�
// 		_nop_();
// 		VER_WRbytes(EE_addr, IRAM, len, r);
// //        ECRunQKwh(); 							//ˢ������޹�
// //        ECRunQKwh_RQKwh();
// 	}
// }



////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
*����˵�����й���ϵ�������
*���������Դ��ַs_addr,Ŀ���ַt_addr,������RAM
*��������������Դ��ַs_addr
*��    �أ���
*��    ����
*******************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////
// void A_energy(unsigned char *s_addr, unsigned char *t_addr)      //, unsigned char RAM)
// {
// 	BCD_ADD_N(s_addr, t_addr, 4);
// }

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
*����˵����n��BCD�����
*���������Դ����1��ַdp_0, Դ����2��ַdp_1�����ݳ���n
*���������dp_0������
*��    �أ���
*��    ����n<=4,���ֽ���ǰ
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
*����˵����n��BCD�����
*���������Դ����1��ַdp_0, Դ����2��ַdp_1�����ݳ���n
*���������dp_0������
*��    �أ���
*��    ����n<=4,���ֽ���ǰ
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
// 			//����λ����
// 			if(temp0)
// 			{
// 				flag = 0;
// 				temp0--;
// 			}
// 			else
// 			{
// 				temp0 += 9; //��λ
// 				flag = 1;
// 			}
// 		}
// 		if(temp0<temp1)
// 		{
// 			flag = 1;
// 			temp0 +=10;	//��λ
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
// 			//����λ����
// 			if(temp0)
// 			{
// 				flag = 0;
// 				temp0--;
// 			}
// 			else
// 			{
// 				temp0 += 9; //��λ
// 				flag = 1;
// 			}
// 		}
// 		if(temp0<temp1)
// 		{
// 			flag = 1;
// 			temp0 +=10;	//��λ
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
*����˵����BCD�����1
*���������Դ��ַs_addr,Ŀ���ַt_addr,������RAM
*��������������Դ��ַs_addr
*��    �أ���
*��    ����
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
*����˵����BCD��CHECK
*���������Դ��ַs_addr,Ŀ���ַt_addr,������RAM
*��������������Դ��ַs_addr
*��    �أ���
*��    ����
*******************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////
// unsigned char DotBCD_Check(unsigned char Dot)
// {
//     return ( (unsigned char)(((Dot & 0x0f) > 0x09) || ((Dot & 0xf0) > 0x90)) );
// }