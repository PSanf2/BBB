#include <iostream>		// pulls in cin and hex
#include <cstdio>		// pulls in printf()
#include <cstdlib>		// lets me use system()

#include "TCA9548A.h"

using namespace std;

// Functions to put things on the screen.
void printMenu() {
	printf("\n\t\t-----MAIN MENU-----");
	printf("\n\t 1) Set channel");
	printf("\n\t 2) Disable all channels");
	printf("\n\t 3) Print current active channels");
	printf("\n\t 4) Enable a channel");
	printf("\n\t 5) Disable a channel");
	printf("\n\t 6) Check if a channel is enabled.");
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
	
	unsigned int menu_choice, bus, addr, channel, curr;
	
	printf("Input I2C Bus: ");
	getDecInput(&bus);
	
	printf("Input device address in hex: ");
	getHexInput(&addr);
	
	PatricksDrivers::TCA9548A I2CMultiplexer(bus, addr);
	
	system("clear");
	
	do {
		
		printMenu();
		
		getDecInput(&menu_choice);
		
		system("clear");
		
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
				curr = I2CMultiplexer.current();
				printf("\nCurrnet active channels: 0x%X", curr);
				for (int i = 0; i < 8; i++) {
					if (curr & (1 << i))
						printf("\nChannel %u is enabled.", i);
				}
			break;
			
			case 4:
				printf("Input channel: ");
				getDecInput(&channel);
				I2CMultiplexer.enable(channel);
			break;
			
			case 5:
				printf("Input channel: ");
				getDecInput(&channel);
				I2CMultiplexer.disable(channel);
			break;
			
			case 6:
				printf("Input channel: ");
				getDecInput(&channel);
				if (I2CMultiplexer.enabled(channel))
					printf("Channel %u is enabled", channel);
				else
					printf("Channel %u is NOT enabled", channel);
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
