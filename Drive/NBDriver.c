/*==============================================================================
                Copyright(C) 1997-2019.  ChipNorth Tech. Co., Ltd.
--------------------------------------------------------------------------------
Project Name  : NBDriver.c
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

#ifndef  NBDRIVER_C
	#define  NBDRIVER_C
#endif
//-------------------------------------------------------------------------------

//---------------
#include "config.h"
#include "Headconfig.h"
#include "string.h"


M_NBMODE g_NB;
////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** ��������: ReverseCpy
** ��������: ���ǿ�������
** �������: ��
** �������: ��
*******************************************************************************************/
void ReverseCpy(unsigned char xdata * Dst,unsigned char xdata * Src,unsigned char Len)
{
	do
	{
		*Dst = *(Src+Len-1);
		Dst++;
		Len--;
	}while(Len != 0);
}

/*******************************************************************************************
** ��������: NBdata_SendString
** ��������: NBģ�鷢������
** �������: *TXStr �������ݣ����ݳ���len��ĿǰӦС��99���ַ�, *fasongҪ���͵�����
** �������: ��
*******************************************************************************************/
void NBdata_SendString(unsigned char *TXStr,unsigned char len, char *fasong)
{
    //ATָ��ṹ��AT+QLWULDATAEX=<length>,<data>,<mode>
    //char fasong[127];
	//char temp[MAX_UART_DATA_LEN];
    unsigned char changdu;
    //����  ATָ��
    sprintf(&fasong[0],NB_SEND_DATA_LWM2M_RAI);
    changdu = strlen(NB_SEND_DATA_LWM2M_RAI);
    //����  <length>
    if(len/10)
    {
        fasong[changdu] = (len / 10) + 0x30;
        changdu++;
    }
    if (len%10)
    {
        fasong[changdu] = (len % 10) + 0x30;
        changdu++;
    }
    //����  ,
    sprintf(&fasong[changdu],",");
    changdu++;
    //����  <data>
	HexGroupToString(&fasong[changdu],TXStr,len);
	changdu +=len*2;

    //����  ,<mode>
    sprintf(&fasong[changdu],",0x0100\r\n");
    changdu += 9;
    //����
	//UART0_SendString((unsigned char*)fasong,strlen(fasong));
	// Delay_ms(10);//Test
	// UART1_SendString((unsigned char*)fasong,strlen(fasong));//test

}
/*******************************************************************************************
** ��������: ATcmd_Transmit
** ��������: ����ATָ�����ⷵ��ֵ
** �������: *AT_str��������	*Repl_str�ڴ�����ֵ	   �ȴ�ʱ��Wait_ms
** �������: 1����ʧ�� 0���ͳɹ�
*******************************************************************************************/
unsigned char ATcmd_Transmit(unsigned char *AT_str, char *Repl_str, unsigned int Wait_ms)
{
	unsigned char Rx_len;
	unsigned char Repl_len;
	unsigned char Repl_strlen;

	Repl_strlen = strlen(Repl_str)-1;//��ȡ�ڴ�����ֵ����

	Rx_len = 0;
    //ͨ��UART0����ATָ��
	UART0_SendString((unsigned char*)AT_str,strlen(AT_str));

	Delay_ms(200);//�ȴ�����

	NBCount10ms = 0;//NBģ�鳬ʱ����
	RSTSTAT &= Bin(11111000);//�忴�Ź�
	while(Wait_ms>(NBCount10ms*10))
	{
		if(g_NB.UARTReceiveOK)//������ɱ�־
		{
			while(UartRxBuf[Rx_len] == Repl_str[Repl_len]&&(Wait_ms>(NBCount10ms*10)))//�Ա�һ���ַ�
			{
				if(Repl_len == Repl_strlen)//�����ڴ�ֵ������
				{
					NBCount10ms = 0;

					//UART1_SendString((unsigned char*)UartRxBuf,strlen(UartRxBuf));//test
					g_NB.InitCount = 0;//���ü�����
					g_NB.UARTReceiveOK = 0;
					ClearBUFF(UartRxBuf,countof(UartRxBuf));
					return 0;
				}
				RSTSTAT &= Bin(11111000);//�忴�Ź�
				Rx_len++;
				Repl_len++;
				if(Repl_str[Repl_len] != UartRxBuf[Rx_len]) break;//�����ϣ�����
			}
			RSTSTAT &= Bin(11111000);//�忴�Ź�
			Repl_len = 0;
			Rx_len ++;
			if(Rx_len>gBUartLen+1) Rx_len=0;
		}
		RSTSTAT &= Bin(11111000);//�忴�Ź�
	}
	g_NB.UARTReceiveOK = 0;
	g_NB.InitCount++;
	ClearBUFF(UartRxBuf,countof(UartRxBuf));
	return 1;
}

