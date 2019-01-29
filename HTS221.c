#include "hts221.h"

struct HTS221{
		uint8_t H_OUT_L;
		uint8_t H_OUT_H;
		uint8_t H0_rH_X2;
		uint8_t H1_rH_X2;
		uint8_t H0_T0_OUT_L;
		uint8_t H0_T0_OUT_H;
		uint8_t H1_T0_OUT_L;
		uint8_t H1_T0_OUT_H;
		uint8_t T_OUT_L;
		uint8_t T_OUT_H;
		uint8_t T0_rH_X2;
		uint8_t T1_rH_X2;
		uint8_t T0_T0_OUT_L;
		uint8_t T0_T0_OUT_H;
		uint8_t T1_T0_OUT_L;
		uint8_t T1_T0_OUT_H;
}HTS221;




void HTCS221_Init(void){

	i2c_WriteRegister(  HTS221_AADDRES, HTS221_CTRL_REG1,  0x00);
		HTCS221_Read_Cal();
	i2c_WriteRegister(  HTS221_AADDRES, HTS221_AV_CONFG,  0x1b);
	i2c_WriteRegister(   HTS221_AADDRES, HTS221_CTRL_REG1,  0x83);	
	
	
	
}


void  HTCS221_Read_Hum(void){
  HTS221.H_OUT_L=i2c_ReadRegister(  HTS221_AADDRES,HTS221_HUM_OUT_L );
	HTS221.H_OUT_H=i2c_ReadRegister(  HTS221_AADDRES,HTS221_HUM_OUT_H );
	
}

void  HTCS221_Read_Tem(void){
  HTS221.H_OUT_L=i2c_ReadRegister(  HTS221_AADDRES,HTS221_TEM_OUT_L );
	HTS221.H_OUT_H=i2c_ReadRegister(  HTS221_AADDRES,HTS221_TEM_OUT_H );
	
}

void  HTCS221_Set_Hum(void){
	int32_t Hum=0;
	uint16_t H_T =0;
	uint16_t H_0_T0 = 0;
	uint16_t H_1_T0 = 0;
	uint16_t H0_r = 0;
	uint16_t H1_r = 0;
	uint16_t temp = 0;
	mywait(10);
	i2c_ReadRegister(  HTS221_AADDRES, 0x27);
	HTCS221_Read_Hum();
	 Hum=0;
	 H_T = HTS221.H_OUT_L|(HTS221.H_OUT_H<<8);
	 H_0_T0 = HTS221.H0_T0_OUT_L|(HTS221.H0_T0_OUT_H<<8);
	 H_1_T0 = HTS221.H1_T0_OUT_L|(HTS221.H1_T0_OUT_H<<8);
	 H0_r = HTS221.H0_rH_X2;
	 H1_r = HTS221.H1_rH_X2;
	Hum=(H_T-H_0_T0)*(H1_r-H0_r)/(H_1_T0-H_0_T0) + H0_r;
	
	
			humi[3]=(Hum)%10+ASCII_OFFSET;
			temp=(Hum)/10;
			humi[2]=(temp)%10+ASCII_OFFSET;
			temp=(temp)/10;
			humi[1]=(temp)%10+ASCII_OFFSET;
			temp=(temp)/10;
			humi[0]=(temp)%10+ASCII_OFFSET;
			temp=(temp)/10;
	
	

	humi[4]='%';
	humi[5]='%';
	Display_humi(IMAGE_RED,IMAGE_BLACK, Font24_size, Font24_Table, humi);
	
	

	
	
	
}

void  HTCS221_Set_Tem(void){
	float tem=0;
	uint16_t tempc=0;
	float T_T =0;
	int16_t T_0_T0 = 0;
	int16_t T_1_T0 = 0;
	int16_t temp = 0;
	float T0_r = 0;
	float T1_r = 0;
	mywait(2);
	i2c_ReadRegister(  HTS221_AADDRES, 0x27);
	HTCS221_Read_Tem();
	 tem=0;
	 T_T = HTS221.T_OUT_L|(HTS221.T_OUT_H<<8);
	 T_0_T0 = HTS221.T0_T0_OUT_L|(HTS221.T0_T0_OUT_H<<8);
	 T_1_T0 = HTS221.T1_T0_OUT_L|(HTS221.T1_T0_OUT_H<<8);
	 T0_r = HTS221.T0_rH_X2;
	 T1_r = HTS221.T1_rH_X2;
	tem=((float)(T1_r-T0_r))/(T_1_T0-T_0_T0)*(T_T-T_0_T0) + T0_r;
	tempc=(uint16_t)(tem/8);
	
	
			tmp2[3]=(tempc)%10+ASCII_OFFSET;
			temp=(tempc)/10;
			tmp2[2]=(temp)%10+ASCII_OFFSET;
			temp=(temp)/10;
			tmp2[1]=(temp)%10+ASCII_OFFSET;
			temp=(temp)/10;
			tmp2[0]=(temp)%10+ASCII_OFFSET;
			temp=(temp)/10;
	
	

	tmp2[4]='*';
	tmp2[5]='C';
	Display_light(IMAGE_RED,IMAGE_BLACK, Font24_size, Font24_Table, tmp2);
	
	
	
}


void  HTCS221_Read_Cal(void){
	
	HTS221.H0_rH_X2=i2c_ReadRegister(  HTS221_AADDRES, HTS221_H0_rH_X2);
	HTS221.H1_rH_X2=i2c_ReadRegister(  HTS221_AADDRES, HTS221_H1_rH_X2);
	HTS221.H0_T0_OUT_L=i2c_ReadRegister(  HTS221_AADDRES, HTS221_H0_T0_OUT_L);
	HTS221.H0_T0_OUT_H=i2c_ReadRegister(  HTS221_AADDRES, HTS221_H0_T0_OUT_H);
	HTS221.H1_T0_OUT_L=i2c_ReadRegister(  HTS221_AADDRES, HTS221_H1_T0_OUT_L);
	HTS221.H1_T0_OUT_H=i2c_ReadRegister(  HTS221_AADDRES, HTS221_H1_T0_OUT_H);

	HTS221.T0_rH_X2=i2c_ReadRegister(  HTS221_AADDRES, HTS221_T0_rH_X2);
	HTS221.T1_rH_X2=i2c_ReadRegister(  HTS221_AADDRES, HTS221_T1_rH_X2);
	HTS221.T0_T0_OUT_L=i2c_ReadRegister(  HTS221_AADDRES, HTS221_T0_T0_OUT_L);
	HTS221.T0_T0_OUT_H=i2c_ReadRegister(  HTS221_AADDRES, HTS221_T0_T0_OUT_H);
	HTS221.T1_T0_OUT_L=i2c_ReadRegister(  HTS221_AADDRES, HTS221_T1_T0_OUT_L);
	HTS221.T1_T0_OUT_H=i2c_ReadRegister(  HTS221_AADDRES, HTS221_T1_T0_OUT_H);	
	
}






char  humi[] =
	{
		'1',
		'2',
		'3',
		'4',
		'5',
		'6',
		'\0',
  };
	
	char  tmp2[] =
	{
		'1',
		'2',
		'3',
		'4',
		'5',
		'6',
		'\0',
  };














