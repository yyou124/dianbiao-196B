/*==============================================================================
                Copyright(C) 1997-2019.  ChipNorth Tech. Co., Ltd.
--------------------------------------------------------------------------------
Project Name  : RELAYDrive.c
Description   :
Notice        :
			 1:
			 2:
Author        : YouDeqiang
Start Date    : 2018/05/28
Approve Date  :
Version       : V0.0
Function List :
             1:
             2:
RevisionHistory:
Rev#  CheckSum    Date     Author     Comments(Function+Date)
-----+--------+----------+---------+------------------------------------------
0.0     XXXX   2018/05/28   You        Just build the file
==============================================================================*/

#ifndef  DELAYDRIVER_C
	#define  DELAYDRIVER_C
#endif
//-------------------------------------------------------------------------------

//---------------
#include "config.h"
#include "Headconfig.h"





/****************************************************************************
** ��������: RealyCtrl
** ��������: �̵������ؿ���
** �������: On_Offn  0-��  1-��  2- ��5s
** �������: ��
*****************************************************************************/

void RealyCtrl(unsigned char On_Offn)
{

	if(On_Offn ==0)//��
	{
		DelayFlag = 0;
		DELAY_INA_L();
		DELAY_INB_H();
		Delay_ms(200);
		DELAY_INA_L();
		DELAY_INB_L();
		//��EEPROM��д��̵���״̬
		DelayStatus[0] = 0xA5;
		DelayStatus[1] = 0xA5;
		//MemInitSet(&DelayStatus[0], 0xA5, 2);
		VER_WRbytes(RELAY_STATUS,&DelayStatus[0],2, 1);
	}
	else if(On_Offn == 1)//��
	{
		DelayFlag = 1;
		DELAY_INA_H();
		DELAY_INB_L();

		Delay_ms(200);
		DELAY_INA_L();
		DELAY_INB_L();
		//��EEPROM��д��̵���״̬
		DelayStatus[0] = 0x00;
		DelayStatus[1] = 0x00;
		//MemInitSet(&DelayStatus[0], 0x00, 2);
		VER_WRbytes(RELAY_STATUS,&DelayStatus[0],2, 1);
	}
	else if(On_Offn ==2)//��5s
	{
		DelayFlag = 2;
		DELAY_INA_H();
		DELAY_INB_L();

		Delay_ms(200);
		DELAY_INA_L();
		DELAY_INB_L();
	}
}

/****************************************************************************
** ��������: RealyCtrl
** ��������: �̵������ؿ���
** �������: On_Offn  0-��  1-��  2- ��5s
** �������: ��
*****************************************************************************/
void GetRelayStatus(void)
{
	EE_to_RAM(RELAY_STATUS, &DelayStatus[0], 2);
	if((DelayStatus[0] != 0xA5)&&(DelayStatus[1] != 0xA5))
	{
		DelayFlag = 1;//�̵����ϵ�
	}
	else
	{
		DelayFlag = 0;//�̵���ͨ��
	}

}

