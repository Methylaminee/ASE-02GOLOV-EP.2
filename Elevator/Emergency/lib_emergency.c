/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           lib_elevator.c
** Last modified Date:  2019-12-30
** Last Version:        V1.00
** Descriptions:        Atomic elevator functions
** Correlated files:    elevator.h
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/   

#include "../../led/led.h"
#include "../../timer/timer.h"
#include "../elevator.h"

extern const unsigned int LED_ALARM_FLOOR_0;
extern const unsigned int LED_ALARM_FLOOR_1;
extern const unsigned int LED_RESERVED_FLOOR_0;
extern const unsigned int LED_RESERVED_FLOOR_1;
extern const unsigned int LED_CONTROLLER_STATUS;

extern const unsigned int TOUCH_TIMER_MAX_VALUE;
extern const unsigned int MAX_ELEVATOR_POSITION;
extern const unsigned int LOUDSPEAKER_TIMER;
extern const unsigned int EMERGENCY_TIMER;
extern const unsigned int POSITION_TIMER;
extern const unsigned int LOUDSPEAKER_MCR;
extern const unsigned int EMERGENCY_MCR;
extern const unsigned int DEFAULT_MCR;
extern const unsigned int TOUCH_TIMER;
extern const unsigned int IDLE_TIMER;
extern const unsigned int IDLE_MCR;

extern unsigned int requestedFloor;
extern unsigned int isElevatorMovingUpstairs;

unsigned int isEmergency = 0;
unsigned int isRescuing = 0;

void handleEmergency(unsigned int isEnabled) {
	if (isEnabled) {
		isEmergency = 1;
		/*	Stop Elevator	*/
		timer_disable(POSITION_TIMER);
		timer_disable(IDLE_TIMER);
		/*	Status LED, Alarm LEDs, Loudspeaker	*/
		LED_On(LED_ALARM_FLOOR_0);
		LED_On(LED_ALARM_FLOOR_1);
		LED_Off(LED_CONTROLLER_STATUS);
		timer_set_mcr(LOUDSPEAKER_TIMER, LOUDSPEAKER_MCR);
		timer_reset(LOUDSPEAKER_TIMER);
		timer_set_mcr(EMERGENCY_TIMER, EMERGENCY_MCR);
		timer_reset(EMERGENCY_TIMER);
		timer_enable(EMERGENCY_TIMER);
	} else {
		isEmergency = 0;
		timer_disable(LOUDSPEAKER_TIMER);
		timer_disable(EMERGENCY_TIMER);
		LED_Off(LED_ALARM_FLOOR_0);
		LED_Off(LED_ALARM_FLOOR_1);
		LED_Off(LED_CONTROLLER_STATUS);
		timer_set_mcr(EMERGENCY_TIMER, DEFAULT_MCR);
		timer_reset(EMERGENCY_TIMER);
		timer_set_mcr(IDLE_TIMER, IDLE_MCR);
		timer_reset(IDLE_TIMER);
	}
	
	
}

void doRescue(unsigned int floor) {
	if (!isRescuing) {
		isRescuing = 1;
		/*	when called from request panel elevator move to selected floor > RESCUE > loudspeaker stops emit > LEDS usual	behaviour */
		requestedFloor = floor;
		if (floor) {
			/*	Was going Upstairs and help comes from First Floor	*/
			LED_On(LED_RESERVED_FLOOR_1);
		} else {
			/*	Go To Ground Floor	*/
			LED_On(LED_RESERVED_FLOOR_0);
		}
		if (isElevatorMovingUpstairs != floor) {
			/*	If elevator is changing direction */
			timer_set_tc(POSITION_TIMER, MAX_ELEVATOR_POSITION - timer_get_tc(POSITION_TIMER));
		}
		/*	If elevator isn't changing direction */
		timer_enable(POSITION_TIMER);
		isElevatorMovingUpstairs &= floor;
	}
}

/*
requestedFloor = floor;
	if (isElevatorFree) {
		isElevatorFree = 0;
		isDestinationReached = 0;
		goToHome();
		LED_Out(0x0);
		if (requestedFloor == 1) {
			isElevatorMovingUpstairs = 1;
			LED_On(LED_RESERVED_FLOOR_1);
		} else {
			isElevatorMovingUpstairs = 0;
			LED_On(LED_RESERVED_FLOOR_0);
		}
		if (elevatorFloor != requestedFloor) {
			timer_enable(MOVING_TIMER);
			timer_enable(POSITION_TIMER);
			isElevatorBetweenFloors = 1;
			isElevatorMoving = 1;
		}
	}
	return 0;
*/
