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
** 函数名称: ReverseCpy
** 函数描述: 覆盖拷贝处理
** 输入参数: 无
** 输出参数: 无
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
** 函数名称: NBdata_SendString
** 函数描述: NB模块发送数据
** 输入参数: *TXStr 发送数据，数据长度len，目前应小于99个字符, *fasong要发送的数据
** 输出参数: 无
*******************************************************************************************/
void NBdata_SendString(unsigned char *TXStr,unsigned char len, char *fasong)
{
    //AT指令结构：AT+M2MCLISEND==<data>
    //char fasong[127];
	//char temp[MAX_UART_DATA_LEN];
    unsigned char changdu;
    //建立  AT指令
    sprintf(&fasong[0],NB_SEND_DATA_LWM2M_MTK);
    changdu = strlen(NB_SEND_DATA_LWM2M_MTK);
    //建立  <length>
    // if(len/10)
    // {
    //     fasong[changdu] = (len / 10) + 0x30;
    //     changdu++;
    // }
    // if (len%10)
    // {
    //     fasong[changdu] = (len % 10) + 0x30;
    //     changdu++;
    // }
    // //建立  ,
    // sprintf(&fasong[changdu],",");
    // changdu++;
    //建立  <data>
	HexGroupToString(&fasong[changdu],TXStr,len);
	changdu +=len*2;

    //建立  、换行回车
    sprintf(&fasong[changdu],"\r\n");
    changdu += 2;
    //发送
	//UART0_SendString((unsigned char*)fasong,strlen(fasong));
	// Delay_ms(10);//Test
	// UART1_SendString((unsigned char*)fasong,strlen(fasong));//test

}
/*******************************************************************************************
** 函数名称: ATcmd_Transmit
** 函数描述: 发送AT指令，并检测返回值
** 输入参数: *AT_str发送数据	*Repl_str期待返回值	   等待时间Wait_ms
** 输出参数: 1发送失败 0发送成功
*******************************************************************************************/
unsigned char ATcmd_Transmit(unsigned char *AT_str, char *Repl_str, unsigned int Wait_ms)
{
	unsigned char Rx_len;
	unsigned char Repl_len;
	unsigned char Repl_strlen;

	Repl_strlen = strlen(Repl_str)-1;//获取期待返回值长度

	Rx_len = 0;
	Repl_len= 0;
    //通过UART0发送AT指令
	UART0_SendString((unsigned char*)AT_str,strlen(AT_str));

	Delay_ms(300);//等待返回

	NBCount10ms = 0;//NB模块超时处理
	RSTSTAT &= Bin(11111000);//清看门狗
	while(Wait_ms>(NBCount10ms*10))
	{
		if(g_NB.UARTReceiveOK)//接收完成标志
		{
			while(UartRxBuf[Rx_len] == Repl_str[Repl_len]&&(Wait_ms>(NBCount10ms*10)))//对比一个字符
			{
				if(Repl_len == Repl_strlen)//所有期待值都符合
				{
					NBCount10ms = 0;
					g_NB.UARTReceiveOK = 0;
					REN = 1;
					ClearBUFF(UartRxBuf,countof(UartRxBuf));
					return 0;
				}
				RSTSTAT &= Bin(11111000);//清看门狗
				Rx_len++;
				Repl_len++;
				if(Repl_str[Repl_len] != UartRxBuf[Rx_len]) break;//不符合，跳出
			}
			RSTSTAT &= Bin(11111000);//清看门狗
			Repl_len = 0;
			Rx_len ++;
			if(Rx_len>gBUartLen+1) Rx_len=0;
		}
		RSTSTAT &= Bin(11111000);//清看门狗
	}
	g_NB.UARTReceiveOK = 0;
	REN = 1;
	ClearBUFF(UartRxBuf,countof(UartRxBuf));

	return 1;
}

