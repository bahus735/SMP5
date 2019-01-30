#include "I2C.h"
#include "MKL25Z4.h"

/*********************************************************************************
*@Function:I2C_int()
*@Description: simple init or the I2C module
*********************************************************************************/
void I2C_int() {
				SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;     					//Turn on clock to E module
				PORTC_PCR10 = PORT_PCR_MUX(2);    							//Set PTC10 to alt2 [I2C_SCL]
				PORTC_PCR11 = PORT_PCR_MUX(2); 									//Set PTC11 to alt2 [I2C_SDA]
				SIM->SCGC4 |= SIM_SCGC4_I2C1_MASK;							//turn on clok to I2C1
				I2C1->A1=0x00;																	//clear adress of slave
				I2C1->F=0x10;																		//set multiple factor as =4
				I2C1->C1|=I2C_C1_IICEN_MASK;										//set as master and enable interupt
}

/*********************************************************************************
*@Function:i2c_DisableAck()
*@Description: just DisableAck.
*********************************************************************************/
void i2c_DisableAck() {
	I2C1->C1 |= I2C_C1_TXAK_MASK;   //DisableAck acknowledge signal
}

/*********************************************************************************
*@Function:i2c_EnableAck() 
*@Description: just EnableAck.
*********************************************************************************/
void i2c_EnableAck( ){
	I2C1->C1 &= ~I2C_C1_TXAK_MASK;	 //i2c_EnableAck acknowledge signal
}

/*********************************************************************************
*@Function:i2c_RepeatedStart( )
*@Description: tepeted start. we have to clead F register. (Errata)
*********************************************************************************/
void i2c_RepeatedStart( ){
	I2C1->F = 0;										//set multiplay to 0
	I2C1->C1 |= I2C_C1_RSTA_MASK;		//Repeat start
	I2C1->F = 0x10;									//set multiplay to 4
}

/*********************************************************************************
*@Function:i2c_EnterRxMode( )

*@Description: enter Rx Mode
*********************************************************************************/
void i2c_EnterRxMode( ){
	I2C1->C1 &= ~I2C_C1_TX_MASK;		//enter recive mode
}

/*********************************************************************************
*@Function:i2c_EnterTxMode( )
*@Description: enter Tx Mode
*********************************************************************************/
void i2c_EnterTxMode( ){
	I2C1->C1 |= I2C_C1_TX_MASK;		//enter transmit mode
}

/*********************************************************************************
*@Function:i2c_Start( )
*@Description: send start combination
*********************************************************************************/
void i2c_Start( ){	
	I2C1->C1 |= I2C_C1_MST_MASK	;	//set as master

}

/*********************************************************************************
*@Function:i2c_Stop( )

*@Description: sends stop condition
*********************************************************************************/
void i2c_Stop( ){
			I2C1->C1 &= ~I2C_C1_MST_MASK;			//enter slave mode
			I2C1->C1 &= ~I2C_C1_TX_MASK;		//set reciver mode
		  I2C1->C1|=I2C_C1_IICEN_MASK;		//enable interupt
}

/*********************************************************************************
*@Function:i2c_Wait( )
*@Description: wait for i2c to end transmission or reciving
*********************************************************************************/
void i2c_Wait( ){
	uint32_t i = 0;
	i = 1000000;

	while(((I2C1->S & I2C_S_IICIF_MASK) == 0) && i)	{			//wait for interupt flag
		i--;																								//timeout
	}
	I2C1->S |= I2C_S_IICIF_MASK;													//clear interupt flag 
}

/*********************************************************************************
*@Function:i2c_WriteByte( uint8_t data)
*@Arguments: Data-- data that is goint to be written
*@Description: 
*********************************************************************************/
void i2c_WriteByte( uint8_t data){
	I2C1->D = data;																				//write 8-bit data to data register initiate data transfer
}

/*********************************************************************************
*@Function:i2c_ReadByte( )
*@Returns: data that was readed from i2c 
*@Description: 
*********************************************************************************/
uint8_t i2c_ReadByte( ){
	return (uint8_t)( I2C1->D );													//read 8-bit data from data register
}

/*********************************************************************************
*@Function:i2c_WriteRegister( uint8_t SlaveAddress,uint8_t RegisterAddress, uint8_t data)
*@Arguments: SlaveAddress
						 RegisterAddress     
						 data		 
             
*@Description: write data to selected register via i2c
*********************************************************************************/

void i2c_WriteRegister( uint8_t SlaveAddress,uint8_t RegisterAddress, uint8_t data){

	i2c_EnterTxMode();
	i2c_EnableAck( );
	i2c_Start( );

	i2c_WriteByte(SlaveAddress|I2C_WRITE);
	i2c_Wait();

	i2c_WriteByte(RegisterAddress);
	i2c_Wait();
	i2c_WriteByte(data);
	i2c_Wait();
	i2c_Stop();



}

/*********************************************************************************
*@Function:i2c_SetPointer( uint8_t SlaveAddress,uint8_t Pointer)
*@Arguments: SlaveAddress
						 Pointer     
*@Description: sents pointer to multible readings or writings  to form slave
*********************************************************************************/
void i2c_SetPointer( uint8_t SlaveAddress,uint8_t Pointer){


	i2c_Start();
	i2c_WriteByte( ((SlaveAddress ) | I2C_WRITE));
	i2c_Wait();

	i2c_WriteByte(Pointer);
	i2c_Wait();

	i2c_Stop();


}

/*********************************************************************************
*@Function:i2c_ReadRegister( uint8_t SlaveAddress,uint8_t RegisterAddress)
*@Arguments: SlaveAddress
						 RegisterAddress     		 
             
*@Description: reads data from slave register
*********************************************************************************/

uint8_t i2c_ReadRegister( uint8_t SlaveAddress,uint8_t RegisterAddress){
	uint8_t res = 0;
	uint8_t res2 = 0;
	i2c_EnterTxMode();
	i2c_EnableAck( );
	i2c_Start();
	i2c_WriteByte(((SlaveAddress ) | I2C_WRITE));
	i2c_Wait();

	i2c_WriteByte(RegisterAddress);
	i2c_Wait();

	i2c_RepeatedStart();
	i2c_WriteByte( ((SlaveAddress ) | I2C_READ));
	i2c_Wait();
	i2c_EnterRxMode();
	i2c_DisableAck();
	res = i2c_ReadByte();
	i2c_Wait();
	i2c_Stop();
	res2 = i2c_ReadByte();

	return res2;
}



