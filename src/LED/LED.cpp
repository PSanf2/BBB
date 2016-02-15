#include "LED.h"

#include <cstdio>	// pulls in printf()

using namespace std;

namespace PatricksDrivers {
	
	LED::LED(const char* key) : Pin(key) {
		//Gpio = GPIO_Singleton::getInstance();
		//Pin = Gpio->setup(key);
		//Pin->setDirection(OUTPUT);
		//off();
	}
	
	LED::~LED(){
		//Gpio->cleanup(Pin->getKey());
	}
	
	void LED::on() {
		//Pin->setValue(HIGH);
	}
	
	void LED::off() {
		//Pin->setValue(LOW);
	}
	
} // namespace PatricksDrivers
