#include <iostream>		// pulls in cin and hex
#include <cstdio>		// pulls in printf()
#include <cstdlib>		// lets me use system()

#include "LED.h"
#include "Button.h"

#define RED_LED_PIN "P8_14"
#define GREEN_LED_PIN "P8_16"
#define BUTTON_PIN "P8_12"

using namespace std;

PatricksDrivers::LED red_light(RED_LED_PIN);
PatricksDrivers::LED green_light(GREEN_LED_PIN);
PatricksDrivers::Button push_btn(BUTTON_PIN);

bool state = false;

int my_callback(int var) {
	if (state) {
		red_light.off();
		green_light.on();
	} else {
		red_light.on();
		green_light.off();
	}
	state = !state;
	return 0;
}

int main(int argc, char* argv[]) {
	cout << endl;
	cout << "BLOCKING WAIT TEST" << endl;
	cout << "Push the button!" << endl;
	push_btn.wait();
	red_light.on();
	cout << "Push the button again!" << endl;
	push_btn.wait();
	red_light.off();
	green_light.on();
	cout << "One more time!" << endl;
	push_btn.wait();
	green_light.off();
	
	cout << "NON-BLOCKING WAIT TEST" << endl;
	cout << "Push the button a whole bunch of times!" << endl;
	push_btn.wait(&my_callback);
	usleep(10 * 1000000);
	
	cout << "Goodbye." << endl;
	return 0;
}
