/*
	Author	:	Patrick Sanford
	Date	:	Sept 20, 2015
	File	:	CLI_UI_main.cpp for LSM9DS0
	Version	:	0.1
*/

#include <iostream>		// pulls in cin and hex
#include <cstdio>		// pulls in printf()

#include "LSM9DS0.h"

using namespace std;

// Functions to put things on the screen.
void printMenu() {
	printf("\n\t\t-----MAIN MENU-----");
	printf("\n\t 1) Print Main Menu");
	printf("\n\t 2) Read Accel");
	printf("\n\t 3) Read Mag");
	printf("\n\t 4) Read Gyro");
	printf("\n\t 5) Read Temp");
	printf("\n\t 0) Quit");
	printf("\nInput selection ");
}

// Functions to get inputs
void getHexInput(unsigned int *ptr) {
	for (;;) {
		if (cin >> hex >> *ptr) {
			return;
		} else {
			printf("INVALID INPUT.");
			printf("\nInput a hex number: ");
			cin.clear();
			cin.ignore(80, '\n');
		}
	}
}

void getDecInput(unsigned int *ptr) {
	for (;;) {
		if (cin >> dec >> *ptr) {
			return;
		} else {
			printf("INVALID INPUT.");
			printf("\nInput a dec number: ");
			cin.clear();
			cin.ignore(80, '\n');
		}
	}
}

// I want to support command line arguments.
int main(int argc, char* argv[]) {
	
	LSM9DS0 SensorBoard(1, LSM9DS0_ACCELRANGE_2G, LSM9DS0_MAGGAIN_2GAUSS, LSM9DS0_GYROSCALE_245DPS);
	
	unsigned int menu_choice;//, reg, val, ch, on, off;
	
	do {
		
		printMenu();
		
		getDecInput(&menu_choice);
		
		switch (menu_choice) {
			case 1:
				printMenu();
			break;
			
			case 2:
				SensorBoard.readAccel();
			break;
			
			case 3:
				SensorBoard.readMag();
			break;
			
			case 4:
				SensorBoard.readGyro();
			break;
			
			case 5:
				SensorBoard.readTemp();
			break;
			
			case 0:
				; // do nothing
			break;
			
			default:
				printf("\nINVALID SELECTION.");
			
		}
		
	} while (menu_choice != 0);
	
	printf("Goodbye!\n");
	
	return 0;
}
