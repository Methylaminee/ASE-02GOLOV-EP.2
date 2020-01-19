/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           maintenance.h
** Last modified Date:  2019-12-30
** Last Version:        V1.00
** Descriptions:        Atomic elevator functions
** Correlated files:    lib_maintenance.c
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "stdint.h"

void goToHome(void);
void goToSetup(void);
void selectNote(unsigned int isNoteA);
void updateNote(unsigned int isNoteA, int noteValue);
void saveNote(unsigned int isNoteA);
uint8_t* formatIntToString(uint16_t number);