/*******************************************************************************************
** 函数名称: NBdata_Transmit
** 函数描述: NB模块上传数据
** 输入参数: *Tx_str发送数据	TX_len发送长度	*Repl_str期待返回值	   Wait_ms等待时间
** 输出参数: 1发送失败 0发送成功
*******************************************************************************************/
unsigned char NBdata_Transmit(unsigned char *TX_str,unsigned char TX_len,char *Repl_str,unsigned int Wait_ms)
{
	unsigned char Rx_len;
	unsigned char Repl_len;
	unsigned char Repl_strlen;
	//unsigned char len_r;//余数
	unsigned char len;
//	char dest[64];
	char fasong[200];
	Repl_strlen = strlen(Repl_str)-1;//获取期待返回值长度
	Rx_len = 0;
	Repl_len= 0;
	NBdata_SendString((unsigned char*)TX_str,TX_len,fasong);
	len = strlen(fasong);
	//UART的缓冲BUFFER只有64byte，需要对发送字符串进行分割
	UART0_SendString_Limit(fasong,len);
	//发送数据，等待返回

	Delay_ms(300);//等待返回

	NBCount10ms=0;//NB模块超时处理
	RSTSTAT &= Bin(11111000);//清看门狗
	while(Wait_ms>(NBCount10ms*10))
	{
		if(g_NB.UARTReceiveOK)//接收完成标志
		{
			while(UartRxBuf[Rx_len] == Repl_str[Repl_len]&&(Wait_ms>(NBCount10ms*10)))//对比一个字符
			{
				if(Repl_len == Repl_strlen)//所有期待值都符合
				{
					NBCount10ms = 0;
					g_NB.UARTReceiveOK = 0;
					REN = 1;
					ClearBUFF(UartRxBuf,countof(UartRxBuf));

					return 0;
				}
				Rx_len++;
				Repl_len++;
				if(Repl_str[Repl_len] != UartRxBuf[Rx_len]) break;//不符合，跳出
			}
			RSTSTAT &= Bin(11111000);//清看门狗
			Repl_len = 0;
			Rx_len ++;
			if(Rx_len>gBUartLen+1) Rx_len=0;
		}
		RSTSTAT &= Bin(11111000);//清看门狗
	}
	g_NB.UARTReceiveOK = 0;
	REN = 1;
	ClearBUFF(UartRxBuf,countof(UartRxBuf));
	return 1;
}

/*******************************************************************************************
** 函数名称: NBstate_Receive
** 函数描述: nb模块返回的状态
** 输入参数: *AT_str 发送命令	*Repl_str期待返回值		*Get_str返回的状态	   Wait_ms等待时间
** 输出参数: 1发送失败 0发送成功
*******************************************************************************************/
unsigned char NBstate_Receive(unsigned char *AT_str, char *Repl_str,unsigned char *Get_str, unsigned int Wait_ms)
{	unsigned char Rx_len;
	unsigned char Repl_len;
	unsigned char Repl_strlen;
	unsigned char i;
	//unsigned char temp[10];

	Repl_strlen = strlen(Repl_str)-1;//获取期待返回值长度
	Rx_len = 0;
	Repl_len= 0;
    //通过UART0发送AT指令
	UART0_SendString((unsigned char*)AT_str,strlen(AT_str));
		Delay_ms(300);//等待返回

	NBCount10ms=0;//NB模块超时处理
	RSTSTAT &= Bin(11111000);//清看门狗
	while(Wait_ms>(NBCount10ms*10))
		//while(1)
	{
		if(g_NB.UARTReceiveOK)//接收完成标志
		{
			while(UartRxBuf[Rx_len] == Repl_str[Repl_len])//对比一个字符
			{
				if(Repl_len == Repl_strlen)//所有期待值都符合
				{
					//":"后面有空格，加一位
					for(i=0;i<gBUartLen-Rx_len-2;i++)
						Get_str[i] = UartRxBuf[i+Rx_len+2];
					//字符串函数能否在单片机中使用？
					// p = strtok((char *)UartRxBuf, " ");//分离收到的字符串
					// p = strtok(NULL, ":");//取后半段
					// strcpy(Get_str,p);
					NBCount10ms = 0;
					g_NB.UARTReceiveOK = 0;
					REN = 1;
					ClearBUFF(UartRxBuf,countof(UartRxBuf));
					return 0;
				}
				Rx_len++;
				Repl_len++;
				if(Repl_str[Repl_len] != UartRxBuf[Rx_len]) break;//不符合，跳出
			}
			RSTSTAT &= Bin(11111000);//清看门狗
			Repl_len = 0;
			Rx_len ++;
			if(Rx_len>gBUartLen+1) Rx_len=0;
		}
	}
	g_NB.UARTReceiveOK = 0;
	REN = 1;
	ClearBUFF(UartRxBuf,countof(UartRxBuf));
	return 1;

}
/*******************************************************************************************
** 函数名称: NBdata_Receive_MTK
** 函数描述: nb模块接收到的数据
** 输入参数: *Repl_str UART接收到的数据string		*Get_str 读出的数据HEX格式	*len 接收的数据长度
** 输出参数: 是否成功接收到数据
*******************************************************************************************/
unsigned char NBdata_Receive_MTK(char *Repl_str, unsigned char *Get_str, unsigned char *len)
{
    //假定为透传模式，平台发送数据为112233
    //BC26模块输入形式： "\r\n+QLWDATARECV: 19,1,0,7,112233\r\n"
     char dest[99];  //必须256
     char changdu[4] = {0x00,0x00,0x00,0x00}; //接收数据长度可为0-99以内
     char shuzu[] = NB_DATA_RECEIVE;    //标识字符
     char fuhao = 0;
     unsigned char changdu_len = 0;
     unsigned char Rx_len = 0;
     unsigned char shuzu_len = 0;
     unsigned char i;
	 REN = 0;
	while(1)
	{
		while (Repl_str[Rx_len] == shuzu[shuzu_len]) //对比一个字符
		{
			if (shuzu_len == strlen(shuzu)-1) //所有字符都符合
			{
				while(Rx_len<MAX_UART_DATA_LEN+1)
				{
				if(Repl_str[Rx_len] == ',') //","个数查询
				{
					fuhao++;
				}
				Rx_len++;
				if((fuhao == 3)&&(Repl_str[Rx_len] != ','))  //接收长度数据
				{
					changdu[changdu_len] = Repl_str[Rx_len];
					changdu_len++;
				}
				if(fuhao == 4) //接收数据
				{
					i = StrToInt(changdu)-1;
					i = i / 2;
					*len = i;
					Rx_len++;//去掉特殊字符
					for (i = 0;i<((*len)*2);i++)
					{
					dest[i] = Repl_str[Rx_len];
					Rx_len++;
					}
					StringToHexGroup(Get_str, dest, (*len)*2); //将数据转换为HEX格式

					return 1;
				}
				if (Rx_len > MAX_UART_DATA_LEN + 1)

				return 0;
				}
			}
			Rx_len++;
			shuzu_len++;
			if (Repl_str[Rx_len] != shuzu[shuzu_len])
			break; //不符合，跳出
		}
		shuzu_len = 0;
		Rx_len++;
		if (Rx_len > MAX_UART_DATA_LEN + 1)

		return 0;
	}
}
/*******************************************************************************************
** 函数名称: NB_Init
** 函数描述: nb模块初始化
** 输入参数: 无
** 输出参数: 无
*******************************************************************************************/

