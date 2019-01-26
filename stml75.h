#if !defined(STML75_H)
#define STML75_H
#include "MKL25Z4.h"



#define STML75_TEMP_REG_ADDR 		0x00
#define STML75_HIST_REG_ADDR 		0x05
#define STML75_SDOW_REG_ADDR		0x07
#define STML75_ADDR							0x90
#define ASCII_OFFSET						0x40

char  tempe[] =
	{
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
  };

uint16_t  stml75_Read_Temp(void);

#endif
