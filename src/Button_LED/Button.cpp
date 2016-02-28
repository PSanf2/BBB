#include "Button.h"

using namespace std;

namespace PatricksDrivers {
	
	Button::Button(const char* key) : Pin(key) {
		Pin.direction(BBIO::INPUT);
		Pin.edge(BBIO::RISING);
	}
	
	void Button::wait() {
		Pin.waitForEdge();
	}
	
	void Button::wait(BBIO::GPIO::CallbackType callback) {
		Pin.waitForEdge(callback);
	}
	
} // namespace
