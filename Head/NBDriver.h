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


#define		NB_RETRY_TIME			60*30					//��ʼ��ʧ������ʱ��
#define		NB_Init_OK				0xA5
#define     NB_Init_NOT_OK			0x00

#define	    NB_SEND_DATA_LWM2M_MTK	"AT+M2MCLISEND="	//MTK2526ͨ��LWM2MЭ�鷢������
#define     NB_SEND_DATA_LWM2M      "AT+QLWULDATAEX="	//ͨ��LWM2MЭ�鷢������
#define     NB_SEND_DATA_LWM2M_RAI	"AT+QLWULDATAEX="	//ͨ��LWM2MЭ�鷢�ʹ���RAI��ʶ������
#define		SERVER_BAND				"8"					//����NBģ��Ƶ��	5������ 8���ƶ�����ͨ
#define		SERVER_ADDRESS			"180.101.147.115"	//NBģ��ע���ַ
#define		SERVER_PORT				"5683"				//NBģ��˿�

#define     CON_SENDING_STATUS		"AT+QLWULDATASTATUS?\r\n"	//��ѯNB���ݷ������
#define		NB_DATA_RECEIVE			"+NNMI"				//NBƽ̨�·�����ǰ׺

#define		LWM2M_STATUS_1			"MO_DATA_ENABLED"	//�ܹ���������
#define		LWM2M_STATUS_2			"NO_UE_IP"			//���ܷ�������
#define		LWM2M_STATUS_3			"INIITIALISED"		//���ܷ�������


//�������

#define		SIMCARD_DETECT				"AT+CIMI\r\n"		//���SIM��״̬
#define		SINGAL_DETECT				"AT+CESQ\r\n"		//�ź�ǿ�ȼ��
#define		NETWORK_DETECT				"AT+CEREG?\r\n"		//פ�����
#define     PDPADDR_DETECT				"AT+CGPADDR=1\r\n"  //PDP��ַ��ѯ
//NB�ϴ�Э����ض���


typedef struct
{
	unsigned char Head[4];			//֡ͷ
	unsigned char ReadOrWirte;		//��дָ��
	unsigned char IDS;				//�����ʶ��
	unsigned char DataField[4];		//������
	unsigned char CRC;				//У����
}NB_PROTOCOL_NORMAL;

typedef struct
{
	unsigned char UARTReceiveOK;				//���ڽ���FLAG
	unsigned char DataReceiveOK;				//�������ݴ���FLAG
	unsigned char InitCount;		//NB��ʼ��������
	unsigned int ReInitTime;		//NB���³�ʼ��ʱ�������
	unsigned char InitStep;			//NB��ʼ������
	unsigned char InitState[2];
} M_NBMODE;






extern M_NBMODE g_NB;
//extern unsigned char g_NB.UARTReceiveOK;	  //NB����UART�������FLAG

//extern unsigned char idata Data_buffer[40];//NB���ݻ�����

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
