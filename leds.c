#include "leds.h"
#include "MKL25Z4.h"


const uint32_t green_mask= 1UL<<19;				
const uint32_t red_mask= 1UL<<18;
const uint32_t blue_mask= 1UL<<1;

void InitLED(void){
SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
PORTB->PCR[19] = PORT_PCR_MUX(1UL);
PTB->PDDR |= green_mask;
PORTB->PCR[18]= PORT_PCR_MUX(1UL);
PTB->PDDR |= red_mask;
SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
PORTD->PCR[1] |= PORT_PCR_MUX(1UL);
PTD->PDDR |= blue_mask;
ledsOff();

}



void ledsOff (void){
PTD->PSOR= blue_mask;
PTB->PSOR= red_mask;
PTB->PSOR= green_mask;

}
void ledsOn (void){
PTD->PCOR= blue_mask;
PTB->PCOR= red_mask;
PTB->PCOR= green_mask;
}
void ledGreenOff (void){
	PTB->PSOR= green_mask;
}
void ledGreenOn (void){
	PTB->PCOR= green_mask;
}
void ledBlueOff (void){
	PTD->PSOR= blue_mask;
}
void ledBlueOn (void){
	PTD->PCOR= blue_mask;
}
void ledRedOff (void){
	PTB->PSOR= red_mask;
}
void ledRedOn (void){
	PTB->PCOR= red_mask;
}

























