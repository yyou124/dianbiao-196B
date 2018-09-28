/*==============================================================================
                Copyright(C) 1997-2019.  ChipNorth Tech. Co., Ltd.
--------------------------------------------------------------------------------
Project Name  : NBDriver.h
Description   :
Notice        :
			 1:
			 2:
Author        : YouDeqiang
Start Date    : 2018/05/23
Approve Date  :
Version       : V0.0
Function List :
             1:
             2:
RevisionHistory:
Rev#  CheckSum    Date     Author     Comments(Function+Date)
-----+--------+----------+---------+------------------------------------------
0.0     XXXX   2018/05/23   You        Just build the file
==============================================================================*/


#define		NB_RETRY_TIME			60*30					//初始化失败重试时间
#define		NB_Init_OK				0xA5
#define     NB_Init_NOT_OK			0x00

#define	    NB_SEND_DATA_LWM2M_MTK	"AT+M2MCLISEND="	//MTK2526通过LWM2M协议发送数据
#define     NB_SEND_DATA_LWM2M      "AT+QLWULDATAEX="	//通过LWM2M协议发送数据
#define     NB_SEND_DATA_LWM2M_RAI	"AT+QLWULDATAEX="	//通过LWM2M协议发送带有RAI标识的数据
#define		SERVER_BAND				"8"					//配置NB模块频段	5，电信 8，移动、联通
#define		SERVER_ADDRESS			"180.101.147.115"	//NB模块注册地址
#define		SERVER_PORT				"5683"				//NB模块端口

#define     CON_SENDING_STATUS		"AT+QLWULDATASTATUS?\r\n"	//查询NB数据发送情况
#define		NB_DATA_RECEIVE			"+NNMI"				//NB平台下发数据前缀

#define		LWM2M_STATUS_1			"MO_DATA_ENABLED"	//能够发送数据
#define		LWM2M_STATUS_2			"NO_UE_IP"			//无能发送数据
#define		LWM2M_STATUS_3			"INIITIALISED"		//不能发送数据


//错误相关

#define		SIMCARD_DETECT				"AT+CIMI\r\n"		//检测SIM卡状态
#define		SINGAL_DETECT				"AT+CESQ\r\n"		//信号强度检测
#define		NETWORK_DETECT				"AT+CEREG?\r\n"		//驻网检测
#define     PDPADDR_DETECT				"AT+CGPADDR=1\r\n"  //PDP地址查询
//NB上传协议相关定义


typedef struct
{
	unsigned char Head[4];			//帧头
	unsigned char ReadOrWirte;		//读写指令
	unsigned char IDS;				//命令标识符
	unsigned char DataField[4];		//数据域
	unsigned char CRC;				//校验码
}NB_PROTOCOL_NORMAL;

typedef struct
{
	unsigned char UARTReceiveOK;				//串口接收FLAG
	unsigned char DataReceiveOK;				//接收数据处理FLAG
	unsigned char InitCount;		//NB初始化计数器
	unsigned int ReInitTime;		//NB重新初始化时间计数器
	unsigned char InitStep;			//NB初始化步骤
	unsigned char InitState[2];
} M_NBMODE;






extern M_NBMODE g_NB;
//extern unsigned char g_NB.UARTReceiveOK;	  //NB数据UART接收完成FLAG

//extern unsigned char idata Data_buffer[40];//NB数据缓冲器

//extern void 		   ClearBUFF(unsigned char * cleardata, unsigned int len);
extern void ReverseCpy(unsigned char xdata * Dst,unsigned char xdata * Src,unsigned char Len);
extern unsigned char Receive_Cmp( char *Repl_str, unsigned int Wait_ms);
extern void 		 NBdata_SendString(unsigned char *TXStr,unsigned char len, char *fasong);
extern unsigned char ATcmd_Transmit(unsigned char *AT_str,char *Repl_str,unsigned int Wait_ms);
extern unsigned char NBdata_Transmit(unsigned char *TX_str,unsigned char TX_len,char *Repl_str,unsigned int Wait_ms);
extern unsigned char NBstate_Receive(unsigned char *AT_str, char *Repl_str, unsigned char *Get_str, unsigned int Wait_ms);
extern unsigned char NBdata_Receive(unsigned char *Repl_str, unsigned char *Get_str, unsigned char *len);


//extern void 		   NBdata_Transmit_Porcess(unsigned char *TX_str,unsigned char TX_len);
extern void 		   NB_Init(void);
extern void 			NB_Error(void);
//extern unsigned char NB_check(void);


//#endif
//------------------------------------------------------------------------------

//----------------
