/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           button.h
** Last modified Date:  2019-12-29
** Last Version:        V1.00
** Descriptions:        Prototypes of functions included in the lib_button, IRQ_button.c files
** Correlated files:    lib_button.c, funct_button.c
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

/* lib_button */
void BUTTON_init(void);

/* IRQ_button */
void EINT1_IRQHandler(void);
void EINT2_IRQHandler(void);
void EINT3_IRQHandler(void);

