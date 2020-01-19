/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           lib_timer.h
** Last modified Date:  2019-12-30
** Last Version:        V1.00
** Descriptions:        atomic functions to be used by higher sw levels
** Correlated files:    lib_timer.c, funct_timer.c, IRQ_timer.c
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "timer.h"

const unsigned int MOVING_TIMER = 0;
const unsigned int FLOOR_REACHED_TIMER = 0;
const unsigned int EMERGENCY_TIMER = 1;
const unsigned int POSITION_TIMER = 2;
const unsigned int IDLE_TIMER = 3;
const unsigned int TOUCH_TIMER = 3;
const unsigned int LOUDSPEAKER_TIMER = 3;

const unsigned int IDLE_TIMER_MAX_VALUE = 0x59682F00;
const unsigned int TOUCH_TIMER_MAX_VALUE = 0x000004E2;
const unsigned int MOVING_TIMER_MAX_VALUE = 0x00BEBC20;
const unsigned int POSITION_TIMER_MAX_VALUE = 0x0ABA9500;
const unsigned int EMERGENCY_TIMER_MAX_VALUE = 0x005F5E10;
const unsigned int FLOOR_REACHED_TIMER_MAX_VALUE = 0x047868C0;
const unsigned int EMERGENCY_ALARM_TIMER_MAX_VALUE = 0x02FAF080;

const unsigned int IDLE_MCR = 0x38;
const unsigned int DEFAULT_MCR = 0x7;
const unsigned int EMERGENCY_MCR = 0x18;
const unsigned int DEFAULT_MCR_BASE = 0x3;
const unsigned int LOUDSPEAKER_MCR = 0xC0;
const unsigned int FLOOR_REACHED_MCR = 0x1C8;

void TIMER_init(void) {
	
	timer_init(0, 0x00BEBC20, 0x004C4B40, 0x047868C0, 0x0ABA9500, DEFAULT_MCR_BASE);	/* TIMER0 Initialization - MR0 	2Hz(0.5s) 		- MR1/MR2 	5Hz(0.2s for 3s) 	*/
	timer_init(1, 0x02FAF080, 0x005F5E10, 0, 0, DEFAULT_MCR);								/* TIMER2 Initialization - MR0 	(2s)      		-	MR1 			4Hz(0.25s)      	*/
	timer_init(2, 0x0ABA9500, 0, 0, 0, DEFAULT_MCR);												/* TIMER1 Initialization - MR0  1388mHz(7.2s)         											*/
	timer_init(3, 0x4E2, 0x59682F00, 0, 0, DEFAULT_MCR_BASE);								/* TIMER3 Initialization - MR0 	20kHz(50us) 	- MR1     	166mHz(60s)				*/
	timer_enable(TOUCH_TIMER);
}			

void timer_enable( uint8_t timer_num ) {
  if (timer_num == 0) {
		LPC_TIM0->TCR = 1;
	} else if (timer_num == 1) {
		LPC_TIM1->TCR = 1;
  } else if (timer_num == 2) {
		LPC_TIM2->TCR = 1;
	} else {
		LPC_TIM3->TCR = 1;
	}
  return;
}

void timer_disable( uint8_t timer_num ) {
  if ( timer_num == 0 ) {
		LPC_TIM0->TCR = 0;
  } else  if (timer_num == 1) {
		LPC_TIM1->TCR = 0;
  }  else if (timer_num == 2) {
		LPC_TIM2->TCR = 0;
	} else {
		LPC_TIM3->TCR = 0;
	}
  return;
}

void timer_reset( uint8_t timer_num ) {
  uint32_t regVal;

  if ( timer_num == 0 ) {
		regVal = LPC_TIM0->TCR;
		regVal |= 0x02;
		LPC_TIM0->TCR = regVal;
  } else if (timer_num == 1) {
		regVal = LPC_TIM1->TCR;
		regVal |= 0x02;
		LPC_TIM1->TCR = regVal;
  } else if (timer_num == 2) {
		regVal = LPC_TIM2->TCR;
		regVal |= 0x02;
		LPC_TIM2->TCR = regVal;
	} else {
		regVal = LPC_TIM3->TCR;
		regVal |= 0x02;
		LPC_TIM3->TCR = regVal;
	}
  return;
}

void timer_set_loudspeaker_freq( uint32_t timerInterval ) {
	LPC_TIM3->MR2 = timerInterval;
}	

void timer_set_mcr( uint8_t timer_num, uint32_t setupValue ) {
	if ( timer_num == 0 ) {
		LPC_TIM0->MCR = setupValue;
  } else if (timer_num == 1) {
		LPC_TIM1->MCR = setupValue;
  } else if (timer_num == 2) {
		LPC_TIM2->MCR = setupValue;
	} else {
		LPC_TIM3->MCR = setupValue;
	}
}	

void timer_set_tc( uint8_t timer_num, uint32_t timerInterval ) {
	if ( timer_num == 0 ) {
		LPC_TIM0->TC = timerInterval;
  } else if (timer_num == 1) {
		LPC_TIM1->TC = timerInterval;
  } else if (timer_num == 2) {
		LPC_TIM2->TC = timerInterval;
	} else {
		LPC_TIM3->TC = timerInterval;
	}
  return;
}

uint32_t timer_get_tc( uint8_t timer_num ) {
	return LPC_TIM2->TC;
}

