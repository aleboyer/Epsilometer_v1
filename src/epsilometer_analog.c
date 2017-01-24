/***************************************************************************//**
 *   @file   AD7124.c
 *   @brief  Implementation of AD7124 Driver.
 *   @author DNechita (Dan.Nechita@analog.com)
********************************************************************************
 * Copyright 2012(c) Analog Devices, Inc.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  - Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  - Neither the name of Analog Devices, Inc. nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *  - The use of this software may or may not infringe the patent rights
 *    of one or more patent holders.  This license does not release you
 *    from the requirement that you obtain separate licenses from these
 *    patent holders to use this software.
 *  - Use of the software either in source or binary form, must be run
 *    on or directly connected to an Analog Devices Inc. component.
 *
 * THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT,
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL ANALOG DEVICES BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, INTELLECTUAL PROPERTY RIGHTS, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
********************************************************************************
 *   SVN Revision: 903
*******************************************************************************/

/******************************************************************************/
/***************************** Include Files **********************************/
/******************************************************************************/
#include "ep_analog.h"
#include "ep_common.h"

#include "em_usart.h"
#include "em_cmu.h"


void define_ADC_configuration(void){
	AD7124 COMMON_SETUP = AD7124_RESET_DEFAULT;
	COMMON_SETUP.ERROR_EN = AD7124_ERREN_REG_REF_DET_ERR_EN | AD7124_ERREN_REG_SPI_IGNORE_ERR_EN;
	COMMON_SETUP.CONFIG_0 = AD7124_CONFIG_BIPOLAR |
				AD7124_CONFIG_BURNOUT_OFF |
				AD7124_CONFIG_INTREF |
				AD7124_CONFIG_GAIN_1;
	COMMON_SETUP.ADC_CONTROL = AD7124_CTRL_CS_EN(0) |
				AD7124_CTRL_HIG_PWR |
				AD7124_CTRL_CONT_MODE |
				AD7124_CTRL_REF_EN(1) |
				AD7124_CTRL_CLKSEL(2);

	COMMON_SETUP.FILTER_0 = 0x06003C; // 0x3C for 320hz on sinc4 filter

	AD7124 TEMP_SETUP = COMMON_SETUP;
	TEMP_SETUP.CHANNEL_0 = AD7124_CH_EN | AD7124_CH_AINP(0) | AD7124_CH_AINM(1);

	AD7124 SHR_SETUP = COMMON_SETUP;
	SHR_SETUP.CHANNEL_0 = AD7124_CH_EN | AD7124_CH_AINP(0) | AD7124_CH_AINM(1);
	//SHR_SETUP.CONFIG_0 = AD7124_CONFIG_UNIPOLAR;  //sml added this for analog test dev board (with grounded AINM)

	AD7124 ACCL_SETUP = COMMON_SETUP;
	ACCL_SETUP.CHANNEL_0 = AD7124_CH_EN | AD7124_CH_AINP(0) | AD7124_CH_AINM(1); //TODO: GND reference to refin1-
	ACCL_SETUP.CONFIG_0 = AD7124_CONFIG_UNIPOLAR;

	AD7124 COND_SETUP = COMMON_SETUP;
	COND_SETUP.CHANNEL_0 = AD7124_CH_EN | AD7124_CH_AINP(0) | AD7124_CH_AINM(1);

	AD7124 OFF_SETUP = AD7124_RESET_DEFAULT;
	OFF_SETUP.ADC_CONTROL = OFF_SETUP.ADC_CONTROL | AD7124_CTRL_PWRDOWN_MODE;


	uint32_t delay = 0xFFFF;
	while (delay--); // Delay after power on to ensure registers are ready to write
	AD7124_ConfigureDevice(sensors[0], TEMP_SETUP);
	AD7124_ConfigureDevice(sensors[1], TEMP_SETUP);
	AD7124_ConfigureDevice(sensors[2], SHR_SETUP);
	AD7124_ConfigureDevice(sensors[3], SHR_SETUP);
	AD7124_ConfigureDevice(sensors[4], COND_SETUP);
	AD7124_ConfigureDevice(sensors[5], ACCL_SETUP);
	AD7124_ConfigureDevice(sensors[6], ACCL_SETUP);
	AD7124_ConfigureDevice(sensors[7], ACCL_SETUP);
}