void NB_Init(void)
{
	char p[32];
	unsigned char recivedata[64];
	char imei[16];
	char *dest = p;

	if(g_NB.ReInitTime||!g_NB.InitStep)//没有到初始化时间，或者初始化结束
	{
		return;
	}
	else
	{
		//TEST
		// g_NB.InitState[0] = NB_Init_OK;
		// g_NB.InitState[1] = NB_Init_OK;
		if((g_NB.InitCount & 0xF0) == 0x30) //复位次数超过3次
		{
			//NB_Error();
			g_NB.InitCount = 0x00;
			g_NB.ReInitTime = NB_RETRY_TIME;
			g_NB.InitStep = 1;
			g_NB.InitState[0] = NB_Init_Reset;
			g_NB.InitState[1] = NB_Init_Reset;
			sprintf(dest,"AT+CFUN=0\r\n");//关机

			UART0_SendString((unsigned char *)dest,11);

			//显示警告
			g_Flag.ALARM[0] = 0x0f;
			g_Flag.ALARM[1] = 0x00;
			return;
		}
		if((g_NB.InitCount & 0x0F) > 0x09)//单步骤初始化次数等于10次
		{
			g_NB.InitCount &= 0xF0;			//低四位置零
			g_NB.InitCount += 0x10;			//高四位加一
			sprintf(dest,"AT+QRST=1\r\n");//复位
			UART0_SendString((unsigned char *)dest,11);
			g_NB.ReInitTime = 15;
			g_NB.InitStep = 1;
			return;
		}
		switch(g_NB.InitStep)
		{
			case 1:
			{
				g_NB.InitCount ++;
				g_NB.ReInitTime = 10;					//10秒后进行初始化
				sprintf(dest,"AT+SM=LOCK\r\n");			//关闭PSM模式

				if(!ATcmd_Transmit(dest,"OK",2000))
				{
					g_NB.InitCount &= 0xF0;			//低四位置零
					g_NB.InitStep = 2;					//进行初始化步骤2
					break;
				}
				else break;
			}
			case 2:
			{
				g_NB.InitCount ++;
				g_NB.ReInitTime = 30;					//10秒后进行初始化
				sprintf(dest,"AT+CFUN=1\r\n");


				if(!ATcmd_Transmit(dest,"OK",2000))
				{
					g_NB.InitCount &= 0xF0;			//低四位置零
					g_NB.InitStep = 3;					//进行初始化步骤3
					break;
				}
				else
					break;
			}
			case 3:
			{
				g_NB.InitCount ++;
				g_NB.ReInitTime = 10;					//10秒后进行初始化
				sprintf(dest,"AT+CGPADDR=1\r\n");		//查询模块联网状态
				//if(!ATcmd_Transmit(dest,"OK",2000))
				if(!NBstate_Receive(dest,"+CGPADDR:",recivedata,2000))
				{
					g_NB.InitCount &= 0xF0;			//低四位置零
					g_NB.InitStep = 4;					//进行初始化步骤4
					break;
				}
				else
				{
					g_NB.ReInitTime = 30;		//30秒后再次查询驻网状态
					break;
				}
			}
			case 4:
			{
				g_NB.InitCount ++;
				g_NB.ReInitTime = 10;					//10秒后进行初始化
				sprintf(dest,"AT+CGSN=1\r\n");			//获取模块IMEI号码
				if(!NBstate_Receive(dest,"+CGSN:",recivedata,2000))
				{
					strncpy((char *)imei, (char *)recivedata, 15);	//取得IEMI号码15位，
					imei[15] = '\0';					//加一位结束符"\0"
					if(strlen(imei)==15)				//判断IMEI号码的正确性
					{
						sprintf(dest,"AT+M2MCLINEW=%s,%s,\"%s\",90\r\n",SERVER_ADDRESS,SERVER_PORT,imei);//向IOT平台注册
						if(!ATcmd_Transmit(dest,"OK",2000))
						{
							g_NB.InitStep = 5;					//进行初始化步骤5
							g_NB.InitCount &= 0xF0;			//低四位置零
							break;
						}
						else		//注册失败，重启
						{
							sprintf(dest,"AT+QRST=1\r\n");
							UART0_SendString((unsigned char *)dest,11);
							g_NB.ReInitTime = 10;
							g_NB.InitStep = 1;
							break;
						}
					}
					else
					{
						break;
					}
				}

			}
			case 5:
			{
				g_NB.InitCount ++;
				g_NB.ReInitTime = 0;				//初始化完成
				sprintf(dest,"AT+CEREG?\r\n");
				if(!ATcmd_Transmit(dest,"OK",2000))
				{
					g_NB.InitStep = 0;				//初始化结束
					g_NB.InitCount &= 0xF0;			//低四位置零

					MemInitSet(&g_NB.InitState[0],NB_Init_OK,2);//置位 NBinit FLAG
					VER_WRbytes(EE_NB_STATE, &g_NB.InitState[0], 2, 1);
					//NB_Error();
					g_Tran.AutoReportFlag = COUNT_DOWN;
					g_Tran.AutoReportCount = 1;//1分钟后 发送上电握手信号
					break;
				}
				else break;
			}
			default:break;
		}/*switch finish*/
	}
}
/****************************************************************************
** 函数名称: NB_Error
** 函数描述: 检测NB模块的物理状态
** 输入参数: 无
** 输出参数: 无
*****************************************************************************/
void NB_Error(void)
{
	char p[32];
	char *dest = p;
	unsigned char get_char[32];
//	unsigned char temp[6];

	//检测SIM卡
	sprintf(dest,SIMCARD_DETECT);
	if(!ATcmd_Transmit((unsigned char *)dest,"OK",2000))
	{
		g_Flag.ALARM[0] = 0x00;
		g_Flag.ALARM[1] = 0x00;
	}
	else
	{
		g_Flag.ALARM[0] = 0x01;//无SIM卡或者SIM卡没插好
		g_Flag.ALARM[1] = 0x00;
		return;
	}
	//检测信号
	sprintf(dest,SINGAL_DETECT);
	NBstate_Receive((unsigned char *)dest,"+CSQ:",(char *)get_char,2000);
	dest = strtok((char *)get_char,",");
	if(strlen(dest)== 2)
	{
		if((get_char[0] == 0x30+9) && (get_char[1] == 0x30+9))//Return value: 99,99
		{
			g_Flag.ALARM[0] = 0x02;//SIM卡没插
			g_Flag.ALARM[1] = 0x00;
			return;
		}
	}
	else if(strlen(dest == 1))
	{
		if(get_char[0] < 0x30+5)//信号强度低 或者SIM卡未激活
		{
			g_Flag.ALARM[0] = 0x03;
			g_Flag.ALARM[1] = 0x00;
			return;
		}
	}
	//检测驻网
	sprintf(dest,NETWORK_DETECT);
	NBstate_Receive((unsigned char *)dest,"+CEREG: ",(char *)get_char,2000);
	if((get_char[2] == 0x30) ||	(get_char[2] == 0x32) || (get_char[2] == 0x34))//返回 +CEREG: 0,0 未能驻网
	{
		g_Flag.ALARM[0] = 0x04;
		g_Flag.ALARM[1] = 0x00;
		return;
	}
	else
	{
		g_Flag.ALARM[0] = 0x00;
		g_Flag.ALARM[1] = 0x00;
	}
	//IOT平台
	//无ACK回应


}
/****************************************************************************
** 函数名称: void check()
** 函数描述: 检测NB模块与物联网平台的状态
** 输入参数: 无
** 输出参数: NB模块状态
*****************************************************************************/
// unsigned char NB_check(void)
// {
// 	unsigned char number;
// 	unsigned char statu;
// 	char p[32];
// 	char Data_buffer[32];
// 	char *date;
// 	char *dest;

