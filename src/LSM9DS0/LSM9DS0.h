#ifndef __LSM9DS0_H
#define __LSM9DS0_H

#include "../lib/I2C_IO_Singleton.h"
#include <cstdio>

namespace PatricksDrivers {
	
	// Linear Acceleration: mg per LSB
	#define LSM9DS0_ACCEL_MG_LSB_2G			(0.061F)
	#define LSM9DS0_ACCEL_MG_LSB_4G			(0.122F)
	#define LSM9DS0_ACCEL_MG_LSB_6G			(0.183F)
	#define LSM9DS0_ACCEL_MG_LSB_8G			(0.244F)
	#define LSM9DS0_ACCEL_MG_LSB_16G		(0.732F) // Is this right? Was expecting 0.488F

	// Magnetic Field Strength: gauss range
	#define LSM9DS0_MAG_MGAUSS_2GAUSS		(0.08F)
	#define LSM9DS0_MAG_MGAUSS_4GAUSS		(0.16F)
	#define LSM9DS0_MAG_MGAUSS_8GAUSS		(0.32F)
	#define LSM9DS0_MAG_MGAUSS_12GAUSS		(0.48F)

	// Angular Rate: dps per LSB
	#define LSM9DS0_GYRO_DPS_DIGIT_245DPS	(0.00875F)
	#define LSM9DS0_GYRO_DPS_DIGIT_500DPS	(0.01750F)
	#define LSM9DS0_GYRO_DPS_DIGIT_2000DPS	(0.07000F)

	// Temperature: LSB per degree celsius
	#define LSM9DS0_TEMP_LSB_DEGREE_CELSIUS	(8)  // 1^0C = 8, 25^o = 200, etc.

	// This comes from the Adafruit_Sensor.h file.
	#define SENSORS_GRAVITY_EARTH			(9.80665F)              /**< Earth's gravity in m/s^2 */
	#define SENSORS_GRAVITY_MOON			(1.6F)                  /**< The moon's gravity in m/s^2 */
	#define SENSORS_GRAVITY_SUN				(275.0F)                /**< The sun's gravity in m/s^2 */
	#define SENSORS_GRAVITY_STANDARD		(SENSORS_GRAVITY_EARTH)
	#define SENSORS_MAGFIELD_EARTH_MAX		(60.0F)                 /**< Maximum magnetic field on Earth's surface */
	#define SENSORS_MAGFIELD_EARTH_MIN		(30.0F)                 /**< Minimum magnetic field on Earth's surface */
	#define SENSORS_PRESSURE_SEALEVELHPA	(1013.25F)              /**< Average sea level pressure is 1013.25 hPa */
	#define SENSORS_DPS_TO_RADS				(0.017453293F)          /**< Degrees/s to rad/s multiplier */
	#define SENSORS_GAUSS_TO_MICROTESLA		(100)                   /**< Gauss to micro-Tesla multiplier */

	// typedefs
	// The typedefs below were coppied from the Adafruit code.
	// It's being used here under the BSD license that Adafruit published their code under.
	// https://github.com/adafruit/Adafruit_LSM9DS0_Library
	// Written by Kevin Townsend for Adafruit Industries
	typedef enum
	{
	  LSM9DS0_REGISTER_WHO_AM_I_G          = 0x0F,
	  LSM9DS0_REGISTER_CTRL_REG1_G         = 0x20,
	  LSM9DS0_REGISTER_CTRL_REG3_G         = 0x22,
	  LSM9DS0_REGISTER_CTRL_REG4_G         = 0x23,
	  LSM9DS0_REGISTER_OUT_X_L_G           = 0x28,
	  LSM9DS0_REGISTER_OUT_X_H_G           = 0x29,
	  LSM9DS0_REGISTER_OUT_Y_L_G           = 0x2A,
	  LSM9DS0_REGISTER_OUT_Y_H_G           = 0x2B,
	  LSM9DS0_REGISTER_OUT_Z_L_G           = 0x2C,
	  LSM9DS0_REGISTER_OUT_Z_H_G           = 0x2D,
	} lsm9ds0GyroRegisters_t;

	typedef enum
	{
	  LSM9DS0_REGISTER_TEMP_OUT_L_XM       = 0x05,
	  LSM9DS0_REGISTER_TEMP_OUT_H_XM       = 0x06,
	  LSM9DS0_REGISTER_STATUS_REG_M        = 0x07,
	  LSM9DS0_REGISTER_OUT_X_L_M           = 0x08,
	  LSM9DS0_REGISTER_OUT_X_H_M           = 0x09,
	  LSM9DS0_REGISTER_OUT_Y_L_M           = 0x0A,
	  LSM9DS0_REGISTER_OUT_Y_H_M           = 0x0B,
	  LSM9DS0_REGISTER_OUT_Z_L_M           = 0x0C,
	  LSM9DS0_REGISTER_OUT_Z_H_M           = 0x0D,
	  LSM9DS0_REGISTER_WHO_AM_I_XM         = 0x0F,
	  LSM9DS0_REGISTER_INT_CTRL_REG_M      = 0x12,
	  LSM9DS0_REGISTER_INT_SRC_REG_M       = 0x13,
	  LSM9DS0_REGISTER_CTRL_REG1_XM        = 0x20,
	  LSM9DS0_REGISTER_CTRL_REG2_XM        = 0x21,
	  LSM9DS0_REGISTER_CTRL_REG5_XM        = 0x24,
	  LSM9DS0_REGISTER_CTRL_REG6_XM        = 0x25,
	  LSM9DS0_REGISTER_CTRL_REG7_XM        = 0x26,
	  LSM9DS0_REGISTER_OUT_X_L_A           = 0x28,
	  LSM9DS0_REGISTER_OUT_X_H_A           = 0x29,
	  LSM9DS0_REGISTER_OUT_Y_L_A           = 0x2A,
	  LSM9DS0_REGISTER_OUT_Y_H_A           = 0x2B,
	  LSM9DS0_REGISTER_OUT_Z_L_A           = 0x2C,
	  LSM9DS0_REGISTER_OUT_Z_H_A           = 0x2D,
	} lsm9ds0MagAccelRegisters_t;

