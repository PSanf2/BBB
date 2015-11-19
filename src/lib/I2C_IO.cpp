#include "I2C_IO.h"

#include <string>			// pulls in string library
#include <sys/ioctl.h>		// lets me use ioctl()
#include <linux/i2c-dev.h>	// lets me use I2C_SLAVE
#include <fcntl.h>			// lets me use O_RDWR

using namespace std;

namespace PatricksDrivers {
	
	I2C_IO::I2C_IO(unsigned char bus, unsigned char addr) {
		_bus = bus;
		_addr = addr;
		_error_code = 0;
		_file = -1;
	}
	
	int I2C_IO::open() {
		string file_name;
		// figure out the file name
		if (_bus == 0)
			file_name = BBB_I2C_0;
		else
			file_name = BBB_I2C_1;
		// open the bus
		if ((_file = ::open(file_name.c_str(), O_RDWR)) < 0) {
			_error_code = 1;
			return 1;
		}
		// initiate communication with the device
		if (ioctl(_file, I2C_SLAVE, _addr) < 0) {
			_error_code = 2;
			return 2;
		}
		
		_error_code = 0;
		return 0;
	}
	
	bool I2C_IO::is_open() {
		if (_file != -1)
			return true;
		else
			return false;
	}
	
	void I2C_IO::close() {
		::close(_file);
		_file = -1;
	}
	
	unsigned char I2C_IO::readRegister(unsigned int reg) {
		
	}
	
	unsigned char* I2C_IO::readRegisters(unsigned int reg, unsigned char num) {
		
	}
	
	int I2C_IO::writeRegister(unsigned int reg, unsigned char val) {
		
	}
	
	int I2C_IO::write(unsigned char val) {
		
	}
	
	I2C_IO::~I2C_IO() {
		if (is_open())
			close();
	}
	
}
