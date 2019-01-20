#include "DMA.h"
#include "MKL25Z4.h"
void DMA_init(uint8_t *Buffer){

DMA0->DMA->DAR=0X0;
DMA0->DMA->SAR=0X0;
DMA0->DMA->DCR=0X0;
 //Enable DMA clocking
    SIM->SCGC6 |= SIM_SCGC6_DMAMUX_MASK;    // Enable clock to DMA mux
    SIM->SCGC7 |= SIM_SCGC7_DMA_MASK;       // Enable clock to DMA
		// reset DMAMUX
		DMAMUX0->CHCFG[0] = 0X00;
		DMA0->DMA[0 ].DSR_BCR = DMA_DSR_BCR_DONE_MASK;  // clear/reset DMA status
		// Set up another DMA channel to write to the SPI, in order to force Reads, and link to above
    DMA0->DMA[0].SAR = (uint32_t)Buffer;// set source address: dummy memory buffer for writes
    DMA0->DMA[0].DAR = (uint32_t)&(SPI0->D); // set dest address: SPI0_Data register
    DMA0->DMA[0].DSR_BCR |= DMA_DSR_BCR_BCR_MASK & 0x1388; // length of transfer
    DMA0->DMA[0].DCR = DMA_DCR_EINT_MASK | DMA_DCR_ERQ_MASK | DMA_DCR_CS_MASK | /*DMA_DCR_EADREQ_MASK | */ DMA_DCR_SINC_MASK |
    DMA_DCR_SSIZE(0x01) | /*DMA_DCR_DINC_MASK |*/ DMA_DCR_DSIZE(0x01) | DMA_DCR_START_MASK | DMA_DCR_D_REQ_MASK |
    DMA_DCR_LINKCC(0x10) | DMA_DCR_LCH1(Buffer);
        // 8-bit transfers, link to DMA0, etc
		 // Configure DMAMUX0
    DMAMUX0->CHCFG[0] = DMAMUX_CHCFG_ENBL_MASK | DMAMUX_CHCFG_TRIG_MASK | DMAMUX_CHCFG_SOURCE(DMA_MUX_SRC_SPI0_Transmit);
 
    // Enable DMA features within the SPI registers   
    SPI0->C1 |= SPI_C1_SPIE_MASK | SPI_C1_SPE_MASK | SPI_C1_SPTIE_MASK;
    SPI0->C2 |= SPI_C2_TXDMAE_MASK | SPI_C2_RXDMAE_MASK;
}
