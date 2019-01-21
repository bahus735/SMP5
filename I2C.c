#include "I2C.h"
#include "MKL25Z4.h"

void I2C_int() {
				SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;     		//Turn on clock to E module
				PORTE_PCR0 = PORT_PCR_MUX(2);    					//Set PTE0 to alt2 [I2C_SDA]
				PORTE_PCR1 = PORT_PCR_MUX(2); 						//Set PTE1 to alt2 [I2C_SCL]
				SIM->SCGC4 |= SIM_SCGC4_I2C1_MASK;				//turn on clok to I2C1
				I2C1->A1=0x00;
				I2C1->F=0x10;
				I2C1->C1|=I2C_C1_MST_MASK|I2C_C1_IICEN_MASK;
}


void i2c_DisableAck() {
	I2C1->C1 |= I2C_C1_TXAK_MASK;
}


void i2c_EnableAck( ){
	I2C1->C1 &= ~I2C_C1_TXAK_MASK;
}


void i2c_RepeatedStart( ){
	I2C0->F = 0;
	I2C1->C1 |= I2C_C1_RSTA_MASK;
	I2C1->F = 0x10;
}

void i2c_EnterRxMode( ){
	I2C1->C1 &= ~I2C_C1_TX_MASK;
}

void i2c_Start( ){
	I2C1->C1 |= I2C_C1_TX_MASK;
	I2C1->C1 |= I2C_C1_MST_MASK;
}


void i2c_Stop( ){
	I2C1->C1 &= ~I2C_C1_MST_MASK;
	I2C1->C1 &= ~I2C_C1_TX_MASK;
}


void i2c_Wait( ){
	uint32_t i = 0;
	i = 1000000;

	while(((I2C1->S & I2C_S_IICIF_MASK) == 0) && i)	{
		i--;
	}
	I2C1->S |= I2C_S_IICIF_MASK;
}

void i2c_WriteByte( uint8_t data){
	I2C1->D = (uint8_t)data;
}


uint8_t i2c_ReadByte( ){
	return (uint8_t)( I2C1->D );
}



void i2c_WriteRegister( uint8_t SlaveAddress,uint8_t RegisterAddress, uint8_t data){
	//uint8_t res = 0;

	i2c_Start();
	i2c_WriteByte( ((SlaveAddress << 1) | I2C_WRITE));
	i2c_Wait();

	i2c_WriteByte(RegisterAddress);
	i2c_Wait();

	i2c_WriteByte(data);
	i2c_Wait();

	i2c_Stop();

	mywait(50);

	/*
	i2c_RepeatedStart(i2c);
	i2c_WriteByte(i2c, ((SlaveAddress << 1) | I2C_READ));
	i2c_Wait(i2c);
	i2c_EnterRxMode(i2c);
	i2c_DisableAck(i2c);
	res = i2c_ReadByte(i2c);
	i2c_Wait(i2c);
	i2c_Stop(i2c);
	res = i2c_ReadByte(i2c);
	delay(50);
	return res;*/
}


uint8_t i2c_ReadRegister( uint8_t SlaveAddress,uint8_t RegisterAddress){
	uint8_t res = 0;

	i2c_Start();
	i2c_WriteByte(((SlaveAddress << 1) | I2C_WRITE));
	i2c_Wait();

	i2c_WriteByte(RegisterAddress);
	i2c_Wait();

	i2c_RepeatedStart();

	i2c_WriteByte( ((SlaveAddress << 1) | I2C_READ));
	i2c_Wait();

	i2c_EnterRxMode();
	i2c_DisableAck();

	res = i2c_ReadByte();
	i2c_Wait();
	i2c_Stop();
	res = i2c_ReadByte();
	mywait(50);

	return res;
}


/*
void i2c_ReadMultRegister( uint8_t SlaveAddress,uint8_t RegisterAddress,uint8_t n_data, uint8_t *res){
	uint8_t i = 0;

	i2c_Start();
	i2c_WriteByte( ((SlaveAddress << 1) | I2C_WRITE));
	i2c_Wait();

	i2c_WriteByte(RegisterAddress);
	i2c_Wait();

	i2c_RepeatedStart();
d
	i2c_WriteByte( ((SlaveAddress << 1) | I2C_READ));
	i2c_Wait();

	i2c_EnterRxMode();
	i2c_EnableAck();

	i = i2c_ReadByte();
	i2c_Wait();

	for(i=0;i<(n_data-2);i++)
	{
		*res = i2c_ReadByte();
		res++;
		i2c_Wait();
	}

	i2c_DisableAck();

	*res = i2c_ReadByte();
	res++;
	i2c_Wait();

	i2c_Stop();

	*res = i2c_ReadByte();

	mywait(50);

}
*/
