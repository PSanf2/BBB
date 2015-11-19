#include "LSM9DS0.h"

using namespace std;

namespace PatricksDrivers {
	
	LSM9DS0::LSM9DS0() {
		LSM9DS0(1, LSM9DS0_ACCELRANGE_2G, LSM9DS0_MAGGAIN_2GAUSS, LSM9DS0_GYROSCALE_245DPS);
	}
	
	LSM9DS0::LSM9DS0(
		unsigned char bus,
		lsm9ds0AccelRange_t accelRange,
		lsm9ds0MagGain_t magGain,
		lsm9ds0GyroScale_t gyroScale)
	{
			_bus = bus;
			_range = accelRange;
			_gain = magGain;
			_scale = gyroScale;
	}
	
	LSM9DS0::~LSM9DS0() {
		
	}
	
}
