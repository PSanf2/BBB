#include <iostream>		// pulls in cin and hex
#include <cstdio>		// pulls in printf()
#include <cstdlib>		// lets me use system()

#include "LightSensor.h"
#include "Button.h"

#define BUTTON_PIN "P8_14"
#define SENSOR_PIN "P9_38" // pins 38 and 40 atm

using namespace std;

PatricksDrivers::Button push_btn(BUTTON_PIN);
PatricksDrivers::LightSensor sensor(SENSOR_PIN);

int main(int argc, char* argv[]) {
	
	cout << endl;
	cout << "Push the button to get a light sensor reading!" << endl;
	for (int i = 0; i < 5; i++) {
		push_btn.wait();
		int reading;
		reading = sensor.value();
		cout << "Light level: ";
		cout << reading << endl;
	}
	cout << "Goodbye." << endl;
	return 0;
}
