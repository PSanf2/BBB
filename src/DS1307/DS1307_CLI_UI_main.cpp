#include <iostream>		// pulls in cin and hex
#include <cstdio>		// pulls in printf()
#include <cstdlib>		// lets me use system()

#include "DS1307.h"

using namespace std;

// Functions to put things on the screen.
void printMenu() {
	printf("\n\t\t-----MAIN MENU-----");
	printf("\n\t 1) Print Status");
	printf("\n\t 2) Read Time");
	printf("\n\t 3) Write Time");
	printf("\n\t 4) Toggle Clock Halt");
	printf("\n\t 5) Toggle 12/24 Hour Mode");
	printf("\n\t 6) Toggle Output Control");
	printf("\n\t 7) Toggle Square-wave Enable");
	printf("\n\t 8) Set Square Wave Frequency");
	printf("\n\t 9) Memory Read");
	printf("\n\t10) Memory Write");
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
	
	unsigned int menu_choice, bus;
	
	printf("Input I2C Bus: ");
	getDecInput(&bus);
	
	PatricksDrivers::DS1307 RTClock(bus);
	
	system("clear");
	
	do {
		
		printMenu();
		
		getDecInput(&menu_choice);
		
		system("clear");
		
		switch (menu_choice) {
			
			case 1:
				printf("\n-----STATUS-----");
				
				if (RTClock.CH_enabled())
					printf("\n\tClock Halt: ENABLED");
				else
					printf("\n\tClock Halt: DISABLED");
				
				if (RTClock.read_mode() == PatricksDrivers::TWENTYFOURHOURMODE)
					printf("\n\t24 Hour Mode");
				else
					printf("\n\t12 Hour Mode");
				
				if (RTClock.read_out() == PatricksDrivers::LOW)
					printf("\n\tOutput Contorl: LOW");
				else
					printf("\n\tOutput Contorl: HIGH");
				
				if (RTClock.SQWE_enabled())
					printf("\n\tSquare Wave: ENABLED");
				else
					printf("\n\tSquare Wave: DISABLED");
				
				switch (RTClock.read_rate()) {
					case PatricksDrivers::ONE_HZ:
						printf("\n\tFrequency: 1Hz");
					break;
					case PatricksDrivers::FOUR_KHZ:
						printf("\n\tFrequency: 4.096kHz");
					break;
					case PatricksDrivers::EIGHT_KHZ:
						printf("\n\tFrequency: 8.192kHz");
					break;
					case PatricksDrivers::THIRTYTWO_KHZ:
						printf("\n\tFrequency: 32.768kHz");
					break;
				} // switch (RTClock.read_rate())
			break; // case 1
			
			case 2:
				
			break;
			
			case 3:
				
			break;
			
			case 4:
				
			break;
			
			case 5:
				
			break;
			
			case 6:
				
			break;
			
			case 7:
				
			break;
			
			case 8:
				
			break;
			
			case 9:
				
			break;
			
			case 10:
				
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
