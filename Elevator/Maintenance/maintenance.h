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

typedef struct loudspeaker_note {
	int frequency;
	uint8_t description[10];
} note;

void goToHome(void);
void goToSetup(void);
void selectNote(unsigned int isNoteA);
void updateNote(void);
void saveNote(void);
