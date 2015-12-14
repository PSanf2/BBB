#include <iostream>		// pulls in cin and hex
#include <cstdio>		// pulls in printf()
#include <cstdlib>		// lets me use system()

#include "ADS1X15_Singleton.h"

using namespace std;

// Functions to put things on the screen.
void printMenu() {
	printf("\n\t\t-----MAIN MENU-----");
	printf("\n\t 1) Read Single Ended");
	printf("\n\t 2) Read Differential 0-1");
	printf("\n\t 3) Read Differential 2-3");
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

int main(int argc, char* argv[]) {
	
	unsigned int menu_choice, bus, addr, channel;
	
	printf("Input I2C Bus: ");
	getDecInput(&bus);
	
	printf("Input device address in hex: ");
	getHexInput(&addr);
	
	PatricksDrivers::ADS1115_Singleton SensorBoard(bus, addr);
	
	//system("clear");
	
	do {
		
		printMenu();
		
		getDecInput(&menu_choice);
		
		switch (menu_choice) {
			case 1:
				printf("Input channel: ");
				getDecInput(&channel);
				printf("\nA%i = %i", channel, SensorBoard.readADC_SingleEnded(channel));
			break;
			
			case 2:
				printf("Differential 0-1 = %i", SensorBoard.readADC_Differential_0_1());
			break;
			
			case 3:
				printf("Differential 2-3 = %i", SensorBoard.readADC_Differential_2_3());
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
