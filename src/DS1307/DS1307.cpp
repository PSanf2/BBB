#include "DS1307.h"

#include <cstdio>	// pulls in printf()

using namespace std;

namespace PatricksDrivers {
	
	DS1307::DS1307(unsigned char bus) {
		Device = I2C_IO_Singleton::getInstance();
		_bus = bus;
	}
	
	DS1307::~DS1307() {
		
	}
	
} // namespace
