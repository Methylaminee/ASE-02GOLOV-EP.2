/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           RIT.h
** Last modified Date:  2019-12-30
** Last Version:        V1.00
** Descriptions:        Prototypes of functions included in the lib_RIT, funct_RIT, IRQ_RIT .c files
** Correlated files:    lib_RIT.c, funct_RIT.c, IRQ_RIT.c
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#ifndef __RIT_H
#define __RIT_H

/* init_RIT.c */
extern uint32_t RIT_init(uint32_t RITInterval);
extern void RIT_enable( void );
extern void RIT_disable( void );
extern void RIT_reset( void );
/* IRQ_RIT.c */
extern void RIT_IRQHandler (void);

#endif
