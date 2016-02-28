#include <iostream>		// pulls in cin and hex
#include <cstdio>		// pulls in printf()
#include <cstdlib>		// lets me use system()

#include "LED.h"
#include "Button.h"

#define RED_LED_PIN "P8_14"
#define GREEN_LED_PIN "P8_16"
#define BUTTON_PIN "P8_12"

using namespace std;

int main(int argc, char* argv[]) {
	
	PatricksDrivers::LED red_light(RED_LED_PIN);
	PatricksDrivers::LED green_light(GREEN_LED_PIN);
	PatricksDrivers::Button push_btn(BUTTON_PIN);
	
	cout << endl << "Push the button!" << endl;
	push_btn.wait_for();
	red_light.on();
	cout << "Push the button again!" << endl;
	push_btn.wait_for();
	red_light.off();
	green_light.on();
	cout << "One more time!" << endl;
	push_btn.wait_for();
	green_light.off();
	cout << "Goodbye." << endl;

	return 0;
}
