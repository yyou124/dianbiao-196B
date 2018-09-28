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
	sbit 	gbUartRxdFrameReq     = gBUartCon^0;		//接收完一个数据包标志
	sbit 	gbUartRece			  = gBUartCon^1;		//接收状态标志
	sbit 	gbUartTran			  = gBUartCon^2;		//发送状态标志
unsigned char 	UartTxBuf[MAX_UART_DATA_LEN];				//接收数据缓冲器
unsigned char  UartRxBuf[MAX_UART_DATA_LEN];				//发送数据缓冲器
unsigned char data	gBUartLen;									//字节个数计数器
unsigned char data   gBUartTotalByte;							//一个数据包的字节个数
unsigned int data	gWUartRxdTimeOut;							//传送一个数据包超时计时器
//uart1 register
unsigned char bdata  gBUartCon1;
	sbit 	gbUartRxdFrameReq1     = gBUartCon1^0;		//接收完一个数据包标志
	sbit 	gbUartRece1			  = gBUartCon1^1;		//接收状态标志
	sbit 	gbUartTran1			  = gBUartCon1^2;		//发送状态标志
unsigned char 	UartTxBuf1[MAX_UART_DATA_LEN];				//接收数据缓冲器
unsigned char  UartRxBuf1[MAX_UART_DATA_LEN];				//发送数据缓冲器
unsigned char data	gBUartLen1;									//字节个数计数器
unsigned char data   gBUartTotalByte1;							//一个数据包的字节个数
unsigned int data	gWUartRxdTimeOut1;							//传送一个数据包超时计时器


////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: Init_Uart
** 函数描述: 初始化UART
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
void Init_Uart(void)
{
    PCON=Bin(00000000);                      	//SCON[7..5]作SM0..3用  波特率不加倍
//----------UART0------------------------
    SCON=Bin(01000000);				        	//选用模式1 8位数据 异步 可变波特率 接收中断使能
//	SBRTL = UART_BAUDRATE_9600&0x00FF;     		//设置串口0波特率发生器
//	SBRTH = (UART_BAUDRATE_9600>>8)&0xFF;
	SBRTL = UART_BAUDRATE_115200&0x00FF;
	SBRTH = (UART_BAUDRATE_115200>>8)&0xFF;
	SFINE &= Bin(11110000);
//	SFINE |= (UART_BFINE_9600)&0x0F;       		//设置串口0波特率发生器微调数据寄存器
	SFINE |= (UART_BFINE_115200)&0x0F;
	SBRTH |= 0x80;                          	//串口0波特率发生器使能
//----------UART1------------------------
	SCON1=Bin(01000000);				        //选用模式1 8位数据 异步 可变波特率 接收中断使能
//    SBRTL=UART0_BAUDRATE&0xff;
//    SBRTH=(UART0_BAUDRATE>>8)&0xff;
//    SBRTH|=Bin(10000000);	                	//Bit7:打开波特率发生器，Bit6-0:高七
	SBRT1L = UART_BAUDRATE_9600&0x00FF;     	//设置串口1波特率发生器
	SBRT1H = (UART_BAUDRATE_9600>>8)&0xFF;
	//SFINE &= Bin(00001111);
	//SFINE |= (UART_BFINE_9600)&0xF0;       		//设置串口1波特率发生器微调数据寄存器
	SBRT1H |= 0x80;                          	//串口1波特率发生器使能
	IRCON  = 0x00;								//禁止红外功能
	//清接收缓冲BUFF
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
	gBUartCon = 0x00;							//清自定义寄存器
	gBUartCon1 = 0x00;
	REN=1;                                   	//打开串口0接收中断
	ES0=1;                                   	//使能UART0中断

	REN1 = 1;									//打开串口1接收中断
	IEN1 |= 0x10;	//ES1 = 1;					//使能UART1中断

}



