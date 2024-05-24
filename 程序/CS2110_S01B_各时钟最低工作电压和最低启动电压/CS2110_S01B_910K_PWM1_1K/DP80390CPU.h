/*--------------------------------------------------------------------------
  DP80390_mod.H
 copy from Registers definition for DCD DP80390,DP80390.h
 for CS3A0x/0x, modify part of Timer 0,1,2,Uart,Watchdog
 -----2009-06-15:modify ,niho.
 -----2012-12-19:modify for wpc ,niho.

 Copyright (c) 2003 Keil Elektronik GmbH and Keil Software, Inc.
 All rights reserved.
-------------------------------------------------------------------------- */
#ifndef _DP80390CPU_H_
#define _DP80390CPU_H_

/* ----------------------------------------------------------------------- */
/* ----------------------------------------------------------------------- */
/* Defined configuration peripherals */

/* *********************************************************************** */
/* *********************************************************************** */

/*---------------------------------------------------------------------------
  Defined registers  
---------------------------------------------------------------------------*/

/*  BYTE Register  */

  sfr SP      = 0x81;   
  sfr DPL0    = 0x82; 
  sfr DPL1    = 0x83;  
  sfr PCON    = 0x87;
  sfr TCON    = 0x88;

  sfr TTCON   = 0x86;    
  sfr TMOD    = 0x89;
  sfr TL0     = 0x8A;
  sfr TL1     = 0x8B;
  sfr TH0     = 0x8C;
  sfr TH1     = 0x8D;
  sfr TTMOD2  = 0xC8;

  sfr CKCON   = 0x8E;

  sfr WTST    = 0x92;
  sfr DPX0    = 0x93;

  sfr SCON    = 0x98;
  sfr SBUF    = 0x99;
  sfr SMOD    = 0x9A;

  sfr AP      = 0x9C;
  sfr ACON    = 0x9D;

  sfr P2      = 0xA0;

  sfr PAREG   = 0xA1;
  sfr PADIR   = 0xA2;
  sfr PAIE    = 0xA3;
  sfr PAIES_L = 0xA4;
  sfr PAIES_H = 0xA5;
  sfr PAIFG   = 0xA6;

  sfr IE      = 0xA8;

  sfr PBREG   = 0xA9;
  sfr PBDIR   = 0xAA;
  sfr PBIE    = 0xAB;
  sfr PBIES_L = 0xAC;
  sfr PBIES_H = 0xAD;
  sfr PBIFG   = 0xAE;

  sfr PCREG   = 0xB1;
  sfr PCDIR   = 0xB2;
  sfr PCIE    = 0xB3;
  sfr PCIES_L = 0xB4;
  sfr PCIES_H = 0xB5;
  sfr PCIFG   = 0xB6;

  sfr IP      = 0xB8;

  sfr PDREG   = 0xB9;
  sfr PDDIR   = 0xBA;
  sfr PDIE    = 0xBB;
  sfr PDIES_L = 0xBC;
  sfr PDIES_H = 0xBD;
  sfr PDIFG   = 0xBE;

  sfr PEREG   = 0xC1;
  sfr PEDIR   = 0xC2;
  sfr PEIE    = 0xC3;
  sfr PEIES_L = 0xC4;
  sfr PEIES_H = 0xC5;
  sfr PEIFG   = 0xC6;

  
  sfr TMR2C0  = 0xC9;
  sfr TMR2C1  = 0xCA;
  sfr TMR3C   = 0xCB;
  sfr TMR2    = 0xCC;
  sfr TMR3L   = 0xCD;
  sfr TMR3H   = 0xCE;
  sfr TMRINT  = 0xCF;

  sfr PSW     = 0xD0;

  sfr WDCON   = 0xD8;
  sfr WDCNTM  = 0xD9;
  sfr WDCNTH  = 0xDA;
  sfr XDATA_CTL  = 0xDB;

  sfr ACC     = 0xE0;

  sfr STATUS  = 0xE9;
  sfr MXAX    = 0xEA;
  sfr TA      = 0xEB;

  sfr B       = 0xF0;
/*-------------------------------------------------------------------------
  BIT Register : bit addressable 
  -------------------------------------------------------------------------*/


#endif /*_DP80390_H_*/