/*******************************************************************************************
** ��������: NBdata_Transmit
** ��������: NBģ���ϴ�����
** �������: *Tx_str��������	TX_len���ͳ���	*Repl_str�ڴ�����ֵ	   Wait_ms�ȴ�ʱ��
** �������: 1����ʧ�� 0���ͳɹ�
*******************************************************************************************/
unsigned char NBdata_Transmit(unsigned char *TX_str,unsigned char TX_len,char *Repl_str,unsigned int Wait_ms)
{
	unsigned char Rx_len;
	unsigned char Repl_len;
	unsigned char Repl_strlen;
	//unsigned char len_r;//����
	unsigned char len;
//	char dest[64];
	char fasong[200];
	Repl_strlen = strlen(Repl_str)-1;//��ȡ�ڴ�����ֵ����
	Rx_len = 0;
	NBdata_SendString((unsigned char*)TX_str,TX_len,fasong);
	len = strlen(fasong);
	//UART�Ļ���BUFFERֻ��64byte����Ҫ�Է����ַ������зָ�
	UART0_SendString_Limit(fasong,len);
	//�������ݣ��ȴ�����

	Delay_ms(200);

	NBCount10ms=0;//NBģ�鳬ʱ����
	RSTSTAT &= Bin(11111000);//�忴�Ź�
	while(Wait_ms>(NBCount10ms*10))
	{
		if(g_NB.UARTReceiveOK)//������ɱ�־
		{
			while(UartRxBuf[Rx_len] == Repl_str[Repl_len]&&(Wait_ms>(NBCount10ms*10)))//�Ա�һ���ַ�
			{
				if(Repl_len == Repl_strlen)//�����ڴ�ֵ������
				{
					NBCount10ms = 0;

					//UART1_SendString((unsigned char*)UartRxBuf,strlen(UartRxBuf));//test
					g_NB.InitCount = 0;//���ü�����
					g_NB.UARTReceiveOK = 0;
					ClearBUFF(UartRxBuf,countof(UartRxBuf));
					return 0;
				}
				Rx_len++;
				Repl_len++;
				if(Repl_str[Repl_len] != UartRxBuf[Rx_len]) break;//�����ϣ�����
			}
			RSTSTAT &= Bin(11111000);//�忴�Ź�
			Repl_len = 0;
			Rx_len ++;
			if(Rx_len>gBUartLen+1) Rx_len=0;
		}
		RSTSTAT &= Bin(11111000);//�忴�Ź�
	}
	g_NB.UARTReceiveOK = 0;
	g_NB.InitCount++;
	ClearBUFF(UartRxBuf,countof(UartRxBuf));
	return 1;
}

/*******************************************************************************************
** ��������: NBstate_Receive
** ��������: nbģ�鷵�ص�״̬
** �������: *AT_str ��������	*Repl_str�ڴ�����ֵ		*Get_str���ص�״̬	   Wait_ms�ȴ�ʱ��
** �������: 1����ʧ�� 0���ͳɹ�
*******************************************************************************************/
unsigned char NBstate_Receive(unsigned char *AT_str, char *Repl_str, char *Get_str, unsigned int Wait_ms)
{	unsigned char Rx_len;
	unsigned char Repl_len;
	unsigned char Repl_strlen;
	char *p;

	Repl_strlen = strlen(Repl_str)-1;//��ȡ�ڴ�����ֵ����
	Rx_len = 0;
    //ͨ��UART0����ATָ��
	UART0_SendString((unsigned char*)AT_str,strlen(AT_str));
	Delay_ms(300);//�ȴ�����

	NBCount10ms = 0;//NBģ�鳬ʱ����
	RSTSTAT &= Bin(11111000);//�忴�Ź�
	while(Wait_ms>(NBCount10ms*10))
	{
		if(g_NB.UARTReceiveOK)//������ɱ�־
		{
			while(UartRxBuf[Rx_len] == Repl_str[Repl_len]&&(Wait_ms>(NBCount10ms*10)))//�Ա�һ���ַ�
			{
				if(Repl_len == Repl_strlen)//�����ڴ�ֵ������
				{
					p = strtok((char *)Repl_str, ":");//�����յ����ַ���
					p = strtok(NULL, ":");//ȡ����
					strcpy(Get_str,p);
					NBCount10ms = 0;
					//UART1_SendString((unsigned char*)UartRxBuf,strlen(UartRxBuf));//test
					g_NB.InitCount = 0;//���ü�����
					g_NB.UARTReceiveOK = 0;
					ClearBUFF(UartRxBuf,countof(UartRxBuf));
					return 0;
				}
				Rx_len++;
				Repl_len++;
				if(Repl_str[Repl_len] != UartRxBuf[Rx_len]) break;//�����ϣ�����
			}
			RSTSTAT &= Bin(11111000);//�忴�Ź�
			Repl_len = 0;
			Rx_len ++;
			if(Rx_len>gBUartLen+1) Rx_len=0;
		}
	}
	g_NB.InitCount ++;
	g_NB.UARTReceiveOK = 0;
	ClearBUFF(UartRxBuf,countof(UartRxBuf));
	return 1;

}
/*******************************************************************************************
** ��������: NBdata_Receive
** ��������: nbģ����յ�������
** �������: *Repl_str UART���յ�������string		*Get_str ����������HEX��ʽ	*len ���յ����ݳ���
** �������: �Ƿ�ɹ����յ�����
*******************************************************************************************/
unsigned char NBdata_Receive(unsigned char *Repl_str, unsigned char *Get_str, unsigned char *len)
{
	char *p;
	unsigned char flag = 0;
	p = strtok((char *)Repl_str, ":");//�����յ����ַ���
	if(strcmp(p,NB_DATA_RECEIVE))//�ж�ǰ׺��ʽ
	{
		flag = 0;
		//*len = 0;
	}
	else
	{
		p = strtok(NULL, ":");//ȡ����
		p = strtok(p,",");
		p = strtok(NULL, ",");//ȡ����
		StringToHexGroup(Get_str, p, strlen(p));//���ַ���ת��ΪHEX���
		*len = strlen(p) / 2;//����HEX����
		flag = 1;
	}
	return flag;
}
/*******************************************************************************************
** ��������: NB_Init
** ��������: nbģ���ʼ��
** �������: ��
** �������: ��
*******************************************************************************************/

