/*==============================================================================
                Copyright(C) 1997-2019.  Sinowealth Tech. Co., Ltd.
--------------------------------------------------------------------------------
Project Name  : PIMA.c
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
#ifndef  COMMU_C
	#define  COMMU_C
#endif
//-------------------------------------------------------------------------------

//---------------

#include "config.h"
#include "Headconfig.h"

M_Transimt g_Tran;
/****************************************************************************
** ��������: ProtocolReport
** ��������: Э��������װ����  ����
** �������: DataBuildҪ���͵�����
** �������: Ҫ���͵����ݳ���
*****************************************************************************/
unsigned char ProtocolReport(unsigned char *DataBuild, unsigned char item)
{
    unsigned char buf[6];
	unsigned char protocol_len;
    //֡ͷ
	buf[0] = NB_FRAME_HEAD_1;
	buf[1] = NB_FRAME_HEAD_2;
	memcpy (&DataBuild[0],&buf[0],NB_FRAME_HEAD_NUM);
	protocol_len = NB_FRAME_HEAD_NUM;//2
	//���
	MemInitSet(&buf[0],0x00,6);//buff����
	memcpy(&buf[0],&param_data.meter_address[0],6);
	memcpy(&DataBuild[protocol_len], &buf[0], 6);
	protocol_len +=6;
	//���Ҵ���
	buf[0] = param_data.meter_factory[0];//
	memcpy(&DataBuild[protocol_len], &buf[0], 1);
	protocol_len ++;
	//������
    buf[0]= item;
    memcpy(&DataBuild[protocol_len], &buf[0], 1);
	protocol_len +=2;//�ճ����ݳ���
	switch(item)
    {
        case NB_PROTOCOL_PowerON :
        {
            break;
        }
        case NB_PROTOCOL_JICHAO:
        {
            //��������
            //�õ���
            MemInitSet(&buf[0],0x00,6);//buff����
            memcpy(&buf[0],&KWH_PN[0],6);
            memcpy(&DataBuild[protocol_len], &buf[0], 6);
            protocol_len +=6;
            //��ѹ
            MemInitSet(&buf[0],0x00,4);
            ReverseCpy(&buf[0],&g_InsBCD.Voltage[0],3);
            memcpy(&DataBuild[protocol_len], &buf[0], 4);
            protocol_len += 4;
            //����
            MemInitSet(&buf[0],0x00,4);
            ReverseCpy(&buf[0],&g_InsBCD.Current[0],3);
            memcpy(&DataBuild[protocol_len], &buf[0], 4);
            protocol_len += 4;
            //Ƶ��
            MemInitSet(&buf[0],0x00,4);
            ReverseCpy(&buf[0],&g_InsBCD.Frequence[0],2);
            memcpy(&DataBuild[protocol_len], &buf[0], 4);
            protocol_len += 4;
            //�й�����
            MemInitSet(&buf[0],0x00,4);
            ReverseCpy(&buf[0],&g_InsBCD.Power[0],4);
            memcpy(&DataBuild[protocol_len],&buf[0],4);
            protocol_len += 4;
            //�޹�����
            MemInitSet(&buf[0],0x00,4);
            ReverseCpy(&buf[0],&g_InsBCD.RPower[0],4);
            memcpy(&DataBuild[protocol_len], &buf[0], 4);
            protocol_len += 4;
            //���ڹ���
            MemInitSet(&buf[0],0x00,4);
            ReverseCpy(&buf[0],&g_InsBCD.APower[0],4);
            memcpy(&DataBuild[protocol_len], &buf[0], 4);
            protocol_len += 4;
            //��������
            MemInitSet(&buf[0],0x00,4);
            ReverseCpy(&buf[0],&g_InsBCD.PowerFactor[0],2);
            memcpy(&DataBuild[protocol_len], &buf[0], 4);
            protocol_len += 4;
            //����
            MemInitSet(&buf[0],0x00,6);
            memcpy(&buf[0],&_UINT8_RTC[0],6);
            memcpy(&DataBuild[protocol_len], &buf[0], 6);
            protocol_len += 6;
            break;
        }
        default :break;
    }
    //���ݳ���
    DataBuild[2+6+1+1] = HEX2BCD(protocol_len -2-6-1-1-1);//��ȥ֡ͷ������
    //У��λ
	DataBuild[protocol_len] = UARTcmdCheck(&DataBuild[0], protocol_len);
	protocol_len++;
	//������
	DataBuild[protocol_len] = 0X16;
	protocol_len++;
	return protocol_len;
}
/****************************************************************************
** ��������: ProtocolBuild
** ��������: Э��������װ����
** �������: itemҪ��װ����Ŀ 	DataBuildҪ���͵�����
** �������: �������ݳ���
*****************************************************************************/
unsigned char ProtocolBuild(unsigned char item, unsigned char *DataBuild)
{
	unsigned char buf[6];
	unsigned char protocol_len;
	//֡ͷ
	buf[0] = NB_FRAME_HEAD_1;
	buf[1] = NB_FRAME_HEAD_2;
	memcpy (&DataBuild[0],&buf[0],NB_FRAME_HEAD_NUM);
	protocol_len = NB_FRAME_HEAD_NUM;
	//���
	MemInitSet(&buf[0],0x00,6);//buff����
	memcpy(&buf[0],&param_data.meter_address[0],6);
	memcpy(&DataBuild[protocol_len], &buf[0], 6);
	protocol_len +=6;
	//���Ҵ���
	buf[0] = param_data.meter_factory[0];//
	memcpy(&DataBuild[protocol_len], &buf[0], 1);
	protocol_len ++;
	//�����ֽ�
	if(((item>>4)&0x0F) == 0x01)
	{
		buf[0] = item&0x0F;//������Ӧ
		buf[0] += 0x80;
	}
	else if(((item>>4)&0x0F) == 0x02)
	{
		buf[0] = item&0x0F;//������Ӧ
		buf[0] += 0x80;
	}
	else
	{
		buf[0] = 0x00;
	}
	memcpy(&DataBuild[protocol_len], &buf[0], 1);
	protocol_len ++;
	//��������
	switch(item)
	{
		case NB_PROTOCOL_kWh://�õ���
		{
			//���ݳ���
			buf[0] = 6;
			DataBuild[protocol_len] = buf[0];
			protocol_len++;
			MemInitSet(&buf[0], 0x00, 6); //buff����
			memcpy(&buf[0],&KWH_PN[0],6);
			memcpy(&DataBuild[protocol_len], &buf[0], 6);
			protocol_len+=6;
			break;
		}
		case NB_PROTOCOL_V://��ѹ
		{
			buf[0] = NB_FRAME_DATA_NUM;
			DataBuild[protocol_len] = buf[0];
			protocol_len++;
			MemInitSet(&buf[0],0x00,4);
			ReverseCpy(&buf[0],&g_InsBCD.Voltage[0],3);
			//memcpy(&buf[0],&g_InsBCD.Voltage[0],3);
			memcpy(&DataBuild[protocol_len], &buf[0], 4);
			protocol_len += NB_FRAME_DATA_NUM;
			break;
		}
		case NB_PROTOCOL_I://����
		{
			buf[0] = NB_FRAME_DATA_NUM;
			DataBuild[protocol_len] = buf[0];
			protocol_len++;
			MemInitSet(&buf[0],0x00,4);
			ReverseCpy(&buf[0],&g_InsBCD.Current[0],3);
			//memcpy(&buf[0],&g_InsBCD.Current[0],3);
			memcpy(&DataBuild[protocol_len], &buf[0], 4);
			protocol_len += NB_FRAME_DATA_NUM;
			break;
		}
		case NB_PROTOCOL_Hz://Ƶ��
		{
			buf[0] = NB_FRAME_DATA_NUM;
			DataBuild[protocol_len] = buf[0];
			protocol_len++;
			MemInitSet(&buf[0],0x00,4);
			ReverseCpy(&buf[0],&g_InsBCD.Frequence[0],2);
			//memcpy(&buf[0],&g_InsBCD.Frequence[0],2);
			memcpy(&DataBuild[protocol_len], &buf[0], 4);
			protocol_len += NB_FRAME_DATA_NUM;
			break;
		}
		case NB_PROTOCOL_Power://�й�����
		{
			buf[0] = NB_FRAME_DATA_NUM;
			DataBuild[protocol_len] = buf[0];
			protocol_len++;
			MemInitSet(&buf[0],0x00,4);
			ReverseCpy(&buf[0],&g_InsBCD.Power[0],4);
			//memcpy(&buf[0],&g_InsBCD.Power[0],4);
			memcpy(&DataBuild[protocol_len], &buf[0], 4);
			protocol_len += NB_FRAME_DATA_NUM;
			break;
		}
		case NB_PROTOCOL_rPower://�޹�����
		{
			buf[0] = NB_FRAME_DATA_NUM;
			DataBuild[protocol_len] = buf[0];
			protocol_len++;
			MemInitSet(&buf[0],0x00,4);
			ReverseCpy(&buf[0],&g_InsBCD.RPower[0],4);
			//memcpy(&buf[0],&g_InsBCD.RPower[0],4);
			memcpy(&DataBuild[protocol_len], &buf[0], 4);
			protocol_len += NB_FRAME_DATA_NUM;
			break;
		}
		case NB_PROTOCOL_aPower://���ڹ���
		{
			buf[0] = NB_FRAME_DATA_NUM;
			DataBuild[protocol_len] = buf[0];
			protocol_len++;
			MemInitSet(&buf[0],0x00,4);
			ReverseCpy(&buf[0],&g_InsBCD.APower[0],4);
			//memcpy(&buf[0],&g_InsBCD.APower[0],4);
			memcpy(&DataBuild[protocol_len], &buf[0], 4);
			protocol_len += NB_FRAME_DATA_NUM;
			break;
		}
		case NB_PROTOCOL_FactorP://��������
		{
			buf[0] = NB_FRAME_DATA_NUM;
			DataBuild[protocol_len] = buf[0];
			protocol_len++;
			MemInitSet(&buf[0],0x00,4);
			ReverseCpy(&buf[0],&g_InsBCD.PowerFactor[0],2);
			//memcpy(&buf[0],&g_InsBCD.PowerFactor[0],2);
			memcpy(&DataBuild[protocol_len], &buf[0], 4);
			protocol_len += NB_FRAME_DATA_NUM;
			break;
		}
		case NB_PROTOCOL_Date://����
		{
			buf[0] = 6;
			DataBuild[protocol_len] = buf[0];
			protocol_len++;
			MemInitSet(&buf[0],0x00,6);//buff����
			memcpy(&buf[0],&_UINT8_RTC[0],6);
			memcpy(&DataBuild[protocol_len], &buf[0], 6);
			protocol_len += 6;
			break;
		}

		case NB_PROTOCOL_RelayStatus://�̵���״̬
		{
			buf[0] = 2;
			DataBuild[protocol_len] = buf[0];
			protocol_len++;
			MemInitSet(&buf[0],0x00,2);//buff����
			memcpy(&buf[0],&DelayStatus[0],2);
			memcpy(&DataBuild[protocol_len], &buf[0], 2);
			protocol_len += 2;
			break;
		}
		case NB_PROTOCOL_Timeservice: //�㲥��ʱ
		{
			protocol_len = 0x00;
			break;
		}
		case NB_PROTOCOL_kWh_rst:	//�õ�������
		{
			buf[0] = 0x00;
			DataBuild[protocol_len] = buf[0];
			protocol_len++;
			break;
		}
		case NB_PROTOCOL_RelayCtrl:	//�̵�������
		{
			buf[0] = 0x00;
			DataBuild[protocol_len] = buf[0];
			protocol_len++;
			break;
		}
		case NB_PROTOCOL_FactoryID:	//�޸ĳ��̱�ʶ
		{
			buf[0] = 0x00;
			DataBuild[protocol_len] = buf[0];
			protocol_len++;
			break;
		}
		default:
			break;
	}
	//У��λ
	DataBuild[protocol_len] = UARTcmdCheck(&DataBuild[0], protocol_len);
	protocol_len++;
	//������
	DataBuild[protocol_len] = 0X16;
	protocol_len++;
	return protocol_len;
}
/****************************************************************************
** ��������: Commu_Return_Error
** ��������: Э����󷵻غ���
** �������: itemҪ��װ����Ŀ
** �������: �������ݳ���
*****************************************************************************/
unsigned char Commu_Return_Error(unsigned char *DataBuild, unsigned char item)
{
	unsigned char buf[6];
	unsigned char protocol_len;
	//֡ͷ
	buf[0] = NB_FRAME_HEAD_1;
	buf[1] = NB_FRAME_HEAD_2;
	memcpy (&DataBuild[0],&buf[0],NB_FRAME_HEAD_NUM);
	protocol_len = NB_FRAME_HEAD_NUM;
	//���
	MemInitSet(&buf[0],0x00,6);//buff����
	memcpy(&buf[0],&param_data.meter_address[0],6);
	memcpy(&DataBuild[protocol_len], &buf[0], 6);
	protocol_len +=6;
	//���Ҵ���
	buf[0] = param_data.meter_factory[0];//
	memcpy(&DataBuild[protocol_len], &buf[0], 1);
	protocol_len ++;
	//�����ֽ�,�쳣��Ӧ
	buf[0] = item &0x0F;
	buf[0] += 0xD0;
	memcpy(&DataBuild[protocol_len], &buf[0], 1);
	protocol_len ++;
	//���ݳ���
	buf[0] = 0x00;
	memcpy(&DataBuild[protocol_len], &buf[0], 1);
	protocol_len ++;
	//У��λ
	DataBuild[protocol_len] = UARTcmdCheck(&DataBuild[0], protocol_len);
	protocol_len++;
	//������
	DataBuild[protocol_len] = 0X16;
	protocol_len++;
	return protocol_len;
}
/****************************************************************************
** ��������: Comm_Recive_Process
** ��������: Э����մ�����(��Ӧ��̨����)
** �������: DataRecieve���յ������� 	len�������ݵĳ���
** �������: �������ݳ���
*****************************************************************************/
unsigned char Commu_Recive_Process(unsigned char *DataRecieve, unsigned char *DataBuild,unsigned char len)
{
	unsigned char len_trasmit;
	unsigned char data_len;
	unsigned char Bufftemp[6];
	//�жϱ��(Ĭ�ϱ��),���Ǳ�Ų���Ӧ
	if(!(CmpBUFF(&DataRecieve[2],&param_data.meter_address_def[0],6)||CmpBUFF(&DataRecieve[2],&param_data.meter_address[0],6)))
	{
		len_trasmit = 0;
		return	len_trasmit;
	}

	//���ֹͣλ�������ϣ����󷵻�
	if(DataRecieve[len-1] != 0x16)
	{
		len_trasmit = Commu_Return_Error(&DataBuild[0],DataRecieve[9]);
		return	len_trasmit;
	}
	//���У��λ,�����ϣ����󷵻�
	if(DataRecieve[len-2] != UARTcmdCheck(DataRecieve,len-2))
	{
		len_trasmit = Commu_Return_Error(&DataBuild[0],DataRecieve[9]);
		return	len_trasmit;
	}
	//���֡ͷ,�����ϣ����󷵻�
	memcpy(&Bufftemp[0],&DataRecieve[0],NB_FRAME_HEAD_NUM);
	if(Bufftemp[0] != NB_FRAME_HEAD_1)
	{
		len_trasmit = Commu_Return_Error(&DataBuild[0],DataRecieve[9]);
		return len_trasmit;
	}
	if(Bufftemp[1] != NB_FRAME_HEAD_2)
	{
		len_trasmit = Commu_Return_Error(&DataBuild[0],DataRecieve[9]);
		return len_trasmit;
	}
	//Ѱ�������򳤶�
	data_len = DataRecieve[10];
	//�ж���������
	switch(DataRecieve[9])
	{
		case NB_PROTOCOL_kWh://�õ���
		{
			len_trasmit = ProtocolBuild(NB_PROTOCOL_kWh, DataBuild);		//�����ϱ�����
			//NBdata_Transmit(DataBuild,len,"OK",1000);	//�����ϱ�����
			break;
		}
		case NB_PROTOCOL_V://��ѹ
		{
			len_trasmit = ProtocolBuild(NB_PROTOCOL_V, DataBuild);   //�����ϱ�����
			//NBdata_Transmit(DataBuild, len, "OK", 1000);	   //�����ϱ�����
			break;
		}
		case NB_PROTOCOL_I://����
		{
			len_trasmit = ProtocolBuild(NB_PROTOCOL_I, DataBuild);   //�����ϱ�����
			//NBdata_Transmit(DataBuild, len, "OK", 1000);	   //�����ϱ�����
			break;
		}
		case NB_PROTOCOL_Hz://Ƶ��
		{
			len_trasmit = ProtocolBuild(NB_PROTOCOL_Hz, DataBuild);  //�����ϱ�����
			//NBdata_Transmit(DataBuild, len, "OK", 1000);	   //�����ϱ�����
			break;
		}
		case NB_PROTOCOL_Power://�й�����
		{
			len_trasmit = ProtocolBuild(NB_PROTOCOL_Power, DataBuild); //�����ϱ�����
			//NBdata_Transmit(DataBuild, len, "OK", 1000);	   //�����ϱ�����
			break;
		}
		case NB_PROTOCOL_rPower://�޹�����
		{
			len_trasmit = ProtocolBuild(NB_PROTOCOL_rPower, DataBuild); //�����ϱ�����
			//NBdata_Transmit(DataBuild, len, "OK", 1000);	   //�����ϱ�����
			break;
		}
		case NB_PROTOCOL_aPower://���ڹ���
		{
			len_trasmit = ProtocolBuild(NB_PROTOCOL_aPower, DataBuild); //�����ϱ�����
			//NBdata_Transmit(DataBuild, len, "OK", 1000);	   //�����ϱ�����
			break;
		}
		case NB_PROTOCOL_FactorP://��������
		{
			len_trasmit = ProtocolBuild(NB_PROTOCOL_FactorP, DataBuild); //�����ϱ�����
			//NBdata_Transmit(DataBuild, len, "OK", 1000);	   //�����ϱ�����
			break;
		}
		case NB_PROTOCOL_Date://����
		{
			len_trasmit = ProtocolBuild(NB_PROTOCOL_Date, DataBuild); //�����ϱ�����
			//NBdata_Transmit(DataBuild, len, "OK", 1000);	   //�����ϱ�����
			break;
		}
		case NB_PROTOCOL_RelayStatus://�̵���״̬
		{
			len_trasmit = ProtocolBuild(NB_PROTOCOL_RelayStatus,DataBuild);

			break;
		}
		case NB_PROTOCOL_Timeservice: //�㲥��ʱ
		{
			memcpy(&_UINT8_RTC[0],&DataRecieve[11],6);//д��ʱ��
			len_trasmit = ProtocolBuild(NB_PROTOCOL_Timeservice, DataBuild);
			len_trasmit = 0;

			break;
		}
		case NB_PROTOCOL_kWh_rst:	//�õ�������
		{

			memcpy(&KWH_PN[0],&DataRecieve[11],6);//����
			KWHD = DataRecieve[11];
			memcpy(&kwh_value.integer[0],&DataRecieve[12],5);
			VER_WRbytes(EE_KWH0, &kwh_value.integer[0], 5, 1);
			len_trasmit = ProtocolBuild(NB_PROTOCOL_kWh_rst, DataBuild);
			break;
		}
		case NB_PROTOCOL_RelayCtrl:	//�̵�������
		{
			if(DataRecieve[11]==0x10)//ͨ��
			{
				RealyCtrl(0);
			}
			else if(DataRecieve[11]==0x20)//�ϵ�
			{
				RealyCtrl(1);
			}
			else if(DataRecieve[11]==0x30)//�ϵ�5S
			{
				RealyCtrl(2);
			}
			len_trasmit = ProtocolBuild(NB_PROTOCOL_RelayCtrl, DataBuild);
			break;
		}
		case NB_PROTOCOL_FactoryID:	//�޸ĳ��̱�ʶ
		{
			param_data.meter_factory[0] = DataRecieve[11];
			SEQ_write(EE_Meter_Factory, &param_data.meter_factory[0], 1);
			len_trasmit = ProtocolBuild(NB_PROTOCOL_FactoryID, DataBuild);
			break;
		}
		default:
		{
			len_trasmit = 0;
			break;
		}
	}/*switch finish*/
		//ִ��
	return len_trasmit;
}


