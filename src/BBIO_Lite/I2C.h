/*
 * Mod By:	Patrick Sanford
 * Version:	0.0
 * Date:	Feb 12, 2016
 * GitHub:	https://github.com/PSanf2/BBB
 * 			https://github.com/adafruit
 * Desc:	This is my modification of the Adafruit Beaglebone IO Python
 * 			library.
 */

#ifndef __BBIO_I2C_H
#define __BBIO_I2C_H

#include "Common.h"	// Inclue BBIO_Lite Common library.

namespace BBIO {
	
	#define BBB_I2C_0 "/dev/i2c-0"
	#define BBB_I2C_1 "/dev/i2c-1"
	
	class I2C {
		public:
			I2C();
			~I2C();
			int get_error();
			
			unsigned char write(
				unsigned char bus,
				unsigned char addr,
				unsigned char num,
				unsigned char* vals
			); // write
			
			unsigned char* read(
				unsigned char bus,
				unsigned char addr,
				unsigned char reg,
				unsigned char num,
				unsigned char* result
			); // read
			
			unsigned char* read(
				unsigned char bus,
				unsigned char addr,
				unsigned char num,
				unsigned char* result
			); // read
			
			unsigned char writeRegister(
				unsigned char bus,
				unsigned char addr,
				unsigned char reg,
				unsigned char val
			); // writeRegister
			
			unsigned char* readRegister(
				unsigned char bus,
				unsigned char addr,
				unsigned char reg,
				unsigned char* result
			); // readRegister
			
		private:
			unsigned char error_code;
			int file;
			int open(unsigned char bus, unsigned char addr);
			void close();
			bool is_open();
	}; // class I2C
	
} // namespace BBIO

#endif
