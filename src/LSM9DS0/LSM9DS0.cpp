#include "LSM9DS0.h"

using namespace std;

namespace PatricksDrivers {
	
	LSM9DS0::LSM9DS0(
		unsigned char bus,
		lsm9ds0AccelRange_t accelRange,
		lsm9ds0MagGain_t magGain,
		lsm9ds0GyroScale_t gyroScale
	) {
		Device = I2C_IO_Singleton::getInstance();
		
		_bus = bus;
		_range = accelRange;
		_gain = magGain;
		_scale = gyroScale;
		_err = 0;
		
		// I need to perform the setup tasks here.
		// I'll need to read in a value from select registers, and
		// populate the _accel_mg_lsb, _mag_mgauss_lsb, and _gyro_dps_digit variables
		
		// check the board ID for accel/mag
		unsigned char* result = new unsigned char[1];
		Device->readRegister(_bus, LSM9DS0_ADDRESS_ACCELMAG, LSM9DS0_REGISTER_WHO_AM_I_XM, result);
		if (result[0] != LSM9DS0_XM_ID) {
			delete result;
			_err = 1;
			return;
		}
		delete result;
		
		// check the boar ID for gyro
		result = new unsigned char[1];
		Device->readRegister(_bus, LSM9DS0_ADDRESS_GYRO, LSM9DS0_REGISTER_WHO_AM_I_G, result);
		if (result[0] != LSM9DS0_G_ID) {
			delete result;
			_err = 2;
			return;
		}
		delete result;
		
		// enable devices
		Device->writeRegister(_bus, LSM9DS0_ADDRESS_ACCELMAG, LSM9DS0_REGISTER_CTRL_REG1_XM, 0x67);
		Device->writeRegister(_bus, LSM9DS0_ADDRESS_ACCELMAG, LSM9DS0_REGISTER_CTRL_REG5_XM, 0xF0);
		Device->writeRegister(_bus, LSM9DS0_ADDRESS_ACCELMAG, LSM9DS0_REGISTER_CTRL_REG7_XM, 0x00);
		
		result = new unsigned char[1];
		Device->readRegister(_bus, LSM9DS0_ADDRESS_ACCELMAG, LSM9DS0_REGISTER_CTRL_REG5_XM, result);
		result[0] |= 0x80;
		Device->writeRegister(_bus, LSM9DS0_ADDRESS_ACCELMAG, LSM9DS0_REGISTER_CTRL_REG5_XM, result[0]);
		delete result;
		
		Device->writeRegister(_bus, LSM9DS0_ADDRESS_GYRO, LSM9DS0_REGISTER_CTRL_REG1_G, 0x0F);
		
		// perform the required setup tasks
		result = new unsigned char[1];
		Device->readRegister(_bus, LSM9DS0_ADDRESS_ACCELMAG, LSM9DS0_REGISTER_CTRL_REG2_XM, result);
		result[0] &= ~(0x38);
		result[0] |= _range;
		Device->writeRegister(_bus, LSM9DS0_ADDRESS_ACCELMAG, LSM9DS0_REGISTER_CTRL_REG2_XM, result[0]);
		delete result;
		
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
		
		result = new unsigned char[1];
		Device->readRegister(_bus, LSM9DS0_ADDRESS_ACCELMAG, LSM9DS0_REGISTER_CTRL_REG6_XM, result);
		result[0] &= ~(0x60);
		result[0] |= _gain;
		Device->writeRegister(_bus, LSM9DS0_ADDRESS_ACCELMAG, LSM9DS0_REGISTER_CTRL_REG6_XM, result[0]);
		delete result;
		
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
		
		result = new unsigned char[1];
		Device->readRegister(_bus, LSM9DS0_ADDRESS_GYRO, LSM9DS0_REGISTER_CTRL_REG4_G, result);
		result[0] &= ~(0x30);
		result[0] |= _scale;
		Device->writeRegister(_bus, LSM9DS0_ADDRESS_GYRO, LSM9DS0_REGISTER_CTRL_REG4_G, result[0]);
		delete result;
		
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
		
	} // constructor
	
