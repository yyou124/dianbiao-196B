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
M_Commu g_Commu;
/****************************************************************************
** 函数名称: ProtocolReport
** 函数描述: 协议数据组装函数  集抄
** 输入参数: DataBuild要发送的数据
** 输出参数: 要发送的数据长度
*****************************************************************************/
unsigned char ProtocolReport(unsigned char *DataBuild, unsigned char item)
{
    unsigned char buf[6];
	unsigned char protocol_len;
    //帧头
	buf[0] = NB_FRAME_HEAD_1;
	buf[1] = NB_FRAME_HEAD_2;
	memcpy (&DataBuild[0],&buf[0],NB_FRAME_HEAD_NUM);
	protocol_len = NB_FRAME_HEAD_NUM;//2
	//表号
	MemInitSet(&buf[0],0x00,6);//buff清零
	memcpy(&buf[0],&param_data.meter_address[0],6);
	memcpy(&DataBuild[protocol_len], &buf[0], 6);
	protocol_len +=6;
	//厂家代码
	buf[0] = param_data.meter_factory[0];//
	memcpy(&DataBuild[protocol_len], &buf[0], 1);
	protocol_len ++;
	//控制字
    buf[0]= item;
    memcpy(&DataBuild[protocol_len], &buf[0], 1);
	protocol_len +=2;//空出数据长度
	switch(item)
    {
        case NB_PROTOCOL_PowerON :
        {
            break;
        }
        case NB_PROTOCOL_JICHAO:
        {
            //数据内容
            //用电量
            MemInitSet(&buf[0],0x00,6);//buff清零
            memcpy(&buf[0],&KWH_PN[0],6);
            memcpy(&DataBuild[protocol_len], &buf[0], 6);
            protocol_len +=6;
            //电压
            MemInitSet(&buf[0],0x00,4);
            ReverseCpy(&buf[0],&g_InsBCD.Voltage[0],3);
            memcpy(&DataBuild[protocol_len], &buf[0], 4);
            protocol_len += 4;
            //电流
            MemInitSet(&buf[0],0x00,4);
            ReverseCpy(&buf[0],&g_InsBCD.Current[0],3);
            memcpy(&DataBuild[protocol_len], &buf[0], 4);
            protocol_len += 4;
            //频率
            MemInitSet(&buf[0],0x00,4);
            ReverseCpy(&buf[0],&g_InsBCD.Frequence[0],2);
            memcpy(&DataBuild[protocol_len], &buf[0], 4);
            protocol_len += 4;
            //有功功率
            MemInitSet(&buf[0],0x00,4);
            ReverseCpy(&buf[0],&g_InsBCD.Power[0],4);
            memcpy(&DataBuild[protocol_len],&buf[0],4);
            protocol_len += 4;
            //无功功率
            MemInitSet(&buf[0],0x00,4);
            ReverseCpy(&buf[0],&g_InsBCD.RPower[0],4);
            memcpy(&DataBuild[protocol_len], &buf[0], 4);
            protocol_len += 4;
            //视在功率
            MemInitSet(&buf[0],0x00,4);
            ReverseCpy(&buf[0],&g_InsBCD.APower[0],4);
            memcpy(&DataBuild[protocol_len], &buf[0], 4);
            protocol_len += 4;
            //功率因数
            MemInitSet(&buf[0],0x00,4);
            ReverseCpy(&buf[0],&g_InsBCD.PowerFactor[0],2);
            memcpy(&DataBuild[protocol_len], &buf[0], 4);
            protocol_len += 4;
            //日期
            MemInitSet(&buf[0],0x00,6);
			GetRealTime();	//读RTC处理
            memcpy(&buf[0],&_UINT8_RTC[0],6);
            memcpy(&DataBuild[protocol_len], &buf[0], 6);
            protocol_len += 6;
            break;
        }
        default :break;
    }
    //数据长度
    DataBuild[2+6+1+1] = HEX2BCD(protocol_len -2-6-1-1-1);//减去帧头，长度
    //校验位
	DataBuild[protocol_len] = UARTcmdCheck(&DataBuild[0], protocol_len);
	protocol_len++;
	//结束符
	DataBuild[protocol_len] = 0X16;
	protocol_len++;
	return protocol_len;
}
/****************************************************************************
** 函数名称: ProtocolBuild
** 函数描述: 协议数据组装函数
** 输入参数: item要组装的项目 	DataBuild要发送的数据
** 输出参数: 发送数据长度
*****************************************************************************/
unsigned char ProtocolBuild(unsigned char item, unsigned char *DataBuild)
{
	unsigned char buf[6];
	unsigned char protocol_len;
	//帧头
	buf[0] = NB_FRAME_HEAD_1;
	buf[1] = NB_FRAME_HEAD_2;
	memcpy (&DataBuild[0],&buf[0],NB_FRAME_HEAD_NUM);
	protocol_len = NB_FRAME_HEAD_NUM;
	//表号
	MemInitSet(&buf[0],0x00,6);//buff清零
	memcpy(&buf[0],&param_data.meter_address[0],6);
	memcpy(&DataBuild[protocol_len], &buf[0], 6);
	protocol_len +=6;
	//厂家代码
	buf[0] = param_data.meter_factory[0];//
	memcpy(&DataBuild[protocol_len], &buf[0], 1);
	protocol_len ++;
	//控制字节
	if(((item>>4)&0x0F) == 0x01)//响应请求类
	{
		buf[0] = item&0x0F;//正常响应
		buf[0] += 0x90;
	}
	else if(((item>>4)&0x0F) == 0x02)//响应控制类
	{
		buf[0] = item&0x0F;//正常响应
		buf[0] += 0xA0;
	}
	else
	{
		buf[0] = 0x00;
	}
	memcpy(&DataBuild[protocol_len], &buf[0], 1);
	protocol_len ++;
	//数据内容
	switch(item)
	{
		case NB_PROTOCOL_kWh://用电量
		{
			//数据长度
			buf[0] = 6;
			DataBuild[protocol_len] = buf[0];
			protocol_len++;
			MemInitSet(&buf[0], 0x00, 6); //buff清零
			memcpy(&buf[0],&KWH_PN[0],6);
			memcpy(&DataBuild[protocol_len], &buf[0], 6);
			protocol_len+=6;
			break;
		}
		case NB_PROTOCOL_V://电压
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
		case NB_PROTOCOL_I://电流
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
		case NB_PROTOCOL_Hz://频率
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
		case NB_PROTOCOL_Power://有功功率
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
		case NB_PROTOCOL_rPower://无功功率
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
		case NB_PROTOCOL_aPower://视在功率
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
		case NB_PROTOCOL_FactorP://功率因数
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
		case NB_PROTOCOL_Date://日期
		{
			buf[0] = 6;
			DataBuild[protocol_len] = buf[0];
			protocol_len++;
			MemInitSet(&buf[0],0x00,6);//buff清零
			GetRealTime();	//读RTC处理
			memcpy(&buf[0],&_UINT8_RTC[0],6);
			memcpy(&DataBuild[protocol_len], &buf[0], 6);
			protocol_len += 6;
			break;
		}

		case NB_PROTOCOL_RelayStatus://继电器状态
		{
			buf[0] = 2;
			DataBuild[protocol_len] = buf[0];
			protocol_len++;
			MemInitSet(&buf[0],0x00,2);//buff清零
			memcpy(&buf[0],&DelayStatus[0],2);
			memcpy(&DataBuild[protocol_len], &buf[0], 2);
			protocol_len += 2;
			break;
		}
		case NB_PROTOCOL_Timeservice: //广播授时
		{
			protocol_len = 0x00;
			break;
		}
		case NB_PROTOCOL_kWh_rst:	//用电量重设
		{
			buf[0] = 0x00;
			DataBuild[protocol_len] = buf[0];
			protocol_len++;
			break;
		}
		case NB_PROTOCOL_RelayCtrl:	//继电器控制
		{
			buf[0] = 0x00;
			DataBuild[protocol_len] = buf[0];
			protocol_len++;
			break;
		}
		case NB_PROTOCOL_FactoryID:	//修改厂商标识
		{
			buf[0] = 0x00;
			DataBuild[protocol_len] = buf[0];
			protocol_len++;
			break;
		}
		default:
			break;
	}
	//校验位
	DataBuild[protocol_len] = UARTcmdCheck(&DataBuild[0], protocol_len);
	protocol_len++;
	//结束符
	DataBuild[protocol_len] = 0X16;
	protocol_len++;
	return protocol_len;
}
/****************************************************************************
** 函数名称: Commu_Return_Error
** 函数描述: 协议错误返回函数
** 输入参数: item要组装的项目
** 输出参数: 发送数据长度
*****************************************************************************/
unsigned char Commu_Return_Error(unsigned char *DataBuild, unsigned char item)
{
	unsigned char buf[6];
	unsigned char protocol_len;
	//帧头
	buf[0] = NB_FRAME_HEAD_1;
	buf[1] = NB_FRAME_HEAD_2;
	memcpy (&DataBuild[0],&buf[0],NB_FRAME_HEAD_NUM);
	protocol_len = NB_FRAME_HEAD_NUM;
	//表号
	MemInitSet(&buf[0],0x00,6);//buff清零
	memcpy(&buf[0],&param_data.meter_address[0],6);
	memcpy(&DataBuild[protocol_len], &buf[0], 6);
	protocol_len +=6;
	//厂家代码
	buf[0] = param_data.meter_factory[0];//
	memcpy(&DataBuild[protocol_len], &buf[0], 1);
	protocol_len ++;
	//控制字节,异常响应
	buf[0] = item &0x0F;
	buf[0] += 0xD0;
	memcpy(&DataBuild[protocol_len], &buf[0], 1);
	protocol_len ++;
	//数据长度
	buf[0] = 0x00;
	memcpy(&DataBuild[protocol_len], &buf[0], 1);
	protocol_len ++;
	//校验位
	DataBuild[protocol_len] = UARTcmdCheck(&DataBuild[0], protocol_len);
	protocol_len++;
	//结束符
	DataBuild[protocol_len] = 0X16;
	protocol_len++;
	return protocol_len;
}
/****************************************************************************
** 函数名称: Comm_Recive_Process
** 函数描述: 协议接收处理函数(响应后台)
** 输入参数: DataRecieve接收到的数据 	len接收数据的长度
** 输出参数: 发送数据长度
*****************************************************************************/
unsigned char Commu_Recive_Process(unsigned char *DataRecieve, unsigned char *DataBuild,unsigned char len)
{
	unsigned char len_trasmit;
	unsigned char data_len;
	unsigned char Bufftemp[6];
	//判断表号(默认表号),不是表号不响应
	if(!(CmpBUFF(&DataRecieve[2],&param_data.meter_address_def[0],6)||CmpBUFF(&DataRecieve[2],&param_data.meter_address[0],6)))
	{
		len_trasmit = 0;
		return	len_trasmit;
	}

	//检查停止位，不符合，错误返回
	if(DataRecieve[len-1] != 0x16)
	{
		len_trasmit = Commu_Return_Error(&DataBuild[0],DataRecieve[9]);
		return	len_trasmit;
	}
	//检查校验位,不符合，错误返回
	if(DataRecieve[len-2] != UARTcmdCheck(DataRecieve,len-2))
	{
		len_trasmit = Commu_Return_Error(&DataBuild[0],DataRecieve[9]);
		return	len_trasmit;
	}
	//检查帧头,不符合，错误返回
	memcpy(&Bufftemp[0],&DataRecieve[0],NB_FRAME_HEAD_NUM);
	if(Bufftemp[0] != DOWN_FRAME_HEAD_1)
	{
		len_trasmit = Commu_Return_Error(&DataBuild[0],DataRecieve[9]);
		return len_trasmit;
	}
	if(Bufftemp[1] != DOWN_FRAME_HEAD_2)
	{
		len_trasmit = Commu_Return_Error(&DataBuild[0],DataRecieve[9]);
		return len_trasmit;
	}
	//寻找数据域长度
	data_len = DataRecieve[10];
	//判断命令类型
	switch(DataRecieve[9])
	{
		//后台返回类型判断
		case NB_ACK_1:
		{
			len_trasmit = 0;
			g_Tran.AutoReportFlag = COUNT_DOWN;
			g_Tran.AutoReportTime = 0; //重置自动上报尝试次数
			//g_Tran.AutoReportCount = NB_AUTO_REPORT_TIME;//重置自动上报时间
			g_Tran.AutoReportCount = g_Commu.AutoReportTimeSet;//重置自动上报时间
			break;
		}
		case NB_ACK_2:
		{
			len_trasmit = 0;
			g_Tran.AutoReportFlag = COUNT_DOWN;
			g_Tran.AutoReportTime = 0; //重置自动上报尝试次数
			//g_Tran.AutoReportCount = NB_AUTO_REPORT_TIME;//重置自动上报时间
			g_Tran.AutoReportCount = g_Commu.AutoReportTimeSet;//重置自动上报时间
			break;
		}
		//后台请求类判断
		case NB_PROTOCOL_kWh://用电量
		{
			len_trasmit = ProtocolBuild(NB_PROTOCOL_kWh, DataBuild);		//建立上报数据
			//NBdata_Transmit(DataBuild,len,"OK",1000);	//发送上报数据
			break;
		}
		case NB_PROTOCOL_V://电压
		{
			len_trasmit = ProtocolBuild(NB_PROTOCOL_V, DataBuild);   //建立上报数据
			//NBdata_Transmit(DataBuild, len, "OK", 1000);	   //发送上报数据
			break;
		}
		case NB_PROTOCOL_I://电流
		{
			len_trasmit = ProtocolBuild(NB_PROTOCOL_I, DataBuild);   //建立上报数据
			//NBdata_Transmit(DataBuild, len, "OK", 1000);	   //发送上报数据
			break;
		}
		case NB_PROTOCOL_Hz://频率
		{
			len_trasmit = ProtocolBuild(NB_PROTOCOL_Hz, DataBuild);  //建立上报数据
			//NBdata_Transmit(DataBuild, len, "OK", 1000);	   //发送上报数据
			break;
		}
		case NB_PROTOCOL_Power://有功功率
		{
			len_trasmit = ProtocolBuild(NB_PROTOCOL_Power, DataBuild); //建立上报数据
			//NBdata_Transmit(DataBuild, len, "OK", 1000);	   //发送上报数据
			break;
		}
		case NB_PROTOCOL_rPower://无功功率
		{
			len_trasmit = ProtocolBuild(NB_PROTOCOL_rPower, DataBuild); //建立上报数据
			//NBdata_Transmit(DataBuild, len, "OK", 1000);	   //发送上报数据
			break;
		}
		case NB_PROTOCOL_aPower://视在功率
		{
			len_trasmit = ProtocolBuild(NB_PROTOCOL_aPower, DataBuild); //建立上报数据
			//NBdata_Transmit(DataBuild, len, "OK", 1000);	   //发送上报数据
			break;
		}
		case NB_PROTOCOL_FactorP://功率因数
		{
			len_trasmit = ProtocolBuild(NB_PROTOCOL_FactorP, DataBuild); //建立上报数据
			//NBdata_Transmit(DataBuild, len, "OK", 1000);	   //发送上报数据
			break;
		}
		case NB_PROTOCOL_Date://日期
		{
			len_trasmit = ProtocolBuild(NB_PROTOCOL_Date, DataBuild); //建立上报数据
			//NBdata_Transmit(DataBuild, len, "OK", 1000);	   //发送上报数据
			break;
		}
		case NB_PROTOCOL_RelayStatus://继电器状态
		{
			len_trasmit = ProtocolBuild(NB_PROTOCOL_RelayStatus,DataBuild);

			break;
		}
		//后台控制类判断
		case NB_PROTOCOL_Timeservice: //广播授时
		{
			memcpy(&_UINT8_RTC[0],&DataRecieve[11],6);//写入时间
			SetRealTime();
			len_trasmit = ProtocolBuild(NB_PROTOCOL_Timeservice, DataBuild);
			len_trasmit = 0;

			break;
		}
		case NB_PROTOCOL_kWh_rst:	//用电量重设
		{

			memcpy(&KWH_PN[0],&DataRecieve[11],6);//电量
			KWHD = DataRecieve[11];
			KWHD1 = KWHD;
			KWHD2 = KWHD;
			KWHD3 = KWHD;
			memcpy(&kwh_value.integer[0],&DataRecieve[12],5);
			VER_WRbytes((unsigned int)EE_KHH_address, &kwh_value.integer[0], 5, 1);
			len_trasmit = ProtocolBuild(NB_PROTOCOL_kWh_rst, DataBuild);
			break;
		}
		case NB_PROTOCOL_RelayCtrl:	//继电器控制
		{
			if(DataRecieve[11]==0x10)//通电
			{
				RealyCtrl(0);
			}
			else if(DataRecieve[11]==0x20)//断电
			{
				RealyCtrl(1);
			}
			else if(DataRecieve[11]==0x30)//断电5S
			{
				RealyCtrl(2);
			}
			len_trasmit = ProtocolBuild(NB_PROTOCOL_RelayCtrl, DataBuild);
			break;
		}
		case NB_PROTOCOL_FactoryID:	//修改厂商标识
		{
			param_data.meter_factory[0] = DataRecieve[11];
			SEQ_write(EE_Meter_Factory, &param_data.meter_factory[0], 1);
			len_trasmit = ProtocolBuild(NB_PROTOCOL_FactoryID, DataBuild);
			break;
		}
		case NB_PROTOCOL_ReportTime: //修改自动上报时间
		{
			g_Commu.AutoReportTimeSet = BCD2toINT(&DataRecieve[11]);//自动上报时间
			g_Commu.AutoReportTimeSetAck = BCD2toINT(&DataRecieve[13]);//ACK相应时间
			VER_WRbytes(EE_Commu_Time,&DataRecieve[11],4, 1);
			g_Tran.AutoReportCount = 1;//1分钟后发出自动上报数据
			len_trasmit = 0;
			break;
		}
		default:
		{
			len_trasmit = 0;
			break;
		}
	}/*switch finish*/
		//执行
	return len_trasmit;
}


