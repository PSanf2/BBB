#include "LightSensor.h"

using namespace std;

namespace PatricksDrivers {
	
	LightSensor::LightSensor(const char* key) : Pin(key) {
		
	}
	
	int LightSensor::value() {
		return Pin.value();
	}
	
} // namespace
