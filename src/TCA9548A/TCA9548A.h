#ifndef __TCA9548A_H
#define __TCA9548A_H

#include "../lib/I2C_IO_Singleton.h"
#include <cstdio>

namespace PatricksDrivers {
	
	class TCA9548A {
		private:
			I2C_IO_Singleton* Device;
			unsigned char _bus;
			unsigned char _addr;
		public:
			TCA9548A(unsigned char bus, unsigned char addr);
			~TCA9548A();
			void select(unsigned char channel);
			void enable(unsigned char channel);
			void disable(unsigned char channel);
			void disableAll();
			unsigned char current();
			bool enabled(unsigned char channel);
	}; // class
	
} // namespace

#endif
