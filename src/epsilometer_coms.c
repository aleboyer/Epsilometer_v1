/*
 * proto Epsilometer_coms.c
 *
 *  Created on: May 24, 2016
 *      Author: leumas64
 */

#include "ep_coms.h"
#include "ep_common.h"
#include "em_usart.h"
#include "em_cmu.h"
#include "efm32wg_uart.h"



/***************************************************************************//**
 * @brief Set up the USART1 communication port
 * @Detail
 * @return
 * @Note  written by leumas64
*******************************************************************************/

void UART_Setup() {
	/*UART1 shit*/
	CMU_ClockEnable(cmuClock_USART1, true); // Enable clock for USART1 module
	GPIO_PinModeSet(gpioPortD, 7, gpioModePushPull, 1); // TX
	GPIO_PinModeSet(gpioPortD, 6, gpioModeInput, 0); // RX

	USART_InitAsync_TypeDef usartInitUSART1 = {
		.enable = usartDisable, 					// Initially disabled
		.refFreq = 0,								// configured reference frequency
		.baudrate = boardSetup_ptr->usartBaudrate, 				    // Baud rate defined in common.h
		.oversampling = usartOVS16, 				// overSampling rate x16
		.databits = USART_FRAME_DATABITS_EIGHT, 	// 8-bit frames
		.parity = USART_FRAME_PARITY_NONE,			// parity - none
		.stopbits = USART_FRAME_STOPBITS_ONE,		// 1 stop bit
	};

	/*Initialize UART registers*/
	USART_InitAsync(USART1, &usartInitUSART1);

	USART1 -> ROUTE = USART_ROUTE_RXPEN | USART_ROUTE_TXPEN
			| USART_ROUTE_LOCATION_LOC2;

	/* Inform NVIC of IRQ changes*/
	NVIC_ClearPendingIRQ(USART1_TX_IRQn);
	NVIC_EnableIRQ(USART1_TX_IRQn);

	USART_Enable(USART1, usartEnable);
	NVIC_SetPriority(USART1_TX_IRQn, 1);
}


/***************************************************************************//**
 * @brief send 3 bytes through TX
 * @Detail send 3 bytes block when TX buffer (2 bytes for 2 FIFO level + 1 bytes for shift register)
 * @return
 * @Note
*******************************************************************************/


void USART1_TX_IRQHandler(void)
{
//TODO change to state machine
	dataLen = pendingSamples*byteSample-txSentBytes;
	switch (dataLen){
		case 0:
			USART_IntDisable(USART1, USART_IEN_TXBL);
			USART_Tx(USART1, 0x1e);
			break;
		case 1:
			USART_Tx(USART1, dataBuffer[txSentBytes % bufferSize]);
			txSentBytes++;
			break;
		default:
			for(int i=0;i<2;i++){
				/* Transmit pending character */
				USART_Tx(USART1, dataBuffer[txSentBytes % bufferSize]);
				txSentBytes++;
			}
			break;
	}

}