void SPI_xfer(uint8_t* writeBuffer, uint8_t* readBuffer, int bytesOut, int bytesIn);

/*
 * CH CFG ->
 * CH Setup ->
 * DIAG ->
 * ADC CTRL ->
 * */

/*******************************************************************************
 * @brief Writes data into a register.
 * @detail
 *
 * @param sensor s -
 * @param registerAddress - Address of the register.
 * @param registerValue - Data value to write.
 * @param bytesNumber - Number of bytes to be written. Must be 3 or less
 *
 * @return none.
*******************************************************************************/
void AD7124_SetRegisterValue(	SensorSpec *s,
								uint8_t registerAddress,
								uint32_t registerValue,
								uint8_t bytes)
{
	/*Issue first CMD to be a write operation to register address*/
	uint8_t writeCmd[4] = {0};
    writeCmd[0] = AD7124_COMM_WRITE | registerAddress;

    /*Load Cmd into write Buffer big endian*/
    for(uint8_t i = 1; i <= bytes; i++) {
    	writeCmd[i] = (registerValue >> ((bytes - i) * 8)) & 0xFF;
    }

    AD7124_ChipSelect(s, LLO);
    SPI_xfer(writeCmd, NULL, bytes + 1, 0);
    AD7124_ChipSelect(s, LHI);

}

/*******************************************************************************
 * @brief Reads the value of a register.
 *
 * @param registerAddress - Address of the register.
 * @param bytesNumber - Number of bytes that will be read.
 *
 * @return value.
*******************************************************************************/
uint32_t AD7124_GetRegisterValue(SensorSpec * s,
									uint8_t registerAddress,
									uint8_t bytes)
{
	/* make tmp buffer for cmd */
	uint8_t cmdBuffer;
	uint8_t tmpReadBuffer[3] = {0};
	uint32_t result = 0;

    cmdBuffer = AD7124_COMM_READ | registerAddress;
    AD7124_ChipSelect(s, LLO);
    SPI_xfer(&cmdBuffer, tmpReadBuffer, 1, bytes);
    AD7124_ChipSelect(s, LHI);

    /* get array ptr back, cat into 32bit word */
    for(uint8_t i = 0; i < bytes; i++) {
//TODO check out endiannes when this
        result += tmpReadBuffer[i] << ((bytes - i - 1) * 8);  //big endian
    }

    return result;
}


/***************************************************************************//**
 * @brief Resets the device. Put all the AD7124 in the same state by sending
 *        0xFF via SPI_xfer
 *
 * @return none.
*******************************************************************************/
void AD7124_Reset(void)
{
   uint8_t registerWord[8];
    
    registerWord[0] = 0xFF;
    registerWord[1] = 0xFF;
    registerWord[2] = 0xFF;
    registerWord[3] = 0xFF;
    registerWord[4] = 0xFF;
    registerWord[5] = 0xFF;
    registerWord[6] = 0xFF;
    registerWord[7] = 0xFF;


    SPI_xfer(registerWord, NULL, 8, 0);
}

/***************************************************************************//**
 * @brief Returns the result of a single conversion.
 *
 * @return regData - Result of a single analog-to-digital conversion.
*******************************************************************************/
void AD7124_StartConversion(SensorSpec *master)
{
    TIMER0->CMD =(TIMER0->CMD &~_TIMER_CMD_START_MASK)| TIMER_CMD_START;// we start the timer right away as soon as the board is powered
	TIMER1->CMD =(TIMER1->CMD &~_TIMER_CMD_START_MASK)| TIMER_CMD_START;//
    AD7124_ChipSelect(master, LLO);
	GPIO_IntEnable(boardSetup_ptr->pinInterupt);
}

