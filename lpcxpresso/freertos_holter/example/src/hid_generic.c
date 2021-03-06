/*
 * @brief HID generic example's callabck routines
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2013
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#include "board.h"
#include <stdint.h>
#include <string.h>
#include "usbd_rom_api.h"
#include "varios.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

static uint16_t *loopback_report;
static uint8_t buff[64];

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

extern const uint8_t HID_ReportDescriptor[];
extern const uint16_t HID_ReportDescSize;

extern xTaskHandle ModeTask;
extern xTaskHandle DisparoTaskHandle;

extern xSemaphoreHandle sem_disp, sem_modo;

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/*  HID get report callback function. */
static ErrorCode_t HID_GetReport(USBD_HANDLE_T hHid, USB_SETUP_PACKET *pSetup, uint8_t * *pBuffer, uint16_t *plength)
{
	/* ReportID = SetupPacket.wValue.WB.L; */
	switch (pSetup->wValue.WB.H) {
	case HID_REPORT_INPUT:
		memcpy(*pBuffer, buff, 64);
		*plength = 64;
		break;

	case HID_REPORT_OUTPUT:
		return ERR_USBD_STALL;			/* Not Supported */

	case HID_REPORT_FEATURE:
		return ERR_USBD_STALL;			/* Not Supported */
	}
	return LPC_OK;
}

extern xQueueHandle cola_config;

/* HID set report callback function. */
static ErrorCode_t HID_SetReport(USBD_HANDLE_T hHid, USB_SETUP_PACKET *pSetup, uint8_t * *pBuffer, uint16_t length)
{
	/* we will reuse standard EP0Buf */
	if (length == 0) {
		return LPC_OK;
	}

	/* ReportID = SetupPacket.wValue.WB.L; */
	switch (pSetup->wValue.WB.H) {
	case HID_REPORT_INPUT:
		return ERR_USBD_STALL;			/* Not Supported */

	case HID_REPORT_OUTPUT:{
		config_t configActual;
		portBASE_TYPE cambio = pdFALSE;

		// Saco el dato actual
		xQueueReceiveFromISR(cola_config, &configActual, &cambio);

		// Pongo el correcto
		configActual.flag_usb = 1;
		xQueueSendToBackFromISR(cola_config, &configActual, &cambio);
		break;
	}

	case HID_REPORT_FEATURE:
		return ERR_USBD_STALL;			/* Not Supported */
	}
	return LPC_OK;
}

