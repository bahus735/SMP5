#include "display.h"
#define  COLORED        1
#define  UNCOLORED      0

void Display_init(uint8_t*Buffor_Red,uint8_t*Buffor_Black,const uint8_t*Font24_size,const uint8_t*Font24_Table,const uint8_t *AGH_logo,const uint8_t *AGH_logo_Size){

	DrawFilledRectangle_EP(Buffor_Red, 0, 0, 200,200 , UNCOLORED);
	DrawFilledRectangle_EP(Buffor_Black, 0, 0, 200, 200, UNCOLORED);


	DrawStringAt_EP(Buffor_Black,0, 2,  "TEMPE",Font24_size,Font24_Table, COLORED);
	DrawStringAt_EP(Buffor_Black,0, 26, "HUMID",Font24_size,Font24_Table, COLORED);
	DrawStringAt_EP(Buffor_Black,0, 50, "LIGHT",Font24_size,Font24_Table, COLORED);
	DrawStringAt_EP(Buffor_Black,0, 74, "PRESS",Font24_size,Font24_Table, COLORED);
	DrawStringAt_EP(Buffor_Black,0, 98, "STATE",Font24_size,Font24_Table, COLORED);
	DrawHorizontalLine_EP(Buffor_Black, 0, 1, 200, COLORED);
	DrawHorizontalLine_EP(Buffor_Black, 0, 2, 200, COLORED);
	DrawHorizontalLine_EP(Buffor_Black, 0, 22, 200, COLORED);
	DrawHorizontalLine_EP(Buffor_Black, 0, 23, 200, COLORED);
	DrawHorizontalLine_EP(Buffor_Black, 0, 46, 200, COLORED);
	DrawHorizontalLine_EP(Buffor_Black, 0, 47, 200, COLORED);
	DrawHorizontalLine_EP(Buffor_Black, 0, 70, 200, COLORED);
	DrawHorizontalLine_EP(Buffor_Black, 0, 71, 200, COLORED);
	DrawHorizontalLine_EP(Buffor_Black, 0, 94, 200, COLORED);
	DrawHorizontalLine_EP(Buffor_Black, 0, 95, 200, COLORED);
	DrawHorizontalLine_EP(Buffor_Black, 0, 118, 200, COLORED);
	DrawHorizontalLine_EP(Buffor_Black, 0, 119, 200, COLORED);
	DrawVerticalLine_EP(Buffor_Black, 85, 0, 118, COLORED);
	DrawVerticalLine_EP(Buffor_Black, 86, 0, 118, COLORED);
  ImageAt(Buffor_Black, 150, 120,AGH_logo_Size,AGH_logo, COLORED);
	ImageAt(Buffor_Red, 149, 120,AGH_logo_Size,AGH_logo, COLORED);
	DisplayFrame_EP(Buffor_Black, Buffor_Red);
	Sleep_EP();


}
