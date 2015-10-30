/*
	Author	:	Patrick Sanford
	Date	:	Sept 20, 2015
	File	:	CLI_UI_main.cpp for LSM9DS0
	Version	:	0.1
*/

#include <iostream>		// pulls in cin and hex
#include <cstdio>		// pulls in printf()

using namespace std;

// Functions to put things on the screen.
void printMenu() {
	printf("\n\t\t-----MAIN MENU-----");
	printf("\n\t 1) Print Main Menu");
	
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
	
	/*
	printf("Input I2C Bus: ");
	unsigned int bus;
	getDecInput(&bus);
	
	printf("Input device address in hex: ");
	unsigned int dev;
	getHexInput(&dev);
	
	PatricksDrivers::PWM_Board ServoBoard(bus, dev);
	*/
	
	unsigned int menu_choice;//, reg, val, ch, on, off;
	
	do {
		
		printMenu();
		
		getDecInput(&menu_choice);
		
		switch (menu_choice) {
			case 1:
				printMenu();
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
