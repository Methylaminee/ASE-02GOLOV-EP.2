/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           lib_led.c
** Last modified Date:  2019-12-30
** Last Version:        V1.00
** Descriptions:        Atomic led init functions
** Correlated files:    led.h, funct_led.c
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "lpc17xx.h"
#include "led.h"

const unsigned int LED_CONTROLLER_STATUS = 0;
const unsigned int LED_ALARM_FLOOR_1 = 4;
const unsigned int LED_RESERVED_FLOOR_1 = 5;
const unsigned int LED_ALARM_FLOOR_0 = 6;
const unsigned int LED_RESERVED_FLOOR_0 = 7;

/*----------------------------------------------------------------------------
  Function that initializes LEDs and switch them off
 *----------------------------------------------------------------------------*/

unsigned char led_value;

void LED_init(void) {
	
  LPC_PINCON->PINSEL4 &= 0xFFFF0000;	//PIN mode GPIO 	(00b value per P2.0 to P2.7)
	LPC_GPIO2->FIODIR   |= 0x000000FF;  //P2.0 + P2.4...P2.7 Output (LEDs on PORT2 defined as Output)
	LPC_GPIO2->FIOSET    = 0x000000FF;	//all LEDs on
	LPC_GPIO2->FIOCLR    = 0x000000FF;  //all LEDs off
	
	led_value = 0;
}

void LED_deinit(void) {

  LPC_GPIO2->FIODIR &= 0xFFFFFF00;  //P2.0...P2.7 Output LEDs on PORT2 defined as Output
}
