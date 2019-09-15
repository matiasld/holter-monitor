/*
 * SD_AJS.h
 *
 *  Created on: 12 de jun. de 2016
 *      Author: Augusto
 *  @note: Header con las funciones y definiciones para manejar una tarjeta SD/MMC.
 *  Estas funciones estan pensadas como maquina de estado y deberan ser llamadas hasta que retornen
 *  el valor que indique que las mismas han terminado, el llamado erroneo de las funciones puede
 *  resultar en un malfuncionamiento del programa o la SD.
 */

#ifndef SD_AJS_H_
#define SD_AJS_H_

#include "chip.h"
#include "SSP.h"

/* Comandos para la SD */
#define GO_IDLE_STATE           0
#define SEND_OP_COND            1
#define SWITCH_FUNC             6
#define SEND_IF_COND            8
#define SEND_CSD                9
#define SEND_CID                10
#define STOP_TRANSMISSION       12
#define SEND_STATUS             13
#define SET_BLOCKLEN            16
#define READ_SINGLE_BLOCK       17
#define READ_MULTIPLE_BLOCK     18
#define WRITE_SINGLE_BLOCK      24
#define WRITE_MULTIPLE_BLOCK    25
#define APP_CMD                 55
#define READ_OCR                58
#define CRC_ON_OFF              59

/* Application specific commands supported by SD.
All these commands shall be preceded with APP_CMD (CMD55). */
#define SD_STATUS               13
#define SD_SEND_OP_COND         41

/* R1 response bit flag definition */
#define R1_NO_ERROR         0x00
#define R1_IN_IDLE_STATE    0x01
#define R1_ERASE_RESET      0x02
#define R1_ILLEGAL_CMD      0x04
#define R1_COM_CRC_ERROR    0x08
#define R1_ERASE_SEQ_ERROR  0x10
#define R1_ADDRESS_ERROR    0x20
#define R1_PARA_ERROR       0x40
#define R1_MASK             0x7F

/*
 * Defines para la maquina de estados de SendSDCMD
 */
typedef enum STATE_MACHINE_SENDSD {
	SD_SEND_CMD_SENDING_CMD = 0,
	SD_SEND_CMD_SENDING_ARG,
	SD_SEND_CMD_SENDING_BUF,
	SD_SEND_CMD_SENDING_CRC,
	SD_SEND_CMD_RECVING_ANS_R1,
	SD_SEND_CMD_RECVING_ANS_ARG,
	SD_SEND_CMD_ENDING
} SD_CMD_MACHINE_T;

#define SD_CMD_SENT		1
#define SD_CMD_SENDING	0

/*
 * Defines para la maquina de estados de SendSDCMD
 */
typedef enum STATE_MACHINE_SDINIT {
	SD_INIT_SET_DELAY = 0,
	SD_INIT_WAIT_DELAY,
	SD_INIT_DUMMY_CLOCKS,
	SD_INIT_CLEAN_RX_FIFO_1,
	SD_INIT_SENDING_CMD0,
	SD_INIT_RCVING_ANS_CMD0,
	SD_INIT_SENDING_CMD8,
	SD_INIT_RCVING_ANS_CMD8,
	SD_INIT_SENDING_ACMD41_4,
	SD_INIT_SENDING_ACMD41_0,
	SD_INIT_SENDING_CMD1,
	SD_INIT_SENDING_CMD58,
	SD_INIT_CHECKING_CCS,
	SD_INIT_SENDING_CMD16,
	SD_INIT_RCVING_ANS_CMD16,
	SD_INIT_FINISH,
	SD_INIT_ERROR
} SD_INIT_MACHINE_T;

#define	SD_INIT_SUCCESS		1
#define SD_INIT_FAILURE		2
#define	SD_INIT_IN_PROGRESS 0

/*
 * Maquina de estados SD_ReadConfiguration
 */
typedef enum STATE_MACHINE_SDCONFIG {
	SD_CONFIG_INITIAL = 0,
	SD_CONFIG_SENDING_CMD58,
	SD_CONFIG_SENDING_CMD10,
	SD_CONFIG_RCVING_CMD10,
	SD_CONFIG_SENDING_CMD9,
	SD_CONFIG_RCVING_CMD9,
	SD_CONFIG_ERASE_BLOCK_SIZE,
	SD_CONFIG_FINISH,
	SD_CONFIG_ERROR
} SD_CONFIG_MACHINE_T;

#define	SD_CONFIG_SUCCESS		1
#define SD_CONFIG_FAILURE		2
#define	SD_CONFIG_IN_PROGRESS 	0

/*
 * Maquina de estados SD_ReadSector
 */
typedef enum STATE_MACHINE_SDRS {
	SD_RS_INIT = 0,
	SD_RS_READING_SINGLE,
	SD_RS_READING_MULTIPLE,
	SD_RS_STOP_CMD,
	SD_RS_FINISH,
	SD_RS_ERROR
} SD_RS_MACHINE_T;

#define	SD_RS_SUCCESS		1
#define SD_RS_FAILURE		2
#define	SD_RS_IN_PROGRESS 	0

/*
 * Maquina de estados SD_WriteSector
 */
typedef enum STATE_MACHINE_SDWS {
	SD_WS_INIT = 0,
	SD_WS_WRITING_SINGLE,
	SD_WS_WRITING_MULTIPLE,
	SD_WS_STOP_CMD,
	SD_WS_FINISH,
	SD_WS_ERROR
} SD_WS_MACHINE_T;

