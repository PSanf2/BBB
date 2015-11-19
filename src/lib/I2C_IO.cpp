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
	
	unsigned char I2C_IO::readRegister(unsigned char reg) {
		// declare a variable to hold the result
		unsigned char buffer[1];
		// open a connection to the device
		// if the open() function returns anything other than 0
		if (open() != 0) {
			// throw a fit and quit
			_error_code = 3;
			return buffer[0];
		}
		// tell the device which register address i want to read from
		// put the address on the buffer
		buffer[0] = reg;
		// write the content of the buffer to the bus
		// if the ::write() function returns something other than 1
		if (::write(_file, buffer, 1) != 1) {
			// throw a fit and quit
			_error_code = 4;
			return buffer[0];
		}
		// read a character from the bus into the buffer
		// if the ::read() function returns something other than 1
		if (::read(_file, buffer, 1) != 1) {
			// throw a fit and quit
			_error_code = 5;
			return buffer[0];
		}
		// close the connection to the device
		close();
		// return the result
		_error_code = 0;
		return buffer[0];
	}
	
	unsigned char* I2C_IO::readRegisters(unsigned char reg, unsigned char num) {
		// declare a pointer to an array
		unsigned char* buffer = new unsigned char[num];
		// open a connection to the device
		if (open() != 0) {
			// or throw a fit and quit
			_error_code = 6;
			return NULL;
		}
		// write the register address onto the bus
		buffer[0] = reg;
		if (::write(_file, buffer, 1) != 1) {
			// or throw a fit and quit
			_error_code = 7;
			return NULL;
		}
		// read the proper number of values off the bus into the aray
		if (::read(_file, buffer, num) != num) {
			// or throw a fit and quit
			_error_code = 8;
			return NULL;
		}
		// close the connection to the device
		close();
		// return the result
		_error_code = 0;
		return buffer;
	}
	
	int I2C_IO::writeRegister(unsigned char reg, unsigned char val) {
		// declare and initialize an array
		unsigned char buffer[2] = {reg, val};
		// open a connection to the device
		if (open() != 0) {
			// or throw a fit and quit
			_error_code = 9;
			return _error_code;
		}
		// write the register and value onto the bus
		if (::write(_file, buffer, 2) != 2) {
			// or throw a fit and quit
			_error_code = 10;
			return _error_code;
		}
		// close the connection to the device
		close();
		// return some information.
		_error_code = 0;
		return _error_code;
	}
	
	int I2C_IO::get_error() {
		return _error_code;
	}
	
	I2C_IO::~I2C_IO() {
		if (is_open())
			close();
	}
	
}
