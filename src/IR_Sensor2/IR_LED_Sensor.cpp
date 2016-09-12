#include "IR_LED_Sensor.h"
#include <pthread.h>	// pulls in stuff needed for multithreading.
#include <cstdio>		// pulls in printf()
#include <cstdlib>
#include <ctime>

using namespace std;

namespace PatricksDrivers {
	
	IR_LED_Sensor::IR_LED_Sensor(const char* led_key, const char* sensor_key) : LED_pin(led_key, 38000), Sensor_pin(sensor_key) {
		LED_pin.stop();
		LED_pin.duty(50);
		Sensor_pin.direction(BBIO::INPUT);
		Sensor_pin.edge(BBIO::FALLING);
	}
	
	void* threadedFunction(void* value) {
		IR_LED_Sensor* ir_led_sensor = static_cast<IR_LED_Sensor*>(value);
		while (ir_led_sensor->threadRunning) {
			for (int i = 0; i < 8; i++) {
				ir_led_sensor->LED_pin.start();
				usleep(1000);
				ir_led_sensor->LED_pin.stop();
				usleep(1000);
			}
			usleep(80000);
		}
		ir_led_sensor->LED_pin.stop();
		return 0;
	}
	
	int IR_LED_Sensor::run(BBIO::GPIO::CallbackType callback) {
		threadRunning = true;
		if (pthread_create(&thread, NULL, &threadedFunction, static_cast<void*>(this))) {
			threadRunning = false;
			return -1;
		}
		Sensor_pin.waitForEdge(callback);
		return 0;
	}
	
	void IR_LED_Sensor::stop() {
		threadRunning = false;
		Sensor_pin.waitForEdgeCancel();
	}
	
} // namespace
