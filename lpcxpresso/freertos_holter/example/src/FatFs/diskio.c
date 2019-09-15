/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
#include "SD_AJS.h"

extern uint8_t CardType;          /* card type */
extern CARDCONFIG CardConfig;      /* Card configuration */

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS result = STA_NOINIT;

	if(!pdrv){
		// Siempre usamos 1 solo disco en nuestra aplicacion
		if(SD_IsInitialized()){
			result = 0;
		} else{
			result = STA_NOINIT;
		}
	} else{
		result = STA_NOINIT;
	}
	return result;
}

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS result = STA_NOINIT;

	if(!pdrv){
		// Siempre usamos un unico disco en nuestra aplicacion
		result = SD_Init();
		if(result == SD_INIT_SUCCESS){
			result = 0;
		} else{
			result = STA_NOINIT;
		}
	}
	return result;
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT result = RES_PARERR;

	if(!pdrv){
		// Siempre usamos un solo disco en nuestra aplicacion
		if(SD_ReadSector(sector, buff, count) == SD_RS_SUCCESS){
			// Leyo bien
			result = RES_OK;
		} else{
			// Hubo algun error
			result = RES_ERROR;
		}
	} else{
		result = RES_PARERR;
	}

	return result;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT result;

	if(!pdrv){
		if(SD_WriteSector(sector, buff, count) == SD_WS_SUCCESS){
			// Escribio bien
			result = RES_OK;
		} else{
			result = RES_ERROR;
		}
	} else{
		result = RES_ERROR;
	}

	return result;
}

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;
	BYTE n, *ptr = buff;

	if(!pdrv){
		switch (cmd) {
		case CTRL_SYNC :		/* Make sure that no pending write process */
			Chip_SSP_Int_FlushData(LPC_SSP1);

			res = RES_OK;

			break;

		case GET_SECTOR_COUNT :	/* Get number of sectors on the disk (DWORD) */
			*(DWORD*)buff = CardConfig.sectorcnt;
			res = RES_OK;
			break;

		case GET_SECTOR_SIZE :	/* Get R/W sector size (WORD) */
			*(WORD*)buff = CardConfig.sectorsize;	//512;
			res = RES_OK;
			break;

		case GET_BLOCK_SIZE :	/* Get erase block size in unit of sector (DWORD) */
			*(DWORD*)buff = CardConfig.blocksize;
			res = RES_OK;
			break;

		case MMC_GET_TYPE :		/* Get card type flags (1 byte) */
			*ptr = CardType;
			res = RES_OK;
			break;

		case MMC_GET_CSD :		/* Receive CSD as a data block (16 bytes) */
			for (n=0;n<16;n++)
				*(ptr+n) = CardConfig.csd[n];
			res = RES_OK;
			break;

		case MMC_GET_CID :		/* Receive CID as a data block (16 bytes) */
			for (n=0;n<16;n++)
				*(ptr+n) = CardConfig.cid[n];
			res = RES_OK;
			break;

		case MMC_GET_OCR :		/* Receive OCR as an R3 resp (4 bytes) */
			for (n=0;n<4;n++)
				*(ptr+n) = CardConfig.ocr[n];
			res = RES_OK;
			break;

		case MMC_GET_SDSTAT :	/* Receive SD status as a data block (64 bytes) */
			for (n=0;n<64;n++)
	            *(ptr+n) = CardConfig.status[n];
	        res = RES_OK;
			break;

		default:
			res = RES_PARERR;
		}
	}

	return res;
}

DWORD get_fattime (void){
	RTC_TIME_T aux;
	DWORD result = 0;

	Chip_RTC_GetFullTime(LPC_RTC, &aux);

	result |= (aux.time[RTC_TIMETYPE_SECOND] & 0b11111);
	result |= (aux.time[RTC_TIMETYPE_MINUTE] & 0b111111) << 5;
	result |= (aux.time[RTC_TIMETYPE_HOUR] & 0b11111) << 11;
	result |= (aux.time[RTC_TIMETYPE_DAYOFMONTH] & 0b11111) << 16;
	result |= (aux.time[RTC_TIMETYPE_MONTH] & 0b1111) << 21;
	result |= (aux.time[RTC_TIMETYPE_YEAR] & 0b1111111) << 25;

	return result;
}