#define	SD_WS_SUCCESS		1
#define SD_WS_FAILURE		2
#define	SD_WS_IN_PROGRESS 	0

/*
 * Maquina de estados SD_RecvDataBlock
 */
typedef enum STATE_MACHINE_RCVB {
	SD_RCV_B_RCVING_TOKEN = 0,
	SD_RCV_B_RCVING_DATA,
	SD_RCV_B_CRC,
	SD_RCV_B_FINISH
} SD_RCV_BLOCK_MACHINE_T;

#define	SD_RCV_B_SUCCESS		1
#define SD_RCV_B_FAILURE		2
#define	SD_RCV_B_IN_PROGRESS 	0

/*
 * Maquina de estados SD_SendDataBlock
 */
typedef enum STATE_MACHINE_SENDB {
	SD_SEND_B_SENDING_TOKEN = 0,
	SD_SEND_B_SENDING_DATA,
	SD_SEND_B_CRC,
	SD_SEND_B_RCVING_ANS,
	SD_SEND_B_WAITING,
	SD_SEND_B_FINISH
} SD_SEND_BLOCK_MACHINE_T;

#define	SD_SEND_B_SUCCESS		1
#define SD_SEND_B_FAILURE		2
#define	SD_SEND_B_IN_PROGRESS 	0

/** Select the card */
#define SD_Select()	  Chip_GPIO_SetPinOutLow(LPC_GPIO, SD_SEL_PINPORT)
/** DeSelect the card */
#define SD_DeSelect() Chip_GPIO_SetPinOutHigh(LPC_GPIO, SD_SEL_PINPORT)

/* Memory card type definitions */
#define CARDTYPE_UNKNOWN        0
#define CARDTYPE_MMC            1   /* MMC */
#define CARDTYPE_SDV1           2   /* V1.x Standard Capacity SD card */
#define CARDTYPE_SDV2_SC        3   /* V2.0 or later Standard Capacity SD card */
#define CARDTYPE_SDV2_HC        4   /* V2.0 or later High/eXtended Capacity SD card */

/* SD/MMC card configuration */
typedef struct tagCARDCONFIG
{
    uint32_t sectorsize;    /* size (in byte) of each sector, fixed to 512bytes */
    uint32_t sectorcnt;     /* total sector number */
    uint32_t blocksize;     /* erase block size in unit of sector */
	uint8_t  ocr[4];		/* OCR */
	uint8_t  cid[16];		/* CID */
	uint8_t  csd[16];		/* CSD */
    uint8_t  status[64];    /* Status */
} CARDCONFIG;

/* Public functions */
/**
 * @brief	Pone un dato en la Tx FIFO cuando puede, manejado por semaforos
 * @param	data 	: Dato a poner en la Tx FIFO
 */
void putDataSSP(uint8_t data);

/**
 * @brief	Inicializa una SD/MMC
 * @return	1 si termino sin errores, 0 si no termino, o el error correspondiente
 */
uint8_t     SD_Init (void);

/**
 * @brief	Lee un sector de la SD
 * @param	sect	: Sector de la SD a leer
 * @param	buf		: Puntero a donde se van a guardar los datos leidos
 * @param   cnt     : Cantidad de sectores a leer
 * @return	1 si leyo bien, 0 si no termino de leer, otros para error
 */
uint8_t     SD_ReadSector (uint32_t sect, uint8_t *buf, uint32_t cnt);

/**
 * @brief	Escribe un sector de la SD
 * @param	sect	: Sector de la SD a escribir
 * @param	buf		: Puntero a donde estan los datos a escribir
 * @param   cnt     : Cantidad de sectores a escribir
 * @return	1 si escribio todo bien, 0 si no termino de escribir, otros para error
 */
uint8_t     SD_WriteSector (uint32_t sect, const uint8_t *buf, uint32_t cnt);

/**
 * @brief	Lee la configuracion de la SD
 * @return	1 si leyo bien, 0 si no termino de leer
 * @note	Guarda la info en la variable global CardConfig
 */
uint8_t     SD_ReadConfiguration (void);

/**
 * @brief	Envia un comando hacia la SD por SPI (SSP1)
 * @param	cmd		: Comando a enviar
 * @param	arg		: Argumento del comando
 * @param   buf		: Puntero al buffer donde se va a guardar la respuesta
 * @param	len		: Cantidad de datos a recibir de la respuesta
 */
void     SendSDCMD (uint8_t cmd, uint32_t arg, uint8_t *buf, uint32_t len, uint8_t *R1);

/**
 * @brief	Recibe un bloque de datos (sin tomar en cuenta el TOKENDATA ni el CRC)
 * @param   buf		: Puntero al buffer donde se va a guardar el bloque
 * @param	len		: Cantidad de datos a recibir del bloque
 */
void     SD_RecvDataBlock (uint8_t *buf, uint32_t len);

/**
 * @brief	Envia un bloque de datos
 * @param   buf		: Puntero al buffer donde estan los datos a enviar
 * @param	len		: Cantidad de datos a enviar
 * @return	SD_DATABLOCK_SENT si lo termino de recibir, SD_DATABLOCK_SENDING si esta en proceso
 */
uint8_t     SD_SendDataBlock (const uint8_t *buf, uint8_t tkn, uint32_t len) ;


uint8_t     SD_WaitForReady (void);

uint8_t		SD_IsInitialized(void);

#endif /* SD_AJS_H_ */
