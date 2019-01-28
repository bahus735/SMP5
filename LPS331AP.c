#include "LPS331AP.h"

struct LPS331AP{
		uint8_t REF_P_XL;
		uint8_t REF_P_L;
		uint8_t REF_P_H;
		uint8_t PRESS_OUT_XL;
		uint8_t PRESS_OUT_L;
		uint8_t PRESS_OUT_H;
}LPS331AP;
void LPS331AP_Init(void){
	i2c_EnterTxMode();
	i2c_EnableAck( );
	i2c_Start( );
	i2c_WriteByte(0xBA|I2C_WRITE);
	i2c_Wait();
	i2c_WriteByte(0x20);
	i2c_Wait();
	i2c_WriteByte(0x40);
	i2c_Wait();
  i2c_RepeatedStart();
	i2c_WriteByte(0xBA|I2C_WRITE);
	i2c_Wait();
	i2c_WriteByte(0x21);
	i2c_Wait();
	i2c_WriteByte(0x84);
	i2c_Wait();
	i2c_Stop();



}


uint16_t  LPS331AP_Read_Press(void){
	uint16_t dummy=0;
	uint16_t data=0;
	uint16_t data1=0;
	uint8_t data2=0;

	i2c_EnterTxMode();
	i2c_EnableAck( );
	i2c_Start( );
	i2c_WriteByte(0xBA|I2C_WRITE);
	i2c_Wait();
	i2c_WriteByte(0x89);
	i2c_Wait();

	i2c_RepeatedStart();
	i2c_WriteByte(0xBA|I2C_READ);
	i2c_Wait();
	i2c_EnterRxMode();
	dummy=i2c_ReadByte( );
	i2c_Wait();
	i2c_EnableAck( );
	i2c_DisableAck();
	data=i2c_ReadByte( )<<8;
	data1=data;
	i2c_Wait();
	
	i2c_Stop();
	data|=i2c_ReadByte( );
	data2=data;
	i2c_Wait();
	
return (dummy);
}

