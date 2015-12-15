#include "TCA9548A_Singleton.h"

#include <cstdio>	// pulls in printf()

using namespace std;

namespace PatricksDrivers {
	
	TCA9548A_Singleton::TCA9548A_Singleton(unsigned char bus, unsigned char addr) {
		Device = I2C_IO_Singleton::getInstance();
		_bus = bus;
		_addr = addr;
	}
	
	TCA9548A_Singleton::~TCA9548A_Singleton() {
		
	}
	
} // namespace
