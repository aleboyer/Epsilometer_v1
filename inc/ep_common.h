/*
 * ep_common.h
 *
 *  Created on: Jun 20, 2016
 *      Author: leumas64
 *      modified Dec 22 2016 A LeBoyer
 */

#ifndef EP_COMMON_H_
#define EP_COMMON_H_


/******************************************************************************/
/***************************** Include Files **********************************/
/******************************************************************************/

#include "stdint.h"
#include "stdlib.h"

/*************************************************************************/
/* define epsilometer parameters info to be put into the header data   */
/*************************************************************************/

//#define CHANNELS {&FP07_1, &SHR_1, &SHR_2, &CON_1, &AXL_X, &AXL_Y,&FP07_2, &AXL_Z}


typedef struct epsiSetup {
	uint32_t 	coreClock;
	uint32_t 	numSensor;
	uint32_t	maxSamples;
	uint32_t    bytePerchannel;
	uint32_t    pinInterupt;
	uint32_t	blockSize;
	uint32_t	sampleFreq;
	uint32_t    MclockFreq;
	uint32_t    timer1Sync;
	uint32_t	spiBaudrate;
	uint32_t	usartBaudrate;
	uint32_t	timeStampFlag;
} epsiSetup, *epsiSetupPtr;


// blockSize size of streamed data in samples number
//

#define EPSI_SETUP_DEFAULT       \
{20000000,                       \
8,                               \
3,                               \
3,								 \
0x200,                           \
1,                               \
60,                              \
614400,                          \
0x13,                            \
1000000,                         \
460800,                          \
1                                \
}


/*************************************************************************/
/**   define global external values  **********************************************/
/*************************************************************************/


uint32_t numChannel;        // 2 temp 2 shear 1 cond 3 accelerometer
uint32_t byteSample;        // numbe of bytes per Sample
uint32_t bufferSize;        // numChannel*maxSamples


/*************************************************************************/
/* define circular buffer for the samples.
 * with timeStampFlag 1 One sample is numChannel * byteSample + 1 byteSample long
 * with timeStampFlag 0 One sample is numChannel * byteSample long
 * the buffer is maxSample long
 */
/*************************************************************************/

volatile uint8_t * dataBuffer;
extern volatile uint32_t pendingSamples; // counter in the background IRQ
extern volatile uint32_t txSentBytes;    //
extern epsiSetupPtr boardSetup_ptr;
volatile uint32_t gulclockset;
extern volatile uint32_t flagSync;
#endif /* EP_COMMON_H_ */
