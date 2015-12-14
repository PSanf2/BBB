#include "ADS1X15_Singleton.h"

#include <unistd.h> // pulls in usleep()
#include <cstdio>	// pulls in printf()

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
	
	unsigned int ADS1015_Singleton::readADC_SingleEnded(unsigned char channel) {
		/*
		 * Whatever is working for the Adafruit code isn't going to work here.
		 * I tried copying the code, and making the changes needed for it to
		 * work here, but kept getting a result of 0 for every channel. I'll
		 * need to go over the datasheet, and pretty much write this from
		 * scratch. The Adafruit code is doing a lot of fancy shit with some
		 * calculations that's always resulting in exactly the same result.
		 * There's no point in doing that. A lot of the #defines can probably
		 * be removed after examining how they're used.
		 */
		 
		/*
		 * Every operation on this chip involves reading/writing two bytes on the bus.
		 * The order is typically to write a register, then read or write two bytes.
		 * The unsigned int types are shifted and &'ed to before every write, and
		 * after every read. The I2C_IO_Singleton class is perfectly able to read/write
		 * two bytes at a time. Since the same shifted and & operations are done for
		 * every read/write to the bus I need to do some processing on the unsigned int
		 * I'm passing around.
		 * On page 11 of the datasheet it explains how to read/write to the device.
		 * I believe that I have everything I need already in the I2C_IO_Singleton class.
		 * The documentation is slightly confusing. When it's talking about doing a write
		 * operation before doing a read it's going on about specifying which register
		 * you'll be reading from to the device. This is already implimented and working
		 * in the exisitng code. The big question I need to be asking is which register
		 * I'll be reading from. It looks like this is almost always going to be a
		 * "conversion register," and I'll always be getting two bytes from it. The
		 * Adafruit code makes it look like this is called the ADS1015_REG_POINTER_CONVERT
		 * and this is defined as 0x00. For writes it looks like everything is going to
		 * ADS1015_REG_POINTER_CONFIG which is 0x01.
		 */
		return 0;
	}
	
	int ADS1015_Singleton::readADC_Differential_0_1() {
		return 0;
	}
	
	int ADS1015_Singleton::readADC_Differential_2_3() {
		return 0;
	}
	
	void ADS1015_Singleton::startComparator_SingleEnded(unsigned char channel, unsigned int threshold) {
		
	}
	
	int ADS1015_Singleton::getLastConversionREsults() {
		return 0;
	}
	
	void ADS1015_Singleton::setGain(adsGain_t gain) {
		_gain = gain;
	}
	
	adsGain_t ADS1015_Singleton::getGain() {
		return _gain;
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
