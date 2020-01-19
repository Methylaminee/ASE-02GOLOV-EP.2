/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           timer.h
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        Prototypes of functions included in the lib_timer, funct_timer, IRQ_timer .c files
** Correlated files:    lib_timer.c, funct_timer.c, IRQ_timer.c
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include <stdint.h>

#ifndef __TIMER_H
#define __TIMER_H

/* init_timer.c */
extern void TIMER_init(void);
extern uint32_t timer_init_base ( uint8_t timer_num, uint32_t TimerInterval, uint32_t timer_mcr );
extern uint32_t timer_init ( uint8_t timer_num, uint32_t firstMatch, uint32_t secondMatch, uint32_t thirdMatch, uint32_t fourthMatch, uint32_t timer_mcr );
extern void timer_enable( uint8_t timer_num );
extern void timer_disable( uint8_t timer_num );
extern void timer_reset( uint8_t timer_num );
extern void timer_set_loudspeaker_freq( uint32_t timerInterval );
extern void timer_set_mr( uint8_t timer_num, uint32_t register_num, uint32_t timerInterval );
extern void timer_set_mcr( uint8_t timer_num, uint32_t setupValue );
extern void timer_set_tc( uint8_t timer_num, uint32_t timerInterval );
extern uint32_t timer_get_tc( uint8_t timer_num );
/* IRQ_timer.c */
extern void TIMER0_IRQHandler (void);
extern void TIMER1_IRQHandler (void);

#endif /* end __TIMER_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
