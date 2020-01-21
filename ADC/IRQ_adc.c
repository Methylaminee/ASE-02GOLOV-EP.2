/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_adc.c
** Last modified Date:  20184-12-30
** Last Version:        V1.00
** Descriptions:        functions to manage A/D interrupts
** Correlated files:    adc.h
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "lpc17xx.h"
#include "adc.h"
#include "../led/led.h"
#include "../timer/timer.h"
#include "../GLCD/glcd.h"
#include "../elevator/maintenance/maintenance.h"

/*----------------------------------------------------------------------------
  A/D IRQ: Executed when A/D Conversion is ready (signal from ADC peripheral)
 *----------------------------------------------------------------------------*/

unsigned short AD_current;   
unsigned short AD_last = 0xFF;     /* Last converted value               */
unsigned short IDX_count = 5;
/* k=1/f'*f/n  k=f/(f'*n) k=25MHz/(f'*45) */

//const int freqs[8]={2120,1890,1684,1592,1417,1263,1125,1062};
//int FRQ_count = 1263;

void ADC_IRQHandler(void) {
	
  AD_current = ((LPC_ADC->ADGDR>>4) & 0xFFF);/* Read Conversion Result             */
  if(AD_current != AD_last){
		/*	Time Rotation > LOWER, Anti Timer Rotation > HIGHER	*/
	  IDX_count = 7 - AD_current*7/0xFFF;
		//FRQ_count = freqs[IDX_count];
		AD_last = AD_current;
  }

}