// 	dest=&p;
// 	statu=0;
// 	while(statu==0)
// 	{
// 		number++;
// 		if(number==3)
// 		{
// 			sprintf(dest,"AT+NRB\r\n");
// 			number=0;
// 			if(ATcmd_Transmit(dest,"OK",5000)) continue;
// 	   	}
// 		//发送查询命令
// 		sprintf(dest,"AT+NMSTATUS?\r\n");
// 		NBstate_Receive(dest,"+NMSTATUS:",Data_buffer,2000);
// 		date=&Data_buffer[0];
// 		//状态确定
// 		sprintf(dest,LWM2M_STATUS_1);
// 		if(strcmp(dest,date)==0) statu = 1;
// 		sprintf(dest,LWM2M_STATUS_2);
// 		if(strcmp(dest,date)==0) statu = 2;
// 		sprintf(dest,LWM2M_STATUS_3);
// 		if(strcmp(dest,date)==0) statu = 3;
// 	}
// 	return statu;
// }
/****************************************************************************
** 函数名称: NBdata_Transmit_Porcess
** 函数描述: NB模块发送程序
** 输入参数: TX_str发送数据 TX_len发送长度
** 输出参数: 无
*****************************************************************************/
// void NBdata_Transmit_Porcess(unsigned char *TX_str,unsigned char TX_len)
// {
// 	unsigned char send_process;	//NB发送上行数据程序
// 	unsigned char statu;		//NB联网状态
// 	unsigned char send_num;		//发送次数
// //	unsigned int  Waitms;
// 	char con_statu[3];

