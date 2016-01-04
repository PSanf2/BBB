/*
	Author:		Patrick Sanford
	Date:		Nov 30, 2015
	Descript:	This is the main function for my updated PCA9685 driver.
*/

#include <iostream>		// pulls in cin and hex
#include <cstdio>		// pulls in printf()

#include "PCA9685.h"

using namespace std;

// Functions to put things on the screen.
void printMenu() {
	printf("\n\t\t-----MAIN MENU-----");

	printf("\n\t 1) Set Refresh Rate");
	printf("\n\t 2) Wake");
	printf("\n\t 3) Sleep");
	printf("\n\t 4) Set Channel PWM");
	printf("\n\t 5) Set All PWM");
	printf("\n\t 6) Reset");
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
	
	printf("Input I2C Bus: ");
	unsigned int bus;
	getDecInput(&bus);
	
	printf("Input device address in hex: ");
	unsigned int dev;
	getHexInput(&dev);
	
	PatricksDrivers::PCA9685 ServoBoard(bus, dev);
	
	unsigned int menu_choice, reg, val, ch, on, off;
	
	do {
		
		printMenu();
		
		getDecInput(&menu_choice);
		
		switch (menu_choice) {
			
			case 1:
				// set refresh
				printf("\nInput refresh rate in Mhz: ");
				getDecInput(&val);
				ServoBoard.setRefresh(val);
			break;
			
			case 2:
				// wake
				ServoBoard.wake();
			break;
			
			case 3:
				// sleep
				ServoBoard.sleep();
			break;
			
			case 4:
				// set channel PWM
				printf("\nInput the channel: ");
				getDecInput(&ch);
				printf("\nInput the ON value: ");
				getDecInput(&on);
				printf("\nInput the OFF value: ");
				getDecInput(&off);
				ServoBoard.setPWM(ch, on, off);
			break;
			
			case 5:
				// set PWM for all channels
				printf("\nInput the ON value: ");
				getDecInput(&on);
				printf("\nInput the OFF value: ");
				getDecInput(&off);
				ServoBoard.setAllPWM(on, off);
			break;
			
			case 6:
				printf("Performing software reset.");
				ServoBoard.reset();
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
