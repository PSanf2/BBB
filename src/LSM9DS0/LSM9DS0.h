#ifndef __LSM9DS0
#define __LSM9DS0

#include "../lib/I2C_IO.h"
#include <cstdio>	// pulls in printf

class LSM9DS0 {
	
	private:
		float accel_mg_lsb;
		float mag_mgauss_lsb;
		float gyro_dps_digit;
	protected:
		;
	public:
		LSM9DS0();
		~LSM9DS0();
		void readAccel();
		void readMag();
		void readGyro();
		void readTemp();
};

#endif