////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: EUART0_ISP
** 函数描述: 串口0中断服务程序
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
void EUART0_ISP(void) interrupt 4
{

//	if(TI)										//transmit work end event.
//	{
//		TI	=	0x00;
//		if(gbUartTran)								//是否正在发送中
//		{
//			/*-----------------发送未完成------------------*/
//			if(gBUartLen <gBUartTotalByte)
//			{
//				SBUF	=	UartTxBuf[gBUartLen];		//将要发送数据载入到外部数据累加器中
//				//TB8	=	P;							//载入奇偶校验位
//				//SBUF=	ACC;						//载入要发送的数据
//				gBUartLen++;						//字节个数计数加一
//			}
//			/*-----------------发送已完成------------------*/
//			else
//			{

//				gBUartLen		= 0x00;				//清字节个数计数器
//				gBUartTotalByte = 0x00;				//清数据字节总数寄存器
//				gbUartTran	    = 0;				//清发送状态标志
//				REN = 1;							//允许接收
//			}
//		}
//	}

	if(RI)										//receive one byte event.
	{
		RI	=	0x00;
		if(!gbUartTran)
		{
			Res = SBUF;
			//--------------------数据接收异常-------------------
			if(gBUartLen == MAX_UART_DATA_LEN )
			{
				gbUartRece	=	0;					//关闭接收
				gBUartLen	=	0x00;
			}
			if(1)
			//if( RB8	== P)							//奇偶校验是否正确
			{
				UartRxBuf[gBUartLen]	=	Res;
				//Res	=	ACC;					//读取接收到的字节
				//UartRxBuf[gBUartLen] = Res;		//保存到数据缓冲器中
				//---------------未开始接收数据--------------
				if(!gbUartRece)
				{
					gbUartRece	=	1;			//设置当前为数据接收状态
					gBUartLen	=	0;			//清字节个数计数器
					gWUartRxdTimeOut = PACKAGE_RXD_TIMEOUT; //设置一个数据包接收超时时间
				}
				gBUartLen++;
			}
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: EUART1_ISP
** 函数描述: 串口1中断服务程序
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
void EUART1_ISP(void) interrupt 11
{
	EA	=	0;
	if(TI1)										//transmit work end event.
	{
		TI1	=	0x00;
		if(gbUartTran1)								//是否正在发送中
		{
			/*-----------------发送未完成------------------*/
			if(gBUartLen1 <gBUartTotalByte1)
			{
				SBUF1	=	UartTxBuf1[gBUartLen1];		//将要发送数据载入到外部数据累加器中
				//TB8	=	P;							//载入奇偶校验位
				//SBUF=	ACC;						//载入要发送的数据
				gBUartLen1++;						//字节个数计数加一
			}
			/*-----------------发送已完成------------------*/
			else
			{

				gBUartLen1		 = 0x00;				//清字节个数计数器
				gBUartTotalByte1 = 0x00;				//清数据字节总数寄存器
				gbUartTran1	     = 0;				//清发送状态标志
				REN1 = 1;							//允许接收
			}
		}
	}

	if(RI1)										//receive one byte event.
	{
		RI1	=	0x00;
		if(!gbUartTran1)
		{
			Res1 = SBUF1;
			//--------------------数据接收异常-------------------
			if(gBUartLen1 == MAX_UART_DATA_LEN )
			{
				gbUartRece1	=	0;					//关闭接收
				gBUartLen1	=	0x00;
			}
			if(1)
			//if( RB8	== P)							//奇偶校验是否正确
			{
				UartRxBuf1[gBUartLen1]	=	Res1;
				//Res	=	ACC;					//读取接收到的字节
				//UartRxBuf[gBUartLen] = Res;		//保存到数据缓冲器中
				//---------------未开始接收数据--------------
				if(!gbUartRece1)
				{
					gbUartRece1	=	1;			//设置当前为数据接收状态
					gBUartLen1	=	0;			//清字节个数计数器
					gWUartRxdTimeOut1 = PACKAGE_RXD_TIMEOUT1;	//设置一个数据包接收超时时间
				}
				gBUartLen1++;
			}
		}
	}

 	EA	=	1;
}
/****************************************************************************
** 函数名称: void	UartRxdTimeout(void)
** 函数描述: 接收数据包超时检测程序
		    每1ms检测一次，当达到接收一个数据包最大时间时，接收完成
** 输入参数: 无
** 输出参数: 无
*****************************************************************************/
void	UartRxdTimeout(void)
{
	//Outtime process
	if(gbUartRece)
	{
		gWUartRxdTimeOut --;
		if(gWUartRxdTimeOut== 0)					//接收完成
		{
			//P1_1 = ~P1_1;
			//P1_0 = ~P1_0;
			gbUartRece = 0;							//清接收数据状态标志
			gbUartTran = 0;							//清发送数据状态标志
			gbUartRxdFrameReq = 1;					//设置接收完一个数据包标志
			g_NB.UARTReceiveOK = 1;
			REN = 0;								//禁止接收
		}
	}

	if(gbUartRece1)
	{
		gWUartRxdTimeOut1 --;
		if(gWUartRxdTimeOut1== 0)					//接收完成
		{
			//P1_1 = ~P1_1;
			//P1_0 = ~P1_0;
			gbUartRece1 = 0;							//清接收数据状态标志
			gbUartTran1 = 0;							//清发送数据状态标志
			gbUartRxdFrameReq1 = 1;					//设置接收完一个数据包标志
			REN1 = 0;								//禁止接收
		}
	}
}


/****************************************************************************
** 函数名称: void UARTAdjustPros(void)
** 函数描述: 数据接收完成处理, 当数据接收完成，进行接收完成处理
** 输入参数: 无
** 输出参数: 无
*****************************************************************************/
/* unsigned char UARTAdjustPros(unsigned char *Rx_Buff, unsigned char*Tx_Buff)
{
	unsigned char init_flag;
	if(*Rx_Buff == 0x68)
	{
		//通信地址忽略
		//CRC校验忽略
		if((*(Rx_Buff+7) == 0x68)
		{
			if(*(Rx_Buff+8) == 0x61)//写命令，初始化
			{
				if(*(Rx_Buff+9) == 0x04)//初始化
				{
					if((*(Rx_Buff+10) == 0xFF)&&(*(Rx_Buff+11) == 0xFF)&&(*(Rx_Buff+12) == 0xFF)&&(*(Rx_Buff+13) == 0xFF)\
					(*(Rx_Buff+14) == 0xFF))		init_flag = 1;
				}
				else if((*(Rx_Buff+9) == 0x05)&& init_flag)//写命令
				{
					if(*(Rx_Buff+9) == 0x05)
				}
			}
			else if(*(Rx_Buff+8) == 0x60)//读命令
			{

			}
		}
	}
} */

/****************************************************************************
** 函数名称: void UARTProcess(void)
** 函数描述: 数据接收完成处理, 当数据接收完成，进行接收完成处理
** 输入参数: 无
** 输出参数: 无
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
	if(_testbit_(gbUartRxdFrameReq1) )				//是否接收完一个数据包
	{
		Uart1Decode();

	}
}
/****************************************************************************
** 函数名称: UARTcmdCheck
** 函数描述: 计算校验位
** 输入参数: *Data 输入数组			Num 数组长度
** 输出参数: 校验位
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
** 函数名称: UART0_SendString
** 函数描述: UART0发送字符串
** 输入参数: 无
** 输出参数: 无
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
//	gbUartTran = 1;									//设置发送数据状态标志
//	TI = 1;
		REN = 1;
   // ES0=1;
}
/*******************************************************************************************
** 函数名称: UART0_SendString_Limit
** 函数描述: UART0发送字符串
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/
void  UART0_SendString_Limit(unsigned char *TXStr,unsigned char len)
{
	unsigned char i,len_q;//商数
	unsigned char len_r;//余数
	char dest[64];
	len_q = len/60;
	len_r = len%60;
	if(len_q)//长度大于50，分割发送
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
	else//小于50直接发送
		UART0_SendString((unsigned char*)TXStr,len);
	gBUartLen		= 0x00;				//清字节个数计数器
	gBUartTotalByte = 0x00;				//清数据字节总数寄存器
	gbUartTran	    = 0;				//清发送状态标志
	REN = 1;							//允许接收

}
/*******************************************************************************************
** 函数名称: UART1_SendString
** 函数描述: UART1发送字符串
** 输入参数: 无
** 输出参数: 无
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
** 函数名称: ClearBUFF
** 函数描述: Buff清零
** 输入参数: 要清零的Buff   该Buff长度
** 输出参数: 无
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
	//gbUartTran = 1;									//设置发送数据状态标志
	//TI = 1;											//软件置一，中断响应
}
/*******************************************************************************************
** 函数名称: ClearBUFF
** 函数描述: Buff清零
** 输入参数: 要清零的Buff   该Buff长度
** 输出参数: 无
*******************************************************************************************/
void ClearBUFF(unsigned char *cleardata, unsigned int len)
{
    unsigned int clear;
	for(clear=0;clear<len;clear++)
		cleardata[clear]=0x00;

	//gBUartCon  = Bin(00000000);
	//gbUartTran = 1;									//设置发送数据状态标志
	//TI = 1;											//软件置一，中断响应
}
/****************************************************************************
** 函数名称: Uart1Decode
** 函数描述: 串口1输入数据解码,485通信
** 输入参数: 无
** 输出参数: 无
*****************************************************************************/
void Uart1Decode(void)
{
unsigned char write_flag;
	unsigned int EE_addr;
	unsigned char EE_addrH;
	unsigned char EE_addrL;
	__U32_Def val;//EMU数据
	unsigned char buf[6];
	//unsigned char temp[4];
	//操作EEPROM
	if(UartRxBuf1[0] == 0xEE)
	{
		if(UartRxBuf1[1] == 0x01)//读
		{
			EE_addrH = UartRxBuf1[3];
			EE_addrL = UartRxBuf1[4];
			EE_addr = EE_addrH;
			EE_addr = (EE_addr<<8) + EE_addrL;
			//读数据
			EE_to_RAM(EE_addr, &UartTxBuf1[2], UartRxBuf1[2]);
			UartTxBuf1[0] = UartRxBuf1[0];
			UartTxBuf1[1] = UartRxBuf1[1];
			gBUartTotalByte1 = 2+UartRxBuf1[2];
			gBUartLen1		= 0x00;
		}
		else if(UartRxBuf1[1] == 0x02)//写
		{
			EE_addrH = UartRxBuf1[3];
			EE_addrL = UartRxBuf1[4];
			EE_addr = EE_addrH;
			EE_addr = (EE_addr<<8) + EE_addrL;
		 	VER_WRbytes(EE_addr,&UartRxBuf1[5],UartRxBuf1[2],1);	//写入数据
			//读出写入数据
			Delay_ms(100);
			EE_to_RAM(EE_addr, &UartTxBuf1[2], UartRxBuf1[2]);
			UartTxBuf1[0] = UartRxBuf1[0];
			UartTxBuf1[1] = UartRxBuf1[1];
			gBUartTotalByte1 = 2+UartRxBuf1[2];
			gBUartLen1		= 0x00;
		}
	}
	else if(UartRxBuf1[0] == 0xED)//操作EMU寄存器
	{
		if(UartRxBuf1[1] == 0x01)//读
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
		else if(UartRxBuf1[1] == 0x02)//写
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
			if(UartRxBuf1[2] == 0xFF)//写入厂家编号
			{
				param_data.meter_factory[0] = UartRxBuf1[3];
				SEQ_write(EE_Meter_Factory,&param_data.meter_factory[0],1);//向EEPROM中写入厂家编号
			}

			else if(UartRxBuf1[2] == 0xAA)//第一次输入表号
			{
				param_data.address_input_flag[0] = UartRxBuf1[3];
				param_data.address_input_flag[1] = UartRxBuf1[4];
				param_data.address_input_flag[2] = UartRxBuf1[5];
				param_data.address_input_flag[3] = UartRxBuf1[6];
				param_data.address_input_flag[4] = UartRxBuf1[7];
				param_data.address_input_flag[5] = UartRxBuf1[8];
				param_data.address_input_flag[6] = 0xA5;
			}
			else if(UartRxBuf1[2] == 0xBB)//第二次输入表号
			{

					buf[0] = UartRxBuf1[3];
					buf[1] = UartRxBuf1[4];
					buf[2] = UartRxBuf1[5];
					buf[3] = UartRxBuf1[6];
					buf[4] = UartRxBuf1[7];
					buf[5] = UartRxBuf1[8];

					param_data.address_input_flag[7] = 0xA5;


			}
			//两次都一致，发出表号
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
				VER_WRbytes(EE_Meter_address,&param_data.meter_address[0],6,1);//向EEPROM中写入表号
			}
			UartTxBuf1[0] = UartRxBuf1[0];
			UartTxBuf1[1] = UartRxBuf1[1];
			gBUartTotalByte1 = 2;
			gBUartLen1		= 0x00;
		}
	}
	else if(UartRxBuf1[0] == 0xFE)//校表写入
	{
		if(UartRxBuf1[1] == 0x02)//写
		{	if(UartRxBuf1[2]==0xD6)//通讯校表初始化
			{
				gbUartAdjust = 0xFF;
				write_flag = 1;
			}
			if (UartRxBuf1[2]==0xD7)//功率增益
			{
				ADJ_ID = Adj5_CHB_Power_GAIN;	//校表步骤Flag
				//符号位判断
				if(UartRxBuf1[3] == 0x01) Sign_Bit = 1;
				else Sign_Bit = 0;
				//误差处理
				Adj_Error = BCD2toINT(&UartRxBuf1[4]);
				write_flag = 1;
			}
			else if (UartRxBuf1[2]==0xD8)//相位1.0
			{
				ADJ_ID = Adj7_CHB_Phase;
				//符号位判断
				if(UartRxBuf1[3] == 0x01) Sign_Bit = 1;
				else Sign_Bit = 0;
				//误差处理
				Adj_Error = BCD2toINT(&UartRxBuf1[4]);
				write_flag = 1;
			}
			else if (UartRxBuf1[2]==0xD9)//功率offset
			{
				ADJ_ID = Adj9_CHB_Offset;
				//电流百分比
				Sign_Bit = BCD2HEX(UartRxBuf1[3]);
				//误差处理
				Adj_Error = BCD2toINT(&UartRxBuf1[4]);
				write_flag = 1;
			}
		}
		gBUartTotalByte1 = 3;							//设置发送字节数为4个
		gBUartLen1		= 0x00;							//清字节个数计数器
		UartTxBuf1[0] = 0xFE;	//帧头
		if(write_flag)	UartTxBuf1[1] = 0x02;
		else	UartTxBuf1[1] = 0x82;
		UartTxBuf1[2] = UartRxBuf1[2];
	}
	gBUartCon1  = Bin(00000000);
	gbUartTran1 = 1;									//设置发送数据状态标志
	TI1 = 1;											//软件置一，中断响应											//软件置一，中断响应
}


