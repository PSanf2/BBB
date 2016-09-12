#include "IR_LED.h"
#include <pthread.h>	// pulls in stuff needed for multithreading.
#include <cstdio>		// pulls in printf()
#include <cstdlib>
#include <ctime>

using namespace std;

namespace PatricksDrivers {
	
	IR_LED::IR_LED(const char* key) : Pin(key, 38000) {
		Pin.stop();
		Pin.duty(50);
	}
	
	// This is a friend function of the class
	// this is where the magic happens
	void* threadedFunction(void* value) {
		IR_LED* ir_led = static_cast<IR_LED*>(value);
		while (ir_led->threadRunning) {
			for (int i = 0; i < 8; i++) {
				ir_led->Pin.start();
				usleep(1000);
				ir_led->Pin.stop();
				usleep(1000);
			}
			usleep(80000);
		}
		ir_led->Pin.stop();
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
