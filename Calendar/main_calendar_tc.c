/* THIS FILE HAS BEEN MODIFIED FROM THE ORIGNAL Energy Micro Application Note */

/******************************************************************************
 * @file
 * @brief Tickless calendar with temperature compensation
 * @author Energy Micro AS
 * @version 2.05
 ******************************************************************************
 * @section License
 * <b>(C) Copyright 2013 Energy Micro AS, http://www.energymicro.com</b>
 *******************************************************************************
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 * 4. The source and compiled code may only be used on Energy Micro "EFM32"
 *    microcontrollers and "EFR4" radios.
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Energy Micro AS has no
 * obligation to support this Software. Energy Micro AS is providing the
 * Software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Energy Micro AS will not be liable for any consequential, incidental, or
 * special damages, or any other relief, or for any claim by any third party,
 * arising from your use of this Software.
 *
 *****************************************************************************/

/* Include emlib */
#include "em_cmu.h"
#include "em_chip.h"
#include "em_device.h"
#include "em_emu.h"
#include "em_rmu.h"
#include "em_rtc.h"

/* Include system clock with temperature compensation */
#include "clock_tc.h"

/* Set RTC frequency */
#define RTC_COUNTS_PER_SEC          32768 

/* Set temperature compensation interval */
#define TC_INTERVAL                 60         /* in seconds */
#define RTC_COUNTS_BETWEEN_TC       (TC_INTERVAL*RTC_COUNTS_PER_SEC)

/* Function prototypes */
void rtcSetup( void );

/* Flags that signal when to do temperature compensation */
extern volatile bool doTemperatureCompensation;

/******************************************************************************
 * @brief  Main function
 *****************************************************************************/
int main_calendar_setup (void)
{
  /* Initialize the temperature compensated calendar */
  Clock_Init_TypeDef initialCalendar  = CLOCK_INIT_DEFAULT;
  initialCalendar.rtcCountsPerSec     = RTC_COUNTS_PER_SEC;
  initialCalendar.tempCompInterval    = TC_INTERVAL;
  clockInit(&initialCalendar);  

  /* Setup RTC */
  rtcSetup();

  /* INCLUDE THIS IN MAIN FILE
  // NOTE: be sure to link in em_int.c as the extern locking counter needs to be defined as well as em_adc.c and em_rtc.c
  #include "clock_tc.h"
  volatile bool doTemperatureCompensation = true;		// Flags that signal when to do temperature compensation
  */

  /* INCLUDE THIS IN MAIN LOOP
  if (doTemperatureCompensation)	// Perform temperature compensation
  {
    doTemperatureCompensation = false;  // Clear doTemperatureCompensation flag
    clockDoTemperatureCompensation();
  }
  */

  return (1);
}

/******************************************************************************
 * @brief   Configure RTC
 *****************************************************************************/
void rtcSetup (void)
{
  RTC_Init_TypeDef rtcInit;

  /* Select LFXO as clock source for the RTC */
//  CMU_ClockSelectSet(cmuClock_LFA,cmuSelect_LFXO);
  // PMM: Changed to LFRCO as the epsilometer board does not have a 32KHz clock...
  CMU_ClockSelectSet(cmuClock_LFA,cmuSelect_LFRCO);
  CMU_ClockEnable(cmuClock_RTC, true);
  
  /* Set RTC pre-scaler */
  CMU_ClockDivSet(cmuClock_RTC,cmuClkDiv_1);
   
  /* Enable clock to low energy modules */
  CMU_ClockEnable(cmuClock_CORELE, true);

  /* RTC configuration struct */
  rtcInit.debugRun  = false;
  rtcInit.comp0Top  = false;
  rtcInit.enable    = false;

  /* Initialize RTC */
  RTC_Init(&rtcInit);

  /* Set RTC compare value for first temperature compensation */
  RTC_CompareSet(0, RTC_COUNTS_BETWEEN_TC );

  /* Enable COMP0 interrupt to trigger temperature compensation, and */
  /*        OF interrupt to keep track of counter overflows */
  RTC_IntEnable(RTC_IEN_COMP0 | RTC_IEN_OF );
  
  /* Enable RTC interrupts */
  NVIC_ClearPendingIRQ(RTC_IRQn);
  NVIC_EnableIRQ(RTC_IRQn);

  /* Enable RTC */
  RTC_Enable(true);
}

/***************************************************************************//**
 * @brief RTC Interrupt Handler
 ******************************************************************************/
void RTC_IRQHandler (void)
{
  /* Interrupt source: compare match 0 */
  /*   Increment compare value and compensate for temperature drift */
  if ( RTC_IntGet() & RTC_IF_COMP0 )
  {
    RTC_CompareSet(0,RTC->COMP0 + RTC_COUNTS_BETWEEN_TC);
    RTC_IntClear(RTC_IFC_COMP0);
  
    /* Set flag for temperature compensation */
    doTemperatureCompensation = true;
  }

  /* Interrupt source: counter overflow */
  /*   Increase overflow counter */
  if ( RTC_IntGet() & RTC_IF_OF )
  {
    clockOverflow( );
    RTC_IntClear(RTC_IFC_OF);
  }
}