// 	send_process = 1;	//步骤1
// 	send_num = 1;
// 	while(send_process)
// 	{
// 		RSTSTAT &= Bin(11111000);//清看门狗
// 		switch(send_process)
// 		{
// 			case 1:	//检查联网状态
// 			{
// 				statu = NB_check();
// 				if(statu == 1) send_process = 2;//能够发送数据
// 				else NB_Init();//重新初始化
// 				break;
// 			}
// 			case 2://发送数据
// 			{
// 				NBdata_Transmit(TX_str,TX_len,"OK",500);
// 				send_num++;
// 				//延时3S
// 				Delay_ms(500);
// 				Delay_ms(500);
// 				RSTSTAT &= Bin(11111000);//清看门狗
// 				Delay_ms(500);
// 				Delay_ms(500);
// 				RSTSTAT &= Bin(11111000);//清看门狗
// 				Delay_ms(500);
// 				Delay_ms(500);
// 				RSTSTAT &= Bin(11111000);//清看门狗
// 			}
// 			case 3://检查发送状态
// 			{
// 				NBstate_Receive(CON_SENDING_STATUS,"QLWULDATASTATUS",con_statu,500);
// 				if(strcmp("4",con_statu)==0) send_process = 0; //发送完成
// 				else
// 				{
// 					if(send_num == 6) send_process = 0; //超过6次，不发送
// 					else if(send_num == 3) NB_Init();	//3次不成功，初始化NB
// 					send_process = 1;
// 				}
// 				break;
// 			}
// 		}/*switch finish*/
// 	}/*while finish*/
// }






