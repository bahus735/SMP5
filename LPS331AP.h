#if !defined(LPS331AP_H_)  
#define LPS331AP_H_
#include "MKL25Z4.h"
#include "I2C.h"
#include "display.h"
#include "epaper.h"
#include "font8x8.h"
 // Register adres definition
#define LPS331_REF_P_XL       0x08
#define LPS331_REF_P_L        0x09
#define LPS331_REF_P_H        0x0A

#define LPS331_WHO_AM_I       0x0F

#define LPS331_RES_CONF       0x10

#define LPS331_CTRL_REG1      0x20
#define LPS331_CTRL_REG2      0x21
#define LPS331_CTRL_REG3      0x22
#define LPS331_INTERRUPT_CFG  0x23
#define LPS331_INT_SOURCE     0x24
#define LPS331_THS_P_L        0x25
#define LPS331_THS_P_H        0x26
#define LPS331_STATUS_REG     0x27

#define LPS331_PRESS_OUT_XL   0x28
#define LPS331_PRESS_OUT_L    0x29
#define LPS331_PRESS_OUT_H    0x2A

#define LPS331_TEMP_OUT_L     0x2B
#define LPS331_TEMP_OUT_H     0x2C

#define LPS331_AMP_CTRL       0x30

#define LPS331_DELTA_PRESS_XL 0x3C
#define LPS331_DELTA_PRESS_L  0x3D
#define LPS331_DELTA_PRESS_H 0x3E
#define ASCII_OFFSET						0x30


extern char  press[];
uint32_t  LPS331AP_Read_Press(void);
void LPS331AP_Init(void);
void LPS331AP_Set_Press(void);

#endif

