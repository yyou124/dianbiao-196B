/*==============================================================================
            Copyright(C) 1997-2019.  Sinowealth Tech. Co., Ltd.
--------------------------------------------------------------------------------
Project No.   : XXX
File Name     : MEASUREMD..H	
Description   : 
              £»    
              £»   
Notice        :
             1:
             2:

Author        : James (email: james.wang@sinowealth.com)
Start Date    : 2015/06/26	
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
0.0 2015/06/26 James Just build the function
0.1
==============================================================================*/

//------------------------------------------------------------------------------
#ifndef MEASUREMD_H
#define MEASUREMD_H

#ifndef MEASUREMDPROC_C
	#define MEASUREMD_EXT	extern
#else
	#define MEASUREMD_EXT         
#endif
//------------------------------------------------------------------------------

//----------------


MEASUREMD_EXT void DemandMeasThread(void);
//MEASUREMD_EXT void MDRst(unsigned char Mode);
MEASUREMD_EXT void Demand_Bill(void);
MEASUREMD_EXT void Demand_Save(void);
MEASUREMD_EXT void Demand_DataToRam(void);

#endif