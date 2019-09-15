/*
 * TFT.c
 *
 *  Created on: 26 de set. de 2016
 *      Author: fabian
 */

#include <string.h>
#include "varios.h"
#include "TFT.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

void Lcd_Put_Data(uint16_t data){
	Chip_GPIO_SetPinState(LPC_GPIO, LCD_B0, (data >> 0) & 1);
	Chip_GPIO_SetPinState(LPC_GPIO, LCD_B1, (data >> 1) & 1);
	Chip_GPIO_SetPinState(LPC_GPIO, LCD_B2, (data >> 2) & 1);
	Chip_GPIO_SetPinState(LPC_GPIO, LCD_B3, (data >> 3) & 1);
	Chip_GPIO_SetPinState(LPC_GPIO, LCD_B4, (data >> 4) & 1);
	Chip_GPIO_SetPinState(LPC_GPIO, LCD_B5, (data >> 5) & 1);
	Chip_GPIO_SetPinState(LPC_GPIO, LCD_B6, (data >> 6) & 1);
	Chip_GPIO_SetPinState(LPC_GPIO, LCD_B7, (data >> 7) & 1);
	Chip_GPIO_SetPinState(LPC_GPIO, LCD_B8, (data >> 8) & 1);
	Chip_GPIO_SetPinState(LPC_GPIO, LCD_B9, (data >> 9) & 1);
	Chip_GPIO_SetPinState(LPC_GPIO, LCD_B10, (data >> 10) & 1);
	Chip_GPIO_SetPinState(LPC_GPIO, LCD_B11, (data >> 11) & 1);
	Chip_GPIO_SetPinState(LPC_GPIO, LCD_B12, (data >> 12) & 1);
	Chip_GPIO_SetPinState(LPC_GPIO, LCD_B13, (data >> 13) & 1);
	Chip_GPIO_SetPinState(LPC_GPIO, LCD_B14, (data >> 14) & 1);
	Chip_GPIO_SetPinState(LPC_GPIO, LCD_B15, (data >> 15) & 1);
}

void Lcd_Write_Com(uint16_t DH){
	Chip_GPIO_SetPinState(LPC_GPIO, LCD_CS, 0); // clear CS pin to ENABLE LCD
	Chip_GPIO_SetPinState(LPC_GPIO, LCD_RS, 0);	// set command mode

	Lcd_Put_Data(DH);

	Chip_GPIO_SetPinState(LPC_GPIO, LCD_WR, 0);
	Chip_GPIO_SetPinState(LPC_GPIO, LCD_WR, 1);
	Chip_GPIO_SetPinState(LPC_GPIO, LCD_CS, 1);	// clear CS pin to ENABLE LCD
}

void Lcd_Write_Data(uint16_t DH){
	Chip_GPIO_SetPinState(LPC_GPIO, LCD_CS, 0); // clear CS pin to ENABLE LCD
	Chip_GPIO_SetPinState(LPC_GPIO, LCD_RS, 1); // set data mode

	Lcd_Put_Data(DH);

	Chip_GPIO_SetPinState(LPC_GPIO, LCD_WR, 0);
	Chip_GPIO_SetPinState(LPC_GPIO, LCD_WR, 1);
	Chip_GPIO_SetPinState(LPC_GPIO, LCD_CS, 1); // clear CS pin to ENABLE LCD
}

void Lcd_Write_Com_Data(uint16_t com,uint16_t dat){
	Lcd_Write_Com (com);
	Lcd_Write_Data(dat);
}

