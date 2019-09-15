/*
 * SD_AJS.c
 *
 *  Created on: 12 de jun. de 2016
 *      Author: Augusto
 */

#include "FreeRTOS.h"
#include "task.h"
#include "SD_AJS.h"
#include "SSP.h"
#include "diskio.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

/* The sector size is fixed to 512bytes in most applications. */
#define SECTOR_SIZE 512

/* Global variables */
static uint8_t inicializada = 0;
uint8_t CardType;          /* card type */
CARDCONFIG CardConfig;      /* Card configuration */

extern xQueueHandle cola_SSP_RX;
extern xQueueHandle cola_SSP_TX;

void putDataSSP(uint8_t data){
	uint8_t aux;

	xQueueSendToBack(cola_SSP_TX, &data, portMAX_DELAY); // No deberia quedarse nunca aca
	NVIC_EnableIRQ(SSP1_IRQn);

	// Descartamos el dato que llego mientras yo escribia
	xQueueReceive(cola_SSP_RX, &aux, portMAX_DELAY);
}

void getDataSSP(uint8_t *data){
	uint8_t aux = 0xFF;

	// Mandamos 0xFF para recibir respuesta
	xQueueSendToBack(cola_SSP_TX, &aux, portMAX_DELAY); // No deberia quedarse nunca aca
	NVIC_EnableIRQ(SSP1_IRQn);

	xQueueReceive(cola_SSP_RX, data, portMAX_DELAY); // No deberia quedarse nunca aca
}

void SendSDCMD(uint8_t cmd, uint32_t arg, uint8_t *buf, uint32_t lenght, uint8_t *R1){
	uint8_t crc = 1; // Dummy CRC, en SPI la SD no lo revisa
	uint32_t i;

    if(cmd == GO_IDLE_STATE)
    	crc = 0x95; // Para este comando hay que mandar un CRC valido

    if(cmd == SEND_IF_COND)
    	crc = 0x87;


    i = uxQueueMessagesWaiting(cola_SSP_RX); // Siempre deberia dar 0, es para temas de debug...

    putDataSSP(cmd | 0x40);

    for(i = 0; i < 4; i++){
    	putDataSSP(arg >> (24 - i*8));
    }

    putDataSSP(crc);

    do{
    	getDataSSP(R1);
    }while(*R1 >= 0xFF);

    if(buf != NULL){
    	for(i = 0; i < lenght; i++){
    		getDataSSP(buf + i);
    	}
    }
}

void SD_RecvDataBlock (uint8_t *buf, uint32_t len){
	uint8_t ans;
	uint32_t i;

	do{
		getDataSSP(&ans);
	}while(ans != 0xFE);

	for(i = 0; i < len; i++){
		getDataSSP(buf + i);
	}

	// Descarto el CRC (2 bytes)
	getDataSSP(&ans);
	getDataSSP(&ans);
}

uint8_t SD_SendDataBlock (const uint8_t *buf, uint8_t tkn, uint32_t len){
	uint32_t i;
	uint8_t ans;

	putDataSSP(tkn);

	for(i = 0; i < len; i++){
		putDataSSP(buf[i]);
	}

	// Descarto el CRC (2 bytes)
	putDataSSP(0xFF);
	putDataSSP(0xFF);

	do{
		getDataSSP(&ans);
	}while(ans == 0xFF);

	if((ans & 0x0F) == 0x05){
		do{
			getDataSSP(&ans);
		}while(ans != 0xFF);
		return SD_SEND_B_SUCCESS;
	}else
		return SD_SEND_B_FAILURE;
}

