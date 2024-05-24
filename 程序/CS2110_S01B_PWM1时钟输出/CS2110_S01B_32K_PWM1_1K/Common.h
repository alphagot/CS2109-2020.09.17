#ifndef   _COMMON_H
#define   _COMMON_H

#define XBYTE        ((unsigned char volatile xdata *) 0x20000L)
#define DBYTE        ((unsigned char volatile data *) 0x00000L)
#define DWORD51        ((unsigned int volatile data *) 0x00000L)
#define PXBYTE        ((unsigned char volatile pdata *) 0x00000L)
#define CBYTE        ((unsigned char volatile code *) 0x00000L)

#endif