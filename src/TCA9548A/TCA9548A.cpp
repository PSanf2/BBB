#include "TCA9548A.h"

#include <cstdio>	// pulls in printf()

using namespace std;

namespace PatricksDrivers {
	
	TCA9548A::TCA9548A(unsigned char bus, unsigned char addr) {
		Device = I2C_IO_Singleton::getInstance();
		_bus = bus;
		_addr = addr;
	}
	
	TCA9548A::~TCA9548A() {
		
	}
	
	void TCA9548A::select(unsigned char channel) {
		if (channel > 7)
			return;
		
		unsigned char* vals = new unsigned char[1];
		vals[0] = (1 << channel);
		Device->write(_bus, _addr, 1, vals);
		delete vals;
	}
	
	void TCA9548A::enable(unsigned char channel) {
		if (channel > 7)
			return;
		
		// read in my channel
		unsigned char curr = current();
		
		// set the bit for the channel
		curr |= (1 << channel);
		
		// set the new value
		unsigned char* vals = new unsigned char[1];
		vals[0] = curr;
		Device->write(_bus, _addr, 1, vals);
		delete vals;
	}
	
	void TCA9548A::disable(unsigned char channel) {
		if (channel > 7)
			return;
		
		// read in my channel
		unsigned char curr = current();
		
		// clear the bit for the channel
		curr &= ~(1 << channel);
		
		// set the new value
		unsigned char* vals = new unsigned char[1];
		vals[0] = curr;
		Device->write(_bus, _addr, 1, vals);
		delete vals;
	}
	
	void TCA9548A::disableAll() {
		unsigned char* vals = new unsigned char[1];
		vals[0] = 0;
		Device->write(_bus, _addr, 1, vals);
		delete vals;
	}
	
	unsigned char TCA9548A::current() {
		unsigned char* val = new unsigned char[1];
		Device->read(_bus, _addr, 1, val);
		unsigned char result = val[0];
		delete val;
		return result;
	}
	
	bool TCA9548A::enabled(unsigned char channel) {
		if (channel > 7)
			return false;

		unsigned char curr = current();
		
		return (curr & (1 << channel));
	}
	
} // namespace