/***************************************************************************//**
 * @brief set register values for AD7124
 * @Detail
 * @return
 * @Note likely written by leumas64
*******************************************************************************/


void AD7124_ConfigureDevice(SensorSpec *s, AD7124 regSetup) {
		AD7124_SetRegisterValue(s, AD7124_REG_ERROREN, regSetup.ERROR_EN, 3);
			s->registers.ERROR_EN = AD7124_GetRegisterValue(s, AD7124_REG_ERROREN, 3);
		AD7124_SetRegisterValue(s, AD7124_REG_CH0, regSetup.CHANNEL_0, 2);
			s->registers.CHANNEL_0 = AD7124_GetRegisterValue(s, AD7124_REG_CH0, 2);
		AD7124_SetRegisterValue(s, AD7124_REG_CFG0, regSetup.CONFIG_0, 2);
			s->registers.CONFIG_0 = AD7124_GetRegisterValue(s, AD7124_REG_CFG0, 2);
		AD7124_SetRegisterValue(s, AD7124_REG_CTRL, regSetup.ADC_CONTROL, 2);
			s->registers.ADC_CONTROL = AD7124_GetRegisterValue(s, AD7124_REG_CTRL, 2);
		AD7124_SetRegisterValue(s, AD7124_REG_FLTR0, regSetup.FILTER_0, 3);
			s->registers.FILTER_0 = AD7124_GetRegisterValue(s, AD7124_REG_FLTR0, 3);
}

/***************************************************************************//**
 * @brief select an AD7124
 * @Detail simply use GPIO_PinModeSet to put the pin associated to the desired
 *         AD7124 in the state you want
 *
 * @return void
 * Note: Likely written by leumas64
*******************************************************************************/

void AD7124_ChipSelect(SensorSpec * s, int state) {
	GPIO_PinModeSet(s->csLoc.gpioPort, s->csLoc.gpioPin, gpioModePushPull, state);
}

/***************************************************************************//**
 * @brief Send and read data through USART0
 * @detail: sent bytesout times writebuffer
 *
 * @return void
 * Note:
*******************************************************************************/
void SPI_xfer (uint8_t* writeBuffer, uint8_t* readBuffer, int bytesOut, int bytesIn) {

	while (bytesOut > 0) {
		USART_SpiTransfer(USART0, *writeBuffer);
		writeBuffer++;
		bytesOut--;
	}

	while (bytesIn > 0) {
		*readBuffer = USART_SpiTransfer(USART0, 0x0);
		readBuffer++;
		bytesIn--;
	}
}

/***************************************************************************//**
 * @brief Initialize the communication via Serial Port Interface (SPI) i.e: USART0
 * @details: turn off MOSI (GPIO PE7), MISO (GPIO PE6), and turn on CLK (GPIO PE5).
 *           set up baud rate of SPI and format (big or little enDian).
 *           Set up Clock idle high, sample on rising edge.
 *           initialize USART and define a routing register
 * @return void
 * Note: Likely written by leumas64. Why MISO =GPIO PE6 and!!! GPIO PE9???
*******************************************************************************/
void SPI_Init() {

	USART_InitSync_TypeDef init = USART_INITSYNC_DEFAULT;

	  /* Enabling clock to USART0 */
	  CMU_ClockEnable(cmuClock_USART0, true);

	  /* IO configuration */
	  GPIO_PinModeSet(gpioPortE, 7,	gpioModePushPull, 0);  /* MOSI */
	  GPIO_PinModeSet(gpioPortE, 6,	gpioModeInput, 0); /* MISO */
	  GPIO_PinModeSet(gpioPortE, 5,	gpioModePushPull, 1);  /* CLK */

	  /* Initialize USART in SPI master mode. */
	  init.baudrate = boardSetup_ptr->spiBaudrate;			// baudRate defined in common.h
	  init.msbf     = true; 			// Analog devices is big enDian
	  init.clockMode = usartClockMode3;
	  USART_InitSync(USART0, &init);

	  /* Enabling pins and setting location, SPI /CS bus controlled independently */
	  USART0->ROUTE = USART_ROUTE_TXPEN | USART_ROUTE_RXPEN |
	                         USART_ROUTE_CLKPEN | USART_ROUTE_LOCATION_LOC1;


}

