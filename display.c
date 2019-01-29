#include "display.h"




/*********************************************************************************
*@Function: Display_init(uint8_t*Buffor_Red,uint8_t*Buffor_Black,const uint8_t*Font24_size,const uint8_t*Font24_Table,const uint8_t *AGH_logo,const uint8_t *AGH_logo_Size)
*@Arguments:  Buffor_Red   -- the pointer to red buffe
*						  Buffor_Black -- the pointer to black buffe
*							Font24_size  -- the pointer to Font size
*							Font24_Table -- the pointer to Font table
*							AGH_logo     -- the pointer to logo
*							AGH_logo_Size-- the pointer to logo size 

*@Description: this function is basicly diplaying on epapaer. it is writing to red and black buffer. 
*							 creats the display orientation and ect.
*********************************************************************************/
void Display_init(uint8_t*Buffor_Red,uint8_t*Buffor_Black,const uint8_t*Font24_size,const uint8_t*Font24_Table,const uint8_t *AGH_logo,const uint8_t *AGH_logo_Size){

	Int_EP();
	DrawFilledRectangle_EP(Buffor_Red, 0, 0, 200,200 , UNCOLORED);
	DrawFilledRectangle_EP(Buffor_Black, 0, 0, 200, 200, UNCOLORED);
	DrawStringAt_EP(Buffor_Black,0, 2,  "TEMPE",Font24_size,Font24_Table, COLORED);
	DrawStringAt_EP(Buffor_Black,0, 26, "HUMID",Font24_size,Font24_Table, COLORED);
	DrawStringAt_EP(Buffor_Black,0, 50, "PRESS",Font24_size,Font24_Table, COLORED);
	DrawStringAt_EP(Buffor_Black,0, 74, "LIGHT",Font24_size,Font24_Table, COLORED);
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
	stml75_Set_Temp();
	LPS331AP_Set_Press();
	HTCS221_Set_Hum();
//	HTCS221_Set_Tem();
	DisplayFrame_EP(Buffor_Black, Buffor_Red);
	Sleep_EP();
}


/*********************************************************************************
*@Function:Display_tempe(uint8_t*Buffor_Red,uint8_t*Buffor_Black,const uint8_t*Font24_size,const uint8_t*Font24_Table,char *temper)
*@Arguments: Buffor_Red   -- the pointer to red buffe
						 Buffor_Black -- the pointer to black buffe
						 Font24_size  -- the pointer to Font size
						 Font24_Table	-- the pointer to Font table
						 temper				-- the pointer to temperature table
						 
*@Description:  function draws character on the buffor
*********************************************************************************/
void Display_tempe(uint8_t*Buffor_Red,uint8_t*Buffor_Black,const uint8_t*Font24_size,const uint8_t*Font24_Table,char *temper){

	DrawStringAt_EP(Buffor_Red,90, 2,temper,Font24_size,Font24_Table, COLORED);



}


/*********************************************************************************
*@Function:Display_humi(uint8_t*Buffor_Red,uint8_t*Buffor_Black,const uint8_t*Font24_size,const uint8_t*Font24_Table,char *temper)
*@Arguments: Buffor_Red   -- the pointer to red buffe
						 Buffor_Black -- the pointer to black buffe
						 Font24_size  -- the pointer to Font size
						 Font24_Table	-- the pointer to Font table
						 humi			  	-- the pointer to humidity table
						 
*@Description:  function draws character on the buffor
*********************************************************************************/

void Display_humi(uint8_t*Buffor_Red,uint8_t*Buffor_Black,const uint8_t*Font24_size,const uint8_t*Font24_Table,char *humi){


	DrawStringAt_EP(Buffor_Red,90, 26, humi,Font24_size,Font24_Table, COLORED);



}


/*********************************************************************************
*@Function:Display_press(uint8_t*Buffor_Red,uint8_t*Buffor_Black,const uint8_t*Font24_size,const uint8_t*Font24_Table,char *press)
*@Arguments: Buffor_Red   -- the pointer to red buffe
						 Buffor_Black -- the pointer to black buffe
						 Font24_size  -- the pointer to Font size
						 Font24_Table	-- the pointer to Font table
						 press				-- the pointer to pressure table
						 
*@Description:  function draws character on the buffor
*********************************************************************************/

void Display_press(uint8_t*Buffor_Red,uint8_t*Buffor_Black,const uint8_t*Font24_size,const uint8_t*Font24_Table,char *press){


	DrawStringAt_EP(IMAGE_RED,90, 50, press,Font24_size,Font24_Table, COLORED);



}


/*********************************************************************************
*@Function:Display_light(uint8_t*Buffor_Red,uint8_t*Buffor_Black,const uint8_t*Font24_size,const uint8_t*Font24_Table,char *light)
*@Arguments: Buffor_Red   -- the pointer to red buffe
						 Buffor_Black -- the pointer to black buffe
						 Font24_size  -- the pointer to Font size
						 Font24_Table	-- the pointer to Font table
						 light				-- the pointer to light table
						 
*@Description:  function draws character on the buffor
*********************************************************************************/

void Display_light(uint8_t*Buffor_Red,uint8_t*Buffor_Black,const uint8_t*Font24_size,const uint8_t*Font24_Table,char *light){


	DrawStringAt_EP(IMAGE_RED,90, 74, light,Font24_size,Font24_Table, COLORED);



}

/*********************************************************************************
*@Function:Display_state(uint8_t*Buffor_Red,uint8_t*Buffor_Black,const uint8_t*Font24_size,const uint8_t*Font24_Table,char *state)
*@Arguments: Buffor_Red   -- the pointer to red buffe
						 Buffor_Black -- the pointer to black buffe
						 Font24_size  -- the pointer to Font size
						 Font24_Table	-- the pointer to Font table
						 state				-- the pointer to state table
						 
*@Description:  function draws character on the buffor
*********************************************************************************/

void Display_state(uint8_t*Buffor_Red,uint8_t*Buffor_Black,const uint8_t*Font24_size,const uint8_t*Font24_Table,char *state){


	DrawStringAt_EP(IMAGE_RED,90, 98, state,Font24_size,Font24_Table, COLORED);


}