	LSM9DS0::~LSM9DS0() {
		
	}
	
	void LSM9DS0::readAccel() {
		unsigned char* result = new unsigned char[6];
		Device->read(_bus, LSM9DS0_ADDRESS_ACCELMAG, 0x80 | LSM9DS0_REGISTER_OUT_X_L_A, 6, result);
		
		unsigned char xlo = result[0];
		signed short xhi = result[1];
		unsigned char ylo = result[2];
		signed short yhi = result[3];
		unsigned char zlo = result[4];
		signed short zhi = result[5];
		
		delete result;
		
		xhi <<= 8;
		xhi |= xlo;
		yhi <<= 8;
		yhi |= ylo;
		zhi <<= 8;
		zhi |= zlo;
		
		accelData.x = xhi;
		accelData.y = yhi;
		accelData.z = zhi;
		
		acceleration.x = accelData.x * _accel_mg_lsb;
		acceleration.x /= 1000;
		acceleration.x *= SENSORS_GRAVITY_STANDARD;
		
		acceleration.y = accelData.y * _accel_mg_lsb;
		acceleration.y /= 1000;
		acceleration.y *= SENSORS_GRAVITY_STANDARD;
		
		acceleration.z = accelData.z * _accel_mg_lsb;
		acceleration.z /= 1000;
		acceleration.z *= SENSORS_GRAVITY_STANDARD;
	}
	
	void LSM9DS0::readMag() {
		unsigned char* result = new unsigned char[6];
		Device->read(_bus, LSM9DS0_ADDRESS_ACCELMAG, 0x80 | LSM9DS0_REGISTER_OUT_X_L_M, 6, result);
		
		unsigned char xlo = result[0];
		signed short xhi = result[1];
		unsigned char ylo = result[2];
		signed short yhi = result[3];
		unsigned char zlo = result[4];
		signed short zhi = result[5];
		
		delete result;
		
		xhi <<= 8;
		xhi |= xlo;
		yhi <<= 8;
		yhi |= ylo;
		zhi <<= 8;
		zhi |= zlo;
		
		magData.x = xhi;
		magData.y = yhi;
		magData.z = zhi;
		
		magnetic.x = magData.x * _mag_mgauss_lsb;
		magnetic.x /= 1000;
		
		magnetic.y = magData.y * _mag_mgauss_lsb;
		magnetic.y /= 1000;
		
		magnetic.z = magData.z * _mag_mgauss_lsb;
		magnetic.z /= 1000;
	}
	
	void LSM9DS0::readGyro() {
		unsigned char* result = new unsigned char[6];
		Device->read(_bus, LSM9DS0_ADDRESS_GYRO, 0x80 | LSM9DS0_REGISTER_OUT_X_L_G, 6, result);
		
		unsigned char xlo = result[0];
		signed short xhi = result[1];
		unsigned char ylo = result[2];
		signed short yhi = result[3];
		unsigned char zlo = result[4];
		signed short zhi = result[5];
		
		delete result;
		
		xhi <<= 8;
		xhi |= xlo;
		yhi <<= 8;
		yhi |= ylo;
		zhi <<= 8;
		zhi |= zlo;
		
		gyroData.x = xhi;
		gyroData.y = yhi;
		gyroData.z = zhi;
		
		gyroscopic.x = gyroData.x * _gyro_dps_digit;
		gyroscopic.y = gyroData.y * _gyro_dps_digit;
		gyroscopic.z = gyroData.z * _gyro_dps_digit;
	}
	
	void LSM9DS0::readTemp() {
		unsigned char* result = new unsigned char[2];
		Device->read(_bus, LSM9DS0_ADDRESS_ACCELMAG, 0x80 | LSM9DS0_REGISTER_TEMP_OUT_L_XM, 2, result);
				
		unsigned char xlo = result[0];
		signed short xhi = result[1];
		
		delete result;
		
		xhi <<= 8;
		xhi |= xlo;
		
		tempData = xhi;
		
		temperature = 21.0 + (float) tempData / 8;
	}
	
} // namespace
