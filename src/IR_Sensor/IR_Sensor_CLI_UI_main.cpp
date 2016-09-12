/*
 * The BBB is set up as follows.
 * P9_09 -> 220 ohm resistor -> IR LED -> P8_02(DGND)
 * P9_04(VDD_3V3) -> IR Sensor -> P8_12 & P9_02(DGND)
 * 
 * The way the sensor and LED are oriented matter.
 * The LED and sensor should be at about the same height, and covered with a minimal amount of
 * shrink tubing on them. Each component should peek above the shrink tubing just a little bit.
 * 
 */

#include <iostream>		// pulls in cin and hex
#include <cstdio>		// pulls in printf()
#include <cstdlib>		// lets me use system()
#include <ctime>

#include "IR_LED.h"
#include "IR_Sensor.h"

#define IR_LED_PIN "P8_19"
#define IR_SENSOR_PIN "P8_12"

using namespace std;

// Functions to put things on the screen.
void printMenu() {
	printf("\n\t\t-----MAIN MENU-----");
	printf("\n\t 1) Print Menu");
	printf("\n\t 2) Run IR LED");
	printf("\n\t 3) Stop IR LED");
	printf("\n\t 4) Listen for bits");
	printf("\n\t 5) Stop listening");
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

clock_t lastDebounceTime;
const int debounceDelay = 5000;

int IR_Sensor_callback(int var) {
	if ((clock() - lastDebounceTime) > debounceDelay) {
		printf("\nBit detected.");
	}
	lastDebounceTime = clock();
}

int main(int argc, char* argv[]) {
	
	unsigned int menu_choice;
	
	PatricksDrivers::IR_LED ir_light(IR_LED_PIN);
	PatricksDrivers::IR_Sensor ir_sensor(IR_SENSOR_PIN);
	
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
				ir_light.run();
			break; // case 2
			
			case 3:
				ir_light.stop();
			break; // case 3
			
			case 4:
				// listen for bits
				ir_sensor.run(&IR_Sensor_callback);
			break; // case 4
			
			case 5:
				// stop listening
				ir_sensor.stop();
			break; // case 5
			
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
