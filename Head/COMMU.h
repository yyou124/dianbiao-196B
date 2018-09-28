/*==============================================================================
            Copyright(C) 1997-2019.  Sinowealth Tech. Co., Ltd.
--------------------------------------------------------------------------------
Project No.   : XXX
File Name     : PIMA..H
Description   :
              ??
              ??
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
#ifndef	COMMU_H
#define	COMMU_H

#ifndef COMMU_C
	#define COMMU_EXT	extern
#else
	#define PIMA_EXT
#endif
//------------------------------------------------------------------------------

//----------------

#define     NB_POWER_ON				15                      //上电信号发出时间
#define     NB_AUTO_REPORT_TIME		60*5					//自动上报时间(s)

#define		NB_FRAME_HEAD_NUM		2				//帧头
#define		NB_FRAME_DATA_NUM		4				//一些数据的长度

#define		NB_FRAME_HEAD_1			0x00            //帧头1
#define		NB_FRAME_HEAD_2			0x01            //帧头2


#define     PROTOCOL_ERROR			0xD0			//错误响应


//后台请求1
#define     NB_PROTOCOL_kWh			0x11				//?????
#define     NB_PROTOCOL_V			0x12				//????
#define     NB_PROTOCOL_I			0x13				//????
#define     NB_PROTOCOL_Hz			0x14				//????
#define     NB_PROTOCOL_Power		0x15				//??????
#define     NB_PROTOCOL_rPower		0x16				//??????
#define     NB_PROTOCOL_aPower		0x17				//??????
#define     NB_PROTOCOL_FactorP		0x18				//??????
#define     NB_PROTOCOL_Date		0x19				//????
#define     NB_PROTOCOL_RelayStatus	0x1A				//?????
//后台请求2
#define 	NB_PROTOCOL_Timeservice 0x08				//????
#define		NB_PROTOCOL_kWh_rst		0x21				//?????
#define     NB_PROTOCOL_RelayCtrl	0x22				//?????
#define     NB_PROTOCOL_FactoryID	0x23				//??????
//主动上报
#define     NB_PROTOCOL_PowerON		0x71				//????
#define     NB_PROTOCOL_JICHAO		0x72				//????

typedef struct
{
    unsigned char PwoerOn;  //首次上电上报
	unsigned char AutoReportFlag;
	unsigned int AutoReportCount;	//自动上报时间
	unsigned int AutoReportTime;	//自动上报尝试次数
} M_Transimt;

extern M_Transimt g_Tran;

extern unsigned char ProtocolReport(unsigned char *DataBuild, unsigned char item);
extern unsigned char ProtocolBuild(unsigned char item, unsigned char *DataBuild);
extern unsigned char Commu_Return_Error(unsigned char *DataBuild, unsigned char item);
extern unsigned char Commu_Recive_Process(unsigned char *DataRecieve, unsigned char *DataBuild,unsigned char len);
extern void 		  CommuProcess(void);


#endif