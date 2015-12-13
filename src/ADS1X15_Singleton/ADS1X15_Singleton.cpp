#include "ADS1X15_Singleton.h"

using namespace std;

namespace PatricksDrivers {
	
	// Class ADS1015
	ADS1015_Singleton::ADS1015_Singleton(unsigned char bus = 1, unsigned char addr = ADS1015_ADDRESS) {
		Device = I2C_IO_Singleton::getInstance();
		_bus = bus;
		_addr = addr;
		_convDelay = ADS1015_CONVERSIONDELAY;
		_bitShift = 4;
		_gain = GAIN_TWOTHIRDS;
	}
	
	ADS1015_Singleton::~ADS1015_Singleton() {
		
	}
	
	// Class ADS1115 (child class of ADS1015)
	ADS1115_Singleton::ADS1115_Singleton(unsigned char bus = 1, unsigned char addr = ADS1015_ADDRESS) {
		Device = I2C_IO_Singleton::getInstance();
		_bus = bus;
		_addr = addr;
		_convDelay = ADS1115_CONVERSIONDELAY;
		_bitShift = 0;
		_gain = GAIN_TWOTHIRDS;
	}
	
} // namespace
