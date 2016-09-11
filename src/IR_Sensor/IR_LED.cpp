#include "IR_LED.h"
#include <pthread.h>	// pulls in stuff needed for multithreading.
#include <cstdio>		// pulls in printf()

using namespace std;

namespace PatricksDrivers {
	
	IR_LED::IR_LED(const char* key) : Pin(key) {
		Pin.direction(BBIO::OUTPUT);
		off();
	}
	
	void IR_LED::on() {
		Pin.value(BBIO::HIGH);
	}
	
	void IR_LED::off() {
		Pin.value(BBIO::LOW);
	}
	
	// This is a friend function of the class
	void* threadedFunction(void* value) {
		IR_LED* ir_led = static_cast<IR_LED*>(value);

		while (ir_led->threadRunning) {
			// this is where the magic happens
			// 1 second = 1,000,000 microseconds
			for (int i = 0; i < 40000; i += 26) {
				ir_led->on();
				usleep(10);
				ir_led->off();
				usleep(10);
			}
			usleep(40000);
		}
		return 0;
	}
	
	int IR_LED::run() {
		threadRunning = true;
		if (pthread_create(&thread, NULL, &threadedFunction, static_cast<void*>(this))) {
			threadRunning = false;
			return -1;
		}
		return 0;
	}
	
	void IR_LED::stop() {
		threadRunning = false;
	}
	
} // namespace PatricksDrivers
