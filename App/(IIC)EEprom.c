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

    for()
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