/*******************************************************************************
 * @brief
 *
 * @return void
*******************************************************************************/
void SPI_deInit() {

}

/*******************************************************************************
 * @brief : set up an interrupt on GPIO pin E9 (A/D MISO)
 * @detail: set MISO (GPIO PE9) high. Set up external interrupt on MISO on falling edge
 *          clear pending interrupt n GPIO_ODD and enable Interrupt on GPIO_ODD
 * @return void
 * Note: Likely written by leumas64
*******************************************************************************/
void setupGpioInt() {
	/*DOUT on gpio PE9*/
	GPIO_PinModeSet(gpioPortE, 9, gpioModeInputPull, 1);

	GPIO_ExtIntConfig(gpioPortE, 9, 9, false, true, false);

	NVIC_ClearPendingIRQ(GPIO_ODD_IRQn);
	NVIC_EnableIRQ(GPIO_ODD_IRQn);
}

/******************************************************************************
 * @brief
 *
 * @return void
 * Note Likely written by leumas64
*******************************************************************************/
/* RDY LOW event handler. Sample taker. Put data into stream buffer /w appropriate timestamp?*/
void GPIO_ODD_IRQHandler(void) {

	uint32_t count;
    uint8_t * count_ptr;


	GPIO_IntDisable(boardSetup_ptr->pinInterupt);
	uint8_t cmdBuffer;
    cmdBuffer = AD7124_COMM_READ | AD7124_REG_DATA;

	if(GPIO->IF != boardSetup_ptr->pinInterupt) return; //Interrupt on PE9

	if (flagSync==1) pendingSamples=0; // had to re-synchronized the ADC

	GPIO_IntDisable(boardSetup_ptr->pinInterupt);

	/* Sample A/D in sequential order */

	for( int i = 0; i < boardSetup_ptr->numSensor ; i++) {
		// AD7124_GetRegisterValue return a uint32 result from the ADC register
		AD7124_ChipSelect(sensors[i], LLO);
		USART_SpiTransfer(USART0, cmdBuffer);
		for(int ii = 0; ii < 3; ii++) {
	//TODO check out endiannes when this
			dataBuffer[(pendingSamples*byteSample+i*3+ii) % bufferSize] = USART_SpiTransfer(USART0, 0x0);
			if (i==2){
				count=pendingSamples % 500;
				count_ptr=(uint8_t*) &count;
				// make a fake signal on shear 1
				dataBuffer[(pendingSamples*byteSample+i*3+ii) % bufferSize] = count_ptr[2-ii];
			}
		}//end for ii
		AD7124_ChipSelect(sensors[i], LHI);
    }//end for

	if (boardSetup_ptr->timeStampFlag){
// TODO define a real time stamp
		uint8_t timeStamp1=1;uint8_t timeStamp2=1;uint8_t timeStamp3=1;
		dataBuffer[(pendingSamples*byteSample+byteSample-3) % bufferSize] =  timeStamp3;
		dataBuffer[(pendingSamples*byteSample+byteSample-2) % bufferSize] =  timeStamp2;
		dataBuffer[(pendingSamples*byteSample+byteSample-1) % bufferSize] =  timeStamp1;
	}
	pendingSamples++; // Increment number of samples available

	AD7124_ChipSelect(sensors[0], LLO); // Select Master to Monitor DRDY pin

	// enable the TX interrupt based on the buffer level
	USART_IntEnable(USART1, UART_IEN_TXBL);
//TODO stream out the data ?
	// streamData(pendingsample)
	GPIO_IntClear(boardSetup_ptr->pinInterupt);
	GPIO_IntEnable(boardSetup_ptr->pinInterupt);


}
