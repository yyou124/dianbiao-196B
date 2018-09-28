/*==============================================================================
                Copyright(C) 1997-2019.  Sinowealth Tech. Co., Ltd.
--------------------------------------------------------------------------------
Project Name  : UARTDRIVER.c
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
#ifndef  UARTDRIVER_C
	#define  UARTDRIVER_C
#endif
//-------------------------------------------------------------------------------

//---------------

#include "config.h"
#include "Headconfig.h"


//uart0 register
unsigned char bdata  gBUartCon;
	sbit 	gbUartRxdFrameReq     = gBUartCon^0;		//������һ�����ݰ���־
	sbit 	gbUartRece			  = gBUartCon^1;		//����״̬��־
	sbit 	gbUartTran			  = gBUartCon^2;		//����״̬��־
unsigned char 	UartTxBuf[MAX_UART_DATA_LEN];				//�������ݻ�����
unsigned char  UartRxBuf[MAX_UART_DATA_LEN];				//�������ݻ�����
unsigned char data	gBUartLen;									//�ֽڸ���������
unsigned char data   gBUartTotalByte;							//һ�����ݰ����ֽڸ���
unsigned int data	gWUartRxdTimeOut;							//����һ�����ݰ���ʱ��ʱ��
//uart1 register
unsigned char bdata  gBUartCon1;
	sbit 	gbUartRxdFrameReq1     = gBUartCon1^0;		//������һ�����ݰ���־
	sbit 	gbUartRece1			  = gBUartCon1^1;		//����״̬��־
	sbit 	gbUartTran1			  = gBUartCon1^2;		//����״̬��־
unsigned char 	UartTxBuf1[MAX_UART_DATA_LEN];				//�������ݻ�����
unsigned char  UartRxBuf1[MAX_UART_DATA_LEN];				//�������ݻ�����
unsigned char data	gBUartLen1;									//�ֽڸ���������
unsigned char data   gBUartTotalByte1;							//һ�����ݰ����ֽڸ���
unsigned int data	gWUartRxdTimeOut1;							//����һ�����ݰ���ʱ��ʱ��


////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** ��������: Init_Uart
** ��������: ��ʼ��UART
** �������: ��
** �������: ��
*******************************************************************************************/
void Init_Uart(void)
{
    PCON=Bin(00000000);                      	//SCON[7..5]��SM0..3��  �����ʲ��ӱ�
//----------UART0------------------------
    SCON=Bin(01000000);				        	//ѡ��ģʽ1 8λ���� �첽 �ɱ䲨���� �����ж�ʹ��
//	SBRTL = UART_BAUDRATE_9600&0x00FF;     		//���ô���0�����ʷ�����
//	SBRTH = (UART_BAUDRATE_9600>>8)&0xFF;
	SBRTL = UART_BAUDRATE_115200&0x00FF;
	SBRTH = (UART_BAUDRATE_115200>>8)&0xFF;
	SFINE &= Bin(11110000);
//	SFINE |= (UART_BFINE_9600)&0x0F;       		//���ô���0�����ʷ�����΢�����ݼĴ���
	SFINE |= (UART_BFINE_115200)&0x0F;
	SBRTH |= 0x80;                          	//����0�����ʷ�����ʹ��
//----------UART1------------------------
	SCON1=Bin(01000000);				        //ѡ��ģʽ1 8λ���� �첽 �ɱ䲨���� �����ж�ʹ��
//    SBRTL=UART0_BAUDRATE&0xff;
//    SBRTH=(UART0_BAUDRATE>>8)&0xff;
//    SBRTH|=Bin(10000000);	                	//Bit7:�򿪲����ʷ�������Bit6-0:����
	SBRT1L = UART_BAUDRATE_9600&0x00FF;     	//���ô���1�����ʷ�����
	SBRT1H = (UART_BAUDRATE_9600>>8)&0xFF;
	//SFINE &= Bin(00001111);
	//SFINE |= (UART_BFINE_9600)&0xF0;       		//���ô���1�����ʷ�����΢�����ݼĴ���
	SBRT1H |= 0x80;                          	//����1�����ʷ�����ʹ��
	IRCON  = 0x00;								//��ֹ���⹦��
	//����ջ���BUFF
	for(gBUartLen = 0; gBUartLen<MAX_UART_DATA_LEN;gBUartLen++)
	{
		UartRxBuf[gBUartLen] = 0x00;
		UartTxBuf[gBUartLen] = 0x00;
		UartRxBuf1[gBUartLen] = 0x00;
		UartTxBuf1[gBUartLen] = 0x00;
	}
	gBUartLen = 0x00;
	gBUartLen1 = 0x00;
	RI = 0x00;
	TI = 0x00;
	gBUartCon = 0x00;							//���Զ���Ĵ���
	gBUartCon1 = 0x00;
	REN=1;                                   	//�򿪴���0�����ж�
	ES0=1;                                   	//ʹ��UART0�ж�

	REN1 = 1;									//�򿪴���1�����ж�
	IEN1 |= 0x10;	//ES1 = 1;					//ʹ��UART1�ж�

}



