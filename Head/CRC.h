/*==============================================================================
            Copyright(C) 1997-2019.  Sinowealth Tech. Co., Ltd.
--------------------------------------------------------------------------------
Project No.   : XXX
File Name     : CRC..H	
Description   : 
              £»    
              £»   
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
#ifndef	CRC_H
#define	CRC_H

#ifndef CRC_C
	#define CRC_EXT	extern
#else		  
	#define CRC_EXT         
#endif
//------------------------------------------------------------------------------

//----------------


CRC_EXT unsigned char Calc_CS(unsigned char *CS_Buffer, unsigned short Len);
CRC_EXT unsigned short CRC16(unsigned char *pData, unsigned short len);


#endif