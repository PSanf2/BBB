#ifndef __ADS1X15_SINGLETON_H
#define __ADS1X15_SINGLETON_H

#include "../lib/I2C_IO_Singleton.h"
#include <cstdio>

namespace PatricksDrivers {
	
	class ADS1X15_Singleton {
		private:
			I2C_IO_Singleton* Device;
			unsigned char _bus;
			unsigned char _addr;
		public:
			ADS1X15_Singleton(unsigned char bus, unsigned char addr);
	}; // class
	
} // namespace

#endif
