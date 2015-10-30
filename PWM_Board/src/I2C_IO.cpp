#include "I2C_IO.h"

#include <string>			// pulls in string library
#include <sys/ioctl.h>		// lets me use ioctl()
#include <linux/i2c-dev.h>	// lets me use I2C_SLAVE
#include <fcntl.h>			// lets me use O_RDWR

using namespace std;

namespace PatricksDrivers {
	
	I2C_IO::I2C_IO(unsigned char bus, unsigned char addr) {
		this->file = -1;
		this->bus = bus;
		this->addr = addr;
		this->error_code = 0;
		open();
	}
	
	int I2C_IO::open() {
		string file_name;
		
		if (bus == 0)
			file_name = BBB_I2C_0;
		else
			file_name = BBB_I2C_1;
		
		// open the bus
		// The use of ::open() is a system call.
		// By leaving off a class when using the scope resolution opertor
		// you're telling the system to call a function in the global scope.
		// In this instance that creates a system call.
		if ((file = ::open(file_name.c_str(), O_RDWR)) < 0) {
			error_code = 1;
			return 1;
		}
		
		// initiate communication with the device
		if (ioctl(file, I2C_SLAVE, addr) < 0) {
			error_code = 2;
			return 2;
		}
		
		error_code = 0;
		return 0;
	}
	
	bool I2C_IO::is_open() {
		if (file != -1)
			return true;
		else
			return false;
	}
	
	void I2C_IO::close() {
		::close(file);
		file = -1;
	}
	
	unsigned char I2C_IO::readRegister(unsigned int reg) {
		if (write(reg) != 0) {
			error_code = 2;
			return 2;
		}
		unsigned char buffer[1];
		if (::read(file, buffer, 1) != 1) {
			error_code = 1;
			return 1;
		}
		
		error_code = 0;
		return buffer[0];
	}
	
	int I2C_IO::writeRegister(unsigned int reg, unsigned char val) {
		unsigned char buffer[2];
		buffer[0] = reg;
		buffer[1] = val;
		if (::write(file, buffer, 2) != 2) {
			error_code = 1;
			return 1;
		}
		error_code = 0;
		return 0;
	}
	
	int I2C_IO::write(unsigned char val) {
		unsigned char buffer[1];
		buffer[0] = val;
		if (::write(file, buffer, 1) != 1) {
			error_code = 1;
			return 1;
		}
		error_code = 0;
		return 0;
	}
	
	I2C_IO::~I2C_IO() {
		if (file != -1)
			close();
	}
	
}
