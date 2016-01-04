#include <iostream>		// pulls in cin and hex
#include <cstdio>		// pulls in printf()
#include <cstdlib>		// lets me use system()

#include "ADS1X15.h"

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

void ADS1015_main(unsigned char bus, unsigned char addr) {
	unsigned int menu_choice, channel;
	
	PatricksDrivers::ADS1015 SensorBoard(bus, addr);
	
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
}

void ADS1115_main(unsigned char bus, unsigned char addr) {
	unsigned int menu_choice, channel;
	
	PatricksDrivers::ADS1115 SensorBoard(bus, addr);
	
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
}

int main(int argc, char* argv[]) {
	
	unsigned int bus, addr, type;
	
	printf("Input I2C Bus: ");
	getDecInput(&bus);
	
	printf("Input device address in hex: ");
	getHexInput(&addr);
	
	printf("Am I using an ADS1015 or ADS 1115?");
	printf("\nInput 1 for ADS1015 or 2 for ADS1115.");
	printf("\nInput selection: ");
	getDecInput(&type);
	while ((type != 1) && (type != 2)) {
		printf("\nINVALID SELECTION.");
		printf("\nInput 1 for ADS1015 or 2 for ADS1115.");
		printf("\nInput selection: ");
		getDecInput(&type);
	}
	
	if (type == 1)
		ADS1015_main(bus, addr);
	else
		ADS1115_main(bus, addr);
	
	printf("Goodbye!\n");
	
	return 0;
}
