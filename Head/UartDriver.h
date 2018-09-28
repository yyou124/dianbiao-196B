/*==============================================================================
            Copyright(C) 1997-2019.  Sinowealth Tech. Co., Ltd.
--------------------------------------------------------------------------------
Project No.   : XXX
File Name     : UARTDRIVER..H
Description   :
              ；
              ；
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
#ifndef	UARTDRIVER_H
#define	UARTDRIVER_H

#ifndef UARTDRIVER_C
	#define UARTDRIVER_EXT	extern
#else
	#define UARTDRIVER_EXT
#endif
//------------------------------------------------------------------------------

//----------------



//UART
#define     UART_BAUDRATE_300		     (32768-FSYSCLK/16/300)
#define     UART_BAUDRATE_600		     (32768-FSYSCLK/16/600)
#define	    UART_BAUDRATE_1200	         (32768-FSYSCLK/16/1200)
#define     UART_BAUDRATE_2400           (32768-FSYSCLK/16/2400)
#define     UART_BAUDRATE_4800           (32768-FSYSCLK/16/4800)
#define     UART_BAUDRATE_9600           (32768-FSYSCLK/16/9600)
#define     UART_BAUDRATE_19200          (32768-FSYSCLK/16/19200)
#define     UART_BAUDRATE_115200         (32768-FSYSCLK/16/115200)

#define     UART_BFINE_300               ((FSYSCLK/300) - 16*(32768 - UART_BAUDRATE_300))   //手算为7，需验证
#define     UART_BFINE_600               ((FSYSCLK/600) - 16*(32768 - UART_BAUDRATE_600))
#define     UART_BFINE_1200              ((FSYSCLK/1200) - 16*(32768 - UART_BAUDRATE_1200))
#define     UART_BFINE_2400              ((FSYSCLK/2400) - 16*(32768 - UART_BAUDRATE_2400))
#define     UART_BFINE_4800              ((FSYSCLK/4800) - 16*(32768 - UART_BAUDRATE_4800))
#define     UART_BFINE_9600              ((FSYSCLK/9600) - 16*(32768 - UART_BAUDRATE_9600))
#define     UART_BFINE_19200             ((FSYSCLK/19200) - 16*(32768 - UART_BAUDRATE_19200))
#define     UART_BFINE_115200            ((FSYSCLK/115200) - 16*(32768 - UART_BAUDRATE_19200))

#define     UART0_BAUDRATE               UART_BAUDRATE_2400		    //UART波特率2400bps
#define     UART1_BAUDRATE               UART_BAUDRATE_2400		    //UART波特率2400bps

#define		IR_ON						 0
#define     IR_38K                       (FSYSCLK/38000/4-1)


#define		BAUDRATE_600			0x02				//baud rate index value set:
#define		BAUDRATE_1200			0x04
#define		BAUDRATE_2400			0x08
#define		BAUDRATE_4800			0x10
#define		BAUDRATE_9600			0x20
#define		BAUDRATE_19200			0x40				//Z character



UARTDRIVER_EXT  void Init_Uart(void);
UARTDRIVER_EXT	void UartInterface(void);
UARTDRIVER_EXT	void UartRxdTimeout(void);
UARTDRIVER_EXT	void UART1Process(void);
UARTDRIVER_EXT  void Uart1Decode(void);
UARTDRIVER_EXT  unsigned char UARTcmdCheck(unsigned char *Data, unsigned char Num);
UARTDRIVER_EXT  void  UART0_SendString(unsigned char *TXStr,unsigned char len);
UARTDRIVER_EXT void  UART0_SendString_Limit(unsigned char *TXStr,unsigned char len);
UARTDRIVER_EXT  void  UART1_SendString(unsigned char *TXStr,unsigned char len);
UARTDRIVER_EXT unsigned char  CmpBUFF(unsigned char *data1, unsigned char *data2, unsigned int len);
UARTDRIVER_EXT  void ClearBUFF(unsigned char *cleardata, unsigned int len);
//UARTDRIVER_EXT void test_nb(void);


//  UARTDRIVER_EXT bit gbFgUart5ms;
//  UARTDRIVER_EXT unsigned char xdata gbUartBuff[80];
//  UARTDRIVER_EXT unsigned char xdata gb485Buff[80];
//  UARTDRIVER_EXT unsigned char xdata gb485Len;
//  UARTDRIVER_EXT unsigned char xdata gb485TxdLen;

#define     MAX_UART_DATA_LEN       128
#define     PACKAGE_RXD_TIMEOUT     100
#define     PACKAGE_RXD_TIMEOUT1     20
UARTDRIVER_EXT unsigned char xdata Res;
UARTDRIVER_EXT unsigned char xdata Res1;
UARTDRIVER_EXT unsigned int volatile xdata ADJ_ID;//校表步骤
UARTDRIVER_EXT unsigned char volatile xdata Sign_Bit;//Uart误差符号位
UARTDRIVER_EXT unsigned int  volatile xdata Adj_Error;//Uart误差XX.XX%
UARTDRIVER_EXT unsigned char xdata gbUartAdjust;//通讯校表启动标志


extern unsigned char 	UartTxBuf1[MAX_UART_DATA_LEN];				//接收数据缓冲器
extern unsigned char   UartRxBuf1[MAX_UART_DATA_LEN];				//发送数据缓冲器
extern unsigned char data	gBUartLen;									//字节个数计数器
extern unsigned char data	gBUartLen1;									//字节个数计数器
extern unsigned int data	gWUartRxdTimeOut;							//传送一个数据包超时计时器
/*
UARTDRIVER_EXT unsigned char UART_Rx_Flag;
UARTDRIVER_EXT unsigned char xdata RxBuffer[RxBufferSize];
UARTDRIVER_EXT unsigned char UART_RX_NUM;
*/



#endif