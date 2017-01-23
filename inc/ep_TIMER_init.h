/*
 * ep_TIMER_init.h
 *
 *  Created on: Jan 13, 2017
 *      Author: aleboyer
 */

#ifndef INC_EP_TIMER_INIT_H_
#define INC_EP_TIMER_INIT_H_


#include "em_device.h"

void init_TIMER(uint32_t coreclockCycle,uint32_t timer1PhaseShift);

#endif /* INC_EP_TIMER_INIT_H_ */
