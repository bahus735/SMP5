#include "MKL25Z4.h"

#define LCD_4B_1L                 0x20 			//(4-bit interface, 1 line, 5*7 Pixels
	

#define LCD_MODE_DEC_SOFF         0x04			//Decrement Address Counter Display Shift :OFF
#define LCD_MODE_DEC_SON          0x05			//Decrement Address Counter Display Shift :ON
#define LCD_MODE_INC_SOFF         0x06			//increment Address Counter Display Shift :OFF
#define LCD_MODE_INC_SON          0x07			//increment Address Counter Display Shift :On


#define LCD_ROLL_ONE_RIGHT        0x1E			//Scroll display one character right (all lines)
#define LCD_ROLL_ONE_LEFT         0x18			//Scroll display one character left (all lines)


#define LCD_HOME                  0x2				//Home (move cursor to top/left character position) 
#define LCD_MOVE_ONE_LEFT         0x10			//Move cursor one character left 
#define LCD_MOVE_ONE_RIGHT        0x14			//Move cursor one character right 

#define LCD_CURSOR_UNDERLINE 		  0x0E			//Turn on visible underline cursor
#define LCD_CURSOR_BLINK      	  0x0F			//Turn on visible blinking-block cursor 
#define LCD_CURSOR_INV            0x0C			//Make cursor invisible 

#define LCD_DISPLAY_BLANK         0x08			//Blank the display (without clearing)
#define LCD_DISPLAY_RESTORE       0x0C			//Restore the display (with cursor hidden) 
#define LCD_DIPLAY_CLEAN					0x01 			//Clear Screen 

#define LCD_CURSOR_SET_POS(x) 		(0x80+x)  //Set cursor position (DDRAM address) 
#define LCD_CURSOR_SET_POI(x) 		(0x40+x)	//Set pointer in character-generator RAM (CG RAM address) 



void LCD_e_on(void);
void LCD_e_off(void);
void LCD_rs_on(void);
void LCD_rs_off(void);
void LCD_d0_on(void);
void LCD_d0_off(void);
void LCD_d1_on(void);
void LCD_d1_off(void);
void LCD_d2_on(void);
void LCD_d2_off(void);
void LCD_d3_on(void);
void LCD_d3_off(void);
void LCD_d4_on(void);
void LCD_d4_off(void);
void LCD_d5_on(void);
void LCD_d5_off(void);
void LCD_d6_on(void);
void LCD_d6_off(void);
void LCD_d7_on(void);
void LCD_d7_off(void);
void LCD_d_off(void);
void LCD_8bit_set(uint8_t);
void LCD_8bit_reset(void);
void LCD_COMMAND(uint8_t);
void LCD_INIT(void);
