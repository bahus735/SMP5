#include "MKL25Z4.h"
#include "lowpower.h"


void lowPowerInitialize(void){
	/*Configure Low Leakage Wakeup Unit (LLWU)*/
	LLWU->ME = LLWU_ME_WUME0_MASK; /*Select LPTMR as a source of wakeup. Module 0 i.e. LPTMR in KL25 is selected*/
	
	/*Configure pin PTC3 to wake-up processor*/
	LLWU->PE2=LLWU_PE2_WUPE7(1); /*Pin PTC3 is llwu pin 7 source (LLWU_P7) in KL46. Configure field WUPE7 in register PE2 as 1 for rising edge wake-up*/
	
	/* Write to PMPROT in System mode controller to allow LLS power modes */
	SMC->PMPROT = SMC_PMPROT_ALLS_MASK; /*Allow LLS mode*/
	
	return;
}

void enterLLSMode(void)
{
int volatile dummyread;

	/*Set the STOPM  bits to select the LLS low-power mode*/
	SMC->PMCTRL &= ~SMC_PMCTRL_STOPM_MASK;
	SMC->PMCTRL = SMC_PMCTRL_STOPM(3); // Set STOPM = 0b11
	
	/*set the SLEEPDEEP bit in the ARM core, and then execute the Wake From Interrupt (WFI) instruction to enter LLS mode*/
	dummyread = SMC->PMCTRL; /*wait for write to complete to SMC before stopping core */
	dummyread++;
	SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk; /* Set the SLEEPDEEP bit to enable deep sleep mode */
	//__asm("WFI"); /*assembly WFI instruction will start entry into low-power mode */
	__wfi();
	return;
}
