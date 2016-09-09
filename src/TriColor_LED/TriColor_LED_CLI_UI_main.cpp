#include <iostream>		// pulls in cin and hex
#include <cstdio>		// pulls in printf()
#include <cstdlib>		// lets me use system()

#include "TriColor_LED.h"

#define RED_PIN "P9_16"
#define GREEN_PIN "P9_14"
#define BLUE_PIN "P8_13"

using namespace std;

// Functions to put things on the screen.
void printMenu() {
	printf("\n\t\t-----MAIN MENU-----");
	printf("\n\t 1) Print Menu");
	printf("\n\t 2) Start all");
	printf("\n\t 3) Stop all");
	printf("\n\t 4) Set RGB");
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
	
	unsigned int menu_choice;
	
	PatricksDrivers::TriColor_LED my_led(RED_PIN, GREEN_PIN, BLUE_PIN);
	
	//system("clear");
	
	do {
		
		printMenu();
		
		getDecInput(&menu_choice);
		
		//system("clear");
		
		switch (menu_choice) {
			
			case 1:
				printMenu();
			break;
			
			case 2:
				// start all
				my_led.start();
			break;
			
			case 3:
				// stop all
				my_led.stop();
			break;
			
			case 4:
				// set RGB
				unsigned int r, g, b;
				printf("\nInput duty for r ");
				getDecInput(&r);
				printf("\nInput duty for g ");
				getDecInput(&g);
				printf("\nInput duty for b ");
				getDecInput(&b);
				my_led.duty(r, g, b);
			break;
			
			case 0:
				; // do nothing
			break;
			
			default:
				printf("\nINVALID SELECTION.");
			
		} // switch (menu_choice)
		
	} while (menu_choice != 0);
	
	printf("Goodbye!\n");
	
	return 0;
}
