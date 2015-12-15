#ifndef __TCA9548A_H
#define __TCA9548A_H

#include "../lib/I2C_IO_Singleton.h"
#include <cstdio>

namespace PatricksDrivers {
	
	class TCA9548A_Singleton {
		private:
			I2C_IO_Singleton* Device;
			unsigned char _bus;
			unsigned char _addr;
		public:
			TCA9548A_Singleton(unsigned char bus, unsigned char addr);
			~TCA9548A_Singleton();
	}; // class
	
} // namespace

#endif
