///*
// * Tasks.c
// *
// *  Created on: 9 de nov. de 2016
// *      Author: fabian
// */
//
//#include <string.h>
//#include <stdlib.h>
//#include <stdlib.h>
//#include "board.h"
//#include "FreeRTOS.h"
//#include "task.h"
//#include "semphr.h"
//#include "queue.h"
//#include "TFT.h"
//#include "TIMER.h"
//#include "SSP.h"
//#include "RTC.h"
//#include "PLL.h"
//#include "ff.h"
//#include "integer.h"
//#include "diskio.h"
//#include "app_usbd_cfg.h"
//#include "hid_generic.h"
//#include "varios.h"
//
//// Tarea encargada de la inicializacion de la TFT y TIMERS
//// Una vez realizada se eliminara a si misma
//static void TFTInitAndSetup(void *pvParameters) {
//	while (1) {
//		tftSetup(); // Inicializo pines y la TFT
//
//		LCD_Clear(BLACK); // Limpio todos los pixeles de la pantalla a negro
//
//		SetupBackground(); // Seteo el fondo que vamos a re-utilizar
//
//		xSemaphoreGive(sem_CH1); // Liberamos el semaforo para que comience con el canal 1 activo
//
//		InitTIMER0();
//		InitTIMER1();
//		InitSSP1();
//
//		/* Eliminamos la tarea, se supone que nadie deberia crearla de nuevo, por
//		ser de inicializacion */
//		vTaskDelete(NULL);
//	}
//}
//
//// Tarea encargada de borrar todos los pixeles del canal 1 (cuando cambiamos de modo)
//static void CH1Clean(void *pvParameters){
//	while(1){
//		if(xSemaphoreTake(sem_clean_ch1, portMAX_DELAY) == pdTRUE){
//			uint32_t count = 0;
//			int16_t *buff = (int16_t *) pvParameters;
//
//			// Limpiamos los pixeles
//			for(count = 0; count < UPDATE_LENGHT; count++){
//				LCD_PutPixel(X_AXIS_BEGIN_X+buff[count], Y_AXIS_BEGIN_Y+count, BLACK);
//			}
//
//			// Re-dibujamos los ejes
//			Line(X_AXIS_BEGIN_X, X_AXIS_BEGIN_Y, X_AXIS_BEGIN_X, X_AXIS_BEGIN_Y+X_AXIS_SIZE, COLOR_EJE); // Eje X (tiempo)
//			Line(Y_AXIS_BEGIN_X, Y_AXIS_BEGIN_Y, Y_AXIS_BEGIN_X+Y_AXIS_SIZE, Y_AXIS_BEGIN_Y, COLOR_EJE); // Eje Y (intensidad de señal)
//		}
//	}
//}
//
//// Tarea encargada de actuailzar la señal que dibujamos (CANAL 1)
//static void SignalUpdateCH1(void *pvParameters) {
//	static int16_t buffer[UPDATE_LENGHT];
//	static uint32_t counter = 0;
//	static int16_t buff_datos[10];
//	static uint16_t prom_count = 0;
//
//	xTaskCreate(CH1Clean, (signed char *) "CH1CleanTask",
//				configMINIMAL_STACK_SIZE, (void *) buffer, (tskIDLE_PRIORITY + 3UL),
//				(xTaskHandle *) NULL);
//
//	while(1){
//		// Si hay un dato nuevo en la cola
//		if(xQueueReceive(colaADC0, buff_datos+prom_count, portMAX_DELAY) == pdTRUE){
//			prom_count++;
//
//			// Cuando tomamos 10 muestras realizamos el promedio y ponemos el pixel correspondiente
//			if(prom_count == 10){
//				uint8_t i;
//				int32_t promedio = 0;
//				int16_t cuenta;
//
//				prom_count = 0;
//
//				// Promedio los 10 datos
//				for(i = 0; i < 10; i++){
//					promedio = promedio + buff_datos[i];
//				}
//				promedio /= 10;
//
//				// Resto offset
//				promedio -= 4096/2;
//				cuenta = (promedio * SIGNAL_MAX) / 4096;
//
//				// Me fijo por donde esta la base de tiempos
//				xQueuePeek(cola_base_tiempo, &counter, 0); // Se supone que siempre tiene que tener un valor
//				buffer[counter] = cuenta;
//
//				xSemaphoreTake(sem_TFT, portMAX_DELAY);
//				//Pongo el pixel actual
//				LCD_PutPixel(X_AXIS_BEGIN_X+cuenta, Y_AXIS_BEGIN_Y+counter, SIGNAL_COLOR_CH1);
//
//				//Limpio el/los pixel/es siguientes
//				if(counter == UPDATE_LENGHT-1){
//					LCD_PutPixel(X_AXIS_BEGIN_X+buffer[0], Y_AXIS_BEGIN_Y, WHITE);
//					LCD_PutPixel(X_AXIS_BEGIN_X, Y_AXIS_BEGIN_Y, WHITE);
//				} else{
//					LCD_PutPixel(X_AXIS_BEGIN_X+buffer[counter+1], Y_AXIS_BEGIN_Y+counter+1, BLACK);
//					LCD_PutPixel(X_AXIS_BEGIN_X, Y_AXIS_BEGIN_Y+counter+1, WHITE);
//				}
//				xSemaphoreGive(sem_TFT);
//			}
//		}
//	}
//}
//
//// Tarea encargada de borrar todos los pixeles del canal 2 (cuando cambiamos de modo)
//static void CH2Clean(void *pvParameters){
//	while(1){
//		if(xSemaphoreTake(sem_clean_ch2, portMAX_DELAY) == pdTRUE){
//			uint32_t count = 0;
//			int16_t *buff = (int16_t *) pvParameters;
//
//			// Limpiamos los pixeles
//			for(count = 0; count < UPDATE_LENGHT; count++){
//				LCD_PutPixel(X_AXIS_BEGIN_X+buff[count], Y_AXIS_BEGIN_Y+count, BLACK);
//			}
//
//			// Re-dibujamos los ejes
//			Line(X_AXIS_BEGIN_X, X_AXIS_BEGIN_Y, X_AXIS_BEGIN_X, X_AXIS_BEGIN_Y+X_AXIS_SIZE, COLOR_EJE); // Eje X (tiempo)
//			Line(Y_AXIS_BEGIN_X, Y_AXIS_BEGIN_Y, Y_AXIS_BEGIN_X+Y_AXIS_SIZE, Y_AXIS_BEGIN_Y, COLOR_EJE); // Eje Y (intensidad de señal)
//		}
//	}
//}
//
//// Tarea encargada de actuailzar la señal que dibujamos (CANAL 2)
//static void SignalUpdateCH2(void *pvParameters) {
//	static int16_t buffer[UPDATE_LENGHT];
//	static uint32_t counter = 0;
//	static int16_t buff_datos[10];
//	static uint16_t prom_count = 0;
//
//	xTaskCreate(CH2Clean, (signed char *) "CH2CleanTask",
//				configMINIMAL_STACK_SIZE, (void *) buffer, (tskIDLE_PRIORITY + 3UL),
//				(xTaskHandle *) NULL);
//
//	while(1){
//		// Si hay un dato nuevo en la cola
//		if(xQueueReceive(colaADC1, buff_datos+prom_count, portMAX_DELAY) == pdTRUE){
//			prom_count++;
//
//			// Cuando tomamos 10 muestras realizamos el promedio y ponemos el pixel correspondiente
//			if(prom_count == 10){
//				uint8_t i;
//				int32_t promedio = 0;
//				int16_t cuenta;
//
//				prom_count = 0;
//
//				// Promedio los 10 datos
//				for(i = 0; i < 10; i++){
//					promedio = promedio + buff_datos[i];
//				}
//				promedio /= 10;
//
//				// Resto offset
//				promedio -= 4096/2;
//				cuenta = (promedio * SIGNAL_MAX) / 4096;
//
//				// Me fijo por donde esta la base de tiempos
//				xQueuePeek(cola_base_tiempo, &counter, 0); // Se supone que siempre tiene que tener un valor
//				buffer[counter] = cuenta;
//
//				xSemaphoreTake(sem_TFT, portMAX_DELAY);
//				//Pongo el pixel actual
//				LCD_PutPixel(X_AXIS_BEGIN_X+cuenta, Y_AXIS_BEGIN_Y+counter, SIGNAL_COLOR_CH2);
//
//				//Limpio el/los pixel/es siguientes
//				if(counter == UPDATE_LENGHT-1){
//					LCD_PutPixel(X_AXIS_BEGIN_X+buffer[0], Y_AXIS_BEGIN_Y, WHITE);
//					LCD_PutPixel(X_AXIS_BEGIN_X, Y_AXIS_BEGIN_Y, WHITE);
//				} else{
//					LCD_PutPixel(X_AXIS_BEGIN_X+buffer[counter+1], Y_AXIS_BEGIN_Y+counter+1, BLACK);
//					LCD_PutPixel(X_AXIS_BEGIN_X, Y_AXIS_BEGIN_Y+counter+1, WHITE);
//				}
//				xSemaphoreGive(sem_TFT);
//			}
//		}
//	}
//}
//
//// Tarea encargada de borrar todos los pixeles del canal 3 (cuando cambiamos de modo)
//static void CH3Clean(void *pvParameters){
//	while(1){
//		if(xSemaphoreTake(sem_clean_ch3, portMAX_DELAY) == pdTRUE){
//			uint32_t count = 0;
//			int16_t *buff = (int16_t *) pvParameters;
//
//			// Limpiamos los pixeles
//			for(count = 0; count < UPDATE_LENGHT; count++){
//				LCD_PutPixel(X_AXIS_BEGIN_X+buff[count], Y_AXIS_BEGIN_Y+count, BLACK);
//			}
//
//			// Re-dibujamos los ejes
//			Line(X_AXIS_BEGIN_X, X_AXIS_BEGIN_Y, X_AXIS_BEGIN_X, X_AXIS_BEGIN_Y+X_AXIS_SIZE, COLOR_EJE); // Eje X (tiempo)
//			Line(Y_AXIS_BEGIN_X, Y_AXIS_BEGIN_Y, Y_AXIS_BEGIN_X+Y_AXIS_SIZE, Y_AXIS_BEGIN_Y, COLOR_EJE); // Eje Y (intensidad de señal)
//		}
//	}
//}
//
//// Tarea encargada de actuailzar la señal que dibujamos (CANAL 3)
//static void SignalUpdateCH3(void *pvParameters) {
//	static int16_t buffer[UPDATE_LENGHT];
//	static uint32_t counter = 0;
//	static int16_t buff_datos[10];
//	static uint16_t prom_count = 0;
//
//	xTaskCreate(CH3Clean, (signed char *) "CH3CleanTask",
//				configMINIMAL_STACK_SIZE, (void *) buffer, (tskIDLE_PRIORITY + 3UL),
//				(xTaskHandle *) NULL);
//
//	while(1){
//		// Si hay un dato nuevo en la cola
//		if(xQueueReceive(colaADC2, buff_datos+prom_count, portMAX_DELAY) == pdTRUE){
//			prom_count++;
//
//			// Cuando tomamos 10 muestras realizamos el promedio y ponemos el pixel correspondiente
//			if(prom_count == 10){
//				uint8_t i;
//				int32_t promedio = 0;
//				int16_t cuenta;
//
//				prom_count = 0;
//
//				// Promedio los 10 datos
//				for(i = 0; i < 10; i++){
//					promedio = promedio + buff_datos[i];
//				}
//				promedio /= 10;
//
//				// Resto offset
//				promedio -= 4096/2;
//				cuenta = (promedio * SIGNAL_MAX) / 4096;
//
//				// Me fijo por donde esta la base de tiempos
//				xQueuePeek(cola_base_tiempo, &counter, 0); // Se supone que siempre tiene que tener un valor
//				buffer[counter] = cuenta;
//
//				xSemaphoreTake(sem_TFT, portMAX_DELAY);
//				//Pongo el pixel actual
//				LCD_PutPixel(X_AXIS_BEGIN_X+cuenta, Y_AXIS_BEGIN_Y+counter, SIGNAL_COLOR_CH3);
//
//				//Limpio el/los pixel/es siguientes
//				if(counter == UPDATE_LENGHT-1){
//					LCD_PutPixel(X_AXIS_BEGIN_X+buffer[0], Y_AXIS_BEGIN_Y, WHITE);
//					LCD_PutPixel(X_AXIS_BEGIN_X, Y_AXIS_BEGIN_Y, WHITE);
//				} else{
//					LCD_PutPixel(X_AXIS_BEGIN_X+buffer[counter+1], Y_AXIS_BEGIN_Y+counter+1, BLACK);
//					LCD_PutPixel(X_AXIS_BEGIN_X, Y_AXIS_BEGIN_Y+counter+1, WHITE);
//				}
//				xSemaphoreGive(sem_TFT);
//			}
//		}
//	}
//}
