#include "LSM9DS0.h"

using namespace std;

namespace PatricksDrivers {
	
	LSM9DS0::LSM9DS0(unsigned char bus, unsigned char addr) : I2C_IO(bus, addr) {
		
	}
	
	LSM9DS0::~LSM9DS0() {
		
	}
	
}