void tftSetup(void){
	// Seteo pines
	Chip_IOCON_PinMux(LPC_IOCON, LCD_B0, MD_PLN, IOCON_FUNC0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, LCD_B0);
	Chip_IOCON_PinMux(LPC_IOCON, LCD_B1, MD_PLN, IOCON_FUNC0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, LCD_B1);
	Chip_IOCON_PinMux(LPC_IOCON, LCD_B2, MD_PLN, IOCON_FUNC0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, LCD_B2);
	Chip_IOCON_PinMux(LPC_IOCON, LCD_B3, MD_PLN, IOCON_FUNC0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, LCD_B3);
	Chip_IOCON_PinMux(LPC_IOCON, LCD_B4, MD_PLN, IOCON_FUNC0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, LCD_B4);
	Chip_IOCON_PinMux(LPC_IOCON, LCD_B5, MD_PLN, IOCON_FUNC0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, LCD_B5);
	Chip_IOCON_PinMux(LPC_IOCON, LCD_B6, MD_PLN, IOCON_FUNC0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, LCD_B6);
	Chip_IOCON_PinMux(LPC_IOCON, LCD_B7, MD_PLN, IOCON_FUNC0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, LCD_B7);
	Chip_IOCON_PinMux(LPC_IOCON, LCD_B8, MD_PLN, IOCON_FUNC0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, LCD_B8);
	Chip_IOCON_PinMux(LPC_IOCON, LCD_B9, MD_PLN, IOCON_FUNC0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, LCD_B9);
	Chip_IOCON_PinMux(LPC_IOCON, LCD_B10, MD_PLN, IOCON_FUNC0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, LCD_B10);
	Chip_IOCON_PinMux(LPC_IOCON, LCD_B11, MD_PLN, IOCON_FUNC0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, LCD_B11);
	Chip_IOCON_PinMux(LPC_IOCON, LCD_B12, MD_PLN, IOCON_FUNC0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, LCD_B12);
	Chip_IOCON_PinMux(LPC_IOCON, LCD_B13, MD_PLN, IOCON_FUNC0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, LCD_B13);
	Chip_IOCON_PinMux(LPC_IOCON, LCD_B14, MD_PLN, IOCON_FUNC0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, LCD_B14);
	Chip_IOCON_PinMux(LPC_IOCON, LCD_B15, MD_PLN, IOCON_FUNC0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, LCD_B15);

	Chip_IOCON_PinMux(LPC_IOCON, LCD_CS, MD_PLN, IOCON_FUNC0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, LCD_CS);
	Chip_IOCON_PinMux(LPC_IOCON, LCD_RS, MD_PLN, IOCON_FUNC0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, LCD_RS);
	Chip_IOCON_PinMux(LPC_IOCON, LCD_WR, MD_PLN, IOCON_FUNC0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, LCD_WR);
	Chip_IOCON_PinMux(LPC_IOCON, LCD_RD, MD_PLN, IOCON_FUNC0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, LCD_RD);
	Chip_IOCON_PinMux(LPC_IOCON, LCD_REST, MD_PLN, IOCON_FUNC0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, LCD_REST);
	Chip_IOCON_PinMux(LPC_IOCON, LCD_BCKLIGHT, MD_PLN, IOCON_FUNC0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, LCD_BCKLIGHT);
	Chip_GPIO_SetPinOutLow(LPC_GPIO, LCD_BCKLIGHT); // Apago el backlight hasta inicializar la TFT

	Chip_GPIO_SetPinState(LPC_GPIO, LCD_CS, 1);
	Chip_GPIO_SetPinState(LPC_GPIO, LCD_RS, 1);
	Chip_GPIO_SetPinState(LPC_GPIO, LCD_WR, 1);
	Chip_GPIO_SetPinState(LPC_GPIO, LCD_RD, 1);
	Chip_GPIO_SetPinState(LPC_GPIO, LCD_REST, 1);
	vTaskDelay(5/portTICK_RATE_MS);
	Chip_GPIO_SetPinState(LPC_GPIO, LCD_REST, 0);
	vTaskDelay(10/portTICK_RATE_MS);
	Chip_GPIO_SetPinState(LPC_GPIO, LCD_REST, 1);
	vTaskDelay(50/portTICK_RATE_MS);

	Lcd_Write_Com_Data(0x0000,0x0001); //Start Oscillation OSCEN=1
	vTaskDelay(15/portTICK_RATE_MS);

	Lcd_Write_Com_Data(0x0003,0xAAAC); //Power Control (1)
	Lcd_Write_Com_Data(0x000C,0x0002); //Power Control (2)
	Lcd_Write_Com_Data(0x000D,0x000A); //Power Control (3)
	Lcd_Write_Com_Data(0x000E,0x2C00); //Power Control (4)
	Lcd_Write_Com_Data(0x001E,0x00B8); //Power Control (5)
	vTaskDelay(15/portTICK_RATE_MS);

	#ifdef Reverse
	Lcd_Write_Com_Data(0x0001,0x293F);              //Driver Output Control RL=0, REV=1, BGR=1, TB=0
	#else
	Lcd_Write_Com_Data(0x0001,0x2B3F);              //Driver Output Control RL=0, REV=1, BGR=1, TB=1
	#endif

	Lcd_Write_Com_Data(0x0002,0x0600);

	//Restore VSYNC mode from low power state
	Lcd_Write_Com_Data(0x0010,0x0000); //Sleep mode cancel
	Lcd_Write_Com_Data(0x0011,0x6070); //Entry Mode
									   //DFM   0x4000 = 262L color
									   //DFM   0x6000 = 65K color
									   //AM    0x0000 = horizontal display
									   //AM    0x0008 = Vertical display
									   //ID[0] 0x0000 = horizontal decrement
									   //ID[0] 0x0010 = horizontal increment
									   //ID[1] 0x0000 = Vertical decrement
									   //ID[1] 0x0020 = Vertical increment

	vTaskDelay(30/portTICK_RATE_MS);
	Lcd_Write_Com_Data(0x0005,0x0000);              //Compare register
	Lcd_Write_Com_Data(0x0006,0x0000);              //Compare register

	//Horizontal and Vertical porch are for DOTCLK mode operation
	Lcd_Write_Com_Data(0x0016,0xEF1C);              //Horizontal Porch
	Lcd_Write_Com_Data(0x0017,0x0003);              //Vertical Porch
	//Display Control
	Lcd_Write_Com_Data(0x0007,0x0233);              //Display Control
													//D1 0x0000 = display off
													//D1 0x0002 = display on
													//D0 0x0000 = internal display halt
													//D0 0x0001 = internal display operate

	Lcd_Write_Com_Data(0x000B,0x0000);              //Frame cycle control
	Lcd_Write_Com_Data(0x000F,0x0000); 				// Gate Scan Position
	vTaskDelay(20/portTICK_RATE_MS);

	//Vertical Scroll Control
	Lcd_Write_Com_Data(0x0041,0x0000);              //Vertical Scroll Control
	Lcd_Write_Com_Data(0x0042,0x0000);              //Vertical Scroll Control

	//1st Screen driving position
	Lcd_Write_Com_Data(0x0048,0x0000);              //Start position. 0
	Lcd_Write_Com_Data(0x0049,0x013F);              //End position.   319

	//Source RAM address
	Lcd_Write_Com_Data(0x0044,0xEF00);              //Horizontal RAM address position start/end setup
													//dec 239
													//dec 0, i.e. horizontal ranges from 0 -> 239
													//POR value is 0xEF00 anyway. This address must be set before RAM write

	Lcd_Write_Com_Data(0x0045,0x0000);              //Vertical RAM address start position setting (0x0000 = dec 0)
	Lcd_Write_Com_Data(0x0046,0x013F);              //Vertical RAM address end position setting   (0x013F = dec 319)

	vTaskDelay(20/portTICK_RATE_MS);

	//Gamma control
	Lcd_Write_Com_Data(0x0030,0x0707);
	Lcd_Write_Com_Data(0x0031,0x0204);
	Lcd_Write_Com_Data(0x0032,0x0204);
	Lcd_Write_Com_Data(0x0033,0x0502);
	Lcd_Write_Com_Data(0x0034,0x0507);
	Lcd_Write_Com_Data(0x0035,0x0204);
	Lcd_Write_Com_Data(0x0036,0x0204);
	Lcd_Write_Com_Data(0x0037,0x0502);
	Lcd_Write_Com_Data(0x003A,0x0302);
	Lcd_Write_Com_Data(0x003B,0x0302);
	//RAM address set
	Lcd_Write_Com_Data(0x004F,0x0000);              //Make initial settings for the GDDRAM X
	Lcd_Write_Com_Data(0x004E,0x0000);              //Make initial settings for the GDDRAM Y
	vTaskDelay(20/portTICK_RATE_MS);
	Lcd_Write_Com(0x0022);
}

void LCD_Reset2(void)
{
	Lcd_Write_Com_Data(0x0007,0x0021);
	Lcd_Write_Com_Data(0x0000,0x0001);
	Lcd_Write_Com_Data(0x0007,0x0023);
	Lcd_Write_Com_Data(0x0010,0x0000);
	vTaskDelay(90/portTICK_RATE_MS);
	Lcd_Write_Com_Data(0x0007,0x0033);
}

void Lcd_SetCursor(uint16_t x, uint16_t y)
{
#ifdef Horizontal
	Lcd_Write_Com_Data(0x004E,x);                 //initial settings for the GDDRAM X address in the address counter (AC).
	Lcd_Write_Com_Data(0x004F,y);                 //initial settings for the GDDRAM Y address in the address counter (AC).
#else
	Lcd_Write_Com_Data(0x004E,y);                 //initial settings for the GDDRAM X address in the address counter (AC).
	Lcd_Write_Com_Data(0x004F,x);                 //initial settings for the GDDRAM Y address in the address counter (AC).
#endif
  Lcd_Write_Com(0x0022);
}

void Lcd_SetArea(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
#ifdef Horizontal
    Lcd_Write_Com_Data(0x0044,((x2<<8) | x1));  //Source RAM address window
    Lcd_Write_Com_Data(0x0045,y1);              //Gate RAM address window
    Lcd_Write_Com_Data(0x0046,y2);              //Gate RAM address window
#else
    Lcd_Write_Com_Data(0x0044,((y2<<8) | y1));  //Source RAM address window
    Lcd_Write_Com_Data(0x0045,x1);              //Gate RAM address window
    Lcd_Write_Com_Data(0x0046,x2);              //Gate RAM address window
#endif
    vTaskDelay(1/portTICK_RATE_MS);
	Lcd_SetCursor(x1, y1);
}

