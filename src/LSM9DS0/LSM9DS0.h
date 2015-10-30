#ifndef __LSM9DS0
#define __LSM9DS0

#include "../lib/I2C_IO.h"

namespace PatricksDrivers {

	class LSM9DS0 : public I2C_IO {
		public:
			LSM9DS0(unsigned char bus, unsigned char addr);
			~LSM9DS0();
	};

}

#endif

