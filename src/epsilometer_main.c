/**************************************************************************//**
 * @file
 * @brief Empty Project
 * @author Energy Micro AS
 * @version 3.20.2
 ******************************************************************************
 * @section License
 * <b>(C) Copyright 2014 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silicon Labs Software License Agreement. See 
 * "http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt"  
 * for details. Before using this software for any purpose, you must agree to the 
 * terms of that agreement.
 *
 ******************************************************************************/

/*standard c library */
#include "stdint.h"
#include "stdio.h"

/*energy micro library */
#include "em_device.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_usart.h"
#include "em_chip.h"

/* SD card library */
#include "microsd.h"
#include "ff.h"
#include "diskio.h"

/*local library */
#include "ep_common.h"
#include "ep_analog.h"
#include "ep_coms.h"
#include "ep_CMU_init.h"
#include "ep_GPIO_init.h"
#include "ep_TIMER_init.h"
#include "ep_SDcard.h"
#include "clock_tc.h"


#include "clock_tc.h"
/* Flags that signal when to do temperature compensation and display update  */
volatile bool doTemperatureCompensation = true;         // Flags that signal when to do temperature compensation


#define STREAMMODE
//#define STORAGEMODE

/******************************************************************************/
/*************************** List of Possible Channels / Sensor *******************************/
/******************************************************************************/

epsiSetup boardSetup = EPSI_SETUP_DEFAULT;
epsiSetupPtr boardSetup_ptr =  &boardSetup;

SensorSpec fp07_1 = {"T1", AD7124_RESET_DEFAULT, {gpioPortA, 0}};  //PA0
SensorSpec fp07_2 = {"T2", AD7124_RESET_DEFAULT, {gpioPortC, 5}};  //PC5
SensorSpec shr_1  = {"S1", AD7124_RESET_DEFAULT, {gpioPortC, 6}};  //PC6
SensorSpec shr_2  = {"S2", AD7124_RESET_DEFAULT, {gpioPortC, 7}};  //PC7
SensorSpec con_1  = {"C1", AD7124_RESET_DEFAULT, {gpioPortC, 12}}; //PC12
SensorSpec ax     = {"AX", AD7124_RESET_DEFAULT, {gpioPortC, 14}}; //PC14
SensorSpec ay     = {"AY", AD7124_RESET_DEFAULT, {gpioPortC, 15}}; //PC15
SensorSpec az     = {"AZ", AD7124_RESET_DEFAULT, {gpioPortA, 1}};  //PA1


volatile SensorSpec_ptr sensors[8]={&fp07_1, &fp07_2, &shr_1, &shr_2, &con_1, &ax, &ay, &az};


volatile uint32_t pendingSamples = 0; // counter in the background IRQ
volatile uint32_t txSentBytes    = 0; // counter in the background IRQ
volatile uint32_t numSync        = 0;    // number of sync signal sent
volatile uint32_t flagSync       = 0;    // flag to reset pending sample in the interrupt.





/******************************************************************************
 * @brief
 *   Main function.
 * @details
 *   Configures the DVK for SPI
 *****************************************************************************/
int main(void) {
    /* Initialize chip - handle erratas */
    CHIP_Init();
	/* Use 20 MHZ HFRCO as core clock frequency*/
    /* use of an interrupt on CMU to trigger the Oscillator when ready */
    init_CMU();
	// define GPIO pin mode for ADC and the 485, PA2 to send MCLOCK (for ADCs), and PE7 to send SYNC
    init_GPIO();


	SPI_Init();     // Initialize the SPI interface for the Analog front end
	AD7124_Reset(); // Place devices into a known state

	setupGpioInt(); // Setup GPIO Interrupt but leave disabled
	define_ADC_configuration();


	main_calendar_setup();



	// set variable
	numChannel       = boardSetup_ptr->numSensor + boardSetup_ptr->timeStampFlag;               // 2 temp + 2 shear +  1 cond + 3 accelerometer = 8 sensor max
	byteSample       = boardSetup_ptr->numSensor*3+ boardSetup_ptr->timeStampFlag * 4;          // 3 bytes per sensor channel * number of sensor channel + (4) bytes for RTC->CNT
	bufferSize       = boardSetup_ptr->maxSamples*byteSample;                                   //
	uint32_t coreclockCycle   = boardSetup_ptr->coreClock/boardSetup_ptr->MclockFreq/2-1;       //
	uint32_t timer1PhaseShift = .5 * coreclockCycle;

	// uint32 array where data are stored and from where data are sent to the serial port
    dataBuffer       = malloc(sizeof(uint8_t)*bufferSize);

    //set up timer MCLOCK and SYNC
	init_TIMER(coreclockCycle,timer1PhaseShift);

	/****************************************************************
	 * Primitive Sampling routine
	 * ***************************************************************/

    #ifdef STREAMMODE
	// define UART route baud rate endianness ect ....
		UART_Setup();
	#endif

    static int cur_min = -1, cur_sec = -1;
    struct tm tsplt;

 	time_t t = time(NULL);
    localtime_r (&t, &tsplt);
	//initSD();

	// initialize the TX state (epsilometer_coms),
	// start conversion: send SYNC signal and enable the sampling interrupt
	AD7124_StartConversion(sensors[0]);

	/****************************************************************
	 * Primitive Sampling routine
	 ****************************************************************/
	while (1) {

//		  time_t t;
//	      time( &t );
    	  time_t t = time(NULL);
	      localtime_r (&t, &tsplt);

	      if (doTemperatureCompensation)	// Perform temperature compensation
	  	  {
	  	    doTemperatureCompensation = false;  // Clear doTemperatureCompensation flag
	  	    clockDoTemperatureCompensation();
	  	  }


	}// end while loop

}// end main
