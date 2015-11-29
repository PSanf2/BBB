/*
 * Author:	Patrick Sanford
 * Version:	v0.2
 * Date:	November 29, 2015
 * GitHub:	https://github.com/PSanf2/BBB
 */

#ifndef __I2C_IO_SINGLETON_H
#define __I2C_IO_SINGLETON_H

namespace PatricksDrivers {
	#define BBB_I2C_0 = "/dev/i2c-0"
	#define BBB_I2C_1 = "/dev/i2c-1"
	
	class I2C_IO_Singleton {
		public:
			static I2C_IO_Singleton* getInstance();
			// readRegister
			// readRegisters
			// writeRegister
			// writeRegisters
			// writeAddress
			// get_error
		protected:
			;
		private:
			I2C_IO_Singleton();
			I2C_IO_Singleton(const I2C_IO_Singleton&);
			I2C_IO_Singleton& operator= (const I2C_IO_Singleton&);
			~I2C_IO_Singleton();
			// open
			// close
			// is_open
	};
}

#endif