	typedef enum
	{
	  LSM9DS0_ACCELRANGE_2G                = (0b000 << 3),
	  LSM9DS0_ACCELRANGE_4G                = (0b001 << 3),
	  LSM9DS0_ACCELRANGE_6G                = (0b010 << 3),
	  LSM9DS0_ACCELRANGE_8G                = (0b011 << 3),
	  LSM9DS0_ACCELRANGE_16G               = (0b100 << 3)
	} lsm9ds0AccelRange_t;

	typedef enum
	{
	  LSM9DS0_ACCELDATARATE_POWERDOWN      = (0b0000 << 4),
	  LSM9DS0_ACCELDATARATE_3_125HZ        = (0b0001 << 4),
	  LSM9DS0_ACCELDATARATE_6_25HZ         = (0b0010 << 4),
	  LSM9DS0_ACCELDATARATE_12_5HZ         = (0b0011 << 4),
	  LSM9DS0_ACCELDATARATE_25HZ           = (0b0100 << 4),
	  LSM9DS0_ACCELDATARATE_50HZ           = (0b0101 << 4),
	  LSM9DS0_ACCELDATARATE_100HZ          = (0b0110 << 4),
	  LSM9DS0_ACCELDATARATE_200HZ          = (0b0111 << 4),
	  LSM9DS0_ACCELDATARATE_400HZ          = (0b1000 << 4),
	  LSM9DS0_ACCELDATARATE_800HZ          = (0b1001 << 4),
	  LSM9DS0_ACCELDATARATE_1600HZ         = (0b1010 << 4)
	} lm9ds0AccelDataRate_t;

	typedef enum
	{
	  LSM9DS0_MAGGAIN_2GAUSS               = (0b00 << 5),  // +/- 2 gauss
	  LSM9DS0_MAGGAIN_4GAUSS               = (0b01 << 5),  // +/- 4 gauss
	  LSM9DS0_MAGGAIN_8GAUSS               = (0b10 << 5),  // +/- 8 gauss
	  LSM9DS0_MAGGAIN_12GAUSS              = (0b11 << 5)   // +/- 12 gauss
	} lsm9ds0MagGain_t;

	typedef enum
	{
	  LSM9DS0_MAGDATARATE_3_125HZ          = (0b000 << 2),
	  LSM9DS0_MAGDATARATE_6_25HZ           = (0b001 << 2),
	  LSM9DS0_MAGDATARATE_12_5HZ           = (0b010 << 2),
	  LSM9DS0_MAGDATARATE_25HZ             = (0b011 << 2),
	  LSM9DS0_MAGDATARATE_50HZ             = (0b100 << 2),
	  LSM9DS0_MAGDATARATE_100HZ            = (0b101 << 2)
	} lsm9ds0MagDataRate_t;

	typedef enum
	{
	  LSM9DS0_GYROSCALE_245DPS             = (0b00 << 4),  // +/- 245 degrees per second rotation
	  LSM9DS0_GYROSCALE_500DPS             = (0b01 << 4),  // +/- 500 degrees per second rotation
	  LSM9DS0_GYROSCALE_2000DPS            = (0b10 << 4)   // +/- 2000 degrees per second rotation
	} lsm9ds0GyroScale_t;

	typedef struct vector_s
	{
	  float x;
	  float y;
	  float z;
	} lsm9ds0Vector_t;
	// End Kevin Townsend typedefs

	typedef enum
	{
		LSM9DS0_ADDRESS_GYRO = 0x6B,
		LSM9DS0_ADDRESS_ACCELMAG = 0x1D,
		LSM9DS0_XM_ID = 0b01001001,
		LSM9DS0_G_ID = 0b11010100
	} lsm9ds0DeviceInfo;
	
	class LSM9DS0 {
		private:
			I2C_IO_Singleton* Device;
			float _accel_mg_lsb;
			float _mag_mgauss_lsb;
			float _gyro_dps_digit;
			unsigned char _bus;
			int _err;
			lsm9ds0AccelRange_t _range;
			lsm9ds0MagGain_t _gain;
			lsm9ds0GyroScale_t _scale;
		public:
			// these hold human readable information
			lsm9ds0Vector_t acceleration;
			lsm9ds0Vector_t magnetic;
			lsm9ds0Vector_t gyroscopic;
			float temperature;
			// these hold raw sensor daya
			lsm9ds0Vector_t accelData;
			lsm9ds0Vector_t magData;
			lsm9ds0Vector_t gyroData;
			short tempData;
			
			LSM9DS0(
				unsigned char bus,
				lsm9ds0AccelRange_t accelRange,
				lsm9ds0MagGain_t magGain,
				lsm9ds0GyroScale_t gyroScale
			); // constructor
			~LSM9DS0();
			void readAccel();
			void readMag();
			void readGyro();
			void readTemp();
	}; // class
	
} // namespace PatricksDrivers

#endif
