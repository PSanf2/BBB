#include "ADS1X15_Singleton.h"

using namespace std;

namespace PatricksDrivers {
	
	ADS1X15_Singleton::ADS1X15_Singleton(unsigned char bus, unsigned char addr) {
		Device = I2C_IO_Singleton::getInstance();
		_bus = bus;
		_addr = addr;
	}
	
} // namespace