////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** ��������: EUART0_ISP
** ��������: ����0�жϷ������
** �������: ��
** �������: ��
*******************************************************************************************/
void EUART0_ISP(void) interrupt 4
{

//	if(TI)										//transmit work end event.
//	{
//		TI	=	0x00;
//		if(gbUartTran)								//�Ƿ����ڷ�����
//		{
//			/*-----------------����δ���------------------*/
//			if(gBUartLen <gBUartTotalByte)
//			{
//				SBUF	=	UartTxBuf[gBUartLen];		//��Ҫ�����������뵽�ⲿ�����ۼ�����
//				//TB8	=	P;							//������żУ��λ
//				//SBUF=	ACC;						//����Ҫ���͵�����
//				gBUartLen++;						//�ֽڸ���������һ
//			}
//			/*-----------------���������------------------*/
//			else
//			{

//				gBUartLen		= 0x00;				//���ֽڸ���������
//				gBUartTotalByte = 0x00;				//�������ֽ������Ĵ���
//				gbUartTran	    = 0;				//�巢��״̬��־
//				REN = 1;							//�������
//			}
//		}
//	}

	if(RI)										//receive one byte event.
	{
		RI	=	0x00;
		if(!gbUartTran)
		{
			Res = SBUF;
			//--------------------���ݽ����쳣-------------------
			if(gBUartLen == MAX_UART_DATA_LEN )
			{
				gbUartRece	=	0;					//�رս���
				gBUartLen	=	0x00;
			}
			if(1)
			//if( RB8	== P)							//��żУ���Ƿ���ȷ
			{
				UartRxBuf[gBUartLen]	=	Res;
				//Res	=	ACC;					//��ȡ���յ����ֽ�
				//UartRxBuf[gBUartLen] = Res;		//���浽���ݻ�������
				//---------------δ��ʼ��������--------------
				if(!gbUartRece)
				{
					gbUartRece	=	1;			//���õ�ǰΪ���ݽ���״̬
					gBUartLen	=	0;			//���ֽڸ���������
					gWUartRxdTimeOut = PACKAGE_RXD_TIMEOUT; //����һ�����ݰ����ճ�ʱʱ��
				}
				gBUartLen++;
			}
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** ��������: EUART1_ISP
** ��������: ����1�жϷ������
** �������: ��
** �������: ��
*******************************************************************************************/
void EUART1_ISP(void) interrupt 11
{
	EA	=	0;
	if(TI1)										//transmit work end event.
	{
		TI1	=	0x00;
		if(gbUartTran1)								//�Ƿ����ڷ�����
		{
			/*-----------------����δ���------------------*/
			if(gBUartLen1 <gBUartTotalByte1)
			{
				SBUF1	=	UartTxBuf1[gBUartLen1];		//��Ҫ�����������뵽�ⲿ�����ۼ�����
				//TB8	=	P;							//������żУ��λ
				//SBUF=	ACC;						//����Ҫ���͵�����
				gBUartLen1++;						//�ֽڸ���������һ
			}
			/*-----------------���������------------------*/
			else
			{

				gBUartLen1		 = 0x00;				//���ֽڸ���������
				gBUartTotalByte1 = 0x00;				//�������ֽ������Ĵ���
				gbUartTran1	     = 0;				//�巢��״̬��־
				REN1 = 1;							//�������
			}
		}
	}

	if(RI1)										//receive one byte event.
	{
		RI1	=	0x00;
		if(!gbUartTran1)
		{
			Res1 = SBUF1;
			//--------------------���ݽ����쳣-------------------
			if(gBUartLen1 == MAX_UART_DATA_LEN )
			{
				gbUartRece1	=	0;					//�رս���
				gBUartLen1	=	0x00;
			}
			if(1)
			//if( RB8	== P)							//��żУ���Ƿ���ȷ
			{
				UartRxBuf1[gBUartLen1]	=	Res1;
				//Res	=	ACC;					//��ȡ���յ����ֽ�
				//UartRxBuf[gBUartLen] = Res;		//���浽���ݻ�������
				//---------------δ��ʼ��������--------------
				if(!gbUartRece1)
				{
					gbUartRece1	=	1;			//���õ�ǰΪ���ݽ���״̬
					gBUartLen1	=	0;			//���ֽڸ���������
					gWUartRxdTimeOut1 = PACKAGE_RXD_TIMEOUT1;	//����һ�����ݰ����ճ�ʱʱ��
				}
				gBUartLen1++;
			}
		}
	}

 	EA	=	1;
}
/****************************************************************************
** ��������: void	UartRxdTimeout(void)
** ��������: �������ݰ���ʱ������
		    ÿ1ms���һ�Σ����ﵽ����һ�����ݰ����ʱ��ʱ���������
** �������: ��
** �������: ��
*****************************************************************************/
void	UartRxdTimeout(void)
{
	//Outtime process
	if(gbUartRece)
	{
		gWUartRxdTimeOut --;
		if(gWUartRxdTimeOut== 0)					//�������
		{
			//P1_1 = ~P1_1;
			//P1_0 = ~P1_0;
			gbUartRece = 0;							//���������״̬��־
			gbUartTran = 0;							//�巢������״̬��־
			gbUartRxdFrameReq = 1;					//���ý�����һ�����ݰ���־
			g_NB.UARTReceiveOK = 1;
			REN = 0;								//��ֹ����
		}
	}

	if(gbUartRece1)
	{
		gWUartRxdTimeOut1 --;
		if(gWUartRxdTimeOut1== 0)					//�������
		{
			//P1_1 = ~P1_1;
			//P1_0 = ~P1_0;
			gbUartRece1 = 0;							//���������״̬��־
			gbUartTran1 = 0;							//�巢������״̬��־
			gbUartRxdFrameReq1 = 1;					//���ý�����һ�����ݰ���־
			REN1 = 0;								//��ֹ����
		}
	}
}


/****************************************************************************
** ��������: void UARTAdjustPros(void)
** ��������: ���ݽ�����ɴ���, �����ݽ�����ɣ����н�����ɴ���
** �������: ��
** �������: ��
*****************************************************************************/
/* unsigned char UARTAdjustPros(unsigned char *Rx_Buff, unsigned char*Tx_Buff)
{
	unsigned char init_flag;
	if(*Rx_Buff == 0x68)
	{
		//ͨ�ŵ�ַ����
		//CRCУ�����
		if((*(Rx_Buff+7) == 0x68)
		{
			if(*(Rx_Buff+8) == 0x61)//д�����ʼ��
			{
				if(*(Rx_Buff+9) == 0x04)//��ʼ��
				{
					if((*(Rx_Buff+10) == 0xFF)&&(*(Rx_Buff+11) == 0xFF)&&(*(Rx_Buff+12) == 0xFF)&&(*(Rx_Buff+13) == 0xFF)\
					(*(Rx_Buff+14) == 0xFF))		init_flag = 1;
				}
				else if((*(Rx_Buff+9) == 0x05)&& init_flag)//д����
				{
					if(*(Rx_Buff+9) == 0x05)
				}
			}
			else if(*(Rx_Buff+8) == 0x60)//������
			{

			}
		}
	}
} */

/****************************************************************************
** ��������: void UARTProcess(void)
** ��������: ���ݽ�����ɴ���, �����ݽ�����ɣ����н�����ɴ���
** �������: ��
** �������: ��
*****************************************************************************/
void UART1Process(void)
{
	//process of uart
		//TEST
//	UartRxBuf1[0] = 0xEE;
//	UartRxBuf1[1] = 0x01;
//	UartRxBuf1[2] = 0x00;
//	UartRxBuf1[3] = 0x00;
//	gbUartRxdFrameReq1 = 1;
	if(_testbit_(gbUartRxdFrameReq1) )				//�Ƿ������һ�����ݰ�
	{
		Uart1Decode();

	}
}
/****************************************************************************
** ��������: UARTcmdCheck
** ��������: ����У��λ
** �������: *Data ��������			Num ���鳤��
** �������: У��λ
*****************************************************************************/

unsigned char UARTcmdCheck(unsigned char *Data, unsigned char Num)
{
	unsigned char i;
	unsigned char CRC = 0x00;
	for (i=0;i<Num;i++)	CRC += *(Data+i);
	CRC = ~CRC;
	CRC += 0x33;
	return CRC;
}



/*******************************************************************************************
** ��������: UART0_SendString
** ��������: UART0�����ַ���
** �������: ��
** �������: ��
*******************************************************************************************/
void  UART0_SendString(unsigned char *TXStr,unsigned char len)
{
	unsigned char i;
//	unsigned char tempuart0;
	//ES0=0;
    for(i=0;i<len;i++)
    {
		//tempuart0 = *TXStr;
		//UartTxBuf[i]=*TXStr;
		SBUF=*TXStr;
    while(!TI);
     TI=0;
		TXStr++;
    }
//	gBUartTotalByte = len;
//	gBUartCon  = Bin(00000000);
//	gbUartTran = 1;									//���÷�������״̬��־
//	TI = 1;
		REN = 1;
   // ES0=1;
}
/*******************************************************************************************
** ��������: UART0_SendString_Limit
** ��������: UART0�����ַ���
** �������: ��
** �������: ��
*******************************************************************************************/
void  UART0_SendString_Limit(unsigned char *TXStr,unsigned char len)
{
	unsigned char i,len_q;//����
	unsigned char len_r;//����
	char dest[64];
	len_q = len/60;
	len_r = len%60;
	if(len_q)//���ȴ���50���ָ��
	{
		for(i=0;i<len_q;i++)
		{
			strncpy(dest, TXStr+(i*60), 60);
			UART0_SendString((unsigned char*)dest,60);
			Delay_ms(50);
		}
		strncpy(dest, TXStr+(i*60), len_r);
		UART0_SendString((unsigned char*)dest,len_r);
	}
	else//С��50ֱ�ӷ���
		UART0_SendString((unsigned char*)TXStr,len);
	gBUartLen		= 0x00;				//���ֽڸ���������
	gBUartTotalByte = 0x00;				//�������ֽ������Ĵ���
	gbUartTran	    = 0;				//�巢��״̬��־
	REN = 1;							//�������

}
/*******************************************************************************************
** ��������: UART1_SendString
** ��������: UART1�����ַ���
** �������: ��
** �������: ��
*******************************************************************************************/
//void  UART1_SendString(unsigned char *TXStr,unsigned char len)
//{
//	unsigned char i;
//	i=0;
//	IEN1 &= (~0x10); //ES1 = 0;
//    for(;i<len;i++)
//    {

//		//UartTxBuf[i]=*TXStr;
//		SBUF1=*TXStr;
//        while(TI1==0);
//        TI1=0;
//        TXStr++;
//    }
//	IEN1 |= 0x10;	//ES1 = 1;
//}
/*******************************************************************************************
** ��������: ClearBUFF
** ��������: Buff����
** �������: Ҫ�����Buff   ��Buff����
** �������: ��
*******************************************************************************************/
unsigned char  CmpBUFF(unsigned char *data1, unsigned char *data2, unsigned int len)
{
    unsigned int cmp;
	unsigned char flag;
	flag = 1;
	for(cmp=0;cmp<len;cmp++)
	{
		if(data1[cmp]!= data2[cmp])
		{
			flag = 0;
			break;
		}
	}
	return flag;
	//gBUartCon  = Bin(00000000);
	//gbUartTran = 1;									//���÷�������״̬��־
	//TI = 1;											//�����һ���ж���Ӧ
}
/*******************************************************************************************
** ��������: ClearBUFF
** ��������: Buff����
** �������: Ҫ�����Buff   ��Buff����
** �������: ��
*******************************************************************************************/
void ClearBUFF(unsigned char *cleardata, unsigned int len)
{
    unsigned int clear;
	for(clear=0;clear<len;clear++)
		cleardata[clear]=0x00;

	//gBUartCon  = Bin(00000000);
	//gbUartTran = 1;									//���÷�������״̬��־
	//TI = 1;											//�����һ���ж���Ӧ
}
/****************************************************************************
** ��������: Uart1Decode
** ��������: ����1�������ݽ���,485ͨ��
** �������: ��
** �������: ��
*****************************************************************************/
void Uart1Decode(void)
{
unsigned char write_flag;
	unsigned int EE_addr;
	unsigned char EE_addrH;
	unsigned char EE_addrL;
	__U32_Def val;//EMU����
	unsigned char buf[6];
	//unsigned char temp[4];
	//����EEPROM
	if(UartRxBuf1[0] == 0xEE)
	{
		if(UartRxBuf1[1] == 0x01)//��
		{
			EE_addrH = UartRxBuf1[3];
			EE_addrL = UartRxBuf1[4];
			EE_addr = EE_addrH;
			EE_addr = (EE_addr<<8) + EE_addrL;
			//������
			EE_to_RAM(EE_addr, &UartTxBuf1[2], UartRxBuf1[2]);
			UartTxBuf1[0] = UartRxBuf1[0];
			UartTxBuf1[1] = UartRxBuf1[1];
			gBUartTotalByte1 = 2+UartRxBuf1[2];
			gBUartLen1		= 0x00;
		}
		else if(UartRxBuf1[1] == 0x02)//д
		{
			EE_addrH = UartRxBuf1[3];
			EE_addrL = UartRxBuf1[4];
			EE_addr = EE_addrH;
			EE_addr = (EE_addr<<8) + EE_addrL;
		 	VER_WRbytes(EE_addr,&UartRxBuf1[5],UartRxBuf1[2],1);	//д������
			//����д������
			Delay_ms(100);
			EE_to_RAM(EE_addr, &UartTxBuf1[2], UartRxBuf1[2]);
			UartTxBuf1[0] = UartRxBuf1[0];
			UartTxBuf1[1] = UartRxBuf1[1];
			gBUartTotalByte1 = 2+UartRxBuf1[2];
			gBUartLen1		= 0x00;
		}
	}
	else if(UartRxBuf1[0] == 0xED)//����EMU�Ĵ���
	{
		if(UartRxBuf1[1] == 0x01)//��
		{
			val = ReadEMU_REG(UartTxBuf1[3]);
			UartTxBuf1[0] = UartRxBuf1[0];
			UartTxBuf1[1] = UartRxBuf1[1];
			UartTxBuf1[2] = val.TBuf.buf0;
			UartTxBuf1[2] = val.TBuf.buf1;
			UartTxBuf1[2] = val.TBuf.buf2;
			UartTxBuf1[2] = val.TBuf.buf3;
			gBUartTotalByte1 = 6;
			gBUartLen1		= 0x00;
		}
		else if(UartRxBuf1[1] == 0x02)//д
		{
			val.TBuf.buf0 = UartRxBuf1[2];
			val.TBuf.buf1 = UartRxBuf1[3];
			val.TBuf.buf2 = UartRxBuf1[4];
			val.TBuf.buf3 = UartRxBuf1[5];
			WriteEMU_REG(UartTxBuf1[3],val);
			UartTxBuf1[0] = UartRxBuf1[0];
			UartTxBuf1[1] = UartRxBuf1[1];
			gBUartTotalByte1 = 2;
			gBUartLen1		= 0x00;
		}
	}
	else if(UartRxBuf1[0] == 0xFD)
	{
		if(UartRxBuf1[1] == 0x02)
		{
			if(UartRxBuf1[2] == 0xFF)//д�볧�ұ��
			{
				param_data.meter_factory[0] = UartRxBuf1[3];
				SEQ_write(EE_Meter_Factory,&param_data.meter_factory[0],1);//��EEPROM��д�볧�ұ��
			}

			else if(UartRxBuf1[2] == 0xAA)//��һ��������
			{
				param_data.address_input_flag[0] = UartRxBuf1[3];
				param_data.address_input_flag[1] = UartRxBuf1[4];
				param_data.address_input_flag[2] = UartRxBuf1[5];
				param_data.address_input_flag[3] = UartRxBuf1[6];
				param_data.address_input_flag[4] = UartRxBuf1[7];
				param_data.address_input_flag[5] = UartRxBuf1[8];
				param_data.address_input_flag[6] = 0xA5;
			}
			else if(UartRxBuf1[2] == 0xBB)//�ڶ���������
			{

					buf[0] = UartRxBuf1[3];
					buf[1] = UartRxBuf1[4];
					buf[2] = UartRxBuf1[5];
					buf[3] = UartRxBuf1[6];
					buf[4] = UartRxBuf1[7];
					buf[5] = UartRxBuf1[8];

					param_data.address_input_flag[7] = 0xA5;


			}
			//���ζ�һ�£��������
			if((param_data.address_input_flag[6] == 0xA5)&&(param_data.address_input_flag[7] == 0xA5))
			{
				if(param_data.address_input_flag[0] != buf[0]) return;
				if(param_data.address_input_flag[1] != buf[1]) return;
				if(param_data.address_input_flag[2] != buf[2]) return;
				if(param_data.address_input_flag[3] != buf[3]) return;
				if(param_data.address_input_flag[4] != buf[4]) return;
				if(param_data.address_input_flag[5] != buf[5]) return;
				param_data.meter_address[0] = buf[0];
				param_data.meter_address[1] = buf[1];
				param_data.meter_address[2] = buf[2];
				param_data.meter_address[3] = buf[3];
				param_data.meter_address[4] = buf[4];
				param_data.meter_address[5] = buf[5];
				VER_WRbytes(EE_Meter_address,&param_data.meter_address[0],6,1);//��EEPROM��д����
			}
			UartTxBuf1[0] = UartRxBuf1[0];
			UartTxBuf1[1] = UartRxBuf1[1];
			gBUartTotalByte1 = 2;
			gBUartLen1		= 0x00;
		}
	}
	else if(UartRxBuf1[0] == 0xFE)//У��д��
	{
		if(UartRxBuf1[1] == 0x02)//д
		{	if(UartRxBuf1[2]==0xD6)//ͨѶУ���ʼ��
			{
				gbUartAdjust = 0xFF;
				write_flag = 1;
			}
			if (UartRxBuf1[2]==0xD7)//��������
			{
				ADJ_ID = Adj5_CHB_Power_GAIN;	//У����Flag
				//����λ�ж�
				if(UartRxBuf1[3] == 0x01) Sign_Bit = 1;
				else Sign_Bit = 0;
				//����
				Adj_Error = BCD2toINT(&UartRxBuf1[4]);
				write_flag = 1;
			}
			else if (UartRxBuf1[2]==0xD8)//��λ1.0
			{
				ADJ_ID = Adj7_CHB_Phase;
				//����λ�ж�
				if(UartRxBuf1[3] == 0x01) Sign_Bit = 1;
				else Sign_Bit = 0;
				//����
				Adj_Error = BCD2toINT(&UartRxBuf1[4]);
				write_flag = 1;
			}
			else if (UartRxBuf1[2]==0xD9)//����offset
			{
				ADJ_ID = Adj9_CHB_Offset;
				//�����ٷֱ�
				Sign_Bit = BCD2HEX(UartRxBuf1[3]);
				//����
				Adj_Error = BCD2toINT(&UartRxBuf1[4]);
				write_flag = 1;
			}
		}
		gBUartTotalByte1 = 3;							//���÷����ֽ���Ϊ4��
		gBUartLen1		= 0x00;							//���ֽڸ���������
		UartTxBuf1[0] = 0xFE;	//֡ͷ
		if(write_flag)	UartTxBuf1[1] = 0x02;
		else	UartTxBuf1[1] = 0x82;
		UartTxBuf1[2] = UartRxBuf1[2];
	}
	gBUartCon1  = Bin(00000000);
	gbUartTran1 = 1;									//���÷�������״̬��־
	TI1 = 1;											//�����һ���ж���Ӧ											//�����һ���ж���Ӧ
}


