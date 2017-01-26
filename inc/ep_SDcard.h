/*
 * ep_sdcard.h
 *
 *  Created on: Jan 24, 2017
 *      Author: aleboyer
 */

#ifndef INC_EP_SDCARD_H_
#define INC_EP_SDCARD_H_


/* SD card initialisation */


#define TEST_FILENAME "data_test.txt"

FIL fsrc;					// File objects
FATFS Fatfs;				// File system specific
FRESULT res;				// FatFs function common result code
FRESULT res_mount;			// FatFs function mount result code
UINT br, bw;				// File read/write count
DSTATUS resCard;			// SDcard status


//DWORD get_fattime(void) // i think this needs to be defined for some reason?
//{
//  return (28 << 25) | (2 << 21) | (1 << 16);
//}

void writeSD(void);
void initSD(void);


#endif /* INC_EP_SDCARD_H_ */
