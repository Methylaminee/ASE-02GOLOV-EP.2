/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           lib_elevator.c
** Last modified Date:  2019-12-30
** Last Version:        V1.00
** Descriptions:        Atomic elevator functions
** Correlated files:    elevator.h
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/   

#include "elevator.h"
#include "../led/led.h"
#include "../timer/timer.h"
#include "maintenance/maintenance.h"

extern const unsigned int LED_CONTROLLER_STATUS;
extern const unsigned int LED_ALARM_FLOOR_0;
extern const unsigned int LED_ALARM_FLOOR_1;
extern const unsigned int LED_RESERVED_FLOOR_0;
extern const unsigned int LED_RESERVED_FLOOR_1;
extern const unsigned int POSITION_TIMER;
extern const unsigned int MOVING_TIMER;
extern const unsigned int TOUCH_TIMER;
extern const unsigned int IDLE_TIMER;
extern const unsigned int IDLE_MCR;

/* CONSTANTS for Timers */
const unsigned int DEFAULT_ELEVATOR_POSITION = 0x0;				// used to save position when elevator stops, to set 'backward direction timer' as (MAX_POS - CURRENT_POS)
const unsigned int MAX_ELEVATOR_POSITION = 0x0ABA9500;			// max position is reached after 7.2s => @25MHz Match Register is 0x0ABA9500

/* VARIABLES for Elevator Info's */
unsigned int isOnMaintenance = 0;

unsigned int elevatorPosition = 0;
unsigned int requestedFloor = 0;
unsigned int elevatorFloor = 0;
unsigned int isElevatorBetweenFloors = 0;
unsigned int isElevatorMovingUpstairs = 0;
unsigned int isElevatorMoving = 0;
unsigned int isElevatorFree = 1;
unsigned int isJoystickEnabled = 0;
unsigned int isDestinationReached = 0;


unsigned int reserveElevator(unsigned int floor) {
	requestedFloor = floor;
	if (isElevatorFree) {
		isElevatorFree = 0;
		isDestinationReached = 0;
		goToHome();
		timer_disable(TOUCH_TIMER);
		timer_reset(TOUCH_TIMER);
		//timer_set_mcr(IDLE_TIMER, IDLE_MCR);
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
}

unsigned int moveElevator(unsigned int toFloor, unsigned int previousPosition) {
	isElevatorMoving = 1;
	/* TRUE when elevator has changed direction */
	if (isElevatorMovingUpstairs != toFloor) {
		isElevatorMovingUpstairs = toFloor;
		/* TRUE when elevator is moving backwards requestedFloor == toFloor*/
		if (previousPosition > 0) { 
			timer_set_tc(POSITION_TIMER, MAX_ELEVATOR_POSITION - previousPosition);
		}
	}	
	timer_enable(POSITION_TIMER);
	timer_enable(MOVING_TIMER);
	timer_disable(IDLE_TIMER);
	timer_reset(IDLE_TIMER);
	if (isAlarmDetectedFloorOne()) {
		LED_Off(LED_ALARM_FLOOR_1); 
	} else if (isAlarmDetectedFloorZero()){
		LED_Off(LED_ALARM_FLOOR_0);
	}
	isElevatorBetweenFloors = 1;
	return timer_get_tc(POSITION_TIMER);
}

void stopElevator() {
	elevatorPosition = timer_get_tc(POSITION_TIMER);
	isElevatorMoving = 0;
	if (isElevatorBetweenFloors) {
		LED_On(LED_CONTROLLER_STATUS);
		timer_disable(MOVING_TIMER);
		timer_disable(POSITION_TIMER);
		timer_enable(IDLE_TIMER);
	}
}

unsigned int isAlarmDetectedFloorOne() {
	return requestedFloor && !isElevatorFree && LED_isOn(LED_ALARM_FLOOR_1);
}

unsigned int isAlarmDetectedFloorZero() {
	return !requestedFloor && !isElevatorFree && LED_isOn(LED_ALARM_FLOOR_0);
}
