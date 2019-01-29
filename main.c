#include "MKL25Z4.h"
#define GreenLED 1
#define RedLED 0
#define BlueLED 2
#include "leds.h"
#include "spi.h"
#include "i2c.h"
#include "PIT.h"
#include "LPS331AP.h"
#include "epaper.h"
#include "stml75.h"
#include "font8x8.h"
#include "display.h"
#include "HTS221.h"
uint32_t delay= 16;
int status=0;

uint32_t data=0x00 ;
uint8_t recived=0x00 ;









int main(void){
	
pitInicialize ();	
I2C_int();
spi0_Init();
InitLED();	
//LPS331AP_Set_Press();	
	
//	I2C_int();
//	spi0_Init();
//	Int_EP();
//	Display_init(IMAGE_RED,IMAGE_BLACK, Font24_size,Font24_Table,AGH_logo,AGH_logo_Size);
	



	
while(1){
//Display_init(IMAGE_RED,IMAGE_BLACK, Font24_size,Font24_Table,AGH_logo,AGH_logo_Size);
	
}
}

