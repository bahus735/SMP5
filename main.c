#include "MKL25Z4.h"
#define GreenLED 1
#define RedLED 0
#define BlueLED 2
#include "leds.h"
#include "spi.h"
#include "i2c.h"
#include "epaper.h"
#include "stml75.h"
#include "font8x8.h"
#include "display.h"
uint32_t delay= 16;
int status=0;

uint16_t data=0x00 ;
uint8_t recived=0x00 ;









int main(void){
	InitLED();
	//spi0_Init();
	I2C_int();

	data=  stml75_Read_Temp();
	
	 Int_EP();
	Display_init(IMAGE_RED,IMAGE_BLACK, Font24_size,Font24_Table,AGH_logo,AGH_logo_Size);
	/*
	// DrawLine_EP(IMAGE_BLACK, 0, 0, 200, 200, COLORED);
	// DrawHorizontalLine_EP(IMAGE_BLACK, 10, 10, 200, COLORED);
  // DrawRectangle_EP(IMAGE_BLACK, 40, 40, 80, 80, COLORED);
  // DrawFilledRectangle_EP(IMAGE_BLACK, 80, 80, 120, 120, COLORED);
	DrawFilledRectangle_EP(IMAGE_RED, 0, 0, 200,200 , UNCOLORED);
	DrawFilledRectangle_EP(IMAGE_BLACK, 0, 0, 200, 200, UNCOLORED);
	//  DrawVerticalLine_EP(IMAGE_BLACK, 20, 20, 200, COLORED);
 //  DrawCircle(IMAGE_BLACK, 100, 100, 20, COLORED);
 //  DrawFilledCircle_EP(IMAGE_BLACK, 140,140,20, COLORED);
//	DrawCharAt_EP(IMAGE_BLACK, 100, 100, 'a',Font24_size,Font24_Table, COLORED);
	DrawStringAt_EP(IMAGE_BLACK,0, 2,  "TEMPE",Font24_size,Font24_Table, COLORED);
	DrawStringAt_EP(IMAGE_BLACK,0, 26, "HUMID",Font24_size,Font24_Table, COLORED);
	DrawStringAt_EP(IMAGE_BLACK,0, 50, "LIGHT",Font24_size,Font24_Table, COLORED);
	DrawStringAt_EP(IMAGE_BLACK,0, 74, "PRESS",Font24_size,Font24_Table, COLORED);
	DrawStringAt_EP(IMAGE_BLACK,0, 98, "STATE",Font24_size,Font24_Table, COLORED);
	DrawHorizontalLine_EP(IMAGE_BLACK, 0, 1, 200, COLORED);
	DrawHorizontalLine_EP(IMAGE_BLACK, 0, 2, 200, COLORED);
	DrawHorizontalLine_EP(IMAGE_BLACK, 0, 22, 200, COLORED);
	DrawHorizontalLine_EP(IMAGE_BLACK, 0, 23, 200, COLORED);
	DrawHorizontalLine_EP(IMAGE_BLACK, 0, 46, 200, COLORED);
	DrawHorizontalLine_EP(IMAGE_BLACK, 0, 47, 200, COLORED);
	DrawHorizontalLine_EP(IMAGE_BLACK, 0, 70, 200, COLORED);
	DrawHorizontalLine_EP(IMAGE_BLACK, 0, 71, 200, COLORED);
	DrawHorizontalLine_EP(IMAGE_BLACK, 0, 94, 200, COLORED);
	DrawHorizontalLine_EP(IMAGE_BLACK, 0, 95, 200, COLORED);
	DrawHorizontalLine_EP(IMAGE_BLACK, 0, 118, 200, COLORED);
	DrawHorizontalLine_EP(IMAGE_BLACK, 0, 119, 200, COLORED);
	DrawVerticalLine_EP(IMAGE_BLACK, 85, 0, 118, COLORED);
	DrawVerticalLine_EP(IMAGE_BLACK, 86, 0, 118, COLORED);
	DrawStringAt_EP(IMAGE_BLACK,90, 2,  "25,6*C",Font24_size,Font24_Table, COLORED);
	DrawStringAt_EP(IMAGE_BLACK,90, 26, "67,23%",Font24_size,Font24_Table, COLORED);
	DrawStringAt_EP(IMAGE_BLACK,90, 50, "124LUM",Font24_size,Font24_Table, COLORED);
	DrawStringAt_EP(IMAGE_BLACK,90, 74, "2343hpa",Font24_size,Font24_Table, COLORED);
	DrawStringAt_EP(IMAGE_BLACK,90, 98, "ERROR",Font24_size,Font24_Table, COLORED);
  ImageAt(IMAGE_BLACK, 150, 120,AGH_logo_Size,AGH_logo, COLORED);
	//DrawVerticalLine_EP(IMAGE_BLACK, 150, 120, 40, COLORED);
	//DrawVerticalLine_EP(IMAGE_BLACK, 151, 121, 39, COLORED);
	//DrawVerticalLine_EP(IMAGE_BLACK, 152, 122, 38, COLORED);
	//DrawVerticalLine_EP(IMAGE_BLACK, 153, 123, 37, COLORED);

	
	ledRedOff();
	DisplayFrame_EP(IMAGE_BLACK, IMAGE_RED);
	Sleep_EP();
	ledBlueOn();
*/


//if()
	
while(1){
//DCon();
//	mywait(1);
//	DCoff();
//	mywait(1);
}
}
///tesrtewrt
