#include "leds.h"

const uint32_t green_mask= 1UL<<19;				
const uint32_t red_mask= 1UL<<18;
const uint32_t blue_mask= 1UL<<1;

/****************************************************************
*@Function: InitLED(void)
*@Description: initialization of leds. it takes no arguments.
*****************************************************************/

void InitLED(void){
SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;						//enable clock to PORTB
PORTB->PCR[19] = PORT_PCR_MUX(1UL);						//set PTC19 as GPIO
PTB->PDDR |= green_mask;											//set as output
PORTB->PCR[18]= PORT_PCR_MUX(1UL);						//set PTC18 as GPIO
PTB->PDDR |= red_mask;												//set as output
SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;						//enable clock to PORTBD					
PORTD->PCR[1] |= PORT_PCR_MUX(1UL);						//set PTD1 as GPIO
PTD->PDDR |= blue_mask;												//set as output
ledsOff();																		//turn all leds off

}


/****************************************************************
*@Function: InitLED(void)
*@Description: initialization of leds. it takes no arguments.
*****************************************************************/
void ledsOff (void){
PTD->PSOR= blue_mask;
PTB->PSOR= red_mask;
PTB->PSOR= green_mask;

}

/****************************************************************
*@Function: ledsOn (void)
*@Description: all leds on
*****************************************************************/
void ledsOn (void){
PTD->PCOR= blue_mask;
PTB->PCOR= red_mask;
PTB->PCOR= green_mask;
}

/****************************************************************
*@Function: ledGreenOff (void)
*@Description: Green led off
*****************************************************************/
void ledGreenOff (void){
	PTB->PSOR= green_mask;
}

/****************************************************************
*@Function: ledGreenOn (void)
*@Description: Green led on
*****************************************************************/
void ledGreenOn (void){
	PTB->PCOR= green_mask;
}

/****************************************************************
*@Function: ledBlueOff (void)
*@Description: Blue led on
*****************************************************************/
void ledBlueOff (void){
	PTD->PSOR= blue_mask;
}

/****************************************************************
*@Function: ledBlueOn (void)
*@Description: Blue led off
*****************************************************************/
void ledBlueOn (void){
	PTD->PCOR= blue_mask;
}

/****************************************************************
*@Function: ledRedOff (void)
*@Description: Blue Red off
*****************************************************************/
void ledRedOff (void){
	PTB->PSOR= red_mask;
}

/****************************************************************
*@Function: ledRedOn (void)
*@Description: Blue Red on
*****************************************************************/
void ledRedOn (void){
	PTB->PCOR= red_mask;
}

























