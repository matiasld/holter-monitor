/*
 * TIMER.c
 *
 *  Created on: 4 de set. de 2016
 *      Author: fabian
 */

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "TIMER.h"
#include "chip.h"
#include "TFT.h"
#include "../../inc/varios.h"

extern xSemaphoreHandle sem_CH1, sem_CH2, sem_CH3;
extern xQueueHandle cola_base_tiempo;

/**
 * @brief	Handle interrupt from 32-bit timer
 * @return	Nothing
 */
void TIMER0_IRQHandler(void)
{
	static uint32_t counter = 0, sub_counter = 0;
	uint32_t aux;
	portBASE_TYPE cambio = pdFALSE;

	if(Chip_TIMER_MatchPending(LPC_TIMER0, 0)){
		Chip_TIMER_ClearMatch(LPC_TIMER0, 0);	// Limpiamos flag de interrupcion

		// Disparo las conversiones, cada 0.5mseg (500uSeg)
		Chip_ADC_EnableChannel(LPC_ADC, ADC_CH5, ENABLE);
		Chip_ADC_SetStartMode(LPC_ADC, ADC_START_NOW, ADC_TRIGGERMODE_RISING);

		sub_counter++;
		if(sub_counter >= LARGO_PROMEDIO){
			// Entra aca cada LARGO_PROMEDIOmseg (5 mSeg)
			sub_counter = 0;
			counter++;
			counter %= UPDATE_LENGHT;		//Si igual el ancho de la pantalla, vuelve a 0.

			// Limpio la cola para actualizar mi base de tiempo. Mantengo el sincronismo.
			while(xQueueReceiveFromISR(cola_base_tiempo, &aux, &cambio) == pdTRUE);
			xQueueSendToBackFromISR(cola_base_tiempo, &counter, &cambio);
		}
	}

	if(cambio == pdTRUE){
		portEND_SWITCHING_ISR(pdTRUE);
	} else{
		portEND_SWITCHING_ISR(pdFALSE);
	}
}

/*
 * Maneja el PWM del Display.
 */
void TIMER1_IRQHandler(void)
{
	if(Chip_TIMER_MatchPending(LPC_TIMER1, 0)){
		Chip_TIMER_ClearMatch(LPC_TIMER1, 0);
		Chip_GPIO_SetPinOutLow(LPC_GPIO, LCD_BCKLIGHT);
	}

	if(Chip_TIMER_MatchPending(LPC_TIMER1, 1)){
		Chip_TIMER_ClearMatch(LPC_TIMER1, 1);
		Chip_GPIO_SetPinOutHigh(LPC_GPIO, LCD_BCKLIGHT);
	}
}

void InitTIMER0(void){
    // Configuracion TIMER0 encargado de disparar el ADC
	Chip_TIMER_Init(LPC_TIMER0);
	Chip_Clock_SetPCLKDiv(SYSCTL_PCLK_TIMER0, SYSCTL_CLKDIV_1);
	Chip_TIMER_PrescaleSet(LPC_TIMER0, 0);
	Chip_TIMER_SetMatch(LPC_TIMER0, 0, SystemCoreClock / MATCH1);
	Chip_TIMER_MatchEnableInt(LPC_TIMER0, 0);
	Chip_TIMER_ResetOnMatchEnable(LPC_TIMER0, 0);
	Chip_TIMER_ExtMatchControlSet(LPC_TIMER0, 0, TIMER_EXTMATCH_DO_NOTHING, 0);
	Chip_TIMER_Enable(LPC_TIMER0);

	NVIC_ClearPendingIRQ(TIMER0_IRQn);
	NVIC_EnableIRQ(TIMER0_IRQn);
}

void InitTIMER1(void){
	// Configuramos el TIMER que se ocupa del PWM del backlight
	Chip_TIMER_Init(LPC_TIMER1);
	Chip_Clock_SetPCLKDiv(SYSCTL_PCLK_TIMER1, SYSCTL_CLKDIV_1);
    Chip_TIMER_PrescaleSet(LPC_TIMER1, 100); // Frecuencia de clock del TIMER de 1MHz
	Chip_TIMER_SetMatch(LPC_TIMER1, 0, 250);	// Duty inicial: 25%
	Chip_TIMER_SetMatch(LPC_TIMER1, 1, 1000);	// Frecuencia de TIMER1 = 1KHz
	Chip_TIMER_MatchEnableInt(LPC_TIMER1, 0);
	Chip_TIMER_MatchEnableInt(LPC_TIMER1, 1);
	Chip_TIMER_ResetOnMatchDisable(LPC_TIMER1, 0);
	Chip_TIMER_ResetOnMatchEnable(LPC_TIMER1, 1);
	Chip_TIMER_ExtMatchControlSet(LPC_TIMER1, 0, TIMER_EXTMATCH_DO_NOTHING, 0);
	Chip_TIMER_ExtMatchControlSet(LPC_TIMER1, 0, TIMER_EXTMATCH_DO_NOTHING, 1);
	Chip_TIMER_Enable(LPC_TIMER1);

	NVIC_ClearPendingIRQ(TIMER1_IRQn);
	NVIC_EnableIRQ(TIMER1_IRQn);
}
