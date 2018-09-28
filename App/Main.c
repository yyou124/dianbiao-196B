/*==============================================================================
                Copyright(C) 1997-2019.  Sinowealth Tech. Co., Ltd.
--------------------------------------------------------------------------------
Project Name  : MAIN.c
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
#ifndef  MAIN_C
	#define  MAIN_C
#endif
//-------------------------------------------------------------------------------

//---------------

#include "config.h"
#include "Headconfig.h"




////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************
** ��������: main
** ��������: ����������
** �������: ��
** �������: ��
*******************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////

//                                                                                                unsigned char temp[18] = {0x11,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18};
//unsigned char temp_read[18] = {0x00,0x00};
char p[32];

	char *dest = p;

char at[32]="AT+CGSN=1\r\n";
void main(void)
{
	IEN0 = 0x00;                        //�ر����жϣ�ADC��TPS�жϣ���ʱ��0��1��2�жϣ��ⲿ1�жϣ�����0�ж�
	IEN1 = 0x00;                        //�ر�LPD�жϣ�PWM�жϣ�����1�жϣ�RTC�жϣ�EMU�ж�
	PSW = 0x00;                         //�������־λ��UARTͨ��żУ��(BIT0 = 0)
	Change32KToPLL();                   //ϵͳ2��Ƶ��ʱ��Ƶ��4.096MHz

	Delay_ms(500);                      //��ʱ500ms���ȴ�ϵͳ�ȶ�
	Delay_ms(500);                      //��ʱ500ms���ȴ�ϵͳ�ȶ�

	Init_IO();                          //IO�ڳ�ʼ��
	Init_Uart();                        //UART��ʼ����������9600������λ8����У�飬ֹͣλ1
	PWONInit_LCD();                     //LCD��ʼ��
	Init_RTC();                         //RTC��ʼ��
	ReadRTCProc();                      //��RTC����
	CheckRTC();                         //RTCУ�鴦��,��ΪĬ��ʱ��2018-01-01 12: 00: 00
	Init_Timer();                       //TIME��ʼ����Ŀǰ��ʹ�ö�ʱ��1
	g_Flag.Run &= ~F_PWRUP;             //AC MODE

/*
Ŀǰ���õ��ж������
Timer1      ����ж�
UART0       SBUF�ж�
RTC         SEC�ж�
LPD         �ж�
EMU         EEMU(EMU���жϣ���Init_EMU������)PF�����ж�
*/
	IEN1 = (BIT7 | BIT3);               //����LPD�жϡ�RTC�ж�
	EA =1;                              //���жϴ�

	while (1)
	{
//MemInitSet(&kwh_value.integer[0],0x00,5);
//VER_WRbytes_limit(EE_Meter_address,&temp[0],6,1);
//EE_to_RAM(0x121,&temp_read[0],18);

	//SEQ_write(EE_FirstProg_FLAG,&temp[0],4);
        if((g_Flag.Run & F_PWRUP) == 0) //�״��ϵ�
        {
            ACModeWDTProc();            //ACģʽι��                �޸����
            Change32KToPLL();           //ʱ��Ƶ�ʿ���4.096MHz       �޸����
            Init_IO();                  //IO�ڳ�ʼ��                �޸����
            Init_Uart();                //UART��ʼ��                �޸����
            PWONInit_LCD();             //LCD��ʼ��
            Init_RTC();                 //RTC��ʼ��                 �޸����
            Init_RAM();                 //һЩ�����ĳ�ʼ��           �޸���ɣ�У�������ѱ�ע�͵�
            g_Flag.Run |= F_PWRUP;      //���ϵ�ı�־
            Init_Timer();               //TIME��ʼ��                �޸����
            ReadRTCProc();              //��RTC����                 �޸����
            CheckRTC();                 //RTCУ�鴦��                �޸����
            if((LPDCON & 0x08) == 0x08) //VCC>2.7V
            {
                Init_EMU();             //��ʼ��EMU                  �޸����
            }
            else                        //VCC<2.7V
            {
                g_Flag.Run &= ~F_PWRUP; //���µ�ı�־
            }
            //����LPD�ж�,��������Ҫ

            //CheckPowerOff();//�����⣬�˴������VCC<2.7v  �޸����
        }
        else
        {
            ACModeWDTProc();            //AC MODE WDT PROC            �޸����
            EA = 1;
            //����ж�
            TimeTaskThread();           //ʱ����������̴߳���				 �޸����,����У������
            ACInit_IO();                //IO SFRˢ��                   �޸����
            ACInit_RTC();               //RTC SFRˢ��                  �޸����
            ACInit_Timer();             //Timer SFRˢ��                �޸����
            DisplayProc();              //LCD��ʾ�߳�
            EMUTampProc();              //ÿ��ĵ�������˲ʱ��ȡ������	�޸���ɣ�����ϸ�ڴ�����
            EnergyProc();               //�������������õ����ۼ�         �޸���ɣ�ÿ0.01���ۼ�һ��
            Change32KToPLL();           //AC�¼�� PLL                 �޸����
            ACInit_EMUPluse();          //AC ˢ��EMU                   �޸����
            UART1Process();             //UART1ͨѶ����	                �޸����
            CommuProcess();             //ͨ�Ŵ���
            //����LPD�ж�,��������Ҫ
            //CheckPowerOff();          //������			            �޸����
        }
	}/*while finish*/
}/*main finish*/