void LCD_PutPixel(uint16_t x, uint16_t y, uint16_t color)
{
	Lcd_SetCursor(x,y);
	Lcd_Write_Data(color);
}

void LCD_Rect(uint16_t left, uint16_t top, uint16_t right, uint16_t bottom, uint16_t color)
{
	uint16_t x,y;

	Lcd_SetArea(left, top, right, bottom);

	for(y=top; y<=bottom; y++) {
		for(x=left; x<=right; x++){
			Lcd_Write_Data(color);
		}
	}

	Lcd_SetArea(0, 0, SCREEN_HOR_SIZE-1, SCREEN_VER_SIZE-1);
}

void Address_set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{
	Lcd_Write_Com_Data(0x0044,(x2<<8)+x1);
	Lcd_Write_Com_Data(0x0045,y1);
	Lcd_Write_Com_Data(0x0046,y2);
//	Lcd_Write_Com_Data(0x004e,x1);
//	Lcd_Write_Com_Data(0x004f,y1);
    Lcd_Write_Com(0x0022);

}

void LCD_Clear(uint16_t color)
{
  LCD_Rect(0, 0, SCREEN_HOR_SIZE-1, SCREEN_VER_SIZE-1, color);
}

extern xQueueHandle cola_config;
extern xSemaphoreHandle sem_TFT;

