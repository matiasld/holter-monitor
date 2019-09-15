/*
 * varios.h
 *
 *  Created on: 8 de nov. de 2016
 *      Author: fabian
 */

#ifndef VARIOS_H_
#define VARIOS_H_

#include "ff.h"

#define PROM_ON		1
//#define RELEASE		1

#define UPDATE_LENGHT 	300
#define	SIGNAL_MAX		80
#define CANT_PROM		3
#define SIZE_WRITE_SAVE	512

#define ON				1
#define OFF				0

#ifdef RELEASE

#else
#define EXPANSION0		2,7
#define EXPANSION1		1,29
#define EXPANSION2		4,28
#define EXPANSION3		1,23
#define EXPANSION4		1,20
#define EXPANSION5		0,19
#define EXPANSION6		3,26
#define EXPANSION7		1,25
#define EXPANSION8		1,22
#define EXPANSION9		1,19
#define EXPANSION10		0,20
#define EXPANSION11		3,25
#define EXPANSION12		1,27
#define EXPANSION13		1,24
#define EXPANSION14		1,21
#define EXPANSION15		1,18
#define EXPANSION16		2,8
#define EXPANSION17		2,12
#define EXPANSION18		0,16
#define EXPANSION19		0,15
#define EXPANSION20		0,22
#define EXPANSION21		0,17

#define LCD_B0		  	EXPANSION0
#define LCD_B1		  	EXPANSION1
#define LCD_B2		  	EXPANSION2
#define LCD_B3		  	EXPANSION3
#define LCD_B4		  	EXPANSION4
#define LCD_B5		  	EXPANSION5
#define LCD_B6		  	EXPANSION6
#define LCD_B7		  	EXPANSION7
#define LCD_B8		  	EXPANSION8
#define LCD_B9		  	EXPANSION9
#define LCD_B10		  	EXPANSION10
#define LCD_B11 	  	EXPANSION11
#define LCD_B12	      	EXPANSION12
#define LCD_B13		  	EXPANSION13
#define LCD_B14		  	EXPANSION14
#define LCD_B15		  	EXPANSION15

#define LCD_RS 		  	EXPANSION16
#define LCD_WR        	2,9 // LA EXPANSION17 NO ANDA CON MI STICK
#define LCD_RD        	EXPANSION18
#define LCD_CS        	EXPANSION19
#define LCD_REST      	EXPANSION20
#define LCD_BCKLIGHT	EXPANSION21
#endif

#ifdef RELEASE

#else
#define ADC0			1,31
#define ADC1			0,24
#define ADC2			0,25
#endif

#define FS				20000

/* SPI clock rate setting.
SSP0_CLK = SystemCoreClock / divider,
The divider must be a even value between 2 and 254!
In SPI mode, max clock speed is 20MHz for MMC and 25MHz for SD */
#define SPI_CLOCKRATE_LOW   (uint32_t) 400000
#define SPI_CLOCKRATE_HIGH  (uint32_t) 5000000 // MAXIMO PERSONAL PARA TEMAS DE DEBUG
#define SPI_CLOCKRATE_HSD   (uint32_t) 25000000 // MAXIMO PARA TARJETAS SD
#define SPI_CLOCKRATE_HMMC  (uint32_t) 20000000 // MAXIMO PARA TARJETAS MMC

#define LargoUpdateSignal	15
#define	LargoColaSSP		10
#define	LargoColaSD			1024
#define LARGO_PROMEDIO		10

#ifdef RELEASE

#else
#define KEY_DIMMER		0,11 // Key 3 Infotronic
#define KEY_MODO		0,18 // Key 2 Infotronic
#define KEY_DISPARO		2,10 // Key 1 Infotronic
#endif

#define LED_DEBUG		2,1

#define SAVING_NONE		0
#define SAVING_USB		1
#define SAVING_SD		2

#define	CH1_MASK		0x01
#define	CH2_MASK		0x02
#define	CH3_MASK		0x04

typedef enum Modo_Disparo{
	MODO_CH1 = 0,
	MODO_CH2,
	MODO_CH3,
	MODO_CH1PCH2,
	MODO_CH1PCH3,
	MODO_CH2PCH3,
	MODO_CH1PCH2PCH3,
	MODO_MAX
} MODO_T;

typedef enum Nivel_Brillo{
	BRILLO_25 = 0,
	BRILLO_50,
	BRILLO_75,
	BRILLO_99,
	BRILLO_MAX
} BRILLO_T;

typedef struct{
	uint8_t *canales;
	FIL *archivo;
} msg_disparo_t;

typedef struct{
	uint8_t canales_activos;
	BRILLO_T brillo_actual;
	uint8_t flag_usb;
	uint8_t flag_sd;
}config_t;

typedef struct{
	int16_t *buffer;
	uint8_t canal;
}msg_ch_t;

void ADC_SetEP(uint8_t canal, int16_t *dato);

// Tasks declarations:


#endif /* VARIOS_H_ */
