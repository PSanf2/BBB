#include "Button.h"

using namespace std;

namespace PatricksDrivers {
	
	Button::Button(const char* key) : Pin(key) {
		Pin.direction(BBIO::INPUT);
		Pin.edge(BBIO::RISING);
	}
	
	void Button::wait_for() {
		Pin.waitForEdge();
	}
	
} // namespace
