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

const unsigned int DEFAULT_SCREEN_BG = 0xB4DA;

extern unsigned int isElevatorFree;
extern unsigned int isOnMaintenance;
extern const unsigned int TOUCH_TIMER;

uint8_t *note_one_value;
uint8_t *note_two_value;
unsigned int isHomePage = 1;
unsigned int isNoteASelected = 1;

void goToHome() {
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
	//LCD_DrawCircle(190, 50, 40, White, Black);
	//LCD_DrawCircle(190, 50, 25, White, Black);
	//LCD_DrawBitmap(120, 160, 48, 48, POWER_IMAGE_PIXEL_DATA, POWER_IMAGE_BYTES_PER_PIXEL);
	if (isElevatorFree) {
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
		GUI_Text(80, 110,  note_one_value, DEFAULT_SCREEN_BG, White);
		GUI_Text(66, 195, (uint8_t *) "SELECT NOTE 2", White, DEFAULT_SCREEN_BG);
		GUI_Text(80, 220, (uint8_t *) "523 Hz - C", White, DEFAULT_SCREEN_BG);
		isNoteASelected = 1;
	} else {
		LCD_DrawRectangle(30, 80, 180, 20, DEFAULT_SCREEN_BG, 1);
		LCD_DrawRectangle(30, 105, 180, 30, DEFAULT_SCREEN_BG, 1);
		LCD_DrawRectangle(30, 190, 180, 20, White, 1);
		LCD_DrawRectangle(30, 215, 180, 30, White, 1);
		GUI_Text(66, 85, (uint8_t *) "SELECT NOTE 1", White, DEFAULT_SCREEN_BG);
		GUI_Text(80, 110, note_one_value, White, DEFAULT_SCREEN_BG);
		GUI_Text(66, 195, (uint8_t *) "SELECT NOTE 2", DEFAULT_SCREEN_BG, White);
		GUI_Text(80, 220, (uint8_t *) "523 Hz - C", DEFAULT_SCREEN_BG, White);
		isNoteASelected = 0;
	}
	LED_Out(0x0);
}

void updateNote(unsigned int isNoteA, int noteValue) {
	if (isNoteA) {
		GUI_Text(80, 110, (uint8_t *)&noteValue, DEFAULT_SCREEN_BG, White);  
	} else {
		GUI_Text(80, 220, (uint8_t *)&noteValue, DEFAULT_SCREEN_BG, White);
	}
}

void saveNote(unsigned int noteValue) {
	if (isNoteASelected) {
		//note_one_value = noteValue;
	} else {
		//note_two_value = noteValue;
	}
}
