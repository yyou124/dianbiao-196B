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
void main(void)
{
	IEN0 = 0x00;           					//�ر����жϣ�ADC��TPS�жϣ���ʱ��0��1��2�жϣ��ⲿ�ж�0��1������0�ж�
    IEN1 = 0x00;           					//�ر�LPD�жϣ�����1��2�жϣ��ⲿ�ж�2��3��PWM�жϣ�EMU�ж�  
	PSW = 0x00;

	BatSupplySwitch();     					//����LPD��⡢�����Զ��л���LPD�ж�
    Change32KToPLL();	   					//ϵͳ2��Ƶ
    
	Init_IO();			   					//IO�ڳ�ʼ��
    Init_PWM();			   					//PWM��ʼ
    Init_TPS();			   					//TPS��ʼ��
	Init_ADC();			   					//ADC��ʼ��
	Init_LCD();			   					//LCD��ʼ��
	Init_RAM();			   					//RAM��ʼ��
	Init_RTC();			   					//RTC��ʼ��
	ReadRTCProc();		   					//��RTC����
    CheckRTC();			   					//RTCУ�鴦��
	Init_Timer();		   					//TIME��ʼ��
	Init_Uart();		   					//UART��ʼ��

//  AP_RSTCNTINC();
 
    gbLowPWRFlag = 0;
    g_Flag.Run &= ~F_PWRUP;					//AC MODE
	IEN1 = (BIT7 | BIT3);    				//����LPD�жϡ�RTC�ж�	
	 
	EA =1;                                  //���жϴ�    



//TEST  
/*
	KWH_PN[0] = 0XA5;
	KWH_PN[1] = 0XA5;
	KWH_PN[2] = 0XA5;
	KWH_PN[3] = 0XA5;
    VER_WRbytes(LMON_EC, &KWH_PN[0], 4, 1);    	
	KWH_PN[0] = 0X00;
	KWH_PN[1] = 0X00;
	KWH_PN[2] = 0X00;
	KWH_PN[3] = 0X00;
    VER_RDbytes(LMON_EC, &KWH_PN[0],  4);   		
	_nop_();
	_nop_();

*/
//TEST  

	while (1)
	{
		if (gbLowPWRFlag == 0) 		  			//������ѹģʽ, gbLowPWRFlag = 0 Ϊ������ѹģʽ��gbLowPWRFlag = 1 Ϊ���ģʽ
		{      
            if((g_Flag.Run & F_PWRUP) == 0)		//�״��ϵ� 
            {
                ACModeWDTProc();				//AC MODE WDT PROC
                Change32KToPLL();				//��32 TO PLL
				Init_IO();						//IO�ڳ�ʼ��
				Init_RTC();						//RTC��ʼ��
				Init_ADC();						//ADC��ʼ��
                PWONInit_LCD();					//LCD��ʼ��
				Init_RAM();						//RAM��ʼ��
				g_Flag.Run |= F_PWRUP;			//���ϵ�ı�־
				Init_Timer();					//TIME��ʼ��
				Init_Uart();					//UART��ʼ��
                ReadRTCProc();					//��RTC����
                CheckRTC();						//RTCУ�鴦��

                if((LPDCON & 0x40) == 0x40)		//VIN>1.2V
                {
                    Init_EMU();					//��ʼ��EMU

                    PwrCnt = 0x00;                   
                    while(PwrCnt != 0x0A);		//For delay 250ms

                    g_AllDisDly = C_AllDispDly;	//��ȫ����ʾ��־
                }
                else							//VIN<1.2V
                {
                    g_Flag.Run &= ~F_PWRUP;		//���µ�ı�־
                }
                CheckPowerOff();				//����µ紦��
            }
            else
            {             
                ACModeWDTProc();				//AC MODE WDT PROC
                EA = 1;

                BatSupplySwitch();				//����LPD��⡢�����Զ��л���LPD�ж� 					
				  
                TimeTaskThread();        		//�꣬�£��գ�Сʱ�����ӣ�������ʱ�������̴߳���               
				DisplayProc();       			//LCD��ʾ����
         
                ACInit_IO();					//IO SFRˢ��
                ACInit_RTC();					//RTC SFRˢ��
                ACInit_LCD();					//LCD SFRˢ��
                ACInit_Timer();             	//Timer SFRˢ��
                  
                EMUTampProc();			  		//ÿ���˲ʱ����ȡ������ͨ��ѡ���¼��ж�
                EnergyProc();					//��������

                Change32KToPLL();				//AC�¼�� PLL
                ACInit_EMUPluse();				//AC ˢ��EMU

				PIMAAckProc();					//PIMA���������
					                   
	            CheckPowerOff();		        //������			 
            } 			
		}
		else									//���ģʽ, gbLowPWRFlag = 0 Ϊ������ѹģʽ��gbLowPWRFlag = 1 Ϊ���ģʽ
		{
            PDModeWDTProc();					//BAT MODE WDT PROC,���Ź���λ  4096ms
			BatteryInitMode(); 				    //�͹��Ĵ���

			BatModeThreadProc();  	            //���������е�������
			ReadRTCProc();				  		//��RTC����
//            g_Flag.Run |= F_DISP;				//��һ����ˢ��LCD���ı�־
//			  DisplayProc();					//������ʾ����
			BatSupplySwitch();       			//����LPD��⡢�����Զ��л���LPD�ж�
			ChangePLLTo32K();				    //PLL TO 32K����

			RTCCON = 0x00;			 			//�ر�RTC�ڲ���ʱ����CALOUT������Ϊ��ͨIO
            RTCIE = 0x08;            			//����RTC���жϣ��ر�RTC�ڲ���ʱ���жϡ�����0��1�жϡ�RTC���Ӻ�С�������ж�
            IEN0 = 0;                			//�ر����жϣ�ADC��TPS�жϣ���ʱ��0��1��2�жϣ��ⲿ�ж�0��1������0�ж�
            IEN1 = 0;                			//�ر�LPD�жϣ�����1��2�жϣ��ⲿ�ж�2��3��PWM�жϣ�EMU�ж�  
			IEN1 = (BIT7 | BIT3);    			//����LPD�жϡ�RTC�ж�

            g_Flag.Run &= ~F_PWRUP;				//���µ�ı�־
            PwrCnt = 0x00;

            EA = 1;                  			//�������ж�

			SUSLO = 0x55;
			PCON = 0x01;             			//�������ģʽ
			_nop_();
			_nop_();
			_nop_();
			_nop_();
			_nop_();

			CheckPowerOn();         			//����ϵ紦��
	 	}
	}
}