/****************************************************************************
** 函数名称: NBProcess
** 函数描述: NB模块通讯进程
** 输入参数: 无
** 输出参数: 无
*****************************************************************************/

void CommuProcess(void)
{
	unsigned char DataBuild[256];
	//unsigned char DataRecieve[256];
	unsigned char len;
//	unsigned char nb_receive_len;
	//test
//unsigned char temp[64] = "\r\n+QLWDATARECV: 19,1,0,27,01010000000000000012001E16\r\n";
//g_NB.UARTReceiveOK = 1;
//NB_LORA[0] = 0xBB;
//NB_LORA[1] = 0xBB;
//	g_NB.InitState[0] = NB_Init_OK;
//g_NB.InitState[1] = NB_Init_OK;

	//没有到自动上报时间或者没有收到数据
	if(!((g_Tran.AutoReportFlag == REPORT_TIME) ||g_NB.UARTReceiveOK))
	{
		return;
	}
	//启动自动上报进程
	if(g_Tran.AutoReportFlag == REPORT_TIME)
	{

        if(g_Tran.PwoerOn)//首次上电
        {

            len = ProtocolReport(DataBuild,NB_PROTOCOL_PowerON);
            g_Tran.PwoerOn = 0x00;

        }
        else//建立自动上报数据
		    len = ProtocolReport(DataBuild,NB_PROTOCOL_JICHAO);


		UART1_SendString(DataBuild,len);//发送数据


		//不配置ACK回应
		g_Tran.AutoReportFlag = 0;
		//g_Tran.AutoReportCount = NB_AUTO_REPORT_TIME;//重置自动上报时间
		g_Tran.AutoReportCount = g_Commu.AutoReportTimeSet;//重置自动上报时间
		//配置ACK回应
//		g_Tran.AutoReportTime ++;//尝试次数+1
//		g_Tran.AutoReportFlag = WAITING_ACK;
//		//g_Tran.AutoReportCount = NB_AUTO_REPORT_ACk;//设置等待ACK时间
//		g_Tran.AutoReportCount = g_Commu.AutoReportTimeSetAck;//设置等待ACK时间

		return;
	}/*自动上报进程finish*/

	//启动接收进程
	if(g_NB.UARTReceiveOK == 1)
	{
		Uart1Decode();

		len = Commu_Recive_Process(UartRxBuf1,DataBuild,gBUartLen1);
		UART1_SendString(DataBuild,len);//发送数据

	REN1 = 1;
	g_NB.DataReceiveOK = 0;
	g_NB.UARTReceiveOK = 0;
	}/*接收进程 finish*/

}



/* 上传数据，依据协议形式
低位在前，高位在后
00 01 			  //帧头
00 AA 72 00 00
66 55 44 33 22 11 //表号112233445566
AA 				  //厂家代码
72				  //控制字，主动上报
40				  //数据长度40字节
//以下是数据域
XX XX XX XX XX XX //用电量，6字节
XX XX XX XX       //电压，两位小数，4字节
XX XX XX XX       //电流，四位小数，4字节
XX XX XX XX       //频率，两位小数，4字节
XX XX XX XX       //有功功率，两位小数，4字节
XX XX XX XX       //无功功率，两位小数，4字节
XX XX XX XX       //视在功率，两位小数，4字节
XX XX XX XX       //功率因数，三位小数，4字节
XX XX XX XX XX XX //日期，6字节 ss mm hh DD MM YY
//数据域截止
CS				  //校验码
16                //帧结束 */

/*
禾苗
00 AA 72 00 00
A0 A1 A2 A3 A4 A5
01
72
42
01 22 10 00 00 01
68 43 00 00
45 73 00 00
99 21 00 00
99 21 00 00
99 21 00 00
99 21 00 00
99 21 00 00
30 30 11 07 07 18
FFFF//多余
00
16
FFFFFF//多余
*/