/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           joystick.h
** Last modified Date:  2019-12-29
** Last Version:        V1.00
** Descriptions:        Atomic joystick init functions
** Correlated files:    lib_joystick.c, funct_joystick.c
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "joystick.h"
#include "lpc17xx.h"

void JOYSTICK_init(void) {

	LPC_PINCON->PINSEL3		&= 0xF3C3FFFF;		 /* 00b value for P1.25-26 and P1.29 */
	LPC_GPIO1->FIODIR   &= ~(1<<25);				 //P1.25 Input (joysticks on PORT1 defined as Input)
	LPC_GPIO1->FIODIR   &= ~(1<<26);				 //P1.26 Input (joysticks on PORT1 defined as Input)
	LPC_GPIO1->FIODIR   &= ~(1<<29);				 //P1.29 Input (joysticks on PORT1 defined as Input)
}