void SetupBackground(void){
	uint8_t button1[]="DIMMER";
	uint8_t button2[]="CANAL";
	uint8_t button3[]="DISPARO";
	char modo[25]="MODO:";
	char brillo[25]="BRILLO:";
	config_t configActual;

	xQueuePeek(cola_config, &configActual, 0); // No deberia quedarse aca nunca
	switch(configActual.canales_activos){
	case CH1_MASK:{
		strcat(modo, "CH1");

		break;
	}
	case CH2_MASK:{
		strcat(modo, "CH2");

		break;
	}
	case CH3_MASK:{
		strcat(modo, "CH3");

		break;
	}
	case CH1_MASK|CH2_MASK:{
		strcat(modo, "CH1+CH2");

		break;
	}
	case CH1_MASK|CH3_MASK:{
		strcat(modo, "CH1+CH3");

		break;
	}
	case CH2_MASK|CH3_MASK:{
		strcat(modo, "CH2+CH3");

		break;
	}
	case CH1_MASK|CH2_MASK|CH3_MASK:{
		strcat(modo, "CH1+CH2+CH3");

		break;
	}
	default:break;
	}

	switch(configActual.brillo_actual){
	case BRILLO_25:{
		strcat(brillo, "25%");

		break;
	}
	case BRILLO_50:{
		strcat(brillo, "50%");

		break;
	}
	case BRILLO_75:{
		strcat(brillo, "75%");

		break;
	}
	case BRILLO_99:{
		strcat(brillo, "100%");

		break;
	}
	default: break;
	}

	/*
	 * Deshabilitamos conversiones de ADC frenando el TIMER0
	 * ya que el limpiado de toda la pantalla y la nueva puesta del fondo
	 * toma bastante tiempo
	 */

	Chip_TIMER_Disable(LPC_TIMER0);
	xSemaphoreTake(sem_TFT, portMAX_DELAY);

	// Limpiamos la TFT en fondo NEGRO
	LCD_Clear(BCKGD_COLOR);

	// Ejes cartesianos:
	Line(X_AXIS_BEGIN_X, X_AXIS_BEGIN_Y, X_AXIS_BEGIN_X, X_AXIS_BEGIN_Y+X_AXIS_SIZE, AXIS_COLOR); // Eje X (tiempo)
	Line(Y_AXIS_BEGIN_X, Y_AXIS_BEGIN_Y, Y_AXIS_BEGIN_X+Y_AXIS_SIZE, Y_AXIS_BEGIN_Y, AXIS_COLOR); // Eje Y (intensidad de señal)

	// Flechas de los ejes:
	Line(X_AXIS_BEGIN_X, X_AXIS_BEGIN_Y+X_AXIS_SIZE, X_AXIS_BEGIN_X+X_ARROW_SIZE, (X_AXIS_BEGIN_Y+X_AXIS_SIZE)-Y_ARROW_SIZE, AXIS_COLOR);
	Line(X_AXIS_BEGIN_X, X_AXIS_BEGIN_Y+X_AXIS_SIZE, X_AXIS_BEGIN_X-X_ARROW_SIZE, (X_AXIS_BEGIN_Y+X_AXIS_SIZE)-Y_ARROW_SIZE, AXIS_COLOR);

	Line(Y_AXIS_BEGIN_X, Y_AXIS_BEGIN_Y, Y_AXIS_BEGIN_X+Y_ARROW_SIZE, Y_AXIS_BEGIN_Y+X_ARROW_SIZE, AXIS_COLOR);
	Line(Y_AXIS_BEGIN_X, Y_AXIS_BEGIN_Y, Y_AXIS_BEGIN_X+Y_ARROW_SIZE, Y_AXIS_BEGIN_Y-X_ARROW_SIZE, AXIS_COLOR);

	Line(Y_AXIS_BEGIN_X+Y_AXIS_SIZE, Y_AXIS_BEGIN_Y, Y_AXIS_BEGIN_X+Y_AXIS_SIZE-Y_ARROW_SIZE, Y_AXIS_BEGIN_Y+X_ARROW_SIZE, AXIS_COLOR);
	Line(Y_AXIS_BEGIN_X+Y_AXIS_SIZE, Y_AXIS_BEGIN_Y, Y_AXIS_BEGIN_X+Y_AXIS_SIZE-Y_ARROW_SIZE, Y_AXIS_BEGIN_Y-X_ARROW_SIZE, AXIS_COLOR);

	// Recuadros y textos de los botones:
	Line(BUTTON1_X_POS, BUTTON1_Y_POS, BUTTON1_X_POS+BUTTON1_X_SIZE, BUTTON1_Y_POS, COLOR_BUTTON1);
	Line(BUTTON1_X_POS+BUTTON1_X_SIZE, BUTTON1_Y_POS, BUTTON1_X_POS+BUTTON1_X_SIZE, BUTTON1_Y_POS+BUTTON1_Y_SIZE, COLOR_BUTTON1);
	Line(BUTTON1_X_POS, BUTTON1_Y_POS, BUTTON1_X_POS, BUTTON1_Y_POS+BUTTON1_Y_SIZE, COLOR_BUTTON1);
	Line(BUTTON1_X_POS+BUTTON1_X_SIZE, BUTTON1_Y_POS+BUTTON1_Y_SIZE, BUTTON1_X_POS, BUTTON1_Y_POS+BUTTON1_Y_SIZE, COLOR_BUTTON1);
	MostrarString(BUTTON1_X_POS+20, BUTTON1_Y_POS+15, button1, COLOR_BUTTON1, ROTACION90);

	Line(BUTTON2_X_POS, BUTTON2_Y_POS, BUTTON2_X_POS+BUTTON2_X_SIZE, BUTTON2_Y_POS, COLOR_BUTTON2);
	Line(BUTTON2_X_POS+BUTTON2_X_SIZE, BUTTON2_Y_POS, BUTTON2_X_POS+BUTTON2_X_SIZE, BUTTON2_Y_POS+BUTTON2_Y_SIZE, COLOR_BUTTON2);
	Line(BUTTON2_X_POS, BUTTON2_Y_POS, BUTTON2_X_POS, BUTTON2_Y_POS+BUTTON2_Y_SIZE, COLOR_BUTTON2);
	Line(BUTTON2_X_POS+BUTTON2_X_SIZE, BUTTON2_Y_POS+BUTTON2_Y_SIZE, BUTTON2_X_POS, BUTTON2_Y_POS+BUTTON2_Y_SIZE, COLOR_BUTTON2);
	MostrarString(BUTTON2_X_POS+20, BUTTON2_Y_POS+20, button2, COLOR_BUTTON2, ROTACION90);

	Line(BUTTON3_X_POS, BUTTON3_Y_POS, BUTTON3_X_POS+BUTTON3_X_SIZE, BUTTON3_Y_POS, COLOR_BUTTON3);
	Line(BUTTON3_X_POS+BUTTON3_X_SIZE, BUTTON3_Y_POS, BUTTON3_X_POS+BUTTON3_X_SIZE, BUTTON3_Y_POS+BUTTON3_Y_SIZE, COLOR_BUTTON3);
	Line(BUTTON3_X_POS, BUTTON3_Y_POS, BUTTON3_X_POS, BUTTON3_Y_POS+BUTTON3_Y_SIZE, COLOR_BUTTON3);
	Line(BUTTON3_X_POS+BUTTON3_X_SIZE, BUTTON3_Y_POS+BUTTON3_Y_SIZE, BUTTON3_X_POS, BUTTON3_Y_POS+BUTTON3_Y_SIZE, COLOR_BUTTON3);
	MostrarString(BUTTON3_X_POS+20, BUTTON3_Y_POS+13, button3, COLOR_BUTTON3, ROTACION90);

	// Indicacion de modo y brillo:
	MostrarString(MODO_X_POS, MODO_Y_POS, (uint8_t *) modo, COLOR_MODO, ROTACION90);
	MostrarString(BRILLO_X_POS, BRILLO_Y_POS, (uint8_t *) brillo, BRILLO_COLOR, ROTACION90);

	xSemaphoreGive(sem_TFT);

	// Seguimos corriendo el timer
	Chip_TIMER_Enable(LPC_TIMER0);
}
