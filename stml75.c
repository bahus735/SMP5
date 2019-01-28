

#include "stml75.h"



uint16_t  stml75_Read_Temp(void){
	uint16_t dummy=0;
	uint16_t data=0;
	uint16_t data1=0;
	uint8_t data2=0;

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
	i2c_DisableAck();
	data=i2c_ReadByte( )<<8;
	data1=data;
	i2c_Wait();
	
	i2c_Stop();
	data|=i2c_ReadByte( );
	data2=data;
	i2c_Wait();
	
	dummy=data>>7;
return (dummy);
}

void stml75_Set_Temp(){
	uint16_t tempbin=0;
	uint16_t temp=0;
	tempbin= stml75_Read_Temp();
	
	tempe[0]=tempbin>>8;
	if (tempe[0]==0){
			if((tempbin&0x01)==0x01){
			
				tempe[5]=5+ASCII_OFFSET;
			}
			else{
			
				tempe[5]=0+ASCII_OFFSET;
			}
	tempe[0]+=ASCII_OFFSET;
			tempe[3]=(tempbin>>1)%10+ASCII_OFFSET;
			temp=(tempbin>>1)/10;
			tempe[2]=(temp)%10+ASCII_OFFSET;
			temp=(temp)/10;
			tempe[1]=(temp)%10+ASCII_OFFSET;
			temp=(temp)/10;
			tempe[0]='+';
	}
	else{
			tempbin=tempbin&0xFF;													//set sing to U2
															
			if((tempbin&0x01)==0x01){
			
				tempe[5]=5+ASCII_OFFSET;
			}
			else{
			
				tempe[5]=0+ASCII_OFFSET;
			}
			tempbin=tempbin>>1;
			tempbin=(~tempbin&0x7F)+1;
			tempe[3]=(tempbin)%10+ASCII_OFFSET;
			temp=(tempbin)/10;
			tempe[2]=(temp)%10+ASCII_OFFSET;
			temp=(temp)/10;
			tempe[1]=(temp)%10+ASCII_OFFSET;
			temp=(temp)/10;
		tempe[0]='-';
	
	}

	tempe[4]=0x2C;
	
	Display_tempe(IMAGE_RED,IMAGE_BLACK, Font24_size, Font24_Table, tempe);
}



char  tempe[] =
	{
		'1',
		'2',
		'3',
		'4',
		'5',
		'6',
		'\0',
  };
	

	