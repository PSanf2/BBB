/*
 * 
 */

#include <iostream>		// pulls in cin and hex
#include <cstdio>		// pulls in printf()
#include <cstdlib>		// lets me use system()
#include <ctime>

#include "IR_LED_Sensor.h"

#define IR_LED_PIN_1 "P8_19"
#define IR_SENSOR_PIN_1 "P8_12"

using namespace std;

// Functions to put things on the screen.
void printMenu() {
	printf("\n\t\t-----MAIN MENU-----");
	printf("\n\t 1) Print Menu");
	printf("\n\t 2) Run");
	printf("\n\t 3) Stop");
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

clock_t lastDebounceTime1;
const int debounceDelay = 5000;

int IR_Sensor_callback_1(int var) {
	if ((clock() - lastDebounceTime1) > debounceDelay) {
		printf("\nBit detected on sensor 1.");
	}
	lastDebounceTime1 = clock();
}

int main(int argc, char* argv[]) {
	
	unsigned int menu_choice;
	
	PatricksDrivers::IR_LED_Sensor Sensor1(IR_LED_PIN_1, IR_SENSOR_PIN_1);
	
	//system("clear");
	
	do {
		
		printMenu();
		
		getDecInput(&menu_choice);
		
		//system("clear");
		
		switch (menu_choice) {
			
			case 1:
				printMenu();
			break; // case 1
			
			case 2:
				Sensor1.run(&IR_Sensor_callback_1);
			break;
			
			case 3:
				Sensor1.stop();
			break;
			
			case 0:
				; // do nothing
			break; // case 0
			
			default:
				printf("\nINVALID SELECTION.");
			
		} // switch (menu_choice)
		
	} while (menu_choice != 0);
	
	printf("Goodbye!\n");
	
	return 0;
}
