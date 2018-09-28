/*==============================================================================
                Copyright(C) 1997-2019.  Sinowealth Tech. Co., Ltd.
--------------------------------------------------------------------------------
Project Name  : CRC.c
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
#ifndef  CRC_C
	#define  CRC_C
#endif
//-------------------------------------------------------------------------------

//---------------

#include "config.h"
#include "Headconfig.h"




////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** Function name:       Calc_CS
** Descriptions:        ��У��
** input parameters:    unsigned char  *CS_Buffer : ���ݻ������׵�ַ
    	    	    	unsigned short Len��        ���ݳ���
** Returned value:      CS
********************************************************************************************/
unsigned char Calc_CS(unsigned char *CS_Buffer, unsigned short Len)
{
    unsigned char   CS = 0;
    unsigned short  j = 0;

    for(j = 0; j < Len; j++)
    {
        CS += CS_Buffer[j];
    }
    return CS;
}

////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** ��������: CRC16
** ��������: ����ת��Ϊ����
** ��ڲ���:*pData=У��������Ϣ�ֶ�����,len=Count CRCУ�����ݵ���Ϣ�ֶεĳ���		  
** �㷨��  CRC-16-IBM ��0xA001��
** ע�ͣ�  x16 + x15 + x2 + 1 ��0x8005 or 0xA001 (0x4003)��
** ����ֵ��CRC
** CRCѭ��У�麯��
*******************************************************************************************/
unsigned short CRC16(unsigned char *pData, unsigned short len)
{
    unsigned int i, j;
    unsigned short CRC16;

//  CRC16 = 0xFFFF;  	        	//step 1 CRC��ʼֵΪ0xFFFF
    CRC16 = 0x0000;  	        	//step 1 CRC��ʼֵΪ0x0000
    for(i = 0; i < len; i++)    	//step 6
    {
        CRC16 ^= *(pData + i);  	//step 2 CRC��У��������򣬵ó����ݵİ�λ��
        for(j = 0; j < 8; j++)      //step5
        {
            if(CRC16 & 0x1)  		//step 4  ���λ�Ƿ�Ϊ1
            {
                CRC16 >>= 1;  		//step 3
                //CRC16 ^= 0x8005;	//CRC-16-IBM ��0x8005��
                CRC16 ^= 0xA001;	//CRC-16-IBM ��0xA001��
                //CRC16 ^= 0x4003;	//CRC-16-IBM ��0x4003��
            }
            else
			{
                CRC16 >>= 1;   		//step 3
			}
        }      		            	//step 5
    }
    return CRC16;
}