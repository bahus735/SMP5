#ifndef DMA_H
#define DMA_H

#include "MKL25Z4.h"
#define DMA_MUX_SRC_SPI0_Receive (16)
#define DMA_MUX_SRC_SPI0_Transmit (17)
#define DMA_MUX_SRC_SPI1_Receive (18)
#define DMA_MUX_SRC_SPI1_Transmit (19)
#define DMA_MUX_SRC_I2C0 (22)
#define DMA_MUX_SRC_I2C1 (23)
#define DMA_MUX_SRC_TPM0_Channel_0 (24)
#define DMA_MUX_SRC_TPM0_Channel_1 (25)
#define DMA_MUX_SRC_TPM0_Channel_2 (26)
#define DMA_MUX_SRC_TPM0_Channel_3 (27)
#define DMA_MUX_SRC_TPM0_Channel_4 (28)
#define DMA_MUX_SRC_TPM0_Channel_5 (29)
#define DMA_MUX_SRC_TPM1_Channel_0 (32)
#define DMA_MUX_SRC_TPM1_Channel_1 (33)
#define DMA_MUX_SRC_TPM2_Channel_0 (34)
#define DMA_MUX_SRC_TPM2_Channel_1 (35)
#define DMA_MUX_SRC_ADC0 (40)
#define DMA_MUX_SRC_CMP0 (42)
#define DMA_MUX_SRC_DAC0 (45)


void DMA_init(uint8_t *Buffer);

#endif
