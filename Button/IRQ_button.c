#include "button.h"
#include "lpc17xx.h"

#include "../led/led.h"

extern unsigned int reservedOne;
extern unsigned int reservedZero;
extern unsigned int emergencyHappened;

void EINT0_IRQHandler (void) {	  					/* KEY2														 */
	NVIC_DisableIRQ(EINT0_IRQn);								/* disable Button interrupts			 */
	LPC_PINCON->PINSEL4    &= ~(1 << 20);     /* GPIO pin selection 						 */
	emergencyHappened=1;
  LPC_SC->EXTINT &= (1 << 0);     					/* clear pending interrupt         */    
}

void EINT1_IRQHandler (void) {  						/* KEY1														 */
	NVIC_DisableIRQ(EINT1_IRQn);							/* disable Button interrupts			 */
	LPC_PINCON->PINSEL4    &= ~(1 << 22);     /* GPIO pin selection 						 */
	reservedOne=1;
	LPC_SC->EXTINT &= (1 << 1);     					/* clear pending interrupt         */
}

void EINT2_IRQHandler (void) {	  					/* KEY2														 */
NVIC_DisableIRQ(EINT2_IRQn);								/* disable Button interrupts			 */
	LPC_PINCON->PINSEL4    &= ~(1 << 24);     /* GPIO pin selection 						 */
	reservedZero=1;
  LPC_SC->EXTINT &= (1 << 2);     					/* clear pending interrupt         */    
}


