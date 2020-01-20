/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           lib_elevator.c
** Last modified Date:  2019-12-30
** Last Version:        V1.00
** Descriptions:        Atomic elevator functions
** Correlated files:    elevator.h
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/   

#include "stdio.h"
#include "maintenance.h"
#include "../../led/led.h"
#include "../../GLCD/glcd.h"
#include "../../timer/timer.h"
#include "../../adc/adc.h"

const unsigned int SIN_POINTS = 0x2D; //45
const unsigned int DEFAULT_SCREEN_BG = 0xB4DA;

/* k=1/f'*f/n  k=f/(f'*n) k=25MHz/(f'*45) */
note octave[8] = {
	{2120, "262Hz - C4"},
	{1890, "294Hz - D4"},
	{1684, "330Hz - E4"},
	{1592, "349Hz - F4"},
	{1417, "392Hz - G4"},
	{1263, "440Hz - A4"},
	{1125, "494Hz - B4"},
	{1062, "523Hz - C5"}
};

uint16_t SinTable[SIN_POINTS] =                                      
{
    410, 467, 523, 576, 627, 673, 714, 749, 778,
    799, 813, 819, 817, 807, 789, 764, 732, 694, 
    650, 602, 550, 495, 438, 381, 324, 270, 217,
    169, 125, 87 , 55 , 30 , 12 , 2  , 0  , 6  ,   
    20 , 41 , 70 , 105, 146, 193, 243, 297, 353
};

extern unsigned int isElevatorFree;
extern unsigned int isOnMaintenance;
extern const unsigned int TOUCH_TIMER;
extern const unsigned int REFRESH_TIMER;
extern const unsigned int REFRESH_TIMER_MAX_VALUE;

extern int IDX_count;

int note_one_idx = 5;
int note_two_idx = 5;
unsigned int isHomePage = 1;
unsigned int isNoteASelected = 1;

void goToHome() {
	if (isElevatorFree) {
		LCD_Clear(Black);
		LCD_DrawLine(160, 20, 220, 20, White);
		LCD_DrawLine(160, 80, 220, 80, White);
		LCD_DrawLine(160, 20, 160, 80, White);
		LCD_DrawLine(220, 20, 220, 80, White);
		
		LCD_DrawLine(180, 40, 185, 40, White);
		LCD_DrawLine(195, 40, 200, 40, White);
		LCD_DrawLine(180, 60, 200, 60, White);
		LCD_DrawLine(180, 40, 180, 60, White);
		LCD_DrawLine(200, 40, 200, 60, White);
		
		LCD_DrawLine(190, 35, 190, 45, White);
		GUI_Text(0, 285, (uint8_t *) " touch power", Black, White);
	} else {
		GUI_Text(0, 285, (uint8_t *) " free elevator", Black, White);
	}
	GUI_Text(0, 300, (uint8_t *) " to enter maintenance", Black, White);
	isHomePage = 1;
	isOnMaintenance = 0;
}

void goToSetup() {
	LCD_Clear(DEFAULT_SCREEN_BG);
	GUI_Text(76, 10, (uint8_t *) "MAINTENANCE", Yellow, DEFAULT_SCREEN_BG);
	
	LCD_DrawLine(40, 160, 200, 160, White);
	selectNote(1);
	
	LCD_DrawRectangle(10, 280, 100, 30, Green, 1);
	LCD_DrawRectangle(130, 280, 100, 30, Red, 1);
	
	GUI_Text(45, 288, (uint8_t *) "SAVE", White, Green);
	GUI_Text(165, 288, (uint8_t *) "QUIT", White, Red);
	isHomePage = 0;
	isOnMaintenance = 1;
	LED_Out(0x0);
}

void selectNote(unsigned int isNoteA) {
	if (isNoteA) {
		LCD_DrawRectangle(30, 80, 180, 20, White, 1);
		LCD_DrawRectangle(30, 105, 180, 30, White, 1);
		LCD_DrawRectangle(30, 190, 180, 20, DEFAULT_SCREEN_BG, 1);
		LCD_DrawRectangle(30, 215, 180, 30, DEFAULT_SCREEN_BG, 1);
		GUI_Text(66, 85, (uint8_t *) "SELECT NOTE 1", DEFAULT_SCREEN_BG, White);
		GUI_Text(80, 110, octave[IDX_count].description, DEFAULT_SCREEN_BG, White);
		GUI_Text(66, 195, (uint8_t *) "SELECT NOTE 2", White, DEFAULT_SCREEN_BG);
		GUI_Text(80, 220, octave[note_two_idx].description, White, DEFAULT_SCREEN_BG);
		isNoteASelected = 1;
	} else {
		LCD_DrawRectangle(30, 80, 180, 20, DEFAULT_SCREEN_BG, 1);
		LCD_DrawRectangle(30, 105, 180, 30, DEFAULT_SCREEN_BG, 1);
		LCD_DrawRectangle(30, 190, 180, 20, White, 1);
		LCD_DrawRectangle(30, 215, 180, 30, White, 1);
		GUI_Text(66, 85, (uint8_t *) "SELECT NOTE 1", White, DEFAULT_SCREEN_BG);
		GUI_Text(80, 110, octave[note_one_idx].description, White, DEFAULT_SCREEN_BG);
		GUI_Text(66, 195, (uint8_t *) "SELECT NOTE 2", DEFAULT_SCREEN_BG, White);
		GUI_Text(80, 220, octave[IDX_count].description, DEFAULT_SCREEN_BG, White);
		isNoteASelected = 0;
	}
	LED_Out(0x0);
}

void updateNote() {
	if (isNoteASelected) {
		GUI_Text(80, 110, octave[IDX_count].description, DEFAULT_SCREEN_BG, White); 
	} else {
		GUI_Text(80, 220, octave[IDX_count].description, DEFAULT_SCREEN_BG, White);
	}
	LED_Out(0x0);
}

void saveNote() {
	if (isNoteASelected) {
		GUI_Text(70, 35, (uint8_t *) "NOTE 1 SAVED", DEFAULT_SCREEN_BG, Green);
		note_one_idx = IDX_count;
	} else {
		GUI_Text(70, 35, (uint8_t *) "NOTE 2 SAVED", DEFAULT_SCREEN_BG, Green);
		note_two_idx = IDX_count;
	}
	LPC_TIM2->MR0 = REFRESH_TIMER_MAX_VALUE;
	timer_enable(REFRESH_TIMER);
	LED_Out(0x0);
}
