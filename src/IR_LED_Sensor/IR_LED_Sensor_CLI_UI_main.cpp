#include <iostream>		// pulls in cin and hex
#include <cstdio>		// pulls in printf()
#include <cstdlib>		// lets me use system()
#include <ctime>

#include "IR_LED_Sensor.h"

/*
 * Notes on pin selection.
 * You want the IR LED pins to be on different PWM channels.
 * If they're on the same channel then they won't blink reliably.
 * 
 * You want the LEDs and sensors to use seperate ground pins on the BBB.
 * If you use the same ground trace then you end up getting feedback from
 * the LEDs on the sensor channels.
 */

#define IR_LED_PIN_1 "P8_19"
#define IR_SENSOR_PIN_1 "P8_12"

#define IR_LED_PIN_2 "P9_14"
//#define IR_LED_PIN_2 "P8_13" // don't put the second IR LED on the same PWM channel
#define IR_SENSOR_PIN_2 "P8_10"

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

const int debounceDelay = 5000;

clock_t lastDebounceTime1;

int IR_Sensor_callback_1(int var) {
	if ((clock() - lastDebounceTime1) > debounceDelay) {
		printf("\nBit detected on sensor 1.");
	}
	lastDebounceTime1 = clock();
}

clock_t lastDebounceTime2;

int IR_Sensor_callback_2(int var) {
	if ((clock() - lastDebounceTime2) > debounceDelay) {
		printf("\nBit detected on sensor 2.");
	}
	lastDebounceTime2 = clock();
}

int main(int argc, char* argv[]) {
	
	unsigned int menu_choice;
	
	PatricksDrivers::IR_LED_Sensor Sensor1(IR_LED_PIN_1, IR_SENSOR_PIN_1);
	PatricksDrivers::IR_LED_Sensor Sensor2(IR_LED_PIN_2, IR_SENSOR_PIN_2);
	
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
				Sensor2.run(&IR_Sensor_callback_2);
			break;
			
			case 3:
				Sensor1.stop();
				Sensor2.stop();
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
