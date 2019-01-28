#include "epaper.h"
#include "MKL25Z4.h"
#include "spi.h"


extern uint32_t delay;
uint32_t delay2=5;
uint16_t i=0;
uint16_t j=0;
uint16_t bit=0;
uint8_t ROTATE;
#define width 200
#define height 200
void mywait(uint32_t delay){
	uint32_t i = 0;
		for(i = 0; i<(delay*100000);i++){}
}



/****************************************************************
*This part of code holds look up tables for e-papaer
*they need to be set to allow safe e-papaer operations 
*
*****************************************************************/


const uint8_t lut_vcom0[] =
{
  0x0E, 0x14, 0x01, 0x0A, 0x06, 0x04, 0x0A, 0x0A,
  0x0F, 0x03, 0x03, 0x0C, 0x06, 0x0A, 0x00
};

const uint8_t lut_w[] =
{
  0x0E, 0x14, 0x01, 0x0A, 0x46, 0x04, 0x8A, 0x4A,
  0x0F, 0x83, 0x43, 0x0C, 0x86, 0x0A, 0x04
};

const uint8_t lut_b[] = 
{
  0x0E, 0x14, 0x01, 0x8A, 0x06, 0x04, 0x8A, 0x4A,
  0x0F, 0x83, 0x43, 0x0C, 0x06, 0x4A, 0x04
};

const uint8_t lut_g1[] = 
{
  0x8E, 0x94, 0x01, 0x8A, 0x06, 0x04, 0x8A, 0x4A,
  0x0F, 0x83, 0x43, 0x0C, 0x06, 0x0A, 0x04
};

const uint8_t lut_g2[] = 
{
  0x8E, 0x94, 0x01, 0x8A, 0x06, 0x04, 0x8A, 0x4A,
  0x0F, 0x83, 0x43, 0x0C, 0x06, 0x0A, 0x04
};

