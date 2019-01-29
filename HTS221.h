#if !defined(HTS221_H)
#define HTS221_H
#include "MKL25Z4.h"
#include "i2c.h"
#include "display.h"
#include "MKL25Z4.h"
#include "epaper.h"
#include "font8x8.h"


#define HTS221_AV_CONFG 		0x10
#define HTS221_AADDRES  		0xBE
#define HTS221_CTRL_REG1		0x20
#define HTS221_CTRL_REG2 		0x21
#define HTS221_CTRL_REG3 		0x22
#define HTS221_STATUS_REG		0x27
#define HTS221_HUM_OUT_L 		0x28
#define HTS221_HUM_OUT_H 		0x29
#define HTS221_TEM_OUT_L 		0x2A
#define HTS221_TEM_OUT_H 		0x2B
#define HTS221_H0_rH_X2			0x30
#define HTS221_H1_rH_X2			0x31
#define HTS221_H0_T0_OUT_L	0x36
#define HTS221_H0_T0_OUT_H	0x37
#define HTS221_H1_T0_OUT_L	0x3A
#define HTS221_H1_T0_OUT_H	0x3B


#define HTS221_T0_rH_X2			0x32
#define HTS221_T1_rH_X2			0x33
#define HTS221_T0_T0_OUT_L	0x3c
#define HTS221_T0_T0_OUT_H	0x3d
#define HTS221_T1_T0_OUT_L	0x3e
#define HTS221_T1_T0_OUT_H	0x3f
extern char  tmp2[];
extern char  humi[];
struct HTS221 ;
void HTCS221_Init(void);
void  HTCS221_Read_Hum(void);
void  HTCS221_Read_Tem(void);
void  HTCS221_Read_Cal(void);
void HTCS221_Set_Hum(void);
void HTCS221_Set_Tem(void);
#endif