/****************************************************************************
** ��������: NBProcess
** ��������: NBģ��ͨѶ����
** �������: ��
** �������: ��
*****************************************************************************/

void CommuProcess(void)
{
	unsigned char DataBuild[256];
	unsigned char DataRecieve[256];
	unsigned char len;
	unsigned char nb_receive_len;


	//û�е��Զ��ϱ�ʱ�����û���յ�����
	if(!(g_Tran.AutoReportFlag ||g_NB.UARTReceiveOK))
	{
		return;
	}
	//�����Զ��ϱ�����
	if(g_Tran.AutoReportFlag == 1)
	{

        if(g_Tran.PwoerOn)//�״��ϵ�
        {
            len = ProtocolReport(DataBuild,NB_PROTOCOL_PowerON);
            g_Tran.PwoerOn = 0x00;
        }
        else//�����Զ��ϱ�����
		    len = ProtocolReport(DataBuild,NB_PROTOCOL_JICHAO);

		if((NB_LORA[0] == 0xAA)&&(NB_LORA[1] == 0xAA))//��װLORAģ��
		{
			UART0_SendString_Limit(DataBuild,len);//��������
		}
		else if((NB_LORA[0] == 0xBB)&&(NB_LORA[1] == 0xBB))	//��װNBģ��
		{
			if((g_NB.InitState[0] == NB_Init_OK)&&(g_NB.InitState[1] == NB_Init_OK))//�ȵȳ�ʼ������
			{
				NBdata_Transmit(DataBuild,len,"OK",1000);	//�����Զ��ϱ�����
			}
		}
		g_Tran.AutoReportFlag = 0;
		g_Tran.AutoReportCount = NB_AUTO_REPORT_TIME;//�����Զ��ϱ�ʱ��
		return;
	}

	//�������ս���
	if(g_NB.UARTReceiveOK == 1)
	{


		if((NB_LORA[0] == 0xAA)&&(NB_LORA[1] == 0xAA))//��װLORAģ��
		{
			len = Commu_Recive_Process(UartRxBuf,DataBuild,gBUartLen);
			UART0_SendString_Limit(DataBuild,len);//��������
		}
		else if((NB_LORA[0] == 0xBB)&&(NB_LORA[1] == 0xBB))	//��װNBģ��
		{
			if((g_NB.InitState[0] == NB_Init_OK)&&(g_NB.InitState[1] == NB_Init_OK))//�ȵȳ�ʼ������
			{
				//���޸�
				g_NB.DataReceiveOK = NBdata_Receive(UartRxBuf, DataRecieve, (unsigned char *)&nb_receive_len);
				if(g_NB.DataReceiveOK == 0)	return;//�յ��Ĳ���Э������
				else if (g_NB.DataReceiveOK == 1)
				{
					len = Commu_Recive_Process(DataRecieve,DataBuild,nb_receive_len);
					//���޸�
					NBdata_Transmit(DataBuild,len,"OK",1000);	//�����Զ��ϱ�����
				}
			}
		}
	g_NB.DataReceiveOK = 0;
	g_NB.UARTReceiveOK = 0;
	}/*���ս��� finish*/
}



/* �ϴ����ݣ�����Э����ʽ
��λ��ǰ����λ�ں�
00 01 			  //֡ͷ
66 55 44 33 22 11 //���112233445566
AA 				  //���Ҵ���
72				  //�����֣������ϱ�
40				  //���ݳ���40�ֽ�
//������������
XX XX XX XX XX XX //�õ�����6�ֽ�
XX XX XX XX       //��ѹ����λС����4�ֽ�
XX XX XX XX       //��������λС����4�ֽ�
XX XX XX XX       //Ƶ�ʣ���λС����4�ֽ�
XX XX XX XX       //�й����ʣ���λС����4�ֽ�
XX XX XX XX       //�޹����ʣ���λС����4�ֽ�
XX XX XX XX       //���ڹ��ʣ���λС����4�ֽ�
XX XX XX XX       //������������λС����4�ֽ�
XX XX XX XX XX XX //���ڣ�6�ֽ� ss mm hh DD MM YY
//�������ֹ
CS				  //У����
16                //֡���� */