uint8_t SD_Init(void){
	uint32_t i;
	uint8_t R1, R7[4], R3[4], ans;

	if(inicializada)
		return SD_INIT_SUCCESS;

	vTaskDelay(10/portTICK_RATE_MS);

	// CS = HIGH
	SD_DeSelect();

	for(i = 0; i < 11; i++){
		// Dummy clocks
		putDataSSP(0xFF);
	}

	// Limpio la FIFO Rx
	Chip_SSP_Int_FlushData(LPC_SSP1);
	do{
		xQueueReceive(cola_SSP_RX, &ans, 0);
	}while(uxQueueMessagesWaiting(cola_SSP_RX));

	SD_Select();
	vTaskDelay(1/portTICK_RATE_MS);

	SendSDCMD(GO_IDLE_STATE, 0, NULL, 0, &R1);

	if(!(R1 == 0x00 || R1 == 0x01)){
		SD_DeSelect();
		return SD_INIT_FAILURE;
	}

	SendSDCMD(SEND_IF_COND, 0x1AA, R7, 4, &R1);

	if(!(R1 == 0x01 || R1 == 0x00)){
		do{
			SendSDCMD(SD_SEND_OP_COND, 0, NULL, 0, &R1);
		}while(R1 == 0x01);

		if(R1 == 0x00){
			CardType = CARDTYPE_SDV1;
			SD_DeSelect();
			inicializada = 1;
			return SD_INIT_SUCCESS;
		} else{
			do{
				SendSDCMD(SEND_OP_COND, 0, NULL, 0, &R1);
			}while(R1 == 0x01);

			if(R1 == 0x00){
				CardType = CARDTYPE_MMC;
				SD_DeSelect();
				inicializada = 1;
				return SD_INIT_SUCCESS;
			} else{
				SD_DeSelect();
				return SD_INIT_FAILURE;
			}
		}
	}else{
		if(R7[2] == 0x01 && R7[3] == 0xAA){
			do{
				SendSDCMD(APP_CMD, 0, NULL, 0, &R1);

				if( R1 == 0x01 || R1 == 0x00){
					SendSDCMD(SD_SEND_OP_COND, 0x40000000, NULL, 0, &R1);
				} else{
					SD_DeSelect();
					return SD_INIT_FAILURE;
				}

			}while(R1 != 0x00);

			SendSDCMD(READ_OCR, 0, R3, 4, &R1);

			if(R1 == 0x00){
				// R3 me dice si es BLOCK ADDRESSED o BYTE ADDRESSED
				if(R3[0] & 0x40){
					CardType = CARDTYPE_SDV2_HC;
					if(SD_ReadConfiguration() == SD_CONFIG_SUCCESS){
						SD_DeSelect();
						inicializada = 1;
						return SD_INIT_SUCCESS;
					} else{
						SD_DeSelect();
						return SD_INIT_FAILURE;
					}
				} else{
					CardType = CARDTYPE_SDV2_SC;


					SendSDCMD(SET_BLOCKLEN, 0x200, NULL, 0, &R1);

					if(R1 == 0x00){
						SD_DeSelect();
						inicializada = 1;
						return SD_INIT_SUCCESS;
					} else{
						SD_DeSelect();
						return SD_INIT_FAILURE;
					}
				}
			} else{
				SD_DeSelect();
				return SD_INIT_FAILURE;
			}
		} else{
			SD_DeSelect();
			return SD_INIT_FAILURE; // ERROR INICIALIZANDO LA SD
		}
	}

	SD_DeSelect();
	return SD_INIT_FAILURE; // <- No deberia llegar nunca aca.
}

uint8_t SD_ReadConfiguration (void){
	uint8_t R1;
	uint8_t aux, buf[64];

	SD_Select();

	SendSDCMD(READ_OCR, 0, CardConfig.ocr, 4, &R1);

	if(R1 == 0x00){
		SendSDCMD(SEND_CID, 0, NULL, 0, &R1);

		if(R1 == 0x00){
			SD_RecvDataBlock(CardConfig.cid, 16);

			SendSDCMD(SEND_CSD, 0, NULL, 0, &R1);

			if(R1 == 0x00){
				SD_RecvDataBlock(CardConfig.csd, 16);

			    CardConfig.sectorsize = 512;

			    if (((CardConfig.csd[0]>>6) & 0x3) == 0x1) /* CSD V2.0 (for High/eXtended Capacity) */
			    {
			    	uint32_t c_size;

			        /* Read C_SIZE */
			        c_size =  (((uint32_t)CardConfig.csd[7]<<16) + ((uint32_t)CardConfig.csd[8]<<8) + CardConfig.csd[9]) & 0x3FFFFF;
			        /* Calculate sector count */
			       CardConfig.sectorcnt = (c_size + 1) * 1024;

			    } else   /* CSD V1.0 (for Standard Capacity) */
			    {
			    	uint32_t c_size, c_size_mult, read_bl_len;

			        /* C_SIZE */
			        c_size = (((uint32_t)(CardConfig.csd[6]&0x3)<<10) + ((uint32_t)CardConfig.csd[7]<<2) + (CardConfig.csd[8]>>6)) & 0xFFF;
			        /* C_SIZE_MUTE */
			        c_size_mult = ((CardConfig.csd[9]&0x3)<<1) + ((CardConfig.csd[10]&0x80)>>7);
			        /* READ_BL_LEN */
			        read_bl_len = CardConfig.csd[5] & 0xF;
			        /* sector count = BLOCKNR*BLOCK_LEN/512, we manually set SECTOR_SIZE to 512*/
			        //CardConfig.sectorcnt = (c_size+1)*(1<<read_bl_len) * (1<<(c_size_mult+2)) / 512;
			        CardConfig.sectorcnt = (c_size+1) << (read_bl_len + c_size_mult - 7);
			    }

				switch (CardType){
				case CARDTYPE_SDV2_HC:
				case CARDTYPE_SDV2_SC:{
					SendSDCMD(APP_CMD, 0, NULL, 0, &R1);

					if(R1 == 0x00){
						SendSDCMD(SD_STATUS, 0, &aux, 1, &R1);

						if(R1 == 0x00){
							SD_RecvDataBlock(buf, 64);

							CardConfig.blocksize = 16UL << (buf[10] >> 4); /* Calculate block size based on AU size */
						} else{
							SD_DeSelect();
							return SD_CONFIG_FAILURE;
						}
					} else{
						SD_DeSelect();
						return SD_CONFIG_FAILURE;
					}


					break;
				}
				case CARDTYPE_MMC:{
					//CardConfig.blocksize = ((uint16_t)((CardConfig.csd[10] & 124) >> 2) + 1) * (((CardConfig.csd[11] & 3) << 3) + ((CardConfig.csd[11] & 224) >> 5) + 1);
					CardConfig.blocksize = ((uint16_t)((CardConfig.csd[10] & 124) >> 2) + 1) * (((CardConfig.csd[10] & 3) << 3) + ((CardConfig.csd[11] & 224) >> 5) + 1);

					break;
				}
				case CARDTYPE_SDV1:{
					CardConfig.blocksize = (((CardConfig.csd[10] & 63) << 1) + ((uint16_t)(CardConfig.csd[11] & 128) >> 7) + 1) << ((CardConfig.csd[13] >> 6) - 1);

					break;
				}
				default:{
					SD_DeSelect();
					return SD_CONFIG_FAILURE;
					break;
				}
				}

				SD_DeSelect();
				return SD_CONFIG_SUCCESS;
			} else{
				SD_DeSelect();
				return SD_CONFIG_FAILURE;
			}
		} else{
			SD_DeSelect();
			return SD_CONFIG_FAILURE;
		}
	} else{
		SD_DeSelect();
		return SD_CONFIG_FAILURE;
	}

	SD_DeSelect();
	return SD_CONFIG_FAILURE; // <-- No deberia llegar nunca aca.
}

