/*
 * SSP.h
 *
 *  Created on: 10 de jun. de 2016
 *      Author: Augusto
 */

#ifndef SSP_H_
#define SSP_H_

#include "chip.h"

#define BUFFER_SIZE_SSP		100
#define SD_SEL_PINPORT		0,6
#define SPI_Init 			SSP1_Init

// Inicializacion del modulo SSP1
void InitSSP1(void);

#endif /* SSP_H_ */
