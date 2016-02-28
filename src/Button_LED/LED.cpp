#include "LED.h"

using namespace std;

namespace PatricksDrivers {
	
	LED::LED(const char* key) : Pin(key) {
		Pin.direction(BBIO::OUTPUT);
		off();
	}
	
	void LED::on() {
		Pin.value(BBIO::HIGH);
	}
	
	void LED::off() {
		Pin.value(BBIO::LOW);
	}
	
} // namespace PatricksDrivers
