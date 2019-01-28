#if !defined(LPS331AP_H_)  
#define LPS331AP_H_
#include "MKL25Z4.h"
#include "I2C.h"

uint16_t  LPS331AP_Read_Press(void);
void LPS331AP_Init(void);


#endif

