/*==============================================================================
                Copyright(C) 1997-2019.  Sinowealth Tech. Co., Ltd.
--------------------------------------------------------------------------------
Project Name  : EEPROM.c
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
#ifndef  EEPROM_C
	#define  EEPROM_C
#endif
//-------------------------------------------------------------------------------

//---------------

#include "config.h"
#include "Headconfig.h"
/*****************************************************************************************************
 *  Function Name: UCHAR EEPromByteRead(UCHAR nAddrH,UCHAR nAddrL)
 *  Created By:    Cathy.Feng
 *  Created Date:  2016-07-16
 *  Input:         nAddrH, nAddrL
 *  Output:        None
 *  Return:        nTemp
 *  Description:   从指定地址处读取1个byte返回
 *  Modified By:
 *  Modified Date:
 *  History:
 *  Others:
 *****************************************************************************************************/
unsigned char EEPromByteRead(unsigned char EE_addrH, unsigned char EE_addrL)         // 字节读
{
    unsigned char nTemp ;
    unsigned int  xdata adrval ;
    unsigned char code * nAddr ;
    Bit EA_BAK;

    EA_BAK = EA;                   		//保护EA
    EA = 0;							  	//关中断
    //地址范围0x0000~0x07FF; 0~2047
    FLASHCON = 1 ;
//   nInt = nAddrH ;
//   nInt <<= 8 ;
//   nInt += nAddrL ;
    adrval = EE_addrH;
    adrval = EE_addrH*256;
    adrval = adrval+EE_addrL;
    nAddr = (unsigned char code *)adrval;

    nTemp = *nAddr;

    FLASHCON = 0 ;
    EA = EA_BAK;                       //恢复EA
    return (nTemp) ;
}


//	一个扇区256个字节
/*****************************************************************************************************
 *  Function Name: void EEPromByteProgram(UCHAR nAddrH,UCHAR nAddrL, UCHAR nData)
 *  Created By:    Cathy.feng
 *  Created Date:  2016-07-16
 *  Input:         Sector，nAddr nData
 *  Output:        None
 *  Return:
 *  Description:   向指定sector的指定地址处写入1个byte
 *  Modified By:
 *  Modified Date:
 *  History:
 *  Others:
 *****************************************************************************************************/
void EEPromByteProgram(unsigned char nAddrH,unsigned char nAddrL, unsigned char nData)  // 字节编程
{

    Bit EA_BAK;

    EA_BAK = EA;                   		//保护EA
    EA = 0;

    FLASHCON = 1 ;


    XPAGE = nAddrH ;            //指定扇区
    IB_OFFSET = nAddrL ;          //指定扇区中地址
    IB_DATA = nData ;             //要写入的8bit数据

    IB_CON1  = 0x6E ;
    IB_CON2  = 0x05 ;
    IB_CON3  = 0x0A ;
    IB_CON4  = 0x09 ;
    IB_CON5  = 0x06 ;
    _nop_() ;
    _nop_() ;
    _nop_() ;
    _nop_() ;

    FLASHCON = 0 ;

    EA = EA_BAK;                       //恢复EA
}
/*****************************************************************************************************
 *  Function Name: void EEPromSectorErase(UCHAR nAddrH)
 *  Created By:    Cathy.Feng
 *  Created Date:  2016-07-19
 *  Input:         sector number.
 *  Output:        None
 *  Return:
 *  Description:
 *  Modified By:
 *  Modified Date:
 *  History:
 *  Others:
 *****************************************************************************************************/
void EEPromSectorErase(unsigned char nAddrH)// 扇区擦除
{
    Bit EA_BAK;

    EA_BAK = EA;                   		//保护EA
    EA = 0;
    FLASHCON = 1 ;

    XPAGE = nAddrH ;                  //0x00开始

    IB_CON1  = 0xE6 ;
    IB_CON2  = 0x05 ;
    IB_CON3  = 0x0A ;
    IB_CON4  = 0x09 ;
    IB_CON5  = 0x06 ;
    _nop_() ;
    _nop_() ;
    _nop_() ;
    _nop_() ;

    FLASHCON = 0 ;
    EA = EA_BAK;                       //恢复EA
}