void NB_Init(void)
{

	char p[32];
	char *dest = p;

	if(g_NB.ReInitTime||!g_NB.InitStep)//û�е���ʼ��ʱ�䣬���߳�ʼ������
	{
		return;
	}
	else
	{
		if(g_NB.InitCount >10)//10��
		{
			//g_NB.InitCount =0x70;//��λʧ�ܣ�ͨ�Ŵ���
			g_NB.InitStep = 1;
			g_NB.ReInitTime = NB_RETRY_TIME;
				//TEST
g_NB.InitState[0] = NB_Init_OK;
g_NB.InitState[1] = NB_Init_OK;
			return;
		}
		switch(g_NB.InitStep)
		{
			case 1:
			{
				//g_NB.InitCount ++;
				g_NB.ReInitTime = 2;	//2�����г�ʼ��
				sprintf(dest,"AT+NBAND=%s\r\n",SERVER_BAND);		//����Ƶ��Ϊ5�����ſ�

				if(!ATcmd_Transmit(dest,"OK",1000))
				{
					g_NB.InitStep = 2;		//���г�ʼ������4
					//g_NB.InitCount  = 0;	//���ó�ʼ�����Դ���0
					break;
				}
				else break;
			}
			case 2:
			{
				sprintf(dest,"AT+NRB\r\n");
				UART0_SendString((unsigned char *)dest,8);
				g_NB.InitStep = 3;
				g_NB.ReInitTime = 20;
				break;

			}
			case 3:
			{
				//g_NB.InitCount ++;
				g_NB.ReInitTime = 4;	//2�����г�ʼ��
				sprintf(dest,"AT+QREGSWT=1\r\n");						//�Զ�ע��

				if(!ATcmd_Transmit(dest,"OK",1000))
				{
					g_NB.InitStep = 4;		//���г�ʼ������4
					//g_NB.InitCount  = 0;	//���ó�ʼ�����Դ���0
					break;
				}
				else break;
			}

			case 4:
			{
				//g_NB.InitCount ++;
				g_NB.ReInitTime = 2;	//2�����г�ʼ��
				sprintf(dest,"AT+CFUN=1\r\n");						//����Ϊ�շ�ģʽ
				//sprintf(dest,"AT+CIMI\r\n");						//��ѯ�ֻ���
				if(!ATcmd_Transmit(dest,"OK",1000))
				{
					g_NB.InitStep = 5;		//���г�ʼ������5
					//g_NB.InitCount  = 0;	//���ó�ʼ�����Դ���0
					break;
				}
				else break;
			}
			case 5:
			{
				//g_NB.InitCount ++;
				g_NB.ReInitTime = 2;	//2�����г�ʼ��
				sprintf(dest,"AT+NCDP=%s,%s\r\n",SERVER_ADDRESS,SERVER_PORT);//��ַд��
				if(!ATcmd_Transmit(dest,"OK",1000))
				{
					g_NB.InitStep = 6;		//���г�ʼ������6
					//g_NB.InitCount  = 0;	//���ó�ʼ�����Դ���0
					break;
				}
				else break;
			}
			case 6:
			{
				//g_NB.InitCount ++;
				g_NB.ReInitTime = 0;	//2�����г�ʼ��
				sprintf(dest,"AT+QLWSREGIND=0\r\n");				//ע��
				if(!ATcmd_Transmit(dest,"OK",1000))
				{
					g_NB.InitStep = 0;		//��ʼ������
					//g_NB.InitCount  = 0;	//���ó�ʼ�����Դ���0
					MemInitSet(&g_NB.InitState[0],NB_Init_OK,2);//��λNBinit FLAG
					SEQ_write(EE_NB_STATE, &g_NB.InitState[0], 2);
					break;
				}
				else break;
			}
			default:break;
		}/*switch finish*/
//	sprintf(dest,"AT+CSQ\r\n");							//��ѯ�ź�ǿ�Ȳ�ѯ
//	if(ATcmd_Transmit(dest,8,"OK",1000)) continue;
//	sprintf(dest,"AT+CGATT?\r\n");		//����״̬��ѯ
//	if(ATcmd_Transmit(dest,11,"OK",1000)) continue;

	}
}