const uint8_t lut_vcom1[] = 
{
  0x03, 0x1D, 0x01, 0x01, 0x08, 0x23, 0x37, 0x37,
  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const uint8_t lut_red0[] = 
{
  0x83, 0x5D, 0x01, 0x81, 0x48, 0x23, 0x77, 0x77,
  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const uint8_t lut_red1[] = 
{
  0x03, 0x1D, 0x01, 0x01, 0x08, 0x23, 0x37, 0x37,
  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
/****************************************************************
*@Function: void Int_EP(void)
*@Description: initialization of epaper. it takes no arguments.
*e-paper needs to be initalized in certain way. 
*SPI Module must be init for this function to work!!!
*code bellow is based on initilaziation provided in datasheed of the epapaer module
*@link:
*****************************************************************/


void Int_EP(void){

	PORTC_PCR9=PORT_PCR_MUX(0x1);										//set as GPIO. This is PCR9 EPAPER_RST pin
	PORTC_PCR3=PORT_PCR_MUX(0x1);										//set as GPIO. This is PCR3 EPAPER_DC pin
	PORTC_PCR8=PORT_PCR_MUX(0x1);										//set as GPIO. This is PCR8 EPAPER_BUSY pin
	PTC->PDDR|= (EPAPER_RST)|(EPAPER_DC);						//setting output for control pins of e-paper
	PTC->PDDR&= ~EPAPER_BUSY;												//setting input for EPAPER_BUSY pin

	DCon();																					//set DC high						
  SSon();																					//set SS high
	RSTon();																				//set reset high
	
	Reset_EP();																			//reset the epapaer module

	SendCommand_EP(POWER_SETTING);									//send command to write to power settings register

	SendData_EP(0x07);															//send data....
	SendData_EP(0x00);
	SendData_EP(0x08);
	SendData_EP(0x00);

	SendCommand_EP(BOOSTER_SOFT_START);							//send command to write to booster register

	SendData_EP(0x07);															//send data...
	SendData_EP(0x07);
	SendData_EP(0x07);

	SendCommand_EP(POWER_ON);												//power on module
	WaitUntilIdle_EP();															//wait for power up
	
	SendCommand_EP(PANEL_SETTING);									//send command to write to panel setting register 
	
	SendData_EP(0xcf);															//send data...
	
	SendCommand_EP(VCOM_AND_DATA_INTERVAL_SETTING);	//send command to write to vcom and data setting register
	
	SendData_EP(0x17);															//send data...
	
	SendCommand_EP(PLL_CONTROL);										//send data to write to pll control register
	
	SendData_EP(0x39);															//send data...
	
	SendCommand_EP(TCON_RESOLUTION);								// send command to write to resolution register
	
	SendData_EP(0xC8);															//send data...		
	SendData_EP(0x00);
	SendData_EP(0xC8);
	
	SendCommand_EP(VCM_DC_SETTING_REGISTER);				//send command to write to vcm dc setting register
	WaitUntilIdle_EP();															//wait for module to be freed
	SendData_EP(0x0E);															//send data..
	SetLutBw_EP();																	//send LUT data for black
	SetLutRed_EP();																	//send LUT data for red
//	SendCommand_EP(DISPLAY_REFRESH);
//	WaitUntilIdle_EP();	
//	SendCommand_EP(DISPLAY_REFRESH);
}

/*********************************************************************************
*
*
*
*********************************************************************************/

void Sleep_EP() {
    SendCommand_EP(VCOM_AND_DATA_INTERVAL_SETTING);			//send command to write to vcom and data setting register				
    SendData_EP(0x17);
    SendCommand_EP(VCM_DC_SETTING_REGISTER);   		       //to solve Vcom drop write 00 to register
    SendData_EP(0x00);
    SendCommand_EP(POWER_SETTING);      							   //power setting
    SendData_EP(0x02);       														 //gate switch to external
    SendData_EP(0x00);
    SendData_EP(0x00);
    SendData_EP(0x00);
    WaitUntilIdle_EP();																		//wait for power setting to be set
    SendCommand_EP(POWER_OFF);														// power off module. it can be wake up with restet signal


}
		
	

/*********************************************************************************
*@Function: SendCommand_EP(uint8_t command)
*@Arguments: command -- command that you want to send to the epaper module.
*@Description: sending commant on the spi to write the command to the module.
*the DC pin is set to low to indicate that command is being send
*********************************************************************************/
void SendCommand_EP(uint8_t command){	
	DCoff();
	spi0_SendRecive(command);
}
/*********************************************************************************
*@Function: Reset_EP( void)
*@desctiption: reseting the module by setting RST pin to active state(low)
*
*********************************************************************************/



void Reset_EP(void){
RSToff();
mywait(delay);
RSTon();
mywait(delay);
}

/*********************************************************************************
*@Function: SendData_EP(uint8)t data)
*@Arguments:uint8_t data -- data that you want to send to the epaper module.
*@Description: write the data to the e papaer module. before that the CD pin is set high 
*to indicate that data is sended 
*********************************************************************************/

void SendData_EP(uint8_t data){
DCon();
spi0_SendRecive(data);	
}

/*********************************************************************************
*@Function: WaitUntilIdle_EP(void)
*@Description: This function waits untill the e paper pin BUSY is changed to higt state
*that indicates the epaper is ready to recive datacommand
*********************************************************************************/

void WaitUntilIdle_EP(void){
while((BUSYread()==0x00)){

};

}

/*********************************************************************************
*@Function:DCon(void)
*@Description: function that sets DC pin high 
*********************************************************************************/

void DCon(void){
 PTC->PSOR=(EPAPER_DC);
 }

/*********************************************************************************
*@Function:DCoff(void)
*@Description: function that sets DC pin low 
*********************************************************************************/

void DCoff(void){
 PTC->PCOR=(EPAPER_DC); 
 }

/*********************************************************************************
*@Function:RSTon(void)
*@Description: function that sets RST pin high 
*********************************************************************************/
 
void RSTon(void){
 PTC->PSOR=(EPAPER_RST);
 }

/*********************************************************************************
*@Function:RSTon(void)
*@Description: function that sets RST pin low 
*********************************************************************************/
 

void RSToff(void){
 PTC->PCOR=(EPAPER_RST);
 }

 /*********************************************************************************
*@Function:BUSYread 
*@Returns:uint32_t -- show is the BUSY pin is high or low
*@Description: reds the status od BUSY pin that is on PTC8
*********************************************************************************/


uint32_t BUSYread(void){
uint32_t temp	=0x0;
 temp = (PTC->PDIR &  0x00000100 );
 return temp;
 }

 /*********************************************************************************
*@Function:SSon(void) 
*@Description: function that sets SS pin high if spi is not in control of it.
*********************************************************************************/

 void SSon(void){
 PTC->PSOR=(EPAPER_SS);
 }
 /*********************************************************************************
*@Function:SSoff(void) 
*@Description: function that sets SS pin low if spi is not in control of it.
*********************************************************************************/

void SSoff(void){
 PTC->PCOR=(EPAPER_SS);
}
	
/*********************************************************************************
*@Function: SetLutRed_EP()
*@Description: this funtion writes to epaper module the lut for red colored pixels that are needed to 
* operate that module. they are provided by manufactuer but can be change if there 
* is no need of long living display.
*********************************************************************************/

void SetLutRed_EP(){
  unsigned int count;     
  SendCommand_EP( 0x20);         //g vcom
  for(count = 0; count < 15; count++) {
    SendData_EP(lut_vcom0[count]);
  } 
  SendCommand_EP(0x21);        //g ww --
  for(count = 0; count < 15; count++) {
    SendData_EP(lut_w[count]);
  } 
  SendCommand_EP(0x22);         //g bw r
  for(count = 0; count < 15; count++) {
    SendData_EP(lut_b[count]);
  } 
  SendCommand_EP(0x23);         //g wb w
  for(count = 0; count < 15; count++) {
    SendData_EP(lut_g1[count]);
  } 
  SendCommand_EP(0x24);         //g bb b
  for(count = 0; count < 15; count++) {
    SendData_EP(lut_g2[count]);
  }
}

/*********************************************************************************
*@Function: SetLutRed_EP()
*@Description: this funtion writes to epaper module the lut for black colored pixels that are needed to 
* operate that module. they are provided by manufactuer but can be change if there 
* is no need of long living display.
*********************************************************************************/
void SetLutBw_EP() {
  unsigned int count;     
  SendCommand_EP(0x25);
  for(count = 0; count < 15; count++) {
    SendData_EP(lut_vcom1[count]);
  } 
  SendCommand_EP(0x26);
  for(count = 0; count < 15; count++) {
    SendData_EP(lut_red0[count]);
  } 
  SendCommand_EP(0x27);
  for(count = 0; count < 15; count++) {
    SendData_EP(lut_red1[count]);
  } 
}

/*********************************************************************************
*@Function: DisplayFrame_EP( uint8_t* frame_buffer_black,  uint8_t* frame_buffer_red)
*@Arguments: frame_buffer_black-- buffer that holds black pixels
*						 frame_buffer_red  -- buffer that holds red pixels
*@Description: 
*********************************************************************************/


void DisplayFrame_EP( uint8_t* frame_buffer_black,  uint8_t* frame_buffer_red) {
  unsigned char temp;																						//temporary varialble that helps in sending
  if (frame_buffer_black != 0) {
    SendCommand_EP(DATA_START_TRANSMISSION_1);									//send commant to start transmision of black buffor
     mywait(2);																									//waits for delays on module states
    for ( i = 0; i < width * height / 8; i++) {
      temp = 0x00;
      for ( bit = 0; bit < 4; bit++) {
        if ((frame_buffer_black[i] & (0x80 >> bit)) != 0) {			//because pixels are holded in 1 bit in buffer but in module in2 bits here the 4 bits are changed to 8 bits
          temp |= 0xC0 >> (bit * 2);
        }
      }
      SendData_EP(temp);
      temp = 0x00;
      for ( bit = 4; bit < 8; bit++) {
        if ((frame_buffer_black[i] & (0x80 >> bit)) != 0) {
          temp |= 0xC0 >> ((bit - 4) * 2);
        }
      }
      SendData_EP(temp);
    }
    mywait(2);																									//waits for delays on module states
  }
  if (frame_buffer_red != 0) {
    SendCommand_EP(DATA_START_TRANSMISSION_2);									//send command to write red pixels value
    mywait(2);
    for ( i = 0; i < width * height / 8; i++) {
      SendData_EP(frame_buffer_red[i]);
    }
     mywait(2);
  }
  SendCommand_EP(DISPLAY_REFRESH);															//refresh diplay to show the result
  WaitUntilIdle_EP();
}



/*********************************************************************************
*@Function:SetPixel_EP(uint8_t *Buffer,uint8_t x, uint8_t y, uint8_t color)
*@Arguments: Buffer-- the pointer to buffer we want to set pixel in(red or black)
						 x     --	the x value of the point in buffer
						 y		 --	the y value of the point in buffer
             color --	the color(1=red or black depending on buffer) or uncolored(0=white) 
*@Description: 
*********************************************************************************/

void SetPixel_EP(uint8_t *Buffer,uint8_t x, uint8_t y, uint8_t color){
\
    if ( x >= EPD_WIDTH  || y >= EPD_HEIGHT){															//check if writing on the buffer
        return;
    }
    if (color){																												
        Buffer[(x + y * EPD_WIDTH) / 8] &= ~(0x80 >> (x % 8));						//set pixel to color
    }
    else{
        Buffer[(x + y * EPD_WIDTH) / 8] |= 0x80 >> (x % 8);								//set pixel to non color
    }
}


/*********************************************************************************
*@Function:DrawLine_EP(uint8_t *Buffer, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color)
*@Arguments: Buffer -- the pointer to buffer we writing in
						 x0			-- x coordinate of the begging of the line
						 y0			-- y coordinate of the begging of the line
						 x1			-- x coordinate of the ending of the line
						 y1			-- y coordinate of the ending of the line
						 color  --	the color(1=red or black depending on buffer) or uncolored(0=white) 
*@Description: this function uses Bresenham algorithm to draw the best line connecting two points
*@Source: 		https://www.thecrazyprogrammer.com/2017/01/bresenhams-line-drawing-algorithm-c-c.html
*********************************************************************************/

void DrawLine_EP(uint8_t *Buffer, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color){
    // Bresenham algorithm
    int dx = x1 - x0 >= 0 ? x1 - x0 : x0 - x1;
    int sx = x0 < x1 ? 1 : -1;
    int dy = y1 - y0 <= 0 ? y1 - y0 : y0 - y1;
    int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;
    while((x0 != x1) && (y0 != y1)){
        SetPixel_EP(Buffer, x0, y0 , color);
        if (2 * err >= dy){
            err += dy;
            x0 += sx;
        }
        if (2 * err <= dx){
            err += dx;
            y0 += sy;
        }
    }
}

 
/*********************************************************************************
*@Function:DrawHorizontalLine_EP
*@Arguments: Buffer -- the pointer to buffer we writing in
						 x0			-- x coordinate of the begging of the line
						 y0			-- y coordinate of the begging of the line
						 lenght	-- lenght of the horizontal line
						 color  --	the color(1=red or black depending on buffer) or uncolored(0=white) 
*@Description: this function is drawning hozizontal line on the buffor.
*********************************************************************************/


void DrawHorizontalLine_EP(uint8_t *Buffer, uint8_t x, uint8_t y, uint8_t lenght, uint8_t color){
    uint8_t i=0;
		for ( i=x; i<x + lenght; i++){
        SetPixel_EP(Buffer, i, y, color);
    }
}
 
/*********************************************************************************
*@Function:DrawVerticalLine_EP
*@Arguments: Buffer -- the pointer to buffer we writing in
						 x0			-- x coordinate of the begging of the line
						 y0			-- y coordinate of the begging of the line
						 pitch	-- height of the vertical line
						 color  --	the color(1=red or black depending on buffer) or uncolored(0=white) 
*@Description:  this function is drawning vertical line on the buffor.
*********************************************************************************/


 void DrawVerticalLine_EP(uint8_t *Buffer, uint8_t x, uint8_t y, uint8_t pitch, uint8_t color){
	uint8_t i=0;
	 for (i=y; i<y + pitch; i++){
        SetPixel_EP(Buffer, x, i, color);
    }
}
 
/*********************************************************************************
*@Function:DrawRectangle_EP
*@Arguments: Buffer -- the pointer to buffer we writing in
						 x0			-- x coordinate of the begging corner of the rectangle
						 y0			-- y coordinate of the begging corner of the rectangle
						 x1			-- x coordinate of the ending corner of the rectangle
						 y1			-- y coordinate of the ending corner of the rectangle
						 color  --	the color(1=red or black depending on buffer) or uncolored(0=white) 
*@Description: this function is drawning rectangle on the buffer
*********************************************************************************/

 void DrawRectangle_EP(uint8_t *Buffer, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color){
    uint8_t min_x = x1 > x0 ? x0 : x1;																				//this is selecting wich point are minimal position in buffer
    uint8_t max_x = x1 > x0 ? x1 : x0;
    uint8_t min_y = y1 > y0 ? y0 : y1;
    uint8_t max_y = y1 > y0 ? y1 : y0;
    DrawHorizontalLine_EP(Buffer, min_x, min_y, max_x - min_x + 1, color);
    DrawHorizontalLine_EP(Buffer, min_x, max_y, max_x - min_x + 1, color);
    DrawVerticalLine_EP(Buffer, min_x, min_y, max_y - min_y + 1, color);
    DrawVerticalLine_EP(Buffer, max_x, min_y, max_y - min_y + 1, color);
}

/*********************************************************************************
*@Function:DrawFilledRectangle_EP
*@Arguments: Buffer -- the pointer to buffer we writing in
						 x0			-- x coordinate of the begging corner of the rectangle
						 y0			-- y coordinate of the begging corner of the rectangle
						 x1			-- x coordinate of the ending corner of the rectangle
						 y1			-- y coordinate of the ending corner of the rectangle
						 color  --	the color(1=red or black depending on buffer) or uncolored(0=white) 
*@Description: this is drawnig filled rectangle on the buffer.
*********************************************************************************/


void DrawFilledRectangle_EP(uint8_t *Buffer, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color){
    uint8_t min_x = x1 > x0 ? x0 : x1;
    uint8_t max_x = x1 > x0 ? x1 : x0;
    uint8_t min_y = y1 > y0 ? y0 : y1;
    uint8_t max_y = y1 > y0 ? y1 : y0;
	  uint8_t i=0;
    for ( i=min_x; i < max_x+1; i++){
        DrawVerticalLine_EP(Buffer, i, min_y, max_y - min_y + 1, color);
    }
}

/*********************************************************************************
*@Function:DrawCircle
*@Arguments: 
*@Description: not working yet
*********************************************************************************/




void DrawCircle(uint8_t *Buffer, uint8_t x, uint8_t y, uint8_t radius, uint8_t color){
    // Bresenham algorithm
    int16_t x_pos = -radius;
    int16_t y_pos = 0;
    int16_t err = 2 - 2 * radius;
	  int16_t e2 = err;
    if (x >= width || y >= height){
        return;
    }
    while ( 1 ){
        SetPixel_EP(Buffer, x - x_pos, y + y_pos, color);
        SetPixel_EP(Buffer, x + x_pos, y + y_pos, color);
        SetPixel_EP(Buffer, x + x_pos, y - y_pos, color);
        SetPixel_EP(Buffer, x - x_pos, y - y_pos, color);
        if (e2 <= y_pos){
            y_pos += 1;
            err += y_pos * 2 + 1;
            if(-x_pos == y_pos && e2 <= x_pos){
                e2 = 0;
            }
        }
        if (e2 > x_pos){
            x_pos += 1;
            err += x_pos * 2 + 1;
        }
        if (x_pos > 0){
            break;
        }
    }
}
 
/*********************************************************************************
*@Function:DrawFilledCircle_EP
*@Arguments: 
*@Description: not working yet
*********************************************************************************/


void DrawFilledCircle_EP(uint8_t *frame_buffer, uint8_t x, uint8_t y, uint8_t radius, uint8_t color){
    // Bresenham algorithm
    int16_t x_pos = -radius;
    int16_t y_pos = 0;
    int16_t err = 2 - 2 * radius;
	  int16_t e2 = err;
    if (x >= width || y >= height){
        return;
    }
    while ( 1 ){
        SetPixel_EP(frame_buffer, x - x_pos, y + y_pos, color);
        SetPixel_EP(frame_buffer, x + x_pos, y + y_pos, color);
        SetPixel_EP(frame_buffer, x + x_pos, y - y_pos, color);
        SetPixel_EP(frame_buffer, x - x_pos, y - y_pos, color);
        DrawHorizontalLine_EP(frame_buffer, x + x_pos, y + y_pos, 2 * (-x_pos) + 1, color);
        DrawHorizontalLine_EP(frame_buffer, x + x_pos, y - y_pos, 2 * (-x_pos) + 1, color);
        
        if (e2 <= y_pos){
            y_pos += 1;
            err += y_pos * 2 + 1;
            if(-x_pos == y_pos && e2 <= x_pos){
                e2 = 0;
            }
        }
        if (e2 > x_pos){
            x_pos  += 1;
            err += x_pos * 2 + 1;
        }
        if (x_pos > 0){
            break;
        }
    }
}

/*********************************************************************************
*@Function:DrawCharAt_EP
*@Arguments: Buffer 		-- the pointer to buffer we writing in
						 x0					-- x coordinate of the begging of char
						 y0					-- y coordinate of the begging of char
						 ascii_char	-- scii character that needs to be writen
						 fontsize		-- fize of the font used to write char
						 font				-- Pointer to the font table in memory
						 color 		  --	the color(1=red or black depending on buffer) or uncolored(0=white) 
*@Description:  function draws character on the buffor
*********************************************************************************/



void DrawCharAt_EP(uint8_t *Buffer, uint8_t x, uint8_t y, uint8_t ascii_char,const uint8_t *fontsize,const uint8_t* font, uint8_t color) {
    int i, j;
	
    uint16_t char_offset = (ascii_char - ' ') * fontsize[0] * (fontsize[1] / 8 + (fontsize[1] % 8 ? 1 : 0));
    const uint8_t* ptr = &font[char_offset];
 
    for (j = 0; j < fontsize[0]; j++) {
        for (i = 0; i <fontsize[1]; i++) {
            if (*ptr & (0x80 >> (i % 8))) {
                SetPixel_EP(Buffer, x + i, y + j, color);
            }
            if (i % 8 == 7) {
                ptr++;
            }
        }
        if (fontsize[1] % 8 != 0) {
            ptr++;
        }
    }
}


/*********************************************************************************
*@Function:  ImageAt
*@Arguments: Buffer 		-- the pointer to buffer we writing in
						 x					-- x coordinate of the begging of image
						 y					-- y coordinate of the begging of image
						 imagesize	-- size of the image
						 image			-- pointer to image buffor
						 color 		  --	the color(1=red or black depending on buffer) or uncolored(0=white) 
*@Description: its basicly can draw image on the buffor
*********************************************************************************/


void ImageAt(uint8_t *Buffer, uint8_t x, uint8_t y,const uint8_t *imagesize,const uint8_t* image, uint8_t color) {
    int i, j;
	

    const uint8_t* ptr = &image[0];
 
    for (j = 0; j < imagesize[0]; j++) {
        for (i = 0; i <imagesize[1]; i++) {
            if (*ptr & (0x80 >> (i % 8))) {
                SetPixel_EP(Buffer, x + i, y + j, color);
            }
            if (i % 8 == 7) {
                ptr++;
            }
        }
        if (imagesize[1] % 8 != 0) {
            ptr++;
        }
    }
}
 
/*********************************************************************************
*@Function:DrawStringAt_EP
*@Arguments: Buffer 		-- the pointer to buffer we writing in
						 x					-- x coordinate of the begging of string
						 y					-- y coordinate of the begging of string
						 text	-- text ponter to char string
						 image			-- pointer to image buffor
						 fontsize 	-- pointer tothe tabe of sizes
						 font				--pointer to font buffor
						 color			--the color(1=red or black depending on buffer) or uncolored(0=white) 
*@Description: 
*********************************************************************************/

void DrawStringAt_EP(uint8_t *Buffer, uint8_t x, uint8_t y,  char *text,const uint8_t *fontsize,const uint8_t* font, uint8_t color) {
    const char* p_text = text;
    unsigned int counter = 0;
    int refcolumn = x;
    
    /* Send the string character by character on EPD */
    while (*p_text != 0) {
        /* Display one character on EPD */
        DrawCharAt_EP(Buffer, refcolumn, y, *p_text,fontsize, font, color);
        /* Decrement the column position by 16 */
        refcolumn += fontsize[1];
        /* Point on the next character */
        p_text++;
        counter++;
    }
}




/*********************************************************************************

*@Description: buffer that holding black and red pixels value
*********************************************************************************/





 uint8_t IMAGE_BLACK[] = { // kolejna linijka to 50 miejsc w bufforze
/* 0X00,0X01,0XC8,0X00,0XC8,0X00, */
0x03,0xC0,0x0F,0xF0,0x0F,0xF0,0x03,0xC0,0x03,0xC0,0x00,0x00,0x03,0xC0,0x00,0x00,
0x0F,0xF0,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0x0F,0xF0,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0x03,0xC0,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0x03,0xC0,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0x00,0x00,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0x03,0xC0,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0x00,0x00,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF7,0XFF,
0X3F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XE1,0XFE,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XE3,0XFF,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XE3,0XFF,0X9F,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XE0,0X00,0X00,0X00,0X01,
0XFF,0XFF,0XE7,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,0XFF,0XFF,0XE3,0XFF,0X8F,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFE,0XFF,0XFF,0XE3,0XFF,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,0XE1,0XFE,0X1F,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,0XF0,0XFC,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,0XF0,
0X00,0X3F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,0XF8,0X00,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,
0XFF,0XFE,0X01,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0x0F,0xF0,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0X7E,0XFF,0XFF,0XFB,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,0X3E,0XFF,0XFF,0XE0,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,0XFF,
0XFF,0XF0,0X3E,0XFF,0XFF,0XC6,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,0XFF,0XFF,0XE0,0XCE,0XFF,0XFF,0XCE,0X7F,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,
0XFF,0XFF,0XFF,0X81,0X86,0XFF,0XFF,0XCE,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,0XFF,0XFF,0X02,0X06,0XFF,0XFF,
0XC6,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFE,0XFF,0XFF,0XFF,0XFE,0X0C,0X02,0XFF,0XFF,0XE0,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,0XFF,0XFE,0X18,0X02,
0XFF,0XFF,0XFB,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFE,0XFF,0XFF,0XFF,0XFE,0X20,0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,0XFF,0XFE,
0XC0,0X02,0XFF,0XFF,0XFF,0X03,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,0XFF,0XFF,0X00,0X02,0XFF,0XFF,0XF8,0X00,0X7F,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0XFF,0XFF,0X00,0X02,0XFF,0XFF,0XF0,0X00,0X3F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,0XFF,0XFF,0X80,0X06,0XFF,0XFF,0XE0,
0XCC,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,
0XFF,0XFF,0XFF,0XFF,0XC0,0X0E,0XFF,0XFF,0XE3,0XC7,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,0XFF,0XFF,0XC0,0X0E,0XFF,
0XFF,0XE7,0XE7,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFE,0XFF,0XFF,0XFF,0XFF,0X00,0X1E,0XFF,0XFF,0XE7,0XE7,0X8F,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,0XFF,0XFC,0X00,
0X3E,0XFF,0XFF,0XE3,0XC7,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFE,0XFF,0X83,0XFF,0XF0,0X00,0X7E,0XFF,0XFF,0XE0,0X07,0X1F,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFC,0X01,0XFF,
0XE0,0X00,0XFE,0XFF,0XFF,0XF0,0X0F,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XF0,0X07,0XFF,0X80,0X01,0XFE,0XFF,0XFF,0XF8,0X1F,
0XBF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XC0,
0X18,0XFE,0X00,0X03,0XFE,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XC0,0XE0,0XF8,0X00,0X0F,0XFE,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFE,0XC7,0X00,0XE0,0X00,0X1F,0XFE,0XFF,0XFF,0XFF,0XFF,0X9F,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XDC,0X00,0XC0,0X00,0X3F,0XFE,
0XFF,0XFF,0XF8,0X3F,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFE,0XE0,0X00,0X00,0X00,0XFF,0XFE,0XFF,0XFF,0XF0,0X0F,0X1F,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XC0,0X00,0X00,0X03,
0XFF,0XFE,0XFF,0XFF,0XE0,0X03,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFE,0XE0,0X00,0X00,0X07,0XFF,0XFE,0XFF,0XFF,0XE3,0XC1,0X9F,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XE0,0X00,
0X00,0X1F,0XFF,0XFE,0XFF,0XFF,0XE7,0XE0,0X9F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XF0,0X00,0X00,0XFF,0XFF,0XFE,0XFF,0XFF,0XE7,
0XF8,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,
0XF8,0X00,0X03,0XFF,0XFF,0XFE,0XFF,0XFF,0XE7,0XFC,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFC,0X00,0X3F,0XFF,0XFF,0XFE,0XFF,
0XFF,0XE3,0XFE,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFE,0XFF,0XB7,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,0XF1,0XFF,0X1F,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFE,0XFF,0XFF,0XF3,0XFF,0X9F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFE,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,0XFF,0XDF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0XFF,0X87,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFE,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,0XFF,0XC3,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,
0XFF,0XFF,0XFF,0XE3,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFE,0X7F,0XFF,0XFF,0XFF,0XFF,0XFC,0XFF,0XFF,0XFF,0XF3,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X00,0X00,0X00,0X00,
0X00,0X01,0XFF,0XFF,0XFF,0XE3,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XE3,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XC3,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0X87,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X1F,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0X3F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0X3F,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X1F,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X0F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0X87,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XCF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XF8,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF8,0X70,0X3F,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFA,0XFF,0XFF,0XFF,0XFF,0XF0,0X20,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XE0,0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XC0,0X1F,0XFF,0XFF,0XFF,0XE0,
0X02,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X00,0X00,0X0F,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0X00,0X0F,0XFF,0XFF,0XFF,0XE3,0X87,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFC,0X00,0X00,0X03,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0X00,0X07,0XFF,0XFF,
0XFF,0XE7,0X8F,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF0,0X00,0X00,0X01,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFE,0X00,0X02,0X7F,0XFF,0XFF,0XE7,0XCF,0X8F,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XE0,0X00,0X00,0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,0X00,0X00,
0X7F,0XFF,0XFF,0XE7,0XCF,0X9F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XC0,0X00,0X00,0X00,
0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XF8,0X00,0X00,0XFF,0XFF,0XFF,0XE3,0XFF,0X1F,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XC0,0X0F,0XFE,0X00,0X3F,0XFF,0XFF,0XFF,0XFF,0XFF,0XF8,
0X30,0X00,0XFF,0XFF,0XFF,0XF3,0XFE,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X80,0XFF,
0XFF,0XE0,0X3F,0XFF,0XFF,0XFF,0XFF,0XFF,0XF8,0X30,0X00,0XFF,0XFF,0XFF,0XF7,0XFF,
0X3F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X81,0XFF,0XFF,0XF8,0X1F,0XFF,0XFF,0XFF,0XFF,
0XFF,0XF0,0X00,0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0X83,0XFF,0XFF,0XF8,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XF0,0X00,0X00,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X83,0XFF,0XFF,0XFC,0X1F,0XFF,0XFF,
0XFF,0XFF,0XFF,0X90,0X00,0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0X1F,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0X83,0XFF,0XFF,0XFC,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0X10,0X00,0X00,0XFF,
0XFF,0XFF,0XF8,0X1F,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X83,0XFF,0XFF,0XFC,0X1F,
0XFF,0XFF,0XFF,0XFF,0XFC,0X10,0X00,0X00,0XFF,0XFF,0XFF,0XF0,0X0F,0X1F,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0X81,0XFF,0XFF,0XF8,0X1F,0XFF,0XFF,0XFF,0XFF,0XF8,0X18,0X30,
0X00,0XFF,0XFF,0XFF,0XE0,0X03,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X80,0XFF,0XFF,
0XF0,0X3F,0XFF,0XFF,0XFF,0XFF,0XF8,0X08,0X38,0X00,0XFF,0XFF,0XFF,0XE3,0XC1,0X9F,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XC0,0X1F,0XFF,0X80,0X3F,0XFF,0XFF,0XFF,0XFF,0XF0,
0X08,0X30,0X01,0XFF,0XFF,0XFF,0XE7,0XF0,0X9F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XC0,
0X00,0X00,0X00,0X7F,0XFF,0XFF,0XFF,0XFF,0XE0,0X08,0X00,0X01,0XFF,0XFF,0XFF,0XE7,
0XF8,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XE0,0X00,0X00,0X00,0X7F,0XFF,0XFF,0XFF,
0XFF,0XE0,0XE4,0X00,0X01,0XFF,0XFF,0XFF,0XE7,0XFC,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XF0,0X00,0X00,0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0XE0,0XE2,0X00,0X03,0XFF,0XFF,
0XFF,0XE3,0XFE,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF8,0X00,0X00,0X03,0XFF,0XFF,
0XFF,0XFF,0XFF,0XC0,0XE2,0X00,0X07,0XFF,0XFF,0XFF,0XF1,0XFF,0X1F,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFE,0X00,0X00,0X0F,0XFF,0XFF,0XFF,0XFF,0XFF,0XC0,0X41,0X00,0X0F,
0XFF,0XFF,0XFF,0XF3,0XFF,0X9F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XC0,0X00,0X7F,
0XFF,0XFF,0XFF,0XFF,0XFF,0XC0,0X00,0XC0,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XC0,0X00,
0X30,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XC0,0X00,0X07,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XC0,0X00,0X00,0X3F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XC0,0X00,0X00,0X7F,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF8,0X03,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XC0,0X40,0X00,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0X80,0X00,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XC0,0XE0,0X00,0X7F,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,0X00,0X00,0X07,0XFF,
0XFF,0XFF,0XFF,0XFF,0XE0,0XE0,0X00,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XF8,0X00,0X00,0X01,0XFF,0XFF,0XFF,0XFF,0XFF,0XE0,0XE0,0X00,
0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XE0,0X00,0X00,
0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0XE0,0X00,0X00,0X3F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XE0,0X00,0X00,0X00,0X7F,0XFF,0XFF,0XFF,0XFF,0XF0,
0X00,0X00,0X3F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XC0,
0X00,0X60,0X00,0X3F,0XFF,0XFF,0XFF,0XFF,0XF8,0X00,0X01,0X1F,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X80,0X7F,0XFF,0XC0,0X3F,0XFF,0XFF,0XFF,
0XFF,0XF8,0X00,0X03,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0X81,0XFF,0XFF,0XF0,0X1F,0XFF,0XFF,0XFF,0XFF,0XFC,0X00,0X07,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X83,0XFF,0XFF,0XF8,0X1F,0XFF,
0XFF,0XFF,0XFF,0XFF,0X00,0X0F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0X83,0XFF,0XFF,0XFC,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0X80,0X3F,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X83,0XFF,0XFF,0XFC,
0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0X83,0XFF,0XFF,0XFC,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X83,0XFF,
0XFF,0XF8,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X81,0XFF,0XFF,0XF0,0X3F,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0X80,0X7F,0XFF,0XC0,0X3F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XC0,0X00,0X00,0X00,0X3F,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XE0,0X00,0X00,0X00,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF0,0X00,0X00,0X00,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XF8,0X00,0X00,0X01,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,0X00,0X00,
0X07,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X80,0X00,0X3F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFC,0X07,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XE0,0XFF,0XF0,0X7F,0XFF,0XFF,0XFF,0XFF,0X00,0X00,
0X00,0X01,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XC0,
0X7F,0XE0,0X3F,0XFF,0XFF,0XFF,0XFD,0X80,0X00,0X00,0X03,0X1F,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XC0,0X3F,0XE0,0X1F,0XFF,0XFF,0XFF,0XF8,
0X80,0X00,0X00,0X02,0X0F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0X80,0X3F,0XC0,0X1F,0XFF,0XFF,0XFF,0XF0,0X40,0X00,0X00,0X04,0X0F,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X80,0X3F,0XC0,0X1F,0XFF,0XFF,
0XFF,0XF0,0X60,0X00,0X00,0X0C,0X07,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XC0,0X3F,0XE0,0X1F,0XFF,0XFF,0XFF,0XF0,0X20,0X00,0X00,0X08,0X07,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XC0,0X7F,0XE0,0X3F,
0XFF,0XFF,0XFF,0XF0,0X30,0X00,0X00,0X10,0X07,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XF0,0XFF,0XF8,0X7F,0XFF,0XFF,0XFF,0XF0,0X10,0X00,0X00,
0X30,0X07,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XF0,0X18,0X00,0X00,0X20,0X07,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF0,0X08,
0X00,0X00,0X60,0X07,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF0,0X04,0X00,0X00,0X40,0X07,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XF0,0X06,0X00,0X00,0X80,0X07,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF0,0X02,0X00,0X01,0X80,0X07,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0X0F,0XFF,0XFF,
0XFF,0XFF,0XF0,0X03,0X00,0X01,0X00,0X07,0XFF,0XFF,0XFF,0XFF,0XE0,0X00,0X00,0X00,
0XFF,0XFF,0XFF,0XFF,0X3F,0XF8,0X03,0XFF,0XFF,0XFF,0XFF,0XF0,0X01,0X00,0X02,0X00,
0X07,0XFF,0XFF,0XFF,0XFF,0X00,0X00,0X00,0X00,0XFF,0XFF,0XFF,0XF8,0X07,0XF0,0X00,
0XFF,0XFF,0XFF,0XFF,0XF0,0X01,0X80,0X06,0X00,0X07,0XFF,0XFF,0XFF,0XFC,0X00,0X00,
0X00,0X00,0XFF,0XFF,0XFF,0XF0,0X01,0XE0,0X00,0X7F,0XFF,0XFF,0XFF,0XF0,0X00,0X80,
0X04,0X00,0X07,0XFF,0XFF,0XFF,0XF0,0X00,0X38,0X00,0X00,0XFF,0XFF,0XFF,0XE0,0X00,
0XC0,0X00,0X7F,0XFF,0XFF,0XFF,0XF0,0X00,0X40,0X08,0X00,0X07,0XFF,0XFF,0XFF,0XE0,
0X00,0XFE,0X00,0X00,0XFF,0XFF,0XFF,0XC0,0X00,0X00,0X00,0X3F,0XFF,0XFF,0XFF,0XF0,
0X00,0X40,0X08,0X00,0X07,0XFF,0XFF,0XFF,0XC0,0X03,0XFF,0X80,0X00,0XFF,0XFF,0XFF,
0XC0,0X00,0X00,0XE0,0X3F,0XFF,0XFF,0XFF,0XF0,0X00,0X20,0X10,0X00,0X07,0XFF,0XFF,
0XFF,0X80,0X0F,0XFF,0XF8,0X00,0XFF,0XFF,0XFF,0X81,0XF8,0X01,0XF8,0X3F,0XFF,0XFF,
0XFF,0XF0,0X00,0X30,0X30,0X00,0X07,0XFF,0XFF,0XFF,0X00,0X3F,0XFF,0XFC,0X00,0XFF,
0XFF,0XFF,0X83,0XFE,0X03,0XFC,0X1F,0XFF,0XFF,0XFF,0XF0,0X00,0X10,0X20,0X00,0X07,
0XFF,0XFF,0XFE,0X00,0XFF,0XE7,0XFE,0X00,0XFF,0XFF,0XFF,0X83,0XFE,0X07,0XFC,0X1F,
0XFF,0XFF,0XFF,0XF0,0X00,0X18,0X40,0X00,0X07,0XFF,0XFF,0XFC,0X01,0XFF,0X81,0XFF,
0X00,0XFF,0XFF,0XFF,0X87,0XFE,0X07,0XFC,0X1F,0XFF,0XFF,0XFF,0XF0,0X00,0X08,0XC0,
0X00,0X07,0XFF,0XFF,0XFC,0X01,0XFF,0X00,0X7F,0X00,0XFF,0XFF,0XFF,0X87,0XFC,0X0F,
0XFC,0X1F,0XFF,0XFF,0XFF,0XF0,0X00,0X04,0X80,0X00,0X07,0XFF,0XFF,0XF8,0X01,0XFF,
0XC0,0X7F,0X80,0XFF,0XFF,0XFF,0X87,0XFC,0X0F,0XFC,0X1F,0XFF,0XFF,0XFF,0XF0,0X00,
0X05,0X00,0X00,0X07,0XFF,0XFF,0XF8,0X00,0XFF,0XF8,0X00,0X00,0XFF,0XFF,0XFF,0X83,
0XF8,0X1F,0XFC,0X1F,0XFF,0XFF,0XFF,0XF0,0X00,0X03,0X00,0X00,0X07,0XFF,0XFF,0XF0,
0X00,0X1F,0XFE,0X00,0X00,0XFF,0XFF,0XFF,0X81,0XF0,0X0F,0XF8,0X1F,0XFF,0XFF,0XFF,
0XF0,0X00,0X03,0X00,0X00,0X07,0XFF,0XFF,0XF0,0X00,0X07,0XFF,0X80,0X00,0XFF,0XFF,
0XFF,0X80,0X00,0X07,0XF0,0X3F,0XFF,0XFF,0XFF,0XF0,0X00,0X01,0X00,0X00,0X07,0XFF,
0XFF,0XF1,0XFF,0X81,0XFF,0X80,0X00,0XFF,0XFF,0XFF,0XC0,0X00,0X00,0XE0,0X3F,0XFF,
0XFF,0XFF,0XF0,0X00,0X01,0X80,0X00,0X07,0XFF,0XFF,0XE0,0XFF,0X00,0X7F,0X80,0X00,
0XFF,0XFF,0XFF,0XC0,0X00,0X40,0X00,0X3F,0XFF,0XFF,0XFF,0XF0,0X47,0X80,0X80,0X00,
0X07,0XFF,0XFF,0XE0,0X7F,0XC1,0X3F,0X80,0X00,0XFF,0XFF,0XFF,0XE0,0X00,0XE0,0X00,
0X7F,0XFF,0XFF,0XFF,0XF0,0X4F,0XC0,0X40,0X00,0X07,0XFF,0XFF,0XE0,0X3F,0XF9,0X4F,
0X00,0X00,0XFF,0XFF,0XFF,0XF0,0X03,0XE0,0X00,0XFF,0XFF,0XFF,0XFF,0XF0,0X4C,0X40,
0X40,0X00,0X07,0XFF,0XFF,0XE0,0X1F,0XFE,0X78,0X00,0X00,0XFF,0XFF,0XFF,0XFC,0X07,
0XF8,0X01,0XFF,0XFF,0XFF,0XFF,0XF0,0X64,0XC0,0X40,0X00,0X07,0XFF,0XFF,0XE0,0X0F,
0XFF,0X80,0X00,0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,0X03,0XFF,0XFF,0XFF,0XFF,0XF0,
0X7F,0XC0,0X40,0X00,0X07,0XFF,0XFF,0XE0,0X05,0XFF,0XF0,0X00,0X00,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF0,0X1F,0X00,0XC0,0X00,0X07,0XFF,0XFF,
0XE0,0X00,0X3F,0XFC,0X00,0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XF0,0X00,0X00,0X80,0X00,0X07,0XFF,0XFF,0XE0,0X00,0X0F,0XFF,0X00,0X00,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF0,0X00,0X01,0X00,0X00,0X07,
0XFF,0XFF,0XE0,0X00,0X03,0XFF,0X80,0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XF0,0X00,0X01,0X00,0X00,0X07,0XFF,0XFF,0XE0,0X00,0X07,0XFF,0X80,
0X01,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF0,0X00,0X02,0X00,
0X00,0X07,0XFF,0XFF,0XE0,0X00,0X3F,0XFF,0X00,0X01,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XF8,0X3F,0XFF,0XFF,0XFF,0XF0,0X00,0X06,0X00,0X00,0X07,0XFF,0XFF,0XE0,0X00,0XFF,
0XFC,0X00,0X01,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF8,0X3F,0XFF,0XFF,0XFF,0XF0,0X00,
0X05,0X00,0X00,0X07,0XFF,0XFF,0XE0,0X01,0XFF,0XE0,0X00,0X01,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XF8,0X3F,0XFF,0XFF,0XFF,0XF0,0X00,0X0D,0X80,0X00,0X07,0XFF,0XFF,0XE0,
0X01,0XFF,0X00,0X00,0X03,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF8,0X3F,0XFF,0XFF,0XFF,
0XF0,0X00,0X08,0X80,0X00,0X07,0XFF,0XFF,0XE0,0X01,0XFF,0X80,0X00,0X03,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XF8,0X3F,0XFF,0XFF,0XFF,0XF0,0X00,0X10,0X40,0X00,0X07,0XFF,
0XFF,0XE0,0X00,0XFF,0XF0,0X00,0X03,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF8,0X3F,0XFF,
0XFF,0XFF,0XF0,0X00,0X10,0X40,0X00,0X07,0XFF,0XFF,0XE0,0X00,0X1F,0XFE,0X00,0X07,
0XFF,0XFF,0XFF,0XC0,0X00,0X00,0X00,0X3F,0XFF,0XFF,0XFF,0XF0,0X00,0X20,0X20,0X00,
0X07,0XFF,0XFF,0XE0,0X00,0X03,0XFF,0X80,0X07,0XFF,0XFF,0XFF,0X80,0X00,0X00,0X00,
0X3F,0XFF,0XFF,0XFF,0XF0,0X00,0X60,0X30,0X00,0X07,0XFF,0XFF,0XE0,0X00,0X07,0XFF,
0X80,0X0F,0XFF,0XFF,0XFF,0X80,0X00,0X00,0X00,0X3F,0XFF,0XFF,0XFF,0XF0,0X00,0X40,
0X10,0X00,0X07,0XFF,0XFF,0XE0,0X00,0X3F,0XFF,0X80,0X1F,0XFF,0XFF,0XFF,0X80,0X00,
0X00,0X00,0X3F,0XFF,0XFF,0XFF,0XF0,0X00,0XC0,0X08,0X00,0X07,0XFF,0XFF,0XE0,0X01,
0XFF,0XFF,0X00,0X1F,0XFF,0XFF,0XFF,0X80,0X00,0X00,0X00,0X3F,0XFF,0XFF,0XFF,0XF0,
0X00,0X80,0X0C,0X00,0X07,0XFF,0XFF,0XE0,0X01,0XFF,0XF8,0X00,0X3F,0XFF,0XFF,0XFF,
0X80,0X00,0X00,0X00,0X3F,0XFF,0XFF,0XFF,0XF0,0X01,0X80,0X04,0X00,0X07,0XFF,0XFF,
0XE0,0X01,0XFF,0XE0,0X00,0X7F,0XFF,0XFF,0XFF,0XC0,0X00,0X00,0X00,0X3F,0XFF,0XFF,
0XFF,0XF0,0X01,0X00,0X02,0X00,0X07,0XFF,0XFF,0XE0,0X01,0XFE,0X00,0X00,0XFF,0XFF,
0XFF,0XFF,0XC0,0XFF,0XFF,0XF8,0X3F,0XFF,0XFF,0XFF,0XF0,0X02,0X00,0X03,0X00,0X07,
0XFF,0XFF,0XE0,0X01,0XF0,0X00,0X03,0XFF,0XFF,0XFF,0XFF,0XE0,0XFF,0XFF,0XF8,0X3F,
0XFF,0XFF,0XFF,0XF0,0X06,0X00,0X01,0X00,0X07,0XFF,0XFF,0XE0,0X01,0X80,0X00,0X07,
0XFF,0XFF,0XFF,0XFF,0XE0,0XFF,0XFF,0XF8,0X3F,0XFF,0XFF,0XFF,0XF0,0X04,0X00,0X01,
0X80,0X07,0XFF,0XFF,0XE0,0X00,0X00,0X00,0X1F,0XFF,0XFF,0XFF,0XFF,0XE0,0XFF,0XFF,
0XF8,0X3F,0XFF,0XFF,0XFF,0XF0,0X0C,0X00,0X00,0X80,0X07,0XFF,0XFF,0XE0,0X00,0X00,
0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0XF0,0XFF,0XFF,0XF8,0X3F,0XFF,0XFF,0XFF,0XF0,0X08,
0X00,0X00,0X40,0X07,0XFF,0XFF,0XE0,0X00,0X00,0X0F,0XFF,0XFF,0XFF,0XFF,0XFF,0XF0,
0XFF,0XFF,0XF8,0X3F,0XFF,0XFF,0XFF,0XF0,0X18,0X00,0X00,0X60,0X07,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF8,0X3F,0XFF,0XFF,0XFF,
0XF0,0X10,0X00,0X00,0X20,0X07,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF0,0X20,0X00,0X00,0X10,0X07,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XF0,0X60,0X00,0X00,0X18,0X07,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF0,0X40,0X00,0X00,0X08,
0X07,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XF8,0XC0,0X00,0X00,0X04,0X0F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF8,0X80,0X00,
0X00,0X04,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X00,0X00,0X00,0X02,0X3F,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
};

 uint8_t IMAGE_RED[]  = {
/* 0X00,0X01,0XC8,0X00,0XC8,0X00, */
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFE,0X01,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XF1,0XF8,0X00,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XF0,0X00,
0X3F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XF0,0X00,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,
0XE0,0X00,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XE1,0XE3,0X1F,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XF1,0XC3,0XF3,0X0F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XC3,0X3B,0X0F,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XF1,0XC3,0X0F,0X0F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XC3,0X0F,0X0F,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XE3,
0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XC3,0X87,0X0F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XE0,0X7F,0XFF,0XFF,0XFF,0XFF,0XF1,0XC1,
0X03,0X0F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,
0XFF,0XE0,0X0F,0XFF,0XFF,0XFF,0XFF,0XF1,0XE0,0X00,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFC,0X03,0XFF,0XFF,0XFF,0XFF,
0XF1,0XE0,0X00,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0X8F,0XFF,0XFF,0X80,0X7F,0XFF,0XFF,0XFF,0XF1,0XF0,0X00,0X3F,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XF0,0X1F,0XFF,
0XFF,0XFF,0XF1,0XF8,0X00,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0X00,0X1F,0XFF,0XFF,0XFF,0XF1,0XFE,0X00,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XF8,0X07,
0X8F,0XFF,0XFF,0XFF,0XF1,0XFF,0X87,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XE0,0X1F,0X8F,0XC0,0X07,0XFF,0XF1,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,
0XE0,0XFF,0XCF,0X80,0X07,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XE3,0XFF,0XEF,0X00,0X07,0XFF,0XF1,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0X8F,0XFF,0XFF,0XFF,0XFF,0X1F,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0X1F,0XFF,
0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0X8F,0XFF,0XFE,0X01,0XFF,0X1F,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XF0,0X01,0XFF,
0X9F,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XE0,0X01,0XE0,0X80,0X0F,0XFF,0XF1,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XE7,0XFF,0XFF,0X8F,0XFF,0XE0,
0X03,0XE0,0X00,0X07,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XE0,0XFF,0XFF,0X8F,0XFF,0XE2,0X71,0XE0,0X00,0X07,0XFF,0XF1,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XE0,0X3F,0XFF,0X8F,
0XFF,0XE7,0X39,0XE0,0X00,0X0F,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XF0,0X07,0XFF,0X8F,0XFF,0XE7,0X38,0XFF,0XFF,0XFF,0XFF,
0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0X01,
0XFF,0X8F,0XFF,0XE3,0X10,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XC0,0X7F,0X8F,0XFF,0XE3,0X01,0XFF,0X1F,
0XC7,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XF8,0X3F,0X8F,0XFF,0XF3,0X81,0XFF,0X1F,0XC7,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XC0,0X1F,0X8F,0XFF,0XFF,0XC7,
0XFF,0X1F,0XC7,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFE,0X00,0X0F,0X8F,0XFF,0XFF,0XFF,0XF8,0X00,0X07,0XFF,0XF1,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF0,0X07,0X87,0X8F,0XFF,
0XFF,0XFF,0XF8,0X00,0X0F,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XE0,0X1F,0XC7,0X8F,0XFF,0XFF,0XFF,0XF8,0X00,0X0F,0XFF,0XF1,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XE0,0XFF,0XC7,
0X8F,0XFE,0X00,0X01,0XFF,0X1E,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XE3,0XFF,0XC7,0X8F,0XFE,0X00,0X01,0XFF,0X1F,0XFF,
0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XC7,0X8F,0XFE,0X00,0X01,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFE,0X03,0XFF,0XF8,
0X7F,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XC0,0X7F,0XF8,0X1F,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF8,0X00,0XFF,0X8F,0XFF,0XF8,
0X1F,0XF8,0X07,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XF0,0X00,0XFF,0X8F,0XFF,0XFE,0X03,0XF8,0X01,0XFF,0XFF,0XF1,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XE0,0X00,0XFF,0X8F,
0XFF,0XFF,0X83,0XF8,0XC0,0X7F,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XE0,0X00,0XFF,0X8F,0XFF,0XFC,0X07,0XF8,0XF0,0X1F,0XFF,
0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XE3,0X31,
0XFF,0X8F,0XFF,0XE0,0X3F,0XF8,0XFC,0X07,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XE3,0X38,0XFF,0X8F,0XFF,0X00,0XFF,0XF8,0XFF,
0X03,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XE3,0X38,0XFF,0X8F,0XFE,0X07,0XFF,0XF8,0XFF,0XC3,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XE3,0X18,0XFF,0X8F,0XFE,0X00,0X01,
0XF8,0XFF,0XF3,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XE3,0X00,0XFF,0X8F,0XFE,0X00,0X01,0XF8,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XE3,0X80,0XFF,0X8F,0XFE,
0X00,0X01,0XF8,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XF1,0X81,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XC3,0XFF,
0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,0X00,0X00,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFC,0X00,0X00,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,0X00,0X00,0XFF,0X8F,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFC,0X00,0X00,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XE3,0XF1,0XFF,0X8F,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XE3,0XF9,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XE3,0XF8,
0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XE3,0XF8,0XFF,0X80,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X01,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XE1,0XF0,0XFF,0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF0,0X00,0XFF,0X8F,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XF0,0X01,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF8,0X01,0XFF,0X8F,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFE,0X07,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF8,
0X00,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF0,0X00,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XE0,0X00,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XE0,0X00,0XFF,0X8F,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XE3,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XE3,0XFF,0XFF,0X8F,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XE3,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF0,0XFF,
0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XE0,0X00,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XE0,0X00,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XE0,0X00,0XFF,0X8F,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XE0,0X00,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XE0,0X00,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XE0,
0X00,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XE0,0X00,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XE0,0X00,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XE1,0XFF,0X8F,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XF9,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF8,0XFF,0X8F,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF8,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XE0,0X00,
0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XE0,0X00,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XE0,0X01,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XE0,0X03,0XFF,0X8F,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XBF,0XFF,0XFF,0XFF,0XFF,0XFF,
0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0X3F,0XFF,0XFF,0XFF,0X87,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X3F,0XFF,0XFF,0X1E,
0X03,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFE,0XFF,0XBF,0XCF,0XFE,0X3E,0X01,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0X3F,0XFF,0X8F,
0XFE,0X3C,0X01,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X3C,0X0F,0X9F,0XFE,0X3C,0X30,0XFF,0X8F,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF0,
0X03,0XFF,0XFE,0X3C,0X78,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XE0,0X00,0XFF,0XFE,0X38,0X78,0XFF,0X8F,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XC0,0X00,0XFF,0XFE,0X18,0X78,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XC0,0X00,0X7F,0XFE,0X00,0XF8,
0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0X80,0X00,0X3F,0XFF,0X00,0XF8,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X80,0X00,0X3F,0XFF,
0X01,0XF0,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0X80,0X00,0X3F,0XFF,0XC3,0XF1,0XFF,0X8F,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0X00,0X00,
0X31,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0X00,0X00,0X31,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0X80,0X00,0X3F,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X80,0X00,0X3F,0XFF,0XFF,0XFF,0XFF,
0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0X80,0X00,0X7F,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XC0,0X00,0X7F,0XFF,0XFF,
0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XE0,0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF0,0X01,0XFF,
0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X78,0X03,0XDF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0X3F,
0X1F,0X8F,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0X7F,0XFF,0XCF,0XFF,0XFF,0XFF,0XFF,0X8F,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFE,0XFF,0X3F,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X3F,0XFF,0XFF,0XFF,0XFF,
0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0X3F,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X0F,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X0F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,};





