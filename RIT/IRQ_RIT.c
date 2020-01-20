/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "RIT.h"
#include "../led/led.h"
#include "../timer/timer.h"
#include "../adc/adc.h"
#include "../elevator/elevator.h"
#include "../elevator/maintenance/maintenance.h"
#include "../elevator/emergency/emergency.h"

extern const unsigned int IDLE_TIMER;
extern const unsigned int MAX_ELEVATOR_POSITION;
extern const unsigned int EMERGENCY_TIMER;

extern unsigned int isOnMaintenance;
extern unsigned int elevatorPosition;
extern unsigned int elevatorFloor;
extern unsigned int requestedFloor;
extern unsigned int isElevatorFree;
extern unsigned int isElevatorMoving;
extern unsigned int isElevatorMovingUpstairs;
extern unsigned int isElevatorBetweenFloors;
extern unsigned int isJoystickEnabled;
extern unsigned int isDestinationReached;

extern unsigned int isEmergency;

unsigned int moveJoystick=0;
unsigned int reservedOne=0;
unsigned int reservedZero=0;
unsigned int emergencyHappened=0;

void RIT_IRQHandler (void) {				
	/* joystick management */
	if((LPC_GPIO1->FIOPIN & (1<<25)) == 0) {	
		/* Joytick Select pressed */
		if (!isElevatorFree && !isElevatorMoving) {
			timer_disable(IDLE_TIMER);
			timer_reset(IDLE_TIMER);
			isJoystickEnabled = 1;
		}
	} else if((LPC_GPIO1->FIOPIN & (1<<26)) == 0) {
		/* Joystick Down pressed */
		if (isJoystickEnabled && !isElevatorMoving && !isElevatorFree && !isDestinationReached) {
			if ((!elevatorPosition && elevatorFloor) || isElevatorBetweenFloors) {
				moveJoystick++;
				elevatorPosition = moveElevator(0, elevatorPosition);
			}
		}
	} else if((LPC_GPIO1->FIOPIN & (1<<29)) == 0) {
		/* Joystick Up pressed */
		if (isJoystickEnabled && !isElevatorMoving && !isElevatorFree && !isDestinationReached) {
			if ((!elevatorPosition && !elevatorFloor) || isElevatorBetweenFloors) {
				moveJoystick++;
				elevatorPosition = moveElevator(1, elevatorPosition);
			}
		}
	} else if(moveJoystick > 0) {
		moveJoystick = 0;
		if (!isDestinationReached) {
			stopElevator();
		}
	}
	
	/* buttons management */
	/* EMERGENCY - INT0 pressed */
	if (emergencyHappened!=0) {
		if ((LPC_GPIO2->FIOPIN & (1<<10)) == 0 && (isElevatorBetweenFloors || isOnMaintenance)) {
			if (emergencyHappened == 1) {
				emergencyHappened++;				
				if (!isEmergency) {
					timer_enable(EMERGENCY_TIMER);
				} else {
					handleEmergency(0);
				}
			}
		} else {
			emergencyHappened=0;
			if (!isEmergency) {
				timer_disable(EMERGENCY_TIMER);			
				timer_reset(EMERGENCY_TIMER);
			}
			NVIC_EnableIRQ(EINT0_IRQn);							 /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 20);     /* External interrupt 0 pin selection */
		}
	}
	/* FLOOR.1 - KEY1 pressed */
	if (reservedOne!=0) { 
		if((LPC_GPIO2->FIOPIN & (1<<11)) == 0 && isElevatorFree && !isOnMaintenance) {	
			reservedOne++;
			if (!isEmergency) {
				reserveElevator(1);
			} else {
				doRescue(1);
			}
		} else {	/* button released */
			reservedOne=0;			
			NVIC_EnableIRQ(EINT1_IRQn);							 /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 22);     /* External interrupt 0 pin selection */
		}
	}
	/* FLOOR.0 - KEY2 pressed */
	if (reservedZero!=0) {
		if ((LPC_GPIO2->FIOPIN & (1<<12)) == 0 && isElevatorFree && !isOnMaintenance) {	
			reservedZero++;
			if (!isEmergency) {
				reserveElevator(0);
			} else {
				doRescue(0);
			}			
		} else {
			reservedZero=0;			
			NVIC_EnableIRQ(EINT2_IRQn);							 /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 24);     /* External interrupt 1 pin selection */
		}
	}
	if (isOnMaintenance) {
		ADC_start_conversion();
	}
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	
  return;
}