/****************************************************************************
** ��������: void check()
** ��������: ���NBģ����������ƽ̨��״̬
** �������: ��
** �������: NBģ��״̬
*****************************************************************************/
unsigned char NB_check(void)
{
	unsigned char number;
	unsigned char statu;
	char p[32];
	char Data_buffer[32];
	char *date;
	char *dest;

	dest=&p;
	statu=0;
	while(statu==0)
	{
		number++;
		if(number==3)
		{
			sprintf(dest,"AT+NRB\r\n");
			number=0;
			if(ATcmd_Transmit(dest,"OK",5000)) continue;
	   	}
		//���Ͳ�ѯ����
		sprintf(dest,"AT+NMSTATUS?\r\n");
		NBstate_Receive(dest,"+NMSTATUS:",Data_buffer,2000);
		date=&Data_buffer[0];
		//״̬ȷ��
		sprintf(dest,LWM2M_STATUS_1);
		if(strcmp(dest,date)==0) statu = 1;
		sprintf(dest,LWM2M_STATUS_2);
		if(strcmp(dest,date)==0) statu = 2;
		sprintf(dest,LWM2M_STATUS_3);
		if(strcmp(dest,date)==0) statu = 3;
	}
	return statu;
}
/****************************************************************************
** ��������: NBdata_Transmit_Porcess
** ��������: NBģ�鷢�ͳ���
** �������: TX_str�������� TX_len���ͳ���
** �������: ��
*****************************************************************************/
// void NBdata_Transmit_Porcess(unsigned char *TX_str,unsigned char TX_len)
// {
// 	unsigned char send_process;	//NB�����������ݳ���
// 	unsigned char statu;		//NB����״̬
// 	unsigned char send_num;		//���ʹ���
// //	unsigned int  Waitms;
// 	char con_statu[3];

// 	send_process = 1;	//����1
// 	send_num = 1;
// 	while(send_process)
// 	{
// 		RSTSTAT &= Bin(11111000);//�忴�Ź�
// 		switch(send_process)
// 		{
// 			case 1:	//�������״̬
// 			{
// 				statu = NB_check();
// 				if(statu == 1) send_process = 2;//�ܹ���������
// 				else NB_Init();//���³�ʼ��
// 				break;
// 			}
// 			case 2://��������
// 			{
// 				NBdata_Transmit(TX_str,TX_len,"OK",500);
// 				send_num++;
// 				//��ʱ3S
// 				Delay_ms(500);
// 				Delay_ms(500);
// 				RSTSTAT &= Bin(11111000);//�忴�Ź�
// 				Delay_ms(500);
// 				Delay_ms(500);
// 				RSTSTAT &= Bin(11111000);//�忴�Ź�
// 				Delay_ms(500);
// 				Delay_ms(500);
// 				RSTSTAT &= Bin(11111000);//�忴�Ź�
// 			}
// 			case 3://��鷢��״̬
// 			{
// 				NBstate_Receive(CON_SENDING_STATUS,"QLWULDATASTATUS",con_statu,500);
// 				if(strcmp("4",con_statu)==0) send_process = 0; //�������
// 				else
// 				{
// 					if(send_num == 6) send_process = 0; //����6�Σ�������
// 					else if(send_num == 3) NB_Init();	//3�β��ɹ�����ʼ��NB
// 					send_process = 1;
// 				}
// 				break;
// 			}
// 		}/*switch finish*/
// 	}/*while finish*/
// }






