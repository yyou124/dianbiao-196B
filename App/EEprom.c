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
 *  Description:   ��ָ����ַ����ȡ1��byte����
 *  Modified By:
 *  Modified Date:
 *  History:
 *  Others:
 *****************************************************************************************************/
unsigned char EEPromByteRead(unsigned char EE_addrH, unsigned char EE_addrL)         // �ֽڶ�
{
    unsigned char nTemp ;
    unsigned int  xdata adrval ;
    unsigned char code * nAddr ;
    Bit EA_BAK;

    EA_BAK = EA;                   		//����EA
    EA = 0;							  	//���ж�
    //��ַ��Χ0x0000~0x07FF; 0~2047
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
    EA = EA_BAK;                       //�ָ�EA
    return (nTemp) ;
}


//	һ������256���ֽ�
/*****************************************************************************************************
 *  Function Name: void EEPromByteProgram(UCHAR nAddrH,UCHAR nAddrL, UCHAR nData)
 *  Created By:    Cathy.feng
 *  Created Date:  2016-07-16
 *  Input:         Sector��nAddr nData
 *  Output:        None
 *  Return:
 *  Description:   ��ָ��sector��ָ����ַ��д��1��byte
 *  Modified By:
 *  Modified Date:
 *  History:
 *  Others:
 *****************************************************************************************************/
