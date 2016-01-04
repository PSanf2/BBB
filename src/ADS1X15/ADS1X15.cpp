#include "ADS1X15.h"

#include <unistd.h> // pulls in usleep()
#include <cstdio>	// pulls in printf()

using namespace std;

namespace PatricksDrivers {
	
	// Class ADS1015
	ADS1015::ADS1015(unsigned char bus = 1, unsigned char addr = ADS1015_ADDRESS) {
		Device = I2C_IO_Singleton::getInstance();
		_bus = bus;
		_addr = addr;
		_convDelay = ADS1015_CONVERSIONDELAY;
		_bitShift = 4;
		_gain = GAIN_TWOTHIRDS;
	}
	
	ADS1015::~ADS1015() {
		
	}
	
	unsigned int ADS1015::readADC_SingleEnded(unsigned char channel) {
		if (channel > 3)
			return 0;
		
		// Start with default values
		unsigned int config = ADS1015_REG_CONFIG_CQUE_NONE    | // Disable the comparator (default val)
			ADS1015_REG_CONFIG_CLAT_NONLAT  | // Non-latching (default val)
			ADS1015_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
			ADS1015_REG_CONFIG_CMODE_TRAD   | // Traditional comparator (default val)
			ADS1015_REG_CONFIG_DR_1600SPS   | // 1600 samples per second (default)
			ADS1015_REG_CONFIG_MODE_SINGLE;   // Single-shot mode (default)
		
		// Set PGA/voltage range
		config |= _gain;
		
		// Set single-ended input channel
		switch (channel) {
			case (0):
				config |= ADS1015_REG_CONFIG_MUX_SINGLE_0;
			break;
			case (1):
				config |= ADS1015_REG_CONFIG_MUX_SINGLE_1;
			break;
			case (2):
				config |= ADS1015_REG_CONFIG_MUX_SINGLE_2;
			break;
			case (3):
				config |= ADS1015_REG_CONFIG_MUX_SINGLE_3;
			break;
		}

		// Set 'start single-conversion' bit
		config |= ADS1015_REG_CONFIG_OS_SINGLE;

		// Write config register to the ADC
		unsigned char* vals = new unsigned char[3];
		vals[0] = ADS1015_REG_POINTER_CONFIG;
		vals[1] = config >> 8;
		vals[2] = config & 0xFF;
		Device->write(_bus, _addr, 3, vals);
		delete vals;

		// Wait for the conversion to complete
		usleep(_convDelay * 1000);

		// Read the conversion results
		// Shift 12-bit results right 4 bits for the ADS1015
		vals = new unsigned char[2];
		Device->read(_bus, _addr, ADS1015_REG_POINTER_CONVERT, 2, vals);
		unsigned int result = 0;
		result = vals[0] << 8;
		result = result | vals[1];
		result = result >> _bitShift;
		delete vals;
		return result;
	}
	
	int ADS1015::readADC_Differential_0_1() {
		// Start with default values
		unsigned int config = ADS1015_REG_CONFIG_CQUE_NONE | // Disable the comparator (default val)
			ADS1015_REG_CONFIG_CLAT_NONLAT  | // Non-latching (default val)
			ADS1015_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
			ADS1015_REG_CONFIG_CMODE_TRAD   | // Traditional comparator (default val)
			ADS1015_REG_CONFIG_DR_1600SPS   | // 1600 samples per second (default)
			ADS1015_REG_CONFIG_MODE_SINGLE;   // Single-shot mode (default)

		// Set PGA/voltage range
		config |= _gain;

		// Set channels
		config |= ADS1015_REG_CONFIG_MUX_DIFF_0_1; // AIN2 = P, AIN3 = N

		// Set 'start single-conversion' bit
		config |= ADS1015_REG_CONFIG_OS_SINGLE;

		// Write config register to the ADC
		unsigned char* vals = new unsigned char[3];
		vals[0] = ADS1015_REG_POINTER_CONFIG;
		vals[1] = config >> 8;
		vals[2] = config & 0xFF;
		Device->write(_bus, _addr, 3, vals);
		delete vals;

		// Wait for the conversion to complete
		usleep(_convDelay * 1000);

		// Read the conversion results
		vals = new unsigned char[2];
		Device->read(_bus, _addr, ADS1015_REG_POINTER_CONVERT, 2, vals);
		unsigned int result = 0;
		result = vals[0] << 8;
		result = result | vals[1];
		result = result >> _bitShift;
		delete vals;
		
		if (_bitShift == 0)
		{
			return (int) result;
		} else {
			// Shift 12-bit results right 4 bits for the ADS1015,
			// making sure we keep the sign bit intact
			if (result > 0x07FF){
				// negative number - extend the sign to 16th bit
				result |= 0xF000;
			}
			return (int) result;
		}
	}
	
