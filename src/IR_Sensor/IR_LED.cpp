#include "IR_LED.h"
#include <pthread.h>	// pulls in stuff needed for multithreading.
#include <cstdio>		// pulls in printf()

using namespace std;

namespace PatricksDrivers {
	
	IR_LED::IR_LED(const char* key) : Pin(key) {
		
	}
	
	// This is a friend function of the class
	void* threadedFunction(void* value) {
		IR_LED* ir_led = static_cast<IR_LED*>(value);

		while (ir_led->threadRunning) {
			// this is where the magic happens
			
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