void EEPromByteProgram(unsigned char nAddrH,unsigned char nAddrL, unsigned char nData)  // �ֽڱ��
{

    Bit EA_BAK;

    EA_BAK = EA;                   		//����EA
    EA = 0;

    FLASHCON = 1 ;


    XPAGE = nAddrH ;            //ָ������
    IB_OFFSET = nAddrL ;          //ָ�������е�ַ
    IB_DATA = nData ;             //Ҫд���8bit����

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

    EA = EA_BAK;                       //�ָ�EA
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
void EEPromSectorErase(unsigned char nAddrH)// ��������
{
    Bit EA_BAK;

    EA_BAK = EA;                   		//����EA
    EA = 0;
    FLASHCON = 1 ;

    XPAGE = nAddrH ;                  //0x00��ʼ

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
    EA = EA_BAK;                       //�ָ�EA
}


////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** ��������: SEQ_write
** ��������: ��RAM����д���ɸ�������EEPROM
** �������: E2��ʼ��ַEE_addr,E2�豸��ַFRAM_EEPROM,Ŀ��EE_buff��ַ,��������N
** �������:
** ��    �أ�0-�쳣/1-����
** ��    �����ⲿEEPROM��A0A1A2=0,ģ��I2C����,
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

    } while (!I2C_WR1byte(FRAM_EEPROM|((EE_addr>>7)&0x0e)));    //�ȴ�д�豸��ַ

	if (I2C_WR1byte(EE_addr&0xff))                  			//�豸��ַ���ֽ�
	{

		for (i=0; i<N; i++)                             		//����N������Ӧ��
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
** ��������: SEQ_write_limit
** ��������: ��RAM����д���ɸ�������EEPROM,һ�������д�볤�� 16�ֽ�
** �������: E2��ʼ��ַEE_addr,E2�豸��ַFRAM_EEPROM,Ŀ��EE_buff��ַ,��������N
** �������:
** ��    �أ�0-�쳣/1-����
** ��    �����ⲿEEPROM��A0A1A2=0,ģ��I2C����,
*******************************************************************************************/
bit SEQ_write_limit(unsigned int EE_addr, unsigned char *EE_buff, unsigned char N)
{
    unsigned char i;
    unsigned char len_q;//����
    unsigned char len_r;//����

    bit flag=0;
    len_q = N/16;
	len_r = N%16;

    if(len_q)//���ȴ���16�ֽڣ��ָ�д��
    {
        for(i=0;i<len_q;i++)
		{
        flag =  SEQ_write(EE_addr+(i*16),EE_buff+(i*16),16);
        }
        flag = SEQ_write(EE_addr+(i*16),EE_buff+(i*16),len_r);
    }
    else //С��16�ֽ�ֱ��д��
    {
        flag = SEQ_write(EE_addr,EE_buff,N);
    }

    return(flag);
}

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** ��������: EE_to_RAM
** ��������: ��EEPROM�����������ɸ�����
** �������: E2��ʼ��ַEE_addr,E2�豸��ַFRAM_EEPROM,Ŀ��EE_buff��ַ,��������N
** �������:
** ��    �أ�0-�쳣/1-����
** ��    �����ⲿEEPROM��A0A1A2=0,ģ��I2C����,
*******************************************************************************************/
bit EE_to_RAM(unsigned int EE_addr,unsigned char *EE_buff, unsigned char N)
{
    unsigned char i;
    bit flag=0;

    do
    {
        I2C_stop();
        I2C_start();

    } while (!I2C_WR1byte(FRAM_EEPROM|((EE_addr>>7)&0x0e)));     	//�ȴ�д�豸��ַ

	if (I2C_WR1byte(EE_addr&0xff))    								//�豸��ַ
	{

		I2C_start();

		if (I2C_WR1byte((FRAM_EEPROM|((EE_addr>>7)&0x0e))|0x01)) 	//д�豸��ַ�Ķ�
   		{

			for (i=0; i<N-1; i++)       							//����N-1������Ӧ��
			{
				*EE_buff=I2C_RD1byte(0);

		  		EE_buff++;
			}

			*EE_buff=I2C_RD1byte(1);    							//��N����Ӧ��

			flag=1;
		}
	}

	I2C_stop();

	return(flag);
}

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** ��������: VER_RDbytes
** ��������: ��У���EEPROM�����������ɸ�����
** �������: E2��ʼ��ַEE_addr,E2�豸��ַFRAM_EEPROM,Ŀ��EE_buff��ַ,��������N
** �������: ��ȷ�ı������ݣ���ȫ��ʱ�ĵ�2�ݣ�����EE_buff������ַ
** ��    �أ�0-���ݶ���\1-��1�ݶ�\2-��2�ݶ�\ff�쳣
** ��    ����N<=100,��1�ݲ����ٶ��ڶ���
*******************************************************************************************/
unsigned char VER_RDbytes(unsigned int EE_addr, unsigned char *EE_buff, unsigned char N)
{
    unsigned char  xdata temp[D_EEP_BUF_LEN];
    unsigned char  xdata i, sum, xorb;

    if (!EE_to_RAM(EE_addr, &temp[0], N+2)) return(0xff);

    sum=temp[N];
    xorb=temp[N+1];

    SUM_xorb(&temp[0],N);

    if ((temp[N]==sum)&&(temp[N+1]==xorb))   							//��һ�ݶ�
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
			return(2);				//�ڶ��ݶ�
		}
        else
		{
			return(0);
		}
    }
}

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** ��������: VER_WRbytes
** ��������: ��У��д���ɸ����ݵ�EEPROM��
** �������: E2��ʼ��ַEE_addr,E2�豸��ַFRAM_EEPROM,�Ĵ���Դ��ַEE_buff,��������N,������ȷ���firstOK
** �������: ˫��������
** ��    �أ���
** ��    ����N<=100
*******************************************************************************************/
void VER_WRbytes(unsigned int EE_addr, unsigned char *EE_buff, unsigned char N, unsigned char firstOK)
{
    unsigned char  xdata buff[D_EEP_BUF_LEN];
    unsigned char  xdata temp[D_EEP_BUF_LEN];
    unsigned char  xdata i;
    unsigned int xdata ADR_temp1, ADR_temp2;


    for (i=0; i<N; i++) buff[i]=*(EE_buff+i);
    SUM_xorb(&buff[0], N);       							//��д����У���

    if (firstOK!=1)             							//�ȵ�1�ݺ��2��
    {
        ADR_temp1=EE_addr;
        ADR_temp2=EE_addr+EE_BAKADDR;
    }
    else                        							//�ȵ�2�ݺ��1��
    {
        ADR_temp2=EE_addr;
        ADR_temp1=EE_addr+EE_BAKADDR;
    }

    EE_to_RAM(ADR_temp1, &temp[0], N+2);

    for (i=0; i<N+2; i++)   								//���ݲ�ͬʱд
    {
        if (buff[i]!=temp[i])
	    SEQ_write(ADR_temp1+i, &buff[i], 1);
    }

    EE_to_RAM(ADR_temp2, &temp[0], N+2);

    for (i=0; i<N+2; i++)   								//���ݲ�ͬʱд
    {
        if (buff[i]!=temp[i])
	    SEQ_write(ADR_temp2+i, &buff[i], 1);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** ��������: SUM_xorb
** ��������: ����У���(�ۼӺ����)
** �������: RAM��ʼ��ַ,��������n
** �������: n+1��RAM=�ۼӺ�,n+2��RAM=�����
** ��    �أ���
** ��    ������
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
** ��������: VER_CHK
** ��������: ��鱸��������ȷ��
** �������: E2��ʼ��ַEE_addr,E2�豸��ַFRAM_EEPROM,��������N
** �������: ��ȷ��1�ݸ��Ǵ���ı���
** ��    �أ���
** ��    ����N<=100
*******************************************************************************************/
void VER_CHK(unsigned int EE_addr, unsigned char N)
{
    unsigned char  r, i,sum,xorb;
    unsigned int  ADR_temp;
    unsigned char  buff[D_EEP_BUF_LEN], temp[D_EEP_BUF_LEN];


    r=VER_RDbytes(EE_addr, &buff[0], N);            		//������ȷ��1��
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

//    EE_to_RAM(ADR_temp, &temp[0], N+2);             		//�������1�ݵ�temp��
    if (!EE_to_RAM(ADR_temp, &temp[0], N+2))
	{
	    return;
	}

    sum=temp[N];
    xorb=temp[N+1];
    SUM_xorb(&temp[0],N);
    if ((temp[N]==sum)&&(temp[N+1]==xorb))            		//��һ�ݶ�
    {
		for (i=0; i<N+2; i++)                           	//�Ƚ���������, ��һ����
		{
		   if (buff[i]!=temp[i])
		   SEQ_write(ADR_temp+i, &buff[i], 1);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** ��������: MemInitSet
** ��������: ���ڴ�char�����count������ʼ��valueֵ
** �������: �����ַ��Ҫ���µ�ֵ�����¶���ֵ
** �������: ��
** ��    �أ���
** ��    ����count<=65536
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




/*------------------------�жϳ���ר��-----------------------------------*/

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** ��������: VER_RDbytes_INT
** ��������: ��У���EEPROM�����������ɸ�����
** �������: E2��ʼ��ַEE_addr,E2�豸��ַFRAM_EEPROM,Ŀ��EE_buff��ַ,��������N
** �������: ��ȷ�ı������ݣ���ȫ��ʱ�ĵ�2�ݣ�����EE_buff������ַ
** ��    �أ�0-���ݶ���\1-��1�ݶ�\2-��2�ݶ�\ff�쳣
** ��    ����N<=100,��1�ݲ����ٶ��ڶ���
*******************************************************************************************/
 unsigned char VER_RDbytes_INT(unsigned int EE_addr, unsigned char *EE_buff, unsigned char N)
{
    unsigned char  xdata temp[D_EEP_BUF_LEN];
    unsigned char  xdata i, sum, xorb;

    if (!EE_to_RAM_INT(EE_addr, &temp[0], N+2)) return(0xff);

    sum=temp[N];
    xorb=temp[N+1];

    SUM_xorb_INT(&temp[0],N);

    if ((temp[N]==sum)&&(temp[N+1]==xorb))   							//��һ�ݶ�
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
			return(2);				//�ڶ��ݶ�
		}
        else
		{
			return(0);
		}
    }
}

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** ��������: VER_WRbytes
** ��������: ��У��д���ɸ����ݵ�EEPROM��
** �������: E2��ʼ��ַEE_addr,�Ĵ���Դ��ַEE_buff,��������N,������ȷ���firstOK
** �������: ˫��������
** ��    �أ���
** ��    ����N<=100
*******************************************************************************************/
 void VER_WRbytes_INT(unsigned int EE_addr, unsigned char *EE_buff, unsigned char N, unsigned char firstOK)
{
    unsigned char  xdata buff[D_EEP_BUF_LEN];
    unsigned char  xdata temp[D_EEP_BUF_LEN];
    unsigned char  xdata i;
    unsigned int xdata ADR_temp1, ADR_temp2;


    for (i=0; i<N; i++) buff[i]=*(EE_buff+i);
    SUM_xorb_INT(&buff[0], N);       							//��д����У���

    if (firstOK!=1)             							//�ȵ�1�ݺ��2��
    {
        ADR_temp1=EE_addr;
        ADR_temp2=EE_addr+EE_BAKADDR;
    }
    else                        							//�ȵ�2�ݺ��1��
    {
        ADR_temp2=EE_addr;
        ADR_temp1=EE_addr+EE_BAKADDR;
    }

    EE_to_RAM_INT(ADR_temp1, &temp[0], N+2);

    for (i=0; i<N+2; i++)   								//���ݲ�ͬʱд
    {
        if (buff[i]!=temp[i])
	    SEQ_write_INT(ADR_temp1+i, &buff[i], 1);
    }

    EE_to_RAM_INT(ADR_temp2, &temp[0], N+2);

    for (i=0; i<N+2; i++)   								//���ݲ�ͬʱд
    {
        if (buff[i]!=temp[i])
	    SEQ_write_INT(ADR_temp2+i, &buff[i], 1);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** ��������: SUM_xorb
** ��������: ����У���(�ۼӺ����)
** �������: RAM��ʼ��ַ,��������n
** �������: n+1��RAM=�ۼӺ�,n+2��RAM=�����
** ��    �أ���
** ��    ������
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
 *  Description:   ��ָ����ַ����ȡ1��byte����
 *  Modified By:
 *  Modified Date:
 *  History:
 *  Others:
 *****************************************************************************************************/
unsigned char EEPromByteRead_INT(unsigned char EE_addrH, unsigned char EE_addrL)         // �ֽڶ�
{
    unsigned char nTemp ;
    unsigned int  xdata adrval ;
    unsigned char code * nAddr ;
    Bit EA_BAK;

    EA_BAK = EA;                   		//����EA
    EA = 0;							  	//���ж�
    //��ַ��Χ0x0000~0x07FF; 0~2047
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
    EA = EA_BAK;                       //�ָ�EA
    return (nTemp) ;
}


//	һ������256���ֽ�
/*****************************************************************************************************
 *  Function Name: void EEPromByteProgram(UCHAR nAddrH,UCHAR nAddrL, UCHAR nData)
 *  Created By:    Cathy.feng
 *  Created Date:  2016-07-16
 *  Input:         Sector��nAddr nData
 *  Output:        None
 *  Return:
 *  Description:   ��ָ��sector��ָ����ַ��д��1��byte
 *  Modified By:
 *  Modified Date:
 *  History:
 *  Others:
 *****************************************************************************************************/
void EEPromByteProgram_INT(unsigned char nAddrH,unsigned char nAddrL, unsigned char nData)  // �ֽڱ��
{
 Bit EA_BAK;

    EA_BAK = EA;                   		//����EA
    EA = 0;

    FLASHCON = 1 ;


    XPAGE = nAddrH ;            //ָ������
    IB_OFFSET = nAddrL ;          //ָ�������е�ַ
    IB_DATA = nData ;             //Ҫд���8bit����

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

    EA = EA_BAK;                       //�ָ�EA
}

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** ��������: SEQ_write
** ��������: ��RAM����д���ɸ�������EEPROM
** �������: E2Ҫд��ĵ�ַEE_addr,Ŀ������EE_buffָ��,��д����N
** �������:
** ��    �أ�0-�쳣/1-����
** ��    ����
*******************************************************************************************/
 bit SEQ_write_INT(unsigned int EE_addr, unsigned char *EE_buff, unsigned char N)
{
    unsigned char  i;
    unsigned char EE_addrH;     //д���ַ���ֽڣ�����
    unsigned char EE_addrL;     //д���ַ���ֽڣ������е�ַ
    bit flag=0;
    EE_addrL = EE_addr&0xf;
    EE_addrH = (EE_addr>>8)&0x0f;
    if (EE_addrH > 8) return (flag); //����������Χֱ�ӷ���;
    if ((EE_addrL+N) > 256) return (flag); //������ַ��Χֱ�ӷ���

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
** ��������: EE_to_RAM
** ��������: ��EEPROM�����������ɸ�����
** �������: E2��ʼ��ַEE_addr,E2�豸��ַFRAM_EEPROM,Ŀ��EE_buff��ַ,��������N
** �������:
** ��    �أ�0-�쳣/1-����
** ��    �����ⲿEEPROM��A0A1A2=0,ģ��I2C����,
*******************************************************************************************/
 bit EE_to_RAM_INT(unsigned int EE_addr,unsigned char *EE_buff, unsigned char N)
{
    unsigned char i;
    unsigned char EE_addrH;
    unsigned char EE_addrL;
    bit flag=0;
    if(EE_addrH>8) return (flag);
    if((EE_addrL+N)>256) return (flag); //������ֱַ�ӷ���

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
