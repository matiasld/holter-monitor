/*
 * TFT.h
 *
 *  Created on: 26 de set. de 2016
 *      Author: fabian
 */

#ifndef SRC_TFT_H_
#define SRC_TFT_H_

#include "chip.h"
#include "Font.h"
#include "Graphics.h"
#include "Calibri10.h"

#define Horizontal 	1
#define CALIBRI		0

//Colores
#define BLACK                       RGB(0x00, 0x00, 0x00)
#define WHITE                       RGB(0xFF, 0xFF, 0xFF)
#define RED                         RGB(0xFF, 0x00, 0x00)
#define GREEN                       RGB(0x00, 0xFF, 0x00)
#define BLUE                        RGB(0x00, 0x00, 0xFF)
#define YELLOW                      RGB(0xFF, 0xFF, 0x00)
#define MAGENTA                     RGB(0xFF, 0x00, 0xFF)
#define CYAN                        RGB(0x00, 0xFF, 0xFF)
#define GRAY                        RGB(0x80, 0x80, 0x40)
#define SILVER                      RGB(0xA0, 0xA0, 0x80)
#define GOLD                        RGB(0xA0, 0xA0, 0x40)

#define SIGNAL_COLOR_CH1			GREEN
#define SIGNAL_COLOR_CH2			MAGENTA
#define SIGNAL_COLOR_CH3			CYAN

#define RGB(red, green, blue)	((uint32_t)( (( red >> 3 ) << 11 ) | \
								(( green >> 2 ) << 5  ) | \
								( blue  >> 3 )))

#define GetMaxX() 		((uint32_t)SCREEN_HOR_SIZE-1)
#define GetMaxY() 		((uint32_t)SCREEN_VER_SIZE-1)

#ifdef Horizontal
	// Horizontal and vertical screen size
	#define SCREEN_HOR_SIZE    240UL
	#define SCREEN_VER_SIZE    320UL
#else
	// Horizontal and vertical screen size
	#define SCREEN_HOR_SIZE    320UL
	#define SCREEN_VER_SIZE    240UL
#endif

#define X_AXIS_SIZE		310
#define Y_AXIS_SIZE		200
#define	Y_ARROW_SIZE	8
#define X_ARROW_SIZE	3
#define Y_AXIS_BEGIN_X	30
#define X_AXIS_BEGIN_X	Y_AXIS_BEGIN_X+(Y_AXIS_SIZE/2)
#define X_AXIS_BEGIN_Y	5
#define Y_AXIS_BEGIN_Y	5
#define BCKGD_COLOR		BLACK
#define AXIS_COLOR		WHITE

#define	BUTTON1_X_POS	0
#define	BUTTON1_Y_POS	0
#define	BUTTON1_X_SIZE	25
#define	BUTTON1_Y_SIZE	80
#define COLOR_BUTTON1	GOLD

#define BUTTON2_X_POS	0
#define BUTTON2_Y_POS	160-BUTTON1_Y_SIZE/2
#define	BUTTON2_X_SIZE  BUTTON1_X_SIZE
#define	BUTTON2_Y_SIZE 	BUTTON1_Y_SIZE
#define COLOR_BUTTON2	GOLD

#define BUTTON3_X_POS	0
#define BUTTON3_Y_POS	319-BUTTON1_Y_SIZE
#define	BUTTON3_X_SIZE  BUTTON1_X_SIZE
#define	BUTTON3_Y_SIZE 	BUTTON1_Y_SIZE
#define COLOR_BUTTON3	GOLD

#define MODO_X_POS		239
#define MODO_Y_POS		100
#define COLOR_MODO		BLUE

#define SAVING_X_POS	230
#define SAVING_Y_POS	30
#define SAVING_COLOR	RED
#define	SAVING_RADIUS	5
#define SAVE_TXT_X_POS	239
#define SAVE_TXT_Y_POS	SAVING_Y_POS+10

#define BRILLO_X_POS	239
#define BRILLO_Y_POS	240
#define BRILLO_COLOR	GOLD

#define ERROR_X_POS		215
#define	ERROR_Y_POS		50
#define ERROR_COLOR		RED

void Lcd_Put_Data(uint16_t DH);
void Lcd_Write_Com(uint16_t DH);
void Lcd_Write_Data(uint16_t DH);
void Lcd_Write_Com_Data(uint16_t com,uint16_t dat);
void tftSetup(void);
void Address_set(uint16_t x1, uint16_t y1,uint16_t x2,uint16_t y2);
void LCD_Clear(uint16_t color);
void LCD_Rect(uint16_t left, uint16_t top, uint16_t right, uint16_t bottom, uint16_t color);
void LCD_PutPixel(uint16_t x, uint16_t y, uint16_t color);
void Lcd_SetArea(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void Lcd_SetCursor(uint16_t x, uint16_t y);
void LCD_Reset2(void);
void SetupBackground(void);

#endif /* SRC_TFT_H_ */
