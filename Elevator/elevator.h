/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           elevator.h
** Last modified Date:  2019-12-30
** Last Version:        V1.00
** Descriptions:        Atomic elevator functions
** Correlated files:    lib_elevator.c
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

unsigned int reserveElevator(unsigned int floor);
unsigned int isElevatorArrived(void);
unsigned int moveElevator(unsigned int toFloor, unsigned int previousPosition);
void stopElevator(void);
unsigned int isAlarmDetectedFloorOne(void);
unsigned int isAlarmDetectedFloorZero(void);
