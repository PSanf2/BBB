#include <iostream>		// pulls in cin and hex
#include <cstdio>		// pulls in printf()
#include <cstdlib>		// lets me use system()

#include "TCA9548A_Singleton.h"

using namespace std;

// Functions to put things on the screen.
void printMenu() {
	printf("\n\t\t-----MAIN MENU-----");
	printf("\n\t 1) Set channel");
	printf("\n\t 2) Disable all channels");
	printf("\n\t 3) Print current active channels");
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
	
	PatricksDrivers::TCA9548A_Singleton I2CMultiplexer(bus, addr);
	
	do {
		
		printMenu();
		
		getDecInput(&menu_choice);
		
		switch (menu_choice) {
			case 1:
				printf("Input channel: ");
				getDecInput(&channel);
				I2CMultiplexer.select(channel);
			break;
			
			case 2:
				printf("\nAll channels disabled.");
				I2CMultiplexer.disableAll();
			break;
			
			case 3:
				printf("\nCurrnet active channels: %X", I2CMultiplexer.current());
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