uint32_t timer_init ( uint8_t timer_num, uint32_t firstMatch, uint32_t secondMatch, uint32_t thirdMatch, uint32_t fourthMatch, uint32_t timer_mcr ) {
	if ( timer_num == 0 ) {
		LPC_TIM0->MR0 = firstMatch;
		if (secondMatch > 0) {
			LPC_TIM0->MR1 = secondMatch;
		}
		if (thirdMatch > 0) {
			LPC_TIM0->MR2 = thirdMatch;
		}
		if (fourthMatch > 0) {
			LPC_TIM0->MR3 = fourthMatch;
		}
//*** <<< Use Configuration Wizard in Context Menu >>> ***
// <h> timer0 MCR
//   <e.0> MR0I
//	 <i> 1 Interrupt on MR0: an interrupt is generated when MR0 matches the value in the TC. 0
//	 <i> 0 This interrupt is disabled
//   </e>
//   <e.1> MR0R
//	 <i> 1 Reset on MR0: the TC will be reset if MR0 matches it.
//	 <i> 0 Feature disabled.
//   </e>
//   <e.2> MR0S
//	 <i> 1 Stop on MR0: the TC and PC will be stopped and TCR[0] will be set to 0 if MR0 matches the TC
//	 <i> 0 Feature disabled.
//   </e>
//   <e.3> MR1I
//	 <i> 1 Interrupt on MR1: an interrupt is generated when MR0 matches the value in the TC. 0
//	 <i> 0 This interrupt is disabled
//   </e>
//   <e.4> MR1R
//	 <i> 1 Reset on MR1: the TC will be reset if MR0 matches it.
//	 <i> 0 Feature disabled.
//   </e>
//   <e.5> MR1S
//	 <i> 1 Stop on MR1: the TC and PC will be stopped and TCR[1] will be set to 0 if MR1 matches the TC
//	 <i> 0 Feature disabled.
//   </e>
//   <e.6> MR2I
//	 <i> 1 Interrupt on MR2: an interrupt is generated when MR2 matches the value in the TC.
//	 <i> 0 This interrupt is disabled
//   </e>
//   <e.7> MR2R
//	 <i> 1 Reset on MR2: the TC will be reset if MR2 matches it.
//	 <i> 0 Feature disabled.
//   </e>
//   <e.8> MR2S
//	 <i> 1 Stop on MR2: the TC and PC will be stopped and TCR[2] will be set to 0 if MR2 matches the TC
//	 <i> 0 Feature disabled.
//   </e>
//   <e.9> MR3I
//	 <i> 1 Interrupt on MR3: an interrupt is generated when MR3 matches the value in the TC.
//	 <i> 0 This interrupt is disabled
//   </e>
//   <e.10> MR3R
//	 <i> 1 Reset on MR3: the TC will be reset if MR3 matches it.
//	 <i> 0 Feature disabled.
//   </e>
//   <e.11> MR3S
//	 <i> 1 Stop on MR3: the TC and PC will be stopped and TCR[3] will be set to 0 if MR3 matches the TC
//	 <i> 0 Feature disabled.
//   </e>
		LPC_TIM0->MCR = timer_mcr;
// </h>
//*** <<< end of configuration section >>>    ***

		NVIC_EnableIRQ(TIMER0_IRQn);
		return (1);
  } else if ( timer_num == 1 ) {
		LPC_TIM1->MR0 = firstMatch;
		if (secondMatch > 0) {
			LPC_TIM1->MR1 = secondMatch;
		}
		if (thirdMatch > 0) {
			LPC_TIM1->MR2 = thirdMatch;
		}
		if (fourthMatch > 0) {
			LPC_TIM1->MR3 = fourthMatch;
		}
		LPC_TIM1->MCR = timer_mcr;			/* Interrupt and Reset on MR0/MR1 */

		NVIC_EnableIRQ(TIMER1_IRQn);
		return (1);
  } else if (timer_num == 2) {
		LPC_TIM2->MR0 = firstMatch;
		if (secondMatch > 0) {
			LPC_TIM2->MR1 = secondMatch;
		}
		if (thirdMatch > 0) {
			LPC_TIM2->MR2 = thirdMatch;
		}
		if (fourthMatch > 0) {
			LPC_TIM2->MR3 = fourthMatch;
		}
		LPC_TIM2->MCR = timer_mcr;									/* Interrupt, Stop and Reset on MR0 */

		NVIC_EnableIRQ(TIMER2_IRQn);
		return (1);
	} else if (timer_num == 3) {
		LPC_TIM3->MR0 = firstMatch;
		if (secondMatch > 0) {
			LPC_TIM3->MR1 = secondMatch;
		}
		if (thirdMatch > 0) {
			LPC_TIM3->MR2 = thirdMatch;
		}
		if (fourthMatch > 0) {
			LPC_TIM3->MR3 = fourthMatch;
		}
		LPC_TIM3->MCR = timer_mcr;				/* Interrupt and Reset on MR0 */

		NVIC_EnableIRQ(TIMER3_IRQn);
		return (1);
	}
  return (0);
}

uint32_t timer_init_base ( uint8_t timer_num, uint32_t TimerInterval, uint32_t timer_mcr) {
  return timer_init(timer_num, TimerInterval, 0, 0, 0, timer_mcr);
}
