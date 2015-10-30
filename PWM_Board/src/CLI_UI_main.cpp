/*
	Author	:	Patrick Sanford
	Date	:	Sept 20, 2015
	File	:	CLI_UI_main.cpp
	Version	:	0.1
*/

#include <iostream>		// pulls in cin and hex
#include <cstdio>		// pulls in printf()

#include "PWM_Board.h"

using namespace std;

// Functions to put things on the screen.
void printMenu() {
	printf("\n\t\t-----MAIN MENU-----");
	printf("\n\t 1) Print Main Menu");
	printf("\n\t 2) Print Board Status");
	printf("\n\t 3) Read Register");
	printf("\n\t 4) Write Register");
	printf("\n\t 5) Print All Registers");
	printf("\n\t 6) Set Refresh Rate");
	printf("\n\t 7) Wake");
	printf("\n\t 8) Sleep");
	printf("\n\t 9) Set Channel PWM");
	printf("\n\t10) Set All PWM");
	printf("\n\t11) Reset");
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
	
	printf("Input I2C Bus: ");
	unsigned int bus;
	getDecInput(&bus);
	
	printf("Input device address in hex: ");
	unsigned int dev;
	getHexInput(&dev);
	
	PatricksDrivers::PWM_Board ServoBoard(bus, dev);
	
	unsigned int menu_choice, reg, val, ch, on, off;
	
	do {
		
		printMenu();
		
		getDecInput(&menu_choice);
		
		switch (menu_choice) {
			case 1:
				printMenu();
			break;
			
			case 2:
				printf("\n\tPWM_Board Status");
				printf("\n\tbus: 0x%X", ServoBoard.get_bus());
				printf("\n\taddr: 0x%X", ServoBoard.get_addr());
				printf("\n\terror_code: %i", ServoBoard.get_error());
				printf("\n\tis_open() = %s", (ServoBoard.is_open() ? "TRUE" : "FALSE"));
			break;
			
			case 3:
				// read register
				printf("Input register address in hex: ");
				getHexInput(&reg);
				printf("\n\tRegister: 0x%X", reg);
				printf("\tValue: 0x%X", ServoBoard.readRegister(reg));
			break;
			
			case 4:
				// write register
				printf("Input register address in hex: ");
				getHexInput(&reg);
				printf("Input new value in hex: ");
				getHexInput(&val);
				ServoBoard.writeRegister(reg, val);
			break;
			
			case 5:
				// print all registers
				printf("\n\t  0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F");
	
				for (unsigned int i = 0x0; i <= 0xFF; i++) {
					
					if (i % 0x10 == 0) {
						if (i == 0x0) {
							printf("\n0x0%X\t", i);
						} else {
							printf("\n0x%X\t", i);
						}
					}
					
					if (i >= 0x46 && i <= 0xF9) {
						printf(" XX");
						continue;
					}
					
					unsigned char val = ServoBoard.readRegister(i);
					
					if (val < 0x10) {
						printf("  %X", val);
					} else {
						printf(" %X", ServoBoard.readRegister(i));
					}
				}
			break;
			
			case 6:
				// set refresh
				printf("\nInput refresh rate in Mhz: ");
				getDecInput(&val);
				ServoBoard.setRefresh(val);
			break;
			
			case 7:
				// wake
				ServoBoard.wake();
			break;
			
			case 8:
				// sleep
				ServoBoard.sleep();
			break;
			
			case 9:
				printf("\nInput the channel: ");
				getDecInput(&ch);
				printf("\nInput the ON value: ");
				getDecInput(&on);
				printf("\nInput the OFF value: ");
				getDecInput(&off);
				ServoBoard.setPWM(ch, on, off);
			break;
			
			case 10:
				// set PWM for all channels
				printf("\nInput the ON value: ");
				getDecInput(&on);
				printf("\nInput the OFF value: ");
				getDecInput(&off);
				ServoBoard.setAllPWM(on, off);
			break;
			
			case 11:
				printf("Performing software reset.");
				ServoBoard.reset();
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