uint8_t SD_ReadSector (uint32_t sect, uint8_t *buf, uint32_t cnt){
	uint8_t R1;
	uint32_t i;

	/* Convert sector-based address to byte-based address for non SDHC */
	if (CardType != CARDTYPE_SDV2_HC) sect <<= 9;

	SD_Select();

	if(cnt > 1){ // Multiple sector
		SendSDCMD(READ_MULTIPLE_BLOCK, sect, NULL, 0, &R1);

		if(R1 == 0xFC){
			for(i = 0; i < cnt; i++){
				SD_RecvDataBlock(buf + (i)*SECTOR_SIZE, SECTOR_SIZE);
			}

			SendSDCMD(STOP_TRANSMISSION, 0, NULL, 0, &R1);

			if(R1 == 0x00){
				SD_DeSelect();
				return SD_RS_SUCCESS;
			} else{
				SD_DeSelect();
				return SD_RS_FAILURE;
			}
		} else{
			SD_DeSelect();
			return SD_RS_FAILURE;
		}
	} else{ // Single sector
		SendSDCMD(READ_SINGLE_BLOCK, sect, NULL, 0, &R1);

		if(R1 == 0x00){
			SD_RecvDataBlock(buf, SECTOR_SIZE);

			SD_DeSelect();
			return SD_RS_SUCCESS;
		} else{
			SD_DeSelect();
			return SD_RS_FAILURE;
		}
	}
}

uint8_t SD_WriteSector (uint32_t sect, const uint8_t *buf, uint32_t cnt){
	uint8_t R1;
	uint32_t i;

    /* Convert sector-based address to byte-based address for non SDHC */
    if (CardType != CARDTYPE_SDV2_HC) sect <<= 9;

    SD_Select();

    if(cnt > 1){ // Multiple sector
    	SendSDCMD(WRITE_MULTIPLE_BLOCK, sect, NULL, 0, &R1);

    	if(R1 == 0x00){
    		for(i = 0; i < cnt; i++){
    			SD_SendDataBlock(buf + (i)*SECTOR_SIZE, 0xFC, SECTOR_SIZE);
    		}

    		putDataSSP(STOP_TRANSMISSION);
    		putDataSSP(0xFF);

    		R1 = 0x00;
    		while(R1 != 0xFF){
    			getDataSSP(&R1);
    		}

			SD_DeSelect();
			return SD_WS_SUCCESS;
    	} else{
    		SD_DeSelect();
    		return SD_WS_FAILURE;
    	}
    } else{ // Single sector
    	SendSDCMD(WRITE_SINGLE_BLOCK, sect, NULL, 0, &R1);

    	i = uxQueueMessagesWaiting(cola_SSP_RX); // Siempre deberia dar 0, es para temas de debug...

    	putDataSSP(0xFF);
    	putDataSSP(0xFF);

    	if(R1 == 0x00){
    		 SD_SendDataBlock(buf, 0xFE, SECTOR_SIZE); // Asumo que no puede salir mal
    		 SD_DeSelect();
    		 return SD_WS_SUCCESS;
    	} else{
    		SD_DeSelect();
    		return SD_WS_FAILURE;
    	}
    }
}

uint8_t	SD_IsInitialized(void){
	return inicializada;
}
