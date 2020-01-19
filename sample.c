/*----------------------------------------------------------------------------
 * Name:    sample.c
 * Purpose: to control an elevator system
 * Note(s):
 *----------------------------------------------------------------------------
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2017 Politecnico di Torino. All rights reserved.
 *----------------------------------------------------------------------------*/
                  
#include <stdio.h>
#include "LPC17xx.H"                    /* LPC17xx definitions                */
#include "led/led.h"
#include "button/button.h"
#include "joystick/joystick.h"
#include "adc/adc.h"
#include "dac/dac.h"
#include "rit/rit.h"
#include "timer/timer.h"
#include "glcd/glcd.h"
#include "touchpanel/touchpanel.h"
#include "elevator/elevator.h"
#include "elevator/maintenance/maintenance.h"


/* Led external variables from funct_led */
extern unsigned char led_value;					/* defined in funct_led								*/

/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void) {
  	
	SystemInit();  												/* System Initialization (i.e., PLL)  */
  LED_init();                           /* LED Initialization                 */
	
	LCD_Initialization();									/* LCD Initialization									*/
	TP_Init();														/* TouchPanel Initialization					*/
	//TouchPanel_Calibrate();								/* TouchPanel Calibration							*/
	
	goToHome();														/* Shows Maintenance Homepage					*/
	LED_Out(0x0);
	
  BUTTON_init();												/* BUTTON Initialization              */
	JOYSTICK_init();											/* JOYSTICK Initialization            */
	RIT_init(0x004C4B40);									/* Repetitive Interrupt Timer Init		*/
	TIMER_init();													/* Timers 0/1/2/3 Initialization			*/
		
	
	ADC_init();														/* ADC Initialization									*/
	DAC_init();														/* DAC Initialization	  							*/
	
	
	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= 0xFFFFFFFFD;						
		
  while (1) {                           /* Loop forever                       */	
		__ASM("wfi");
  }

}
