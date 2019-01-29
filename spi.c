#include "spi.h"
#include "MKL25Z4.h"
#include "epaper.h"




/*********************************************************************************
*@Function: SPI1_IRQHandler()

*@Description:handle of interrupt from spi0, not used.
*********************************************************************************/
void SPI0_IRQHandler(){
NVIC_ClearPendingIRQ(SPI0_IRQn);
}



/*********************************************************************************
*@Function: spi0_Init()
*@Description: initialization of the spi0 module. witch is used to communicate with e-papaer.
*********************************************************************************/


int spi0_Init(void){
					SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;   					  		//Turn on clock to port C	     				  //Set PTC4 to mux 2 [SPI0_PCS0] Chip Select Auto
					SIM_SCGC4 &= ~SIM_SCGC4_SPI0_MASK;
					SIM_SCGC4 |= SIM_SCGC4_SPI0_MASK; 									//Turn on clock to SPI module
					PORTC->PCR[5] |= PORT_PCR_MUX(0x2); 	     				  //Set PTC5 to mux 2 [SPI0_SCK]
					PORTC->PCR[6] |= PORT_PCR_MUX(0x2); 	    				 	//Set PTC6 to mux 2 [SPI0_MOSI]
					PORTC->PCR[7] |= PORT_PCR_MUX(0x2);			  	 				//Set PTC7 to mux 2 [SPIO_MISO]
					PORTC->PCR[4] |= PORT_PCR_MUX(0x2);									//Set PTC4 to mux 2 [SPIO_SS]
					  							
					 
					
					SPI0->C2 = SPI_C2_MODFEN_MASK;																//Master SS pin acts as slave select output
					SPI0->BR = (SPI_BR_SPPR(2) | SPI_BR_SPR(4));									// setting up baud rate
					SPI0->C1 = SPI_C1_SPE_MASK|SPI_C1_MSTR_MASK|SPI_C1_SSOE_MASK; 	// Set SPI to Master & SS pin to auto SS & spi mode in 0,0									// Enable SPI
	return 1;
}

/*********************************************************************************
*@Function: spi0_SendRecive(uint8_t data)
*@Arguments: data -- one bajt of data that you want to send to via spi.
*@Returns:	 one bajt of data that you read from spi.
*@Description: basic function to send and recive data via spi
*********************************************************************************/


uint8_t spi0_SendRecive(uint8_t data)
{
	while((SPI0->S & SPI_S_SPTEF_MASK)==0);
	SPI0->D = data;
	while((SPI0->S & SPI_S_SPRF_MASK)==0);
	return SPI0->D;

}
