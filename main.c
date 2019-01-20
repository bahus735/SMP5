#include "MKL25Z4.h"
#define GreenLED 1
#define RedLED 0
#define BlueLED 2
#include "leds.h"
#include "LCD.h"
#include "spi.h"
#include "epaper.h"
#include "font8x8.h"

uint32_t delay= 16;
int status=0;

uint8_t data=0x11 ;
uint8_t recived=0x00 ;









int main(void){
	InitLED();
	spi0_Init();
	
	

	 Int_EP();

	

	ledRedOff();
	DisplayFrame_EP(IMAGE_BLACK, IMAGE_RED);
	Sleep_EP();
	ledBlueOn();

	
while(1){
//DCon();
//	mywait(1);
//	DCoff();
//	mywait(1);
}
}
