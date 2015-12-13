/*
 * How am I going to handle the diferences between the 1015 and the 1115?
 * The Adafruit code has 1015 as a base class, and 1115 inherits off it.
 */

#include "ADS1X15_Singleton.h"

using namespace std;

namespace PatricksDrivers {
	
	// Class ADS1015
	ADS1015_Singleton::ADS1015_Singleton(unsigned char bus = 1, unsigned char addr = 0x48) {
		Device = I2C_IO_Singleton::getInstance();
		_bus = bus;
		_addr = addr;
	}
	
	ADS1015_Singleton::~ADS1015_Singleton() {
		
	}
	
	// Class ADS1115 (child class of ADS1015)
	ADS1115_Singleton::ADS1115_Singleton(unsigned char bus = 1, unsigned char addr = 0x48) {
		Device = I2C_IO_Singleton::getInstance();
		_bus = bus;
		_addr = addr;
	}
	
} // namespace
