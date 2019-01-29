#include "PIT.h"

//#include "leds.h"
uint32_t myPeriod=0xE4E1C00;

void pitInicialize (void){

	SIM->SCGC6|=SIM_SCGC6_PIT_MASK;			/* Enable clock to PIT module */
	
	PIT->MCR&=~PIT_MCR_MDIS_MASK;				/* Enable module, freeze timers in dubug mode */
	PIT->MCR|=PIT_MCR_FRZ_MASK;
	

		PIT->CHANNEL[0].LDVAL=myPeriod;						/* Initialize PIT to count down from argument */
		PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK;	/* Generate interrupts (Timer Interrupt Enable) */
		PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK;	/* Timer Enable */
	
	
	NVIC_ClearPendingIRQ(PIT_IRQn);									/* Clear any pending IRG from PIT */
	NVIC_EnableIRQ(PIT_IRQn);												/* Enable the PIT interrupt in the NVIC */

		PIT->MCR=0x00;																/* Enable Module Control Register */
	
	__enable_irq();																	/* Checking if interrupts are not masked globally */
}
void PIT_IRQHandler(void){
	
		if(PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK){	/* Determine which channel triggred interrupt */
			Display_init(IMAGE_RED,IMAGE_BLACK, Font24_size,Font24_Table,AGH_logo,AGH_logo_Size);																						/* Change the state of the red LED */
			PIT->CHANNEL[0].TFLG &= PIT_TFLG_TIF_MASK;	/* Clear interrupt request flag for channel */
		}
	
}