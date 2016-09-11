/*
 * The LED needs to blink at a rate of 38kHz in order for the IR sensor
 * to detect anything. This can be done by driving the LED w/ a GPIO
 * pin, and turning it on and off rapidly.
 * Turn on, wait 13 microseconds, turn off, wait 13 microseconds, repeat.
 * 
 * NOPE. Need to use PWM pin to get a 38kHz signal to the LED. Just blinking
 * it w/ a GPIO doesn't work for shit.
 * 
 * Using a 220 ohm resistor seems to work best on the LED.
 * Want to make it bright, but don't damage the pin.
 * 220 is as low as I'd take it.
 * 
 * The IR sensor uses a 5v reference that requires very little current.
 * The Arduino is capable of powering the sensor, and it doesn't
 * register any pull on my bench power supply. I believe it'll be safe
 * to run the sensor directly off the BBB. The sensor can operate between
 * 3.3v and 5v. Because the GPIO pins are 3.3v tolerant I'll need to use
 * the VDD_3V3 reference off pin P9_03/04 for the sensor. I should be
 * able to connect the sensor just like a button, and use it in a
 * similar manner. The difference is that the sensor stays high unless
 * it gets a signal, so I need to watch for inputs on the falling edge.
 * 
 * I'll need to use multi-threading to pull this off.
 * The LED will need to have a thread to blink the light. That will
 * need to keep running until told to quit, or until the program terminates.
 * The sensor itself will need to have a thread with a callback function to
 * handle events.
 */

#include <iostream>		// pulls in cin and hex
#include <cstdio>		// pulls in printf()
#include <cstdlib>		// lets me use system()

#include "IR_LED.h"
#include "IR_Sensor.h"

#define IR_LED_PIN "P8_19"

using namespace std;

// Functions to put things on the screen.
void printMenu() {
	printf("\n\t\t-----MAIN MENU-----");
	printf("\n\t 1) Print Menu");
	printf("\n\t 2) Run IR LED");
	printf("\n\t 3) Stop IR LED");
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
	
	//PatricksDrivers::LED red_light(RED_LED_PIN);
	PatricksDrivers::IR_LED ir_light(IR_LED_PIN);
	
	//system("clear");
	
	do {
		
		printMenu();
		
		getDecInput(&menu_choice);
		
		//system("clear");
		
		switch (menu_choice) {
			
			case 1:
				printMenu();
			break; // case 1
			
			case 2:
				ir_light.run();
			break; // case 4
			
			case 3:
				ir_light.stop();
			break; // case 5
			
			case 0:
				; // do nothing
			break; // case 0
			
			default:
				printf("\nINVALID SELECTION.");
			
		} // switch (menu_choice)
		
	} while (menu_choice != 0);
	
	printf("Goodbye!\n");
	
	return 0;
}
