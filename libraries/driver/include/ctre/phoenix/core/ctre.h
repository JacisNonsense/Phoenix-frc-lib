/**
 * @file ctre.h
 * Common header for all CTRE HAL modules.
 */
#ifndef CTRE_H
#define CTRE_H

//Bit Defines
#define BIT0 0x01
#define BIT1 0x02
#define BIT2 0x04
#define BIT3 0x08
#define BIT4 0x10
#define BIT5 0x20
#define BIT6 0x40
#define BIT7 0x80
#define BIT8  0x0100
#define BIT9  0x0200
#define BIT10 0x0400
#define BIT11 0x0800
#define BIT12 0x1000
#define BIT13 0x2000
#define BIT14 0x4000
#define BIT15 0x8000

//Signed
typedef	signed char	INT8;
typedef	signed short	INT16;
typedef	signed int	INT32;
typedef	signed long long INT64;

//Unsigned
typedef	unsigned char	UINT8;
typedef	unsigned short	UINT16;
typedef	unsigned int	UINT32;
typedef	unsigned long long UINT64;

//Other
typedef	unsigned char	UCHAR;
typedef unsigned short	USHORT;
typedef	unsigned int	UINT;
typedef unsigned long	ULONG;

#define UNUSED(x) (void)(x)

#include "ctre/Phoenix/core/ErrorCode.h"

#endif /* CTRE_H */