	int ADS1015::readADC_Differential_2_3() {
		// Start with default values
		unsigned int config = ADS1015_REG_CONFIG_CQUE_NONE | // Disable the comparator (default val)
			ADS1015_REG_CONFIG_CLAT_NONLAT  | // Non-latching (default val)
			ADS1015_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
			ADS1015_REG_CONFIG_CMODE_TRAD   | // Traditional comparator (default val)
			ADS1015_REG_CONFIG_DR_1600SPS   | // 1600 samples per second (default)
			ADS1015_REG_CONFIG_MODE_SINGLE;   // Single-shot mode (default)

		// Set PGA/voltage range
		config |= _gain;

		// Set channels
		config |= ADS1015_REG_CONFIG_MUX_DIFF_2_3; // AIN2 = P, AIN3 = N

		// Set 'start single-conversion' bit
		config |= ADS1015_REG_CONFIG_OS_SINGLE;

		// Write config register to the ADC
		unsigned char* vals = new unsigned char[3];
		vals[0] = ADS1015_REG_POINTER_CONFIG;
		vals[1] = config >> 8;
		vals[2] = config & 0xFF;
		Device->write(_bus, _addr, 3, vals);
		delete vals;

		// Wait for the conversion to complete
		usleep(_convDelay * 1000);

		// Read the conversion results
		vals = new unsigned char[2];
		Device->read(_bus, _addr, ADS1015_REG_POINTER_CONVERT, 2, vals);
		unsigned int result = 0;
		result = vals[0] << 8;
		result = result | vals[1];
		result = result >> _bitShift;
		delete vals;
		
		if (_bitShift == 0)
		{
			return (int) result;
		} else {
			// Shift 12-bit results right 4 bits for the ADS1015,
			// making sure we keep the sign bit intact
			if (result > 0x07FF){
				// negative number - extend the sign to 16th bit
				result |= 0xF000;
			}
			return (int) result;
		}
	}
	
	/*
	 * This function is untested. If you test it, and want to make corrections
	 * then please contact the author of the git repository.
	 * https://github.com/PSanf2/BBB
	 */
	void ADS1015::startComparator_SingleEnded(unsigned char channel, unsigned int threshold) {
		// Start with default values
		unsigned int config = ADS1015_REG_CONFIG_CQUE_1CONV   | // Comparator enabled and asserts on 1 match
			ADS1015_REG_CONFIG_CLAT_LATCH   | // Latching mode
			ADS1015_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
			ADS1015_REG_CONFIG_CMODE_TRAD   | // Traditional comparator (default val)
			ADS1015_REG_CONFIG_DR_1600SPS   | // 1600 samples per second (default)
			ADS1015_REG_CONFIG_MODE_CONTIN  | // Continuous conversion mode
			ADS1015_REG_CONFIG_MODE_CONTIN;   // Continuous conversion mode

		// Set PGA/voltage range
		config |= _gain;
			
		// Set single-ended input channel
		switch (channel) {
			case (0):
				config |= ADS1015_REG_CONFIG_MUX_SINGLE_0;
			break;
			
			case (1):
				config |= ADS1015_REG_CONFIG_MUX_SINGLE_1;
			break;
			
			case (2):
				config |= ADS1015_REG_CONFIG_MUX_SINGLE_2;
			break;
			
			case (3):
				config |= ADS1015_REG_CONFIG_MUX_SINGLE_3;
			break;
		}
		
		// Set the high threshold register
		// Shift 12-bit results left 4 bits for the ADS1015
		threshold = threshold << _bitShift;
		unsigned char* vals = new unsigned char[3];
		vals[0] = ADS1015_REG_POINTER_HITHRESH;
		vals[1] = threshold >> 8;
		vals[2] = threshold & 0xFF;
		Device->write(_bus, _addr, 3, vals);
		delete vals;
		
		// Write config register to the ADC
		vals = new unsigned char[3];
		vals[0] = ADS1015_REG_POINTER_CONFIG;
		vals[1] = config >> 8;
		vals[2] = config & 0xFF;
		Device->write(_bus, _addr, 3, vals);
		delete vals;
	}
	
	/*
	 * This function is untested. If you test it, and want to make corrections
	 * then please contact the author of the git repository.
	 * https://github.com/PSanf2/BBB
	 */
	int ADS1015::getLastConversionResults() {
		// Wait for the conversion to complete
		usleep(_convDelay * 1000);
		
		// Read the conversion results
		unsigned char* vals = new unsigned char[2];
		Device->read(_bus, _addr, ADS1015_REG_POINTER_CONVERT, 2, vals);
		unsigned int result = 0;
		result = vals[0] << 8;
		result = result | vals[1];
		result = result >> _bitShift;
		delete vals;
		
		if (_bitShift == 0)
		{
			return (int) result;
		} else {
			// Shift 12-bit results right 4 bits for the ADS1015,
			// making sure we keep the sign bit intact
			if (result > 0x07FF){
				// negative number - extend the sign to 16th bit
				result |= 0xF000;
			}
			return (int) result;
		}
	}
	
	void ADS1015::setGain(adsGain_t gain) {
		_gain = gain;
	}
	
	adsGain_t ADS1015::getGain() {
		return _gain;
	}
	
	// Class ADS1115 (child class of ADS1015)
	ADS1115::ADS1115(unsigned char bus = 1, unsigned char addr = ADS1015_ADDRESS) {
		Device = I2C_IO_Singleton::getInstance();
		_bus = bus;
		_addr = addr;
		_convDelay = ADS1115_CONVERSIONDELAY;
		_bitShift = 0;
		_gain = GAIN_TWOTHIRDS;
	}
	
} // namespace
