/*==============================================================================
            Copyright(C) 1997-2019.  Sinowealth Tech. Co., Ltd.
--------------------------------------------------------------------------------
Project No.   : XXX
File Name     : PIN_CFG..H
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
/*
#ifndef  PIN_CFG_H
#define  PIN_CFG_H


#ifdef PIN_CFG_C
   #define  PIN_CFG_EXT
#else
   #define  PIN_CFG_EXT   extern
#endif
*/
//------------------------------------------------------------------------------

#ifndef PIN_CFG_H
#define PIN_CFG_H

/*
IO¶Ë¿Ú
P0->2~5
P1->0~1
P2->3~5
P3->3~6
P5->4~7
*/
#define D_P0SS      (Bin(00111100)) //P0.2-P0.5??LCD???SEG7 - SEG10?
#define D_P1SS      (Bin(11110001)) //P1.4-P1.7??LCD???SEG1 - SEG4?
#define D_P3SS      (Bin(00000000)) //P3.3-P3.6??I/O
#define D_P5SS      (Bin(00001111)) //P5.0-P5.3??LCD???SEG21 - SEG24?

#define D_P0        (Bin(00000000))
#define D_P0CR      (Bin(11111111))
#define D_P0PCR     (Bin(00000000))

#define D_P1        (Bin(00000000))
#define D_P1CR      (Bin(11111111))
#define D_P1PCR     (Bin(00000000))

#define D_P2        (Bin(00000000))
#define D_P2CR      (Bin(11111111))
#define D_P2PCR     (Bin(00000000))


#define D_P3        (Bin(01111011))
#define D_P3CR      (Bin(11010111))
#define D_P3PCR     (Bin(11111111))



#define D_P5        (Bin(00000000))
#define D_P5CR      (Bin(11111111))
#define D_P5PCR     (Bin(00000000))

#endif