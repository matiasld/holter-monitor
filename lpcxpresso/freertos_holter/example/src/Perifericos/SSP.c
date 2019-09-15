/*
 * SSP.c
 *
 *  Created on: 10 de jun. de 2016
 *      Author: Augusto
 */

#include "varios.h"
#include "chip.h"
#include "SSP.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

extern xQueueHandle cola_SSP_RX;
extern xQueueHandle cola_SSP_TX;

/**
 * @brief	SSP interrupt handler sub-routine
 * @return	Nothing
 */
void SSP1_IRQHandler(void)
{
	portBASE_TYPE cambio_rx, cambio_tx;
	Chip_SSP_DATA_SETUP_T setup;
	uint8_t dato_rx;
	uint8_t dato_tx;

	// No deberia entrar si no pusimos antes un dato en la cola

	xQueueReceiveFromISR(cola_SSP_TX, &dato_tx, &cambio_tx);

	setup.tx_cnt = 0;
	setup.rx_cnt = 0;
	setup.rx_data = &dato_rx;
	setup.tx_data = &dato_tx;
	setup.length = 1;

	// Envio y recibo el dato
	if(Chip_SSP_Int_RWFrames8Bits(LPC_SSP1, &setup) == SUCCESS){
		// Pongo el dato recibido en la cola
		xQueueSendToBackFromISR(cola_SSP_RX, &dato_rx, &cambio_rx);
		NVIC_DisableIRQ(SSP1_IRQn);
	}

	if(Chip_SSP_GetRawIntStatus(LPC_SSP1, SSP_RORRIS)){
		// Si entra aca, es porque llego un dato cuando la RX FIFO estaba llena, GRAVE error...
		while(1);
	}

	if(cambio_tx == pdTRUE || cambio_rx == pdTRUE)
		portEND_SWITCHING_ISR(pdTRUE);
}

void InitSSP1(void){
	/* Set up clock and muxing for SSP1 interface */
	/*
	 * Initialize SSP1 pins connect
	 * P0.7: SCK
	 * P0.6: SSEL
	 * P0.8: MISO
	 * P0.9: MOSI
	 */
	Chip_IOCON_PinMux(LPC_IOCON, 0, 7, IOCON_MODE_INACT, IOCON_FUNC2);
	Chip_IOCON_PinMux(LPC_IOCON, SD_SEL_PINPORT, IOCON_MODE_INACT, IOCON_FUNC0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, SD_SEL_PINPORT);
	Chip_IOCON_PinMux(LPC_IOCON, 0, 8, IOCON_MODE_INACT, IOCON_FUNC2);
	Chip_IOCON_PinMux(LPC_IOCON, 0, 9, IOCON_MODE_INACT, IOCON_FUNC2);

	Chip_SSP_Enable(LPC_SSP1);
	Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_SSP1);
	Chip_Clock_SetPCLKDiv(SYSCTL_CLOCK_SSP1, SYSCTL_CLKDIV_1); // Usamos el clock del CPU
	Chip_SSP_Int_FlushData(LPC_SSP1);/* flush dummy data from SSP FiFO */

	Chip_SSP_Set_Mode(LPC_SSP1, SSP_MODE_MASTER);
	Chip_SSP_SetFormat(LPC_SSP1, SSP_BITS_8, SSP_FRAMEFORMAT_SPI, SSP_CLOCK_CPHA0_CPOL0);
	Chip_SSP_SetBitRate(LPC_SSP1, SPI_CLOCKRATE_LOW);

	Chip_SSP_Int_Enable(LPC_SSP1);	/* enable interrupt */
}
