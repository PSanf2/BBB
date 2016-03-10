#include <iostream>		// pulls in cin and hex
#include <cstdio>		// pulls in printf()
#include <cstdlib>		// lets me use system()

#include "LCD.h"
#include "TriColor_LED.h"

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
	printf("\n\t 2) Hello World!");
	printf("\n\t 3) Clear");
	printf("\n\t 4) Display Control");
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
	
	unsigned int menu_choice, disp_choice, cur_choice, blink_choice;
	
	PatricksDrivers::LCD lcd(
		RS_PIN,
		EN_PIN,
		DATA4_PIN,
		DATA5_PIN,
		DATA6_PIN,
		DATA7_PIN
	);
	
	PatricksDrivers::TriColor_LED backlight(RED_PIN, GREEN_PIN, BLUE_PIN);
	
	backlight.duty(500000, 500000, 500000); // white
	
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
				lcd.clear();
				lcd.home();
				lcd.print("Hello World!");
			break;
			
			case 3:
				lcd.clear();
			break;
			
			case 4:
				printf("\n-----DISPLAY CONTROL-----");
				printf("\nDisplay On/Off? [0\\1]: ");
				getDecInput(&disp_choice);
				printf("\nCursor On/Off? [0\\1]: ");
				getDecInput(&cur_choice);
				printf("\nCursor Blink On/Off? [0\\1]: ");
				getDecInput(&blink_choice);
				
				bool disp, cur, blink;
				
				disp = (bool) disp_choice;
				cur = (bool) cur_choice;
				blink = (bool) blink_choice;
				
				lcd.onOff(disp, cur, blink);
				
			break;
			
			case 0:
				; // do nothing
			break;
			
			default:
				printf("\nINVALID SELECTION.");
			
		} // switch (menu_choice)
		
	} while (menu_choice != 0);
	
	backlight.duty(0, 0, 0);
	
	printf("Goodbye!\n");
	
	return 0;
}
