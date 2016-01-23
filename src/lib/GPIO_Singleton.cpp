#include "GPIO_Singleton.h"

using namespace std;

namespace PatricksDrivers {
	
	// GPIO_Pin methods
	GPIO_Pin::GPIO_Pin(unsigned int pinNum) {
		
	}
	
	GPIO_Pin::~GPIO_Pin() {
		
	}
	
	// GPIO_Singleton methods
	static GPIO_Singleton* GPIO_Singleton_instance;
	
	GPIO_Singleton* GPIO_Singleton::getInstance() {
		if (!GPIO_Singleton_instance)
			GPIO_Singleton_instance = new GPIO_Singleton();
		return GPIO_Singleton_instance;
	}
	
	GPIO_Singleton::GPIO_Singleton() {
		
	}
	
	GPIO_Singleton::~GPIO_Singleton() {
		delete GPIO_Singleton_instance;
	}
} // namespace PatricksDrivers
