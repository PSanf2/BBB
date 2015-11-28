#include "LSM9DS0.h"

using namespace std;

/*
LSM9DS0::LSM9DS0() {
	printf("\n&_accel_mg_lsb = \t%X", &_accel_mg_lsb);
	printf("\n&_mag_mgauss_lsb = \t%X", &_mag_mgauss_lsb);
	printf("\n&_gyro_dps_digit = \t%X", &_gyro_dps_digit);
	LSM9DS0(1, LSM9DS0_ACCELRANGE_2G, LSM9DS0_MAGGAIN_2GAUSS, LSM9DS0_GYROSCALE_245DPS);
}
*/
	
LSM9DS0::LSM9DS0(
	unsigned char bus,
	lsm9ds0AccelRange_t accelRange,
	lsm9ds0MagGain_t magGain,
	lsm9ds0GyroScale_t gyroScale)
{
	printf("\n&_accel_mg_lsb = \t%X", &_accel_mg_lsb);
	printf("\n&_mag_mgauss_lsb = \t%X", &_mag_mgauss_lsb);
	printf("\n&_gyro_dps_digit = \t%X", &_gyro_dps_digit);
	
	_bus = bus;
	_range = accelRange;
	_gain = magGain;
	_scale = gyroScale;
	_err = 0;
	
	// I need to perform the setup tasks here.
	// I'll need to read in a value from select registers, and
	// populate the _accel_mg_lsb, _mag_mgauss_lsb, and _gyro_dps_digit variables
	
	printf("\nLSM9DS0 Constructor called.");
	printf("\n_bus\t= %u", _bus);
	printf("\n_range\t= %f", _range);
	printf("\n_gain\t= %f", _gain);
	printf("\n_scale\t= %f", _scale);
	printf("\n_err\t= %d", _err);
	
	// check the board ID for accel/mag
	I2C_IO* comm;
	
	comm = new I2C_IO(_bus, LSM9DS0_ADDRESS_ACCELMAG);
	if (comm->readRegister(LSM9DS0_REGISTER_WHO_AM_I_XM) != LSM9DS0_XM_ID) {
		_err = 1;
		return;
	}
	delete comm;
	
	printf("\nLSM9DS0_REGISTER_WHO_AM_I_XM\t== LSM9DS0_XM_ID");
	
	comm = new I2C_IO(_bus, LSM9DS0_ADDRESS_GYRO);
	if (comm->readRegister(LSM9DS0_REGISTER_WHO_AM_I_G) != LSM9DS0_G_ID) {
		_err = 2;
		return;
	}
	delete comm;
	
	printf("\nLSM9DS0_REGISTER_WHO_AM_I_G\t== LSM9DS0_G_ID");
	
	// start talking w/ the accel/mag device
	comm = new I2C_IO(_bus, LSM9DS0_ADDRESS_ACCELMAG);
	// enable accelerometer continous
	comm->writeRegister(LSM9DS0_REGISTER_CTRL_REG1_XM, 0x67);
	printf("\ncomm->get_error() = %d", comm->get_error());
	comm->writeRegister(LSM9DS0_REGISTER_CTRL_REG5_XM, 0xF0);
	printf("\ncomm->get_error() = %d", comm->get_error());
	// enable mag continous
	comm->writeRegister(LSM9DS0_REGISTER_CTRL_REG7_XM, 0x00);
	printf("\ncomm->get_error() = %d", comm->get_error());
	// enable temp
	comm->writeRegister(LSM9DS0_REGISTER_CTRL_REG5_XM, (comm->readRegister(LSM9DS0_REGISTER_CTRL_REG5_XM) | 0x80));
	printf("\ncomm->get_error() = %d", comm->get_error());
	// stop talking w/ the device
	delete comm;
	
	// enable gyro
	comm = new I2C_IO(_bus, LSM9DS0_ADDRESS_GYRO);
	comm->writeRegister(LSM9DS0_REGISTER_CTRL_REG1_G, 0x0F);
	printf("\ncomm->get_error() = %d", comm->get_error());
	delete comm;
	
	printf("\nAll devices enabled.");
	
	// perform the required setup tasks
	// (why enable before setup?)
	unsigned char tempReg;
	comm = new I2C_IO(_bus, LSM9DS0_ADDRESS_ACCELMAG);
	// setup accel
	tempReg = comm->readRegister(LSM9DS0_REGISTER_CTRL_REG2_XM);
	printf("\ncomm->get_error() = %d", comm->get_error());
	printf("\ntempReg\t= %u", tempReg);
	tempReg &= ~(0x38);
	printf("\ntempReg\t= %u", tempReg);
	tempReg |= _range;
	printf("\ntempReg\t= %u", tempReg);
	comm->writeRegister(LSM9DS0_REGISTER_CTRL_REG2_XM, tempReg);
	printf("\ncomm->get_error() = %d", comm->get_error());
	switch (_range) {
		case LSM9DS0_ACCELRANGE_2G:
			_accel_mg_lsb = LSM9DS0_ACCEL_MG_LSB_2G;
		break;
		case LSM9DS0_ACCELRANGE_4G:
			_accel_mg_lsb = LSM9DS0_ACCEL_MG_LSB_4G;
		break;
		case LSM9DS0_ACCELRANGE_6G:
			_accel_mg_lsb = LSM9DS0_ACCEL_MG_LSB_6G;
		break;
		case LSM9DS0_ACCELRANGE_8G:
			_accel_mg_lsb = LSM9DS0_ACCEL_MG_LSB_8G;
		break;    
		case LSM9DS0_ACCELRANGE_16G:
			_accel_mg_lsb = LSM9DS0_ACCEL_MG_LSB_16G;
		break;
	}
	printf("\n&_accel_mg_lsb\t\t= %X", &_accel_mg_lsb);
	printf("\n_accel_mg_lsb\t= %f", _accel_mg_lsb);
	// setup mag
	tempReg = comm->readRegister(LSM9DS0_REGISTER_CTRL_REG6_XM);
	printf("\ncomm->get_error() = %d", comm->get_error());
	printf("\ntempReg\t= %u", tempReg);
	tempReg &= ~(0x60);
	printf("\ntempReg\t= %u", tempReg);
	tempReg |= _gain;
	printf("\ntempReg\t= %u", tempReg);
	comm->writeRegister(LSM9DS0_REGISTER_CTRL_REG6_XM, tempReg);
	printf("\ncomm->get_error() = %d", comm->get_error());
	switch(_gain) {
		case LSM9DS0_MAGGAIN_2GAUSS:
			_mag_mgauss_lsb = LSM9DS0_MAG_MGAUSS_2GAUSS;
		break;
		case LSM9DS0_MAGGAIN_4GAUSS:
			_mag_mgauss_lsb = LSM9DS0_MAG_MGAUSS_4GAUSS;
		break;
		case LSM9DS0_MAGGAIN_8GAUSS:
			_mag_mgauss_lsb = LSM9DS0_MAG_MGAUSS_8GAUSS;
		break;
		case LSM9DS0_MAGGAIN_12GAUSS:
			_mag_mgauss_lsb = LSM9DS0_MAG_MGAUSS_12GAUSS;
		break;
	}
	printf("\n&_mag_mgauss_lsb\t= %X", &_mag_mgauss_lsb);
	printf("\n_mag_mgauss_lsb\t= %f", _mag_mgauss_lsb);
	delete comm;
	// setup gyro
	comm = new I2C_IO(_bus, LSM9DS0_ADDRESS_GYRO);
	tempReg = comm->readRegister(LSM9DS0_REGISTER_CTRL_REG4_G);
	printf("\ncomm->get_error() = %d", comm->get_error());
	printf("\ntempReg\t= %u", tempReg);
	tempReg &= ~(0x30);
	printf("\ntempReg\t= %u", tempReg);
	tempReg |= _scale;
	printf("\ntempReg\t= %u", tempReg);
	comm->writeRegister(LSM9DS0_REGISTER_CTRL_REG4_G, tempReg);
	printf("\ncomm->get_error() = %d", comm->get_error());
	switch(_scale) {
		case LSM9DS0_GYROSCALE_245DPS:
			_gyro_dps_digit = LSM9DS0_GYRO_DPS_DIGIT_245DPS;
		break;
		case LSM9DS0_GYROSCALE_500DPS:
			_gyro_dps_digit = LSM9DS0_GYRO_DPS_DIGIT_500DPS;
		break;
		case LSM9DS0_GYROSCALE_2000DPS:
			_gyro_dps_digit = LSM9DS0_GYRO_DPS_DIGIT_2000DPS;
		break;
	}
	printf("\n&_gyro_dps_digit\t= %X", &_gyro_dps_digit);
	printf("\n_gyro_dps_digit\t= %f", _gyro_dps_digit);
	delete comm;
}

LSM9DS0::~LSM9DS0() {
	
}

void LSM9DS0::readAccel() {
	printf("\n_accel_mg_lsb =\t%f", _accel_mg_lsb);
	printf("\n&_accel_mg_lsb = \t%X", &_accel_mg_lsb);
}

void LSM9DS0::readMag() {
	printf("\n_mag_mgauss_lsb =\t%f", _mag_mgauss_lsb);
	printf("\n&_mag_mgauss_lsb = \t%X", &_mag_mgauss_lsb);
}

void LSM9DS0::readGyro() {
	printf("\n_gyro_dps_digit =\t%f", _gyro_dps_digit);
	printf("\n&_gyro_dps_digit = \t%X", &_gyro_dps_digit);
}

void LSM9DS0::readTemp() {
	
}
