#include "I2C.h"
#include "MKL25Z4.h"

void I2C_int() {
				SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;     					//Turn on clock to E module
				PORTC_PCR10 = PORT_PCR_MUX(2);    								//Set PTC10 to alt2 [I2C_SCL]
				PORTC_PCR11 = PORT_PCR_MUX(2); 									//Set PTC11 to alt2 [I2C_SDA]
				SIM->SCGC4 |= SIM_SCGC4_I2C1_MASK;							//turn on clok to I2C1
				I2C1->A1=0x00;																	//clear adress of slave
				I2C1->F=0x10;																		//set multiple factor as =4
				I2C1->C1|=I2C_C1_IICEN_MASK;		//set as master and enable interupt
}


void i2c_DisableAck() {
	I2C1->C1 |= I2C_C1_TXAK_MASK;   //enable acknowledge signal
}


void i2c_EnableAck( ){
	I2C1->C1 &= ~I2C_C1_TXAK_MASK;	 //disable acknowledge signal
}


void i2c_RepeatedStart( ){
	I2C1->F = 0;										//set multiplay to 0
	I2C1->C1 |= I2C_C1_RSTA_MASK;		//Repeat start
	I2C1->F = 0x10;									//set multiplay to 4
}

void i2c_EnterRxMode( ){
	I2C1->C1 &= ~I2C_C1_TX_MASK;		//enter recive mode
}

void i2c_EnterTxMode( ){
	I2C1->C1 |= I2C_C1_TX_MASK;		//enter recive mode
}

void i2c_Start( ){	
	I2C1->C1 |= I2C_C1_MST_MASK;		//set as master
}


void i2c_Stop( ){
			I2C1->C1 &= ~I2C_C1_MST_MASK;			//enter slave mode
			I2C1->C1 &= ~I2C_C1_TX_MASK;		//set reciver mode
}


void i2c_Wait( ){
	uint32_t i = 0;
	i = 1000000;

	while(((I2C1->S & I2C_S_IICIF_MASK) == 0) && i)	{			//wait for interupt flag
		i--;
	}
	I2C1->S |= I2C_S_IICIF_MASK;													//clear interupt flag 38.3.5
}

void i2c_WriteByte( uint8_t data){
	I2C1->D = data;															//write 8-bit data to data register initiate data transfer
}


uint8_t i2c_ReadByte( ){
	return (uint8_t)( I2C1->D );													//read 8-bit data from data register
}



void i2c_WriteRegister( uint8_t SlaveAddress,uint8_t RegisterAddress, uint8_t data){
	//uint8_t res = 0;

	i2c_Start();
	i2c_WriteByte( ((SlaveAddress ) | I2C_WRITE));
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

void i2c_SetPointer( uint8_t SlaveAddress,uint8_t Pointer){
	//uint8_t res = 0;

	i2c_Start();
	i2c_WriteByte( ((SlaveAddress ) | I2C_WRITE));
	i2c_Wait();

	i2c_WriteByte(Pointer);
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
	i2c_WriteByte(((SlaveAddress ) | I2C_WRITE));
	i2c_Wait();

	i2c_WriteByte(RegisterAddress<<1);
	i2c_Wait();

	i2c_RepeatedStart();

	i2c_WriteByte( ((SlaveAddress ) | I2C_READ));
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
