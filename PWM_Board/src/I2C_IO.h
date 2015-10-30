#ifndef _I2C_IO_H
#define _I2C_IO_H

#define BBB_I2C_0 "/dev/i2c-0"
#define BBB_I2C_1 "/dev/i2c-1"

namespace PatricksDrivers {
	
	class I2C_IO {
		public:
			I2C_IO(unsigned char bus, unsigned char addr);
			int open();
			bool is_open();
			void close();
			unsigned char readRegister(unsigned int reg);
			int writeRegister(unsigned int reg, unsigned char val);
			int write(unsigned char val);
			~I2C_IO();
		protected:
			unsigned char bus;
			unsigned char addr;
			int error_code;	// will be used for error checking
		private:
			int file;
	};
	
}

#endif
