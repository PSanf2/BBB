#include "I2C_IO_Singleton.h"

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
		
	}
	
	I2C_IO_Singleton::~I2C_IO_Singleton() {
		delete I2C_IO_Singleton_instance;
	}
} // namepace PatricksDrivers
