/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2019-12-30
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "timer.h"
#include "../led/led.h"
#include "../glcd/glcd.h"
#include "../touchpanel/touchpanel.h"
#include "../dac/dac.h"
#include "../elevator/maintenance/maintenance.h"
#include "../elevator/emergency/emergency.h"

extern const unsigned int LED_ALARM_FLOOR_0;
extern const unsigned int LED_ALARM_FLOOR_1;
extern const unsigned int LED_RESERVED_FLOOR_0;
extern const unsigned int LED_RESERVED_FLOOR_1;
extern const unsigned int LED_CONTROLLER_STATUS;

extern const unsigned int IDLE_TIMER;
extern const unsigned int MOVING_TIMER;
extern const unsigned int EMERGENCY_TIMER;
extern const unsigned int LOUDSPEAKER_TIMER;
extern const unsigned int FLOOR_REACHED_TIMER;
extern const unsigned int FLOOR_REACHED_TIMER_MAX_VALUE;
extern const unsigned int FLOOR_REACHED_MCR;
extern const unsigned int DEFAULT_MCR;
extern const unsigned int IDLE_MCR;

extern unsigned int elevatorPosition;
extern unsigned int elevatorFloor;
extern unsigned int requestedFloor;
extern unsigned int isElevatorFree;
extern unsigned int isElevatorMoving;
extern unsigned int isElevatorMovingUpstairs;
extern unsigned int isElevatorBetweenFloors;
extern unsigned int isDestinationReached;

extern unsigned int isJoystickEnabled;
extern unsigned int isEmergency;
extern unsigned int isRescuing;

extern unsigned int isHomePage;
extern unsigned int isNoteASelected;

extern int count;
unsigned int ticks=0;
uint8_t x = 20;
extern uint8_t *note_one_value;
extern uint8_t *note_two_value;
unsigned int isTouching = 0;

uint16_t SinTable[45] =                                      
{
    410, 467, 523, 576, 627, 673, 714, 749, 778,
    799, 813, 819, 817, 807, 789, 764, 732, 694, 
    650, 602, 550, 495, 438, 381, 324, 270, 217,
    169, 125, 87 , 55 , 30 , 12 , 2  , 0  , 6  ,   
    20 , 41 , 70 , 105, 146, 193, 243, 297, 353
};

/* TIMER0 - 2Hz(0.5s)        */
void TIMER0_IRQHandler(void) {
	if (LPC_TIM0->IR & (1<<0)) {
		if (LED_isOn(LED_CONTROLLER_STATUS)){
			LED_Off(LED_CONTROLLER_STATUS);
		} else {
			LED_On(LED_CONTROLLER_STATUS);
		}
		LPC_TIM0->IR = 1;	
	} else {
		if ((LPC_TIM0->IR & (1<<1)) == 2) {
			if (LED_isOn(LED_CONTROLLER_STATUS)){
				LED_Off(LED_CONTROLLER_STATUS);
			} else {
				LED_On(LED_CONTROLLER_STATUS);
			}
			LPC_TIM0->MR1 += 0x004C4B40;
			LPC_TIM0->IR = 1<<1;
		}
		if ((LPC_TIM0->IR & (1<<2)) == 4) {
			LPC_TIM0->IR = 1<<2;
			LPC_TIM0->MR1 = 0x004C4B40;
			timer_disable(MOVING_TIMER);
			timer_reset(MOVING_TIMER);
			timer_set_mcr(MOVING_TIMER, DEFAULT_MCR);
			isDestinationReached = 0;
			LED_Off(LED_CONTROLLER_STATUS);
		}
	}	
  return;
}

/* TIMER1 - MR0 2s and MR1 4Hz */
void TIMER1_IRQHandler(void) {
	if (LPC_TIM1->IR & (1<<0)) {
		handleEmergency(1);
		ticks=0;
		LPC_TIM1->IR = 1<<0;
	} else {
		if (isEmergency) {
			if (LED_isOn(LED_CONTROLLER_STATUS)){
				LED_Off(LED_CONTROLLER_STATUS);
			} else {
				LED_On(LED_CONTROLLER_STATUS);
			}
			timer_set_loudspeaker_freq(count);
			timer_reset(LOUDSPEAKER_TIMER);
			timer_enable(LOUDSPEAKER_TIMER);
		}
		LPC_TIM1->IR = 1<<1;
	}
  return;
}

/* TIMER2 - 1388mHz(7.2s)    */
void TIMER2_IRQHandler(void) {
	timer_disable(MOVING_TIMER);
	timer_reset(MOVING_TIMER);
	if (isElevatorMovingUpstairs) {
		elevatorFloor = 1;
	} else {
		elevatorFloor = 0;
	}
	if (isJoystickEnabled) {
		//isElevatorFree = 1;
		elevatorPosition = 0;
		isDestinationReached = 1;
	}
	if (isEmergency) {
		isEmergency = !isEmergency;
		isRescuing = 0;
	}
	isElevatorMoving = 0;
	isElevatorBetweenFloors = 0;
	timer_set_mcr(FLOOR_REACHED_TIMER, FLOOR_REACHED_MCR);
	timer_set_mcr(IDLE_TIMER, IDLE_MCR);
	timer_enable(FLOOR_REACHED_TIMER);
	timer_enable(IDLE_TIMER);
  LPC_TIM2->IR = 1;			/* clear interrupt flag */
  return;
}

/* TIMER3 - 166mHz(60s)      */
void TIMER3_IRQHandler(void) {
	if (LPC_TIM3->IR & (1<<0)) {
		if (isElevatorFree) {
			Coordinate * Ptr = Read_Ads7846();
			if (Ptr != (void*)0) {
				//i'm touching
				getDisplayPoint(&display, Ptr, &matrix );
				if(display.x <= 240 && display.x > 0 && !isTouching){
					if (isHomePage) {
						//power btn
						if (display.x > 160 && display.x < 220 && display.y > 20 && display.y < 80) {
							goToSetup();
						}
					} else {
						updateNote(isNoteASelected, count);
						//select note 1/2
						if (display.x > 20 && display.x < 220 && display.y > 60 && display.y < 145) {
							selectNote(1);
						} else if (display.x > 20 && display.x < 220 && display.y > 175 && display.y < 260) {
							selectNote(0);
						} else
						//save
						if (display.x > 10 && display.x < 100 && display.y > 275) {
							saveNote(count);
						} else 
						//quit
						if (display.x > 140 && display.x < 230 && display.y > 275) {
							goToHome();
							LED_Out(0x0);
						}
					}
					isTouching++;
				}
				DAC_convert (count);
			} else {
				//do nothing if touch returns values out of bounds
				isTouching=0;
			}
		}
		LPC_TIM3->IR = 1<<0;			/* clear interrupt flag */
	} 
	if (LPC_TIM3->IR & (1<<1)) {
		if (isElevatorBetweenFloors){
			handleEmergency(1);
		} else {
			isElevatorFree = 1;
			isJoystickEnabled = 0;
			LED_Off(LED_CONTROLLER_STATUS);
			if (requestedFloor == 1) {
				LED_Off(LED_RESERVED_FLOOR_1); 
			} else {
				LED_Off(LED_RESERVED_FLOOR_0);
			}
			goToHome();
		}
		LPC_TIM3->IR = 1<<1;
	}
	if (LPC_TIM3->IR & (1<<2)) {
		if (isEmergency) {
			static int ticks=0;
			/* DAC management */	
			DAC_convert (SinTable[ticks]<<6);
			ticks++;
			if(ticks==45) ticks=0;
		}
		LPC_TIM3->IR = 1<<2;
	}
  return;
}
