/*
 * Author:	Patrick Sanford
 * Version:	v0.2
 * Date:	November 29, 2015
 * GitHub:	https://github.com/PSanf2/BBB
 * Descrip:	This class is for a Singleton object that will be used to
 * 			provide a unified interface for interacting with devices on
 * 			either BBB I2C bus. By intention, this class is going to be
 * 			pretty "dumb." 
 */

#ifndef __I2C_IO_SINGLETON_H
#define __I2C_IO_SINGLETON_H

namespace PatricksDrivers {
	
	// These are visable within the namespace.
	#define BBB_I2C_0 "/dev/i2c-0"
	#define BBB_I2C_1 "/dev/i2c-1"
	
	class I2C_IO_Singleton {
		public:
			static I2C_IO_Singleton* getInstance();
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
			I2C_IO_Singleton();
			I2C_IO_Singleton(const I2C_IO_Singleton&);
			I2C_IO_Singleton& operator= (const I2C_IO_Singleton&);
			~I2C_IO_Singleton();
			int open(unsigned char bus, unsigned char addr);
			void close();
			bool is_open();
	}; // class I2C_IO_Singleton
} // namepace PatricksDrivers

#endif
