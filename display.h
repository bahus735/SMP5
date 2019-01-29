#if !defined(DISPLAY_H)  
#define DISPLAY_H
#include "epaper.h"
#include "MKL25Z4.h"
#include "stml75.h"
#include "HTS221.h"
#include "LPS331AP.h"

#define  COLORED        1
#define  UNCOLORED      0


void Display_init(uint8_t*Buffor_Red,uint8_t*Buffor_Black,const uint8_t*Font24_size,const uint8_t*Font24_Table,const uint8_t *AGH_logo,const uint8_t* AGH_logo_Size);


void Display_tempe(uint8_t*Buffor_Red,uint8_t*Buffor_Black,const uint8_t*Font24_size,const uint8_t*Font24_Table,char *temper);


void Display_humi(uint8_t*Buffor_Red,uint8_t*Buffor_Black,const uint8_t*Font24_size,const uint8_t*Font24_Table,char *humi);


void Display_press(uint8_t*Buffor_Red,uint8_t*Buffor_Black,const uint8_t*Font24_size,const uint8_t*Font24_Table,char *press);


void Display_light(uint8_t*Buffor_Red,uint8_t*Buffor_Black,const uint8_t*Font24_size,const uint8_t*Font24_Table,char *light);


void Display_state(uint8_t*Buffor_Red,uint8_t*Buffor_Black,const uint8_t*Font24_size,const uint8_t*Font24_Table,char *state);




#endif
