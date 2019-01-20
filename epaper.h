#ifndef EPAPER_H
#define EPAPER_H



#include "MKL25Z4.h"

uint32_t BUSYread(void);

//Ports Definition

#define EPAPER_RST  0x200
#define EPAPER_BUSY 0x100
#define EPAPER_DC   0x8
#define EPAPER_SS   0x10
// Display resolution
#define EPD_WIDTH       200
#define EPD_HEIGHT      200

// EPD1IN54B commands
#define PANEL_SETTING                               0x00
#define POWER_SETTING                               0x01
#define POWER_OFF                                   0x02
#define POWER_OFF_SEQUENCE_SETTING                  0x03
#define POWER_ON                                    0x04
#define POWER_ON_MEASURE                            0x05
#define BOOSTER_SOFT_START                          0x06
#define DEEP_SLEEP                                  0x07
#define DATA_START_TRANSMISSION_1                   0x10
#define DATA_STOP                                   0x11
#define DISPLAY_REFRESH                             0x12
#define DATA_START_TRANSMISSION_2                   0x13
#define PLL_CONTROL                                 0x30
#define TEMPERATURE_SENSOR_COMMAND                  0x40
#define TEMPERATURE_SENSOR_CALIBRATION              0x41
#define TEMPERATURE_SENSOR_WRITE                    0x42
#define TEMPERATURE_SENSOR_READ                     0x43
#define VCOM_AND_DATA_INTERVAL_SETTING              0x50
#define LOW_POWER_DETECTION                         0x51
#define TCON_SETTING                                0x60
#define TCON_RESOLUTION                             0x61
#define SOURCE_AND_GATE_START_SETTING               0x62
#define GET_STATUS                                  0x71
#define AUTO_MEASURE_VCOM                           0x80
#define VCOM_VALUE                                  0x81
#define VCM_DC_SETTING_REGISTER                     0x82
#define PROGRAM_MODE                                0xA0
#define ACTIVE_PROGRAM                              0xA1
#define READ_OTP_DATA                               0xA2
#define  COLORED        1
#define  UNCOLORED      0
extern  uint8_t IMAGE_BLACK[];
extern  uint8_t IMAGE_RED[];
extern const  uint8_t Font8_size[];
extern const  uint8_t Font8_Table[];
void mywait(uint32_t );
void  Int_EP(void);
void SendCommand_EP(uint8_t command);
void SendData_EP(uint8_t data);
void WaitUntilIdle_EP(void);
void Reset_EP(void);
void SetLutBw_EP(void);
void SetLutRed_EP(void);
void DisplayFrame_EP( uint8_t* frame_buffer_black,  uint8_t* frame_buffer_red);
void Sleep_EP(void);
void DCon(void);
void DCoff(void);
void RSTon(void);
void RSToff(void);
void SSon(void);
void SSoff(void);
void SetLutBw(void);
void SetLutRed(void);
void Sleep_EP(void);
void SetPixel_EP(uint8_t *Buffer,uint8_t x, uint8_t y, uint8_t color);
void DrawLine_EP(uint8_t *Buffer, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color);
void DrawHorizontalLine_EP(uint8_t *Buffer, uint8_t x, uint8_t y, uint8_t lenght, uint8_t color);
void DrawVerticalLine_EP(uint8_t *Buffer, uint8_t x, uint8_t y, uint8_t pitch, uint8_t color);
void DrawRectangle_EP(uint8_t *Buffer, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color);
void DrawFilledRectangle_EP(uint8_t *Buffer, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color);
void DrawCircle(uint8_t *Buffer, uint8_t x, uint8_t y, uint8_t radius, uint8_t color);
void DrawFilledCircle_EP(uint8_t *frame_buffer, uint8_t x, uint8_t y, uint8_t radius, uint8_t color);
void DrawCharAt_EP(uint8_t *Buffer, uint8_t x, uint8_t y, uint8_t ascii_char,const uint8_t *fontsize,const uint8_t* font, uint8_t color);
void DrawStringAt_EP(uint8_t *Buffer, uint8_t x, uint8_t y, char *text,const uint8_t *fontsize,const uint8_t* font, uint8_t color);
//void Write_Text_EP(uint8_t *Buffor/*int8_t *x_pos,uint8_t *y_pos*/,const char *text,uint8_t* font[]);

#endif
