/*
 * epsilometer_SDcard.c
 *
 *  Created on: Jan 24, 2017
 *      Author: aleboyer
 */

/*energy micro library */
#include "em_gpio.h"

/* SD card library */
#include "microsd.h"
#include "ff.h"
#include "diskio.h"

/*local library */
#include "ep_SDcard.h"


/****************************************************************
 * initialize SD card
 * ***************************************************************/
void initSD(void){

	MICROSD_Init(); //Initialize MicroSD driver
	resCard = disk_initialize(0); //Check micro-SD card status
	switch (resCard) //Use onboard LED's to indicate error.
	{
	case STA_NOINIT: // Drive not initialized
		break;
	case STA_NODISK: // No medium in the drive
		break;
	case STA_PROTECT: // Write protected
		break;
	default:
		break;
	}

	// Initialize filesystem
	res_mount=f_mount(0, &Fatfs,1);
	// Open  the file for write
	res = f_open(&fsrc, TEST_FILENAME, FA_WRITE);
	if (res != FR_OK) {
		//  If file does not exist create it
		res = f_open(&fsrc, TEST_FILENAME, FA_CREATE_ALWAYS | FA_WRITE);
	}
	res = f_lseek(&fsrc, 0);


}


void writeSD(void){
	uint16_t written = 0;

	sd_send_command();
	if (res == FR_OK) {
		f_write(&fsrc, 0x1e, 1, &written);
		f_sync(&fsrc);
		written=0;
	}
		/*	res = f_lseek(&fsrc, 0);  */
		if (res == FR_OK) {
//	 	    f_write(&fsrc, &data, 3, &written);
	 	    f_sync(&fsrc);
	 	    written=0;
		    }

//f_close(&fsrc);
//res = f_open(&fsrc, TEST_FILENAME, FA_WRITE);


}




