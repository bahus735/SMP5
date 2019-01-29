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
	i2c_WriteByte(LPS331_CTRL_REG1);
	i2c_Wait();
	i2c_WriteByte(0x00);
	i2c_Wait();
//	i2c_Stop();
	
	
	i2c_RepeatedStart();
	i2c_WriteByte(0xBA|I2C_WRITE);
	i2c_Wait();
	i2c_WriteByte(0x10);
	i2c_Wait();
	i2c_WriteByte(0x6A);
	i2c_Wait();
	
	i2c_RepeatedStart();
	i2c_WriteByte(0xBA|I2C_WRITE);
	i2c_Wait();
	i2c_WriteByte(0x20);
	i2c_Wait();
	i2c_WriteByte(0xF4);
	i2c_Wait();

	i2c_Stop();



}


uint32_t  LPS331AP_Read_Press(void){
	uint32_t dummy=0;
	uint16_t data0=0;
	uint16_t data1=0;
	uint8_t data2=0;

	i2c_EnterTxMode();
	i2c_EnableAck( );
	i2c_Start( );
	i2c_WriteByte(0xBA|I2C_WRITE);
	i2c_Wait();
	i2c_WriteByte(0x28|0x80);
	i2c_Wait();

	i2c_RepeatedStart();
	i2c_WriteByte(0xBA|I2C_READ);
	i2c_Wait();
	i2c_EnterRxMode();
	dummy=i2c_ReadByte( );
	i2c_Wait();
	i2c_EnableAck( );
	
	data0=i2c_ReadByte( );
	//data1=data;
	i2c_Wait();
	i2c_DisableAck();

	data1=i2c_ReadByte( );
	i2c_Wait();
	i2c_Stop();
	data2=i2c_ReadByte( );
	i2c_Wait();
//	i2c_Stop();
//	i2c_Wait();
	dummy=(data0|(data1<<8)|(data2<<16));
return (dummy);
}

void LPS331AP_Set_Press(){
	uint32_t pressbin=0;
	uint32_t temp=0;
	pressbin= LPS331AP_Read_Press()/4096;
	


			press[3]=(pressbin)%10+ASCII_OFFSET;
			temp=(pressbin)/10;
			press[2]=(temp)%10+ASCII_OFFSET;
			temp=(temp)/10;
			press[1]=(temp)%10+ASCII_OFFSET;
			temp=(temp)/10;
			press[0]=(temp)%10+ASCII_OFFSET;
			temp=(temp)/10;
	
	

	press[4]='m';
	press[5]='b';
	Display_press(IMAGE_RED,IMAGE_BLACK, Font24_size, Font24_Table, press);
}



char  press[] =
	{
		'1',
		'2',
		'3',
		'4',
		'5',
		'6',
		'\0',
  };

