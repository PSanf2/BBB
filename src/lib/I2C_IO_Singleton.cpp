#include "I2C_IO_Singleton.h"

#include <string>			// pulls in the string library
#include <sys/ioctl.h>		// lets me use ioctl()
#include <linux/i2c-dev.h>	// lets me use I2C_SLAVE
#include <fcntl.h>			// lets me use O_RDWR

using namespace std;

namespace PatricksDrivers {
	
	// i'm using a long name becuase i don't want another variable
	// named instance to cause issues elsewhere.
	static I2C_IO_Singleton* I2C_IO_Singleton_instance;
	
	I2C_IO_Singleton* I2C_IO_Singleton::getInstance() {
		if (!I2C_IO_Singleton_instance)
			I2C_IO_Singleton_instance = new I2C_IO_Singleton();
		return I2C_IO_Singleton_instance;
	}
	
	I2C_IO_Singleton::I2C_IO_Singleton() {
		error_code = -1;
		file = -1;
	}
	
	I2C_IO_Singleton::~I2C_IO_Singleton() {
		close();
		delete I2C_IO_Singleton_instance;
	}
	
	int I2C_IO_Singleton::get_error() {
		return error_code;
	}
	
	unsigned char I2C_IO_Singleton::write(
		unsigned char bus,
		unsigned char addr,
		unsigned char num,
		unsigned char* vals
	) {
		// This function is to be used to write an arbitrary number of values to the bus.
		// Typically, this would mean writting register, and a value to a specific device.
		
		error_code = 0;
		
		// open a connection to the device
		if (open(bus, addr) != 0)
			return error_code;
		
		// write the values to the bus
		if (::write(file, vals, num) != num) {
			close();
			error_code = 5;
			return error_code;
		}
		
		close();
		return error_code;
	} // write
	
	unsigned char* I2C_IO_Singleton::read(
		unsigned char bus,
		unsigned char addr,
		unsigned char reg,
		unsigned char num,
		unsigned char* result
	) {
		// This function will be used to read an arbitrary number of values from a device.
		// The results will be written to the memory provided by the pointer, and ultimately
		// returned.
		
		error_code = 0;
		
		// open a connection to the device
		if (open(bus, addr) != 0)
			return NULL;
		
		// write the starting register on the bus
		unsigned char* buf = new unsigned char[1];
		buf[0] = reg;
		if (::write(file, buf, 1) != 1) {
			close();
			error_code = 3;
			delete buf;
			return NULL;
		}
		delete buf;
		
		// read in the proper number of values
		if (::read(file, result, num) != num) {
			close();
			error_code = 4;
			return NULL;
		}
		
		close();
		return result;
	} // read
	
	
	
	
	
	
	
	
	unsigned char* I2C_IO_Singleton::read(
		unsigned char bus,
		unsigned char addr,
		unsigned char num,
		unsigned char* result
	) {
		// This function will be used to read an arbitrary number of values from a device.
		// The results will be written to the memory provided by the pointer, and ultimately
		// returned.
		
		error_code = 0;
		
		// open a connection to the device
		if (open(bus, addr) != 0)
			return NULL;
		
		// read in the proper number of values
		if (::read(file, result, num) != num) {
			close();
			error_code = 4;
			return NULL;
		}
		
		close();
		return result;
	} // read
	
	
	
	
	
	
	
	
	
	
	int I2C_IO_Singleton::open(unsigned char bus, unsigned char addr) {
		
		error_code = 0;
		string file_name;
		
		// figure out the file name
		if (bus == 0)
			file_name = BBB_I2C_0;
		else
			file_name = BBB_I2C_1;
		
		// open the bus
		if ((file = ::open(file_name.c_str(), O_RDWR)) < 0) {
			error_code = 1;
			return error_code;
		}
		
		//  initiate communication with the device
		if (ioctl(file, I2C_SLAVE, addr) < 0) {
			error_code = 2;
			return error_code;
		}
		
		return error_code;
	}
	
	void I2C_IO_Singleton::close() {
		if (file != -1) {
			::close(file);
			file = -1;
		}
	}
	
	bool I2C_IO_Singleton::is_open() {
		if (file != -1)
			return true;
		else
			return false;
	}
	
	unsigned char* I2C_IO_Singleton::readRegister(
		unsigned char bus,
		unsigned char addr,
		unsigned char reg,
		unsigned char* result
	) {
		// reads and returns the value of a single register on a device
		return read(bus, addr, reg, 1, result);
	}
	
	unsigned char I2C_IO_Singleton::writeRegister(
		unsigned char bus,
		unsigned char addr,
		unsigned char reg,
		unsigned char val
	) {
		error_code = 0;
				
		unsigned char* buf = new unsigned char[2];
		buf[0] = reg;
		buf[1] = val;
		
		if (write(bus, addr, 2, buf) != 0) {
			delete buf;
			return error_code;
		}
		
		delete buf;
		return error_code;
	}
	
} // namepace PatricksDrivers
