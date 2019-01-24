
#include "MKL25Z4.h"
#include "stml75.h"
#include "i2c.h"



uint16_t  stml75_Read_Temp(void){
	uint8_t dummy=0;
	uint16_t data=0;
//	uint16_t data1=0;
//	uint8_t data3=0;

	i2c_EnterTxMode();
	i2c_EnableAck( );
	i2c_Start( );
	i2c_WriteByte(0x90|I2C_WRITE);
	i2c_Wait();
	i2c_WriteByte(0x00);
	i2c_Wait();
	i2c_RepeatedStart();
	i2c_WriteByte(0x90|I2C_READ);
	i2c_Wait();
	i2c_EnterRxMode();
	dummy=i2c_ReadByte( );
	i2c_Wait();
	i2c_EnableAck( );

	data=i2c_ReadByte( )<<8;
	i2c_Wait();
	i2c_DisableAck();
	data|=i2c_ReadByte( );
	i2c_Wait();
	i2c_Stop();
	dummy=data>>7;
	return (dummy);
}

void stml75_Set_Temp(){
			uint16_t tempbin=0;
			uint16_t tempbcd=0;
			uint8_t	 sing =0;
			uint8_t	 tens =0;
			uint8_t	 ones =0;
			uint8_t	 point =0;
tempbin =	stml75_Read_Temp();




}