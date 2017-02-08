/*
 * epsilometer_CMU_init.c
 *
 *  Created on: Jan 13, 2017
 *      Author: aleboyer
 */

#include "em_cmu.h"
#include "ep_CMU_init.h"

volatile uint32_t gulclockset=0;

// CMU Interrupt Handler
void CMU_IRQHandler(void)
{
  // Read Interrupt flags
  uint32_t intFlags = CMU_IntGet();

  // Clear interrupt flags register
  CMU_IntClear(CMU_IF_HFXORDY | CMU_IF_HFRCORDY);

    // Check if the Clock Source for External Crystal
  // is Ready or Not?
  if (intFlags & CMU_IF_HFXORDY)
  {
    CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);
    CMU_OscillatorEnable(cmuOsc_HFRCO, false, false);
        //Inform that We are Done
        gulclockset = 1;
  }
}

void init_CMU(void){
	/* Use 14 MHZ HFRCO as core clock frequency*/
	CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFRCO);
	CMU_ClockEnable(cmuClock_GPIO, true);
	CMU_ClockEnable(cmuClock_TIMER0, true);
	CMU_ClockEnable(cmuClock_TIMER1, true);



    // Enable the External Oscillator , true for enabling the O and false to not wait
    CMU_OscillatorEnable(cmuOsc_HFXO,true,false);

    // Enable interrupts for HFXORDY
    CMU_IntEnable(CMU_IF_HFXORDY);

    // Enable CMU interrupt vector in NVIC
    NVIC_EnableIRQ(CMU_IRQn);

    // Wait for the HFXO Clock to be Stable - Or infinite in case of error
    while(gulclockset != 1);

    /* set up the 20 MHz clock */
	CMU->CTRL =(CMU->CTRL &~_CMU_CTRL_HFXOMODE_MASK)| CMU_CTRL_HFXOMODE_DIGEXTCLK;
    CMU->CTRL =(CMU->CTRL &~_CMU_CTRL_CLKOUTSEL0_MASK)| CMU_CTRL_CLKOUTSEL0_HFXO;
    CMU->ROUTE =(CMU->ROUTE &~_CMU_ROUTE_CLKOUT0PEN_MASK)| CMU_ROUTE_CLKOUT0PEN;
}
