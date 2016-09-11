#include "IR_Sensor.h"

using namespace std;

namespace PatricksDrivers {
	
	IR_Sensor::IR_Sensor(const char* key) : Pin(key) {
		Pin.direction(BBIO::INPUT);
		Pin.edge(BBIO::FALLING);
	}
	
	void IR_Sensor::run(BBIO::GPIO::CallbackType callback) {
		Pin.waitForEdge(callback);
	}
	
	void IR_Sensor::stop() {
		Pin.waitForEdgeCancel();
	}
	
} // namespace
