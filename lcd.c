#include "lcd.h"
#include "MKL25Z4.h"
const uint32_t RS= 1UL<<3;				
const uint32_t E= 1UL<<2;
const uint32_t D0= 1UL<<4;
const uint32_t D1= 1UL<<5;
const uint32_t D2= 1UL<<20;
const uint32_t D3= 1UL<<21;
const uint32_t D4= 1UL<<22;
const uint32_t D5= 1UL<<23;
const uint32_t D6= 1UL<<29;
const uint32_t D7= 1UL<<30;

 void LCD_e_on(void){
PTE->PSOR=E;    
}

 void LCD_e_off(void){
PTE->PCOR=E;     
 }
 
 void LCD_rs_on(void){
PTE->PSOR=RS;    
}

void LCD_rs_off(void){
PTE->PCOR=RS;     
 }
void  LCD_d0_on(void){
PTE->PSOR=D0;    
}

void LCD_d0_off(void){
PTE->PCOR=D0;     
 }
 
void   LCD_d1_on(void){
PTE->PSOR=D1;    
}

void LCD_d1_off(void){
PTE->PCOR=D1;     
 }
 
void   LCD_d2_on(void){
PTE->PSOR=D2;    
}

void LCD_d2_off(void){
PTE->PCOR=D2;     
 }
 
void  LCD_d3_on(void){
PTE->PSOR=D3;    
}

void LCD_d3_off(void){
PTE->PCOR=D3;     
 }
void  LCD_d4_on(void){
PTE->PSOR=D3;    
}

void LCD_d4_off(void){
PTE->PCOR=D3;     
 }
void  LCD_d5_on(void){
PTE->PSOR=D5;    
}

void LCD_d5_off(void){
PTE->PCOR=D5;     
 }
void  LCD_d6_on(void){
PTE->PSOR=D6;    
}

void LCD_d6_off(void){
PTE->PCOR=D6;     
 }
void  LCD_d7_on(void){
PTE->PSOR=D7;    
}

void LCD_d7_off(void){
PTE->PCOR=D7;     
 }
void LCD_d_off(void){
PTE->PCOR=D6|D7|D5|D4|D3|D2|D1|D0;     
 }
void LCD_8bit_set(uint8_t b8){
	
	    if((b8 & 0x01)==(0x01))
				{
	LCD_d0_on();
	}
	  	if((b8 & 0x02)==(0x02))
				{
	LCD_d1_on();
	}
			if((b8 & 0x04)==(0x04))
				{
	LCD_d2_on();
	}
			if((b8 & 0x08)==(0x08))
				{
	LCD_d3_on();
	}
			if((b8 & 0x10)==(0x10))
				{
	LCD_d4_on();
	}
			if((b8 & 0x20)==(0x20))
				{
	LCD_d5_on();
	}
			if((b8 & 0x40)==(0x40))
				{
	LCD_d6_on();
	}
			if((b8 & 0x80)==(0x80))
				{
	LCD_d7_on();
	}				
     
 }
void LCD_4bit_reset(void){
     LCD_d_off();
 }
 
void LCD_COMMAND(uint8_t comm){

    LCD_8bit_set(comm);
	  mywait(10);
    LCD_e_on();
	  mywait(10);
    LCD_e_off();
    mywait(10);
	  LCD_d_off();
}

void LCD_INIT(void){
	SIM->SCGC5|=SIM_SCGC5_PORTE_MASK;
	PORTE->PCR[2]=PORT_PCR_MUX(1UL);
	PORTE->PCR[3]=PORT_PCR_MUX(1UL);
	PORTE->PCR[4]=PORT_PCR_MUX(1UL);
	PORTE->PCR[5]=PORT_PCR_MUX(1UL);
	PORTE->PCR[20]=PORT_PCR_MUX(1UL);
	PORTE->PCR[21]=PORT_PCR_MUX(1UL);
	PORTE->PCR[22]=PORT_PCR_MUX(1UL);
	PORTE->PCR[23]=PORT_PCR_MUX(1UL);
	PORTE->PCR[29]=PORT_PCR_MUX(1UL);
	PORTE->PCR[30]=PORT_PCR_MUX(1UL);
	PTE->PDDR|=RS|E|D0|D1|D2|D3|D4|D5|D6|D7;
	LCD_rs_off();
	LCD_COMMAND(0x30);
	LCD_COMMAND(0x01);
	LCD_rs_on();
	LCD_COMMAND(0x48);
	LCD_COMMAND(0x0f);
}
