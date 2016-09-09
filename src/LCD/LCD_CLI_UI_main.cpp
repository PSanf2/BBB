#include <iostream>		// pulls in cin and hex
#include <cstdio>		// pulls in printf()
#include <cstdlib>		// lets me use system()
#include <string>

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
	printf("\n\t 5) Print string");
	printf("\n\t 6) Set cursor position");
	printf("\n\t 7) Scroll Control");
	printf("\n\t 8) Scroll Display");
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
	
	unsigned int menu_choice, disp_choice, cur_choice, blink_choice, row_choice, col_choice;
	unsigned int auto_choice, r2l_choice, dir_choice;
	string in_str;
	char in_buf[256];
	bool disp, cur, blink, aut, r2l, dir;
	
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
				printf("\nDisplay On/Off? [1\\0]: ");
				getDecInput(&disp_choice);
				printf("\nCursor On/Off? [1\\0]: ");
				getDecInput(&cur_choice);
				printf("\nCursor Blink On/Off? [1\\0]: ");
				getDecInput(&blink_choice);
				
				disp = (bool) disp_choice;
				cur = (bool) cur_choice;
				blink = (bool) blink_choice;
				
				lcd.onOff(disp, cur, blink);
				
			break;
			
			case 5:
				printf("\nInput a string: ");
				cin.getline(in_buf, 256);
				getline(cin,in_str);
				lcd.print(in_str.c_str());
				printf("\nLook at the LCD.");
			break;
			
			case 6:
				printf("\nInput row: ");
				getDecInput(&row_choice);
				printf("\nInput col: ");
				getDecInput(&col_choice);
				lcd.curPos(col_choice, row_choice);
			break;
			
			case 7:
				printf("\n-----SCROLL CONTROL-----");
				printf("\nScroll lock On/Off? [1\\0]: ");
				getDecInput(&auto_choice);
				printf("\nL2R or R2L? [1\\0]: ");
				getDecInput(&r2l_choice);
				aut = (bool) auto_choice;
				r2l = (bool) r2l_choice;
				lcd.scroll(aut, r2l);
			break;
			
			case 8:
				printf("\n-----SCROLL DISPLAY-----");
				printf("\nRight/left? [1\\0]: ");
				getDecInput(&dir_choice);
				dir = (bool) dir_choice;
				lcd.scrollDisplay(dir);
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
