
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
	i2c_Stop();
	data|=i2c_ReadByte( );
	i2c_Wait();
	dummy=data>>7;
	return (dummy);
}

void stml75_Set_Temp(){
	uint16_t tempbin=0;
	uint16_t temp=0;
	tempbin=  stml75_Read_Temp();
	
	tempe[0]=tempbin>>8;
	if (tempe[0]==0){
			if((tempbin&0x01)==0x01){
			
				tempe[5]=5;
			}
			else{
			
				tempe[5]=0;
			}
			
			tempe[3]=(tempbin>>1)%10+ASCII_OFFSET;
			temp=(tempbin>>1)/10;
			tempe[2]=(temp)%10+ASCII_OFFSET;
			temp=(temp)/10;
			tempe[1]=(temp)%10+ASCII_OFFSET;
			temp=(temp)/10;
			
	}
	else{
			tempbin=tempbin&0xFF;													//set sing to U2
															
			if((tempbin&0x01)==0x01){
			
				tempe[5]=5;
			}
			else{
			
				tempe[5]=0;
			}
			tempbin=tempbin>>1;
			tempbin=(~tempbin&0x7F)+1;
			tempe[3]=(tempbin)%10+ASCII_OFFSET;
			temp=(tempbin)/10;
			tempe[2]=(temp)%10+ASCII_OFFSET;
			temp=(temp)/10;
			tempe[1]=(temp)%10+ASCII_OFFSET;
			temp=(temp)/10;
	
	
	}

	tempe[4]=0x2C;

	//DrawStringAt_EP(IMAGE_RED,90, 2,  tempe,Font24_size,Font24_Table, COLORED);
	//DrawStringAt_EP(IMAGE_RED,90, 26, tempe,Font24_size,Font24_Table, COLORED);
	//DrawStringAt_EP(IMAGE_RED,90, 50, tempe,Font24_size,Font24_Table, COLORED);
	//DrawStringAt_EP(IMAGE_RED,90, 74, tempe,Font24_size,Font24_Table, COLORED);
	//DrawStringAt_EP(IMAGE_RED,90, 98, tempe,Font24_size,Font24_Table, COLORED);
}