////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: SEQ_write
** 函数描述: 从RAM连续写若干个数据至EEPROM
** 输入参数: E2起始地址EE_addr,E2设备地址FRAM_EEPROM,目标EE_buff地址,所读数量N
** 输出参数:
** 返    回：0-异常/1-正常
** 条    件：外部EEPROM的A0A1A2=0,模拟I2C串口,
*******************************************************************************************/
bit SEQ_write(unsigned int EE_addr, unsigned char *EE_buff, unsigned char N)
{
    unsigned char  i;


    bit flag=0;

	#if IIC_WP == ENABLE
    TRIS_WP_HI();
    WP=0;
	#endif


    do
    {
        I2C_stop();
        I2C_start();

    } while (!I2C_WR1byte(FRAM_EEPROM|((EE_addr>>7)&0x0e)));    //等待写设备地址

	if (I2C_WR1byte(EE_addr&0xff))                  			//设备地址低字节
	{

		for (i=0; i<N; i++)                             		//连续N个数据应答
		{

			if (!I2C_WR1byte(*EE_buff))
            {
               	I2C_stop();

				#if IIC_WP == ENABLE
    			WP=1;
				#endif

                return(0);
            }

	  		EE_buff++;
		}

		flag=1;
	}
	I2C_stop();

	#if IIC_WP == ENABLE
    WP=1;
	#endif

	return(flag);
}
////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: SEQ_write_limit
** 函数描述: 从RAM连续写若干个数据至EEPROM,一次性最大写入长度 16字节
** 输入参数: E2起始地址EE_addr,E2设备地址FRAM_EEPROM,目标EE_buff地址,所读数量N
** 输出参数:
** 返    回：0-异常/1-正常
** 条    件：外部EEPROM的A0A1A2=0,模拟I2C串口,
*******************************************************************************************/
bit SEQ_write_limit(unsigned int EE_addr, unsigned char *EE_buff, unsigned char N)
{
    unsigned char i;
    unsigned char len_q;//商数
    unsigned char len_r;//余数

    bit flag=0;
    len_q = N/16;
	len_r = N%16;

    if(len_q)//长度大于16字节，分割写入
    {
        for(i=0;i<len_q;i++)
		{
        flag =  SEQ_write(EE_addr+(i*16),EE_buff+(i*16),16);
        }
        flag = SEQ_write(EE_addr+(i*16),EE_buff+(i*16),len_r);
    }
    else //小于16字节直接写入
    {
        flag = SEQ_write(EE_addr,EE_buff,N);
    }

    return(flag);
}

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: EE_to_RAM
** 函数描述: 从EEPROM中连续读若干个数据
** 输入参数: E2起始地址EE_addr,E2设备地址FRAM_EEPROM,目标EE_buff地址,所读数量N
** 输出参数:
** 返    回：0-异常/1-正常
** 条    件：外部EEPROM的A0A1A2=0,模拟I2C串口,
*******************************************************************************************/
bit EE_to_RAM(unsigned int EE_addr,unsigned char *EE_buff, unsigned char N)
{
    unsigned char i;
    bit flag=0;

    do
    {
        I2C_stop();
        I2C_start();

    } while (!I2C_WR1byte(FRAM_EEPROM|((EE_addr>>7)&0x0e)));     	//等待写设备地址

	if (I2C_WR1byte(EE_addr&0xff))    								//设备地址
	{

		I2C_start();

		if (I2C_WR1byte((FRAM_EEPROM|((EE_addr>>7)&0x0e))|0x01)) 	//写设备地址的读
   		{

			for (i=0; i<N-1; i++)       							//连续N-1个数据应答
			{
				*EE_buff=I2C_RD1byte(0);

		  		EE_buff++;
			}

			*EE_buff=I2C_RD1byte(1);    							//第N个不应答

			flag=1;
		}
	}

	I2C_stop();

	return(flag);
}

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: VER_RDbytes
** 函数描述: 带校验从EEPROM中连续读若干个数据
** 输入参数: E2起始地址EE_addr,E2设备地址FRAM_EEPROM,目标EE_buff地址,所读数量N
** 输出参数: 正确的备份数据（或全错时的第2份）读到EE_buff连续地址
** 返    回：0-两份都错\1-第1份对\2-第2份对\ff异常
** 条    件：N<=100,第1份不对再读第二份
*******************************************************************************************/
unsigned char VER_RDbytes(unsigned int EE_addr, unsigned char *EE_buff, unsigned char N)
{
    unsigned char  xdata temp[D_EEP_BUF_LEN];
    unsigned char  xdata i, sum, xorb;

    if (!EE_to_RAM(EE_addr, &temp[0], N+2)) return(0xff);

    sum=temp[N];
    xorb=temp[N+1];

    SUM_xorb(&temp[0],N);

    if ((temp[N]==sum)&&(temp[N+1]==xorb))   							//第一份对
    {
        for (i=0; i<N; i++) *(EE_buff+i)=temp[i];
        return(1);
    }
    else
    {
        if (!EE_to_RAM(EE_addr+EE_BAKADDR, &temp[0], N+2)) return(0xff);

        sum=temp[N];
        xorb=temp[N+1];

        for (i=0; i<N; i++) *(EE_buff+i)=temp[i];

        SUM_xorb(&temp[0],N);

        if ((temp[N]==sum)&&(temp[N+1]==xorb))
		{
			return(2);				//第二份对
		}
        else
		{
			return(0);
		}
    }
}

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: VER_WRbytes
** 函数描述: 带校验写若干个数据到EEPROM中
** 输入参数: E2起始地址EE_addr,E2设备地址FRAM_EEPROM,寄存器源地址EE_buff,所读数量N,备份正确编号firstOK
** 输出参数: 双备份数据
** 返    回：无
** 条    件：N<=100
*******************************************************************************************/
void VER_WRbytes(unsigned int EE_addr, unsigned char *EE_buff, unsigned char N, unsigned char firstOK)
{
    unsigned char  xdata buff[D_EEP_BUF_LEN];
    unsigned char  xdata temp[D_EEP_BUF_LEN];
    unsigned char  xdata i;
    unsigned int xdata ADR_temp1, ADR_temp2;


    for (i=0; i<N; i++) buff[i]=*(EE_buff+i);
    SUM_xorb(&buff[0], N);       							//待写数据校验和

    if (firstOK!=1)             							//先第1份后第2份
    {
        ADR_temp1=EE_addr;
        ADR_temp2=EE_addr+EE_BAKADDR;
    }
    else                        							//先第2份后第1份
    {
        ADR_temp2=EE_addr;
        ADR_temp1=EE_addr+EE_BAKADDR;
    }

    EE_to_RAM(ADR_temp1, &temp[0], N+2);

    for (i=0; i<N+2; i++)   								//数据不同时写
    {
        if (buff[i]!=temp[i])
	    SEQ_write(ADR_temp1+i, &buff[i], 1);
    }

    EE_to_RAM(ADR_temp2, &temp[0], N+2);

    for (i=0; i<N+2; i++)   								//数据不同时写
    {
        if (buff[i]!=temp[i])
	    SEQ_write(ADR_temp2+i, &buff[i], 1);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: SUM_xorb
** 函数描述: 计算校验和(累加和异或)
** 输入参数: RAM起始地址,运算数量n
** 输出参数: n+1个RAM=累加和,n+2个RAM=异或码
** 返    回：无
** 条    件：无
*******************************************************************************************/
void SUM_xorb(unsigned char *RAM,unsigned char N)
{
	unsigned char i;

	*(RAM+N)=0;
	*(RAM+N+1)=0;

	for(i=0; i<N; i++)
    {
        *(RAM+N)+=~(*(RAM+i));
    	*(RAM+N+1)^=*(RAM+i);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: VER_CHK
** 函数描述: 检查备份数据正确性
** 输入参数: E2起始地址EE_addr,E2设备地址FRAM_EEPROM,所读数量N
** 输出参数: 正确的1份覆盖错误的备份
** 返    回：无
** 条    件：N<=100
*******************************************************************************************/
void VER_CHK(unsigned int EE_addr, unsigned char N)
{
    unsigned char  r, i,sum,xorb;
    unsigned int  ADR_temp;
    unsigned char  buff[D_EEP_BUF_LEN], temp[D_EEP_BUF_LEN];


    r=VER_RDbytes(EE_addr, &buff[0], N);            		//读出正确的1份
    SUM_xorb(&buff[0], N);

    if (r==1)
	{
		ADR_temp=EE_addr+EE_BAKADDR;
	}
    else if (r==2)
	{
		ADR_temp=EE_addr;
	}
    else
	{
		return;
	}

//    EE_to_RAM(ADR_temp, &temp[0], N+2);             		//读错误的1份到temp中
    if (!EE_to_RAM(ADR_temp, &temp[0], N+2))
	{
	    return;
	}

    sum=temp[N];
    xorb=temp[N+1];
    SUM_xorb(&temp[0],N);
    if ((temp[N]==sum)&&(temp[N+1]==xorb))            		//第一份对
    {
		for (i=0; i<N+2; i++)                           	//比较所有数据, 逐一纠错
		{
		   if (buff[i]!=temp[i])
		   SEQ_write(ADR_temp+i, &buff[i], 1);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: MemInitSet
** 函数描述: 将内存char数组的count个数初始化value值
** 输入参数: 数组地址，要更新的值，更新多少值
** 输出参数: 无
** 返    回：无
** 条    件：count<=65536
*******************************************************************************************/
 void MemInitSet(unsigned char *dst, unsigned char value, unsigned int count)
{
	unsigned int i;

	for(i=0;i<count;i++)
	{
		(*dst) = value;
		dst++;
	}
}




/*------------------------中断程序专用-----------------------------------*/

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: VER_RDbytes_INT
** 函数描述: 带校验从EEPROM中连续读若干个数据
** 输入参数: E2起始地址EE_addr,E2设备地址FRAM_EEPROM,目标EE_buff地址,所读数量N
** 输出参数: 正确的备份数据（或全错时的第2份）读到EE_buff连续地址
** 返    回：0-两份都错\1-第1份对\2-第2份对\ff异常
** 条    件：N<=100,第1份不对再读第二份
*******************************************************************************************/
 unsigned char VER_RDbytes_INT(unsigned int EE_addr, unsigned char *EE_buff, unsigned char N)
{
    unsigned char  xdata temp[D_EEP_BUF_LEN];
    unsigned char  xdata i, sum, xorb;

    if (!EE_to_RAM_INT(EE_addr, &temp[0], N+2)) return(0xff);

    sum=temp[N];
    xorb=temp[N+1];

    SUM_xorb_INT(&temp[0],N);

    if ((temp[N]==sum)&&(temp[N+1]==xorb))   							//第一份对
    {
        for (i=0; i<N; i++) *(EE_buff+i)=temp[i];
        return(1);
    }
    else
    {
        if (!EE_to_RAM_INT(EE_addr+EE_BAKADDR, &temp[0], N+2)) return(0xff);

        sum=temp[N];
        xorb=temp[N+1];

        for (i=0; i<N; i++) *(EE_buff+i)=temp[i];

        SUM_xorb_INT(&temp[0],N);

        if ((temp[N]==sum)&&(temp[N+1]==xorb))
		{
			return(2);				//第二份对
		}
        else
		{
			return(0);
		}
    }
}

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: VER_WRbytes
** 函数描述: 带校验写若干个数据到EEPROM中
** 输入参数: E2起始地址EE_addr,寄存器源地址EE_buff,所读数量N,备份正确编号firstOK
** 输出参数: 双备份数据
** 返    回：无
** 条    件：N<=100
*******************************************************************************************/
 void VER_WRbytes_INT(unsigned int EE_addr, unsigned char *EE_buff, unsigned char N, unsigned char firstOK)
{
    unsigned char  xdata buff[D_EEP_BUF_LEN];
    unsigned char  xdata temp[D_EEP_BUF_LEN];
    unsigned char  xdata i;
    unsigned int xdata ADR_temp1, ADR_temp2;


    for (i=0; i<N; i++) buff[i]=*(EE_buff+i);
    SUM_xorb_INT(&buff[0], N);       							//待写数据校验和

    if (firstOK!=1)             							//先第1份后第2份
    {
        ADR_temp1=EE_addr;
        ADR_temp2=EE_addr+EE_BAKADDR;
    }
    else                        							//先第2份后第1份
    {
        ADR_temp2=EE_addr;
        ADR_temp1=EE_addr+EE_BAKADDR;
    }

    EE_to_RAM_INT(ADR_temp1, &temp[0], N+2);

    for (i=0; i<N+2; i++)   								//数据不同时写
    {
        if (buff[i]!=temp[i])
	    SEQ_write_INT(ADR_temp1+i, &buff[i], 1);
    }

    EE_to_RAM_INT(ADR_temp2, &temp[0], N+2);

    for (i=0; i<N+2; i++)   								//数据不同时写
    {
        if (buff[i]!=temp[i])
	    SEQ_write_INT(ADR_temp2+i, &buff[i], 1);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: SUM_xorb
** 函数描述: 计算校验和(累加和异或)
** 输入参数: RAM起始地址,运算数量n
** 输出参数: n+1个RAM=累加和,n+2个RAM=异或码
** 返    回：无
** 条    件：无
*******************************************************************************************/
 void SUM_xorb_INT(unsigned char *RAM,unsigned char N)
{
	unsigned char i;

	*(RAM+N)=0;
	*(RAM+N+1)=0;

	for(i=0; i<N; i++)
    {
        *(RAM+N)+=~(*(RAM+i));
    	*(RAM+N+1)^=*(RAM+i);
    }
}


/*****************************************************************************************************
 *  Function Name: UCHAR EEPromByteRead(UCHAR nAddrH,UCHAR nAddrL)
 *  Created By:    Cathy.Feng
 *  Created Date:  2016-07-16
 *  Input:         nAddrH, nAddrL
 *  Output:        None
 *  Return:        nTemp
 *  Description:   从指定地址处读取1个byte返回
 *  Modified By:
 *  Modified Date:
 *  History:
 *  Others:
 *****************************************************************************************************/
unsigned char EEPromByteRead_INT(unsigned char EE_addrH, unsigned char EE_addrL)         // 字节读
{
    unsigned char nTemp ;
    unsigned int  xdata adrval ;
    unsigned char code * nAddr ;
    Bit EA_BAK;

    EA_BAK = EA;                   		//保护EA
    EA = 0;							  	//关中断
    //地址范围0x0000~0x07FF; 0~2047
    FLASHCON = 1 ;
//   nInt = nAddrH ;
//   nInt <<= 8 ;
//   nInt += nAddrL ;
    adrval = EE_addrH;
    adrval = EE_addrH*256;
    adrval = adrval+EE_addrL;
    nAddr = (unsigned char code *)adrval;

    nTemp = *nAddr;

    FLASHCON = 0 ;
    EA = EA_BAK;                       //恢复EA
    return (nTemp) ;
}


//	一个扇区256个字节
/*****************************************************************************************************
 *  Function Name: void EEPromByteProgram(UCHAR nAddrH,UCHAR nAddrL, UCHAR nData)
 *  Created By:    Cathy.feng
 *  Created Date:  2016-07-16
 *  Input:         Sector，nAddr nData
 *  Output:        None
 *  Return:
 *  Description:   向指定sector的指定地址处写入1个byte
 *  Modified By:
 *  Modified Date:
 *  History:
 *  Others:
 *****************************************************************************************************/
void EEPromByteProgram_INT(unsigned char nAddrH,unsigned char nAddrL, unsigned char nData)  // 字节编程
{
 Bit EA_BAK;

    EA_BAK = EA;                   		//保护EA
    EA = 0;

    FLASHCON = 1 ;


    XPAGE = nAddrH ;            //指定扇区
    IB_OFFSET = nAddrL ;          //指定扇区中地址
    IB_DATA = nData ;             //要写入的8bit数据

    IB_CON1  = 0x6E ;
    IB_CON2  = 0x05 ;
    IB_CON3  = 0x0A ;
    IB_CON4  = 0x09 ;
    IB_CON5  = 0x06 ;
    _nop_() ;
    _nop_() ;
    _nop_() ;
    _nop_() ;

    FLASHCON = 0 ;

    EA = EA_BAK;                       //恢复EA
}

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: SEQ_write
** 函数描述: 从RAM连续写若干个数据至EEPROM
** 输入参数: E2要写入的地址EE_addr,目标数据EE_buff指针,所写数量N
** 输出参数:
** 返    回：0-异常/1-正常
** 条    件：
*******************************************************************************************/
 bit SEQ_write_INT(unsigned int EE_addr, unsigned char *EE_buff, unsigned char N)
{
    unsigned char  i;
    unsigned char EE_addrH;     //写入地址高字节，扇区
    unsigned char EE_addrL;     //写入地址低字节，扇区中地址
    bit flag=0;
    EE_addrL = EE_addr&0xf;
    EE_addrH = (EE_addr>>8)&0x0f;
    if (EE_addrH > 8) return (flag); //超出扇区范围直接返回;
    if ((EE_addrL+N) > 256) return (flag); //超出地址范围直接返回

    for(i=0;i<N-1;i++)
    {
        EEPromByteProgram_INT(EE_addrH, EE_addrL+i, *EE_buff);
        EE_buff++;
    }
	flag=1;

	return(flag);
}

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** 函数名称: EE_to_RAM
** 函数描述: 从EEPROM中连续读若干个数据
** 输入参数: E2起始地址EE_addr,E2设备地址FRAM_EEPROM,目标EE_buff地址,所读数量N
** 输出参数:
** 返    回：0-异常/1-正常
** 条    件：外部EEPROM的A0A1A2=0,模拟I2C串口,
*******************************************************************************************/
 bit EE_to_RAM_INT(unsigned int EE_addr,unsigned char *EE_buff, unsigned char N)
{
    unsigned char i;
    unsigned char EE_addrH;
    unsigned char EE_addrL;
    bit flag=0;
    if(EE_addrH>8) return (flag);
    if((EE_addrL+N)>256) return (flag); //超出地址直接返回

    EE_addrH = (EE_addr>>8)&0xFF;
    EE_addrL = EE_addr&0x00FF;
    for(i=0;i<N-1;i++)
    {
        *EE_buff = EEPromByteRead_INT(EE_addrH, EE_addrL + i);
        EE_buff++;
    }
    flag=1;

	return(flag);
}
