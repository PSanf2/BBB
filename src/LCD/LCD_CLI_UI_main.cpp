#include <iostream>		// pulls in cin and hex
#include <cstdio>		// pulls in printf()
#include <cstdlib>		// lets me use system()

#include "LCD.h"

#define RS_PIN		"P8_08"
#define EN_PIN		"P8_10"
#define DATA4_PIN	"P8_18"
#define DATA5_PIN	"P8_16"
#define DATA6_PIN	"P8_14"
#define DATA7_PIN	"P8_12"
#define RED_PIN		"P9_16"
#define GREEN_PIN	"P9_14"
#define BLUE_PIN	"P8_13"

using namespace std;

// Functions to put things on the screen.
void printMenu() {
	printf("\n\t\t-----MAIN MENU-----");
	printf("\n\t 1) Print Menu");
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
	
	PatricksDrivers::LCD_RGB_PWM my_lcd(
		RS_PIN,
		EN_PIN,
		DATA4_PIN,
		DATA5_PIN,
		DATA6_PIN,
		DATA7_PIN,
		RED_PIN,
		GREEN_PIN,
		BLUE_PIN
	);
	
	//system("clear");
	
	do {
		
		printMenu();
		
		getDecInput(&menu_choice);
		
		//system("clear");
		
		switch (menu_choice) {
			
			case 1:
				printMenu();
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