/* HID Interrupt endpoint event handler. */
static ErrorCode_t HID_Ep_Hdlr(USBD_HANDLE_T hUsb, void *data, uint32_t event)
{
	USB_HID_CTRL_T *pHidCtrl = (USB_HID_CTRL_T *) data;
	uint8_t data_in[5]; // 5 Para cubrirnos, para este caso con 1 sobra

	switch (event) {
	case USB_EVT_IN:
		/* last report is successfully sent. Do something... */
		break;

	case USB_EVT_OUT:{
		portBASE_TYPE cambio = pdFALSE, cambio_disp = pdFALSE, cambio_modo = pdFALSE;

		// Leer el dato que llego
		USBD_API->hw->ReadEP(hUsb, pHidCtrl->epout_adr, data_in);

		switch(data_in[0]){
		case 'C':{
			// Comenzar a tomar datos, pero con posibilidad de cambio de modo
			config_t configActual;

			// Saco el dato actual
			xQueueReceiveFromISR(cola_config, &configActual, &cambio);

			// Pongo el correcto
			configActual.flag_usb = 1;
			xQueueSendToBackFromISR(cola_config, &configActual, &cambio);

			break;
		}
		case 'G':{
			// Comenzo la grabacion, restringimos botones de grabacion SD y cambio de modo.
			vTaskSuspend(DisparoTaskHandle);
			vTaskSuspend(ModeTask);

			break;
		}
		case 'D':{
			// Enviar dato
			// Update del endpoint de entrada USB
			USBD_API->hw->WriteEP(hUsb, 0x81, buff, 64);

			break;
		}
		case 'X':{
			// Termino la grabacion, permitimos cambio de modo y grabacion de SD
			// Limpio interrupciones de EINT por si alguien apreto algun boton
			NVIC_ClearPendingIRQ(EINT3_IRQn);
			Chip_GPIOINT_ClearIntStatus(LPC_GPIOINT, 0, (1 << 18) | (1 << 11));
			Chip_GPIOINT_ClearIntStatus(LPC_GPIOINT, 2, 1 << 10);
			Chip_GPIOINT_SetIntFalling(LPC_GPIOINT, 0, (1 << 18) | (1 << 11));
			Chip_GPIOINT_SetIntFalling(LPC_GPIOINT, 2, 1 << 10);
			// Tomo semaforos por si los habia otorgado alguien presionando un boton
			xSemaphoreTakeFromISR(sem_disp, &cambio_disp);
			xSemaphoreTakeFromISR(sem_modo, &cambio_modo);

			// Tambien vuelvo a permitir que se modifique el modo o guardar la SD
			vTaskResume(DisparoTaskHandle);
			vTaskResume(ModeTask);

			break;
		}
		case 'T':{
			// Terminar, cambio flag
			config_t configActual;

			// Saco el dato actual
			xQueueReceiveFromISR(cola_config, &configActual, &cambio);

			// Pongo el correcto
			configActual.flag_usb = 0;
			xQueueSendToBackFromISR(cola_config, &configActual, &cambio);

			break;
		}
		}
	}

		break;
	}
	return LPC_OK;
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/* HID init routine */
ErrorCode_t usb_hid_init(USBD_HANDLE_T hUsb,
						 USB_INTERFACE_DESCRIPTOR *pIntfDesc,
						 uint32_t *mem_base,
						 uint32_t *mem_size)
{
	USBD_HID_INIT_PARAM_T hid_param;
	USB_HID_REPORT_T reports_data[1];
	ErrorCode_t ret = LPC_OK;

	memset((void *) &hid_param, 0, sizeof(USBD_HID_INIT_PARAM_T));
	/* HID paramas */
	hid_param.max_reports = 1;
	/* Init reports_data */
	reports_data[0].len = HID_ReportDescSize;
	reports_data[0].idle_time = 0;
	reports_data[0].desc = (uint8_t *) &HID_ReportDescriptor[0];

	if ((pIntfDesc == 0) || (pIntfDesc->bInterfaceClass != USB_DEVICE_CLASS_HUMAN_INTERFACE)) {
		return ERR_FAILED;
	}

	hid_param.mem_base = *mem_base;
	hid_param.mem_size = *mem_size;
	hid_param.intf_desc = (uint8_t *) pIntfDesc;
	/* user defined functions */
	hid_param.HID_GetReport = HID_GetReport;
	hid_param.HID_SetReport = HID_SetReport;
	hid_param.HID_EpIn_Hdlr  = HID_Ep_Hdlr;
	hid_param.HID_EpOut_Hdlr = HID_Ep_Hdlr;
	hid_param.report_data  = reports_data;

	ret = USBD_API->hid->init(hUsb, &hid_param);
	/* allocate USB accessable memory space for report data */
	loopback_report =  (uint16_t *) hid_param.mem_base;
	hid_param.mem_base += 4;
	hid_param.mem_size += 4;
	/* update memory variables */
	*mem_base = hid_param.mem_base;
	*mem_size = hid_param.mem_size;
	return ret;
}







void ADC_SetEP(uint8_t canal, int16_t *dato){
	uint8_t i;

	switch(canal){
	case 1:
		// Roto los datos
		for(i = 54; i >= 6; i = i - 6){
			buff[i] = buff[i - 6];
			buff[i + 1] = buff[i - 5];
		}

		buff[0] = (*dato) & 0xFF;
		buff[1] = ((*dato) & 0xFF00) >> 8;

		break;
	case 2:
		// Roto los datos
		for(i = 56; i >= 8; i = i - 6){
			buff[i] = buff[i - 6];
			buff[i + 1] = buff[i - 5];
		}

		buff[2] = (*dato) & 0xFF;
		buff[3] = ((*dato) & 0xFF00) >> 8;

		break;
	case 3:
		// Roto los datos
		for(i = 58; i >= 10; i = i - 6){
			buff[i] = buff[i - 6];
			buff[i + 1] = buff[i - 5];
		}

		buff[4] = (*dato) & 0xFF;
		buff[5] = ((*dato) & 0xFF00) >> 8;

		break;
	default:break;
	}
}
