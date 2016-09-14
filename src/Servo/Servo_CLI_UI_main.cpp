#include <iostream>		// pulls in cin and hex
#include <cstdio>		// pulls in printf()
#include <cstdlib>		// lets me use system()

#include "Servo.h"

#define SERVO_PIN "P9_22"
#define SERVO_PIN_2 "P9_42" // don't put the second servo on the same PWM channel.

using namespace std;

// Functions to put things on the screen.
void printMenu() {
	printf("\n\t\t-----MAIN MENU-----");
	printf("\n\t 1) Print Menu");
	printf("\n\t 2) Start");
	printf("\n\t 3) Stop");
	printf("\n\t 4) Set All Duty");
	printf("\n\t 5) Set Servo 1 Duty");
	printf("\n\t 6) Set Servo 2 Duty");
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
	unsigned int n;
	
	PatricksDrivers::Servo my_servo(SERVO_PIN);
	PatricksDrivers::Servo my_servo_2(SERVO_PIN_2);
	
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
				my_servo.start();
				my_servo_2.start();
			break;
			
			case 3:
				my_servo.stop();
				my_servo_2.stop();
			break;
			
			case 4:
				printf("\nInput duty: ");
				getDecInput(&n);
				my_servo.duty(n);
				my_servo_2.duty(n);
			break;
			
			case 5:
				printf("\nInput duty: ");
				getDecInput(&n);
				my_servo.duty(n);
			break;
			
			case 6:
				printf("\nInput duty: ");
				getDecInput(&n);
				my_servo_2.duty(n);
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
