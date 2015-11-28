#include "LSM9DS0.h"

using namespace std;

LSM9DS0::LSM9DS0() {
	accel_mg_lsb = 1.0;
	printf("\naccel_mg_lsb =\t%f", accel_mg_lsb);
	printf("\n&accel_mg_lsb = \t%X", &accel_mg_lsb);
	
	mag_mgauss_lsb = 2.0;
	printf("\nmag_mgauss_lsb =\t%f", mag_mgauss_lsb);
	printf("\n&mag_mgauss_lsb = \t%X", &mag_mgauss_lsb);
	
	gyro_dps_digit = 3.0;
	printf("\ngyro_dps_digit =\t%f", gyro_dps_digit);
	printf("\n&gyro_dps_digit = \t%X", &gyro_dps_digit);
}

LSM9DS0::~LSM9DS0() {
	
}

void LSM9DS0::readAccel() {
	printf("\naccel_mg_lsb =\t%f", accel_mg_lsb);
	printf("\n&accel_mg_lsb = \t%X", &accel_mg_lsb);
}

void LSM9DS0::readMag() {
	printf("\nmag_mgauss_lsb =\t%f", mag_mgauss_lsb);
	printf("\n&mag_mgauss_lsb = \t%X", &mag_mgauss_lsb);
}

void LSM9DS0::readGyro() {
	printf("\ngyro_dps_digit =\t%f", gyro_dps_digit);
	printf("\n&gyro_dps_digit = \t%X", &gyro_dps_digit);
}

void LSM9DS0::readTemp() {
	
}
