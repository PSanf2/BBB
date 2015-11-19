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
			unsigned char* readRegisters(unsigned int reg, unsigned char num);
			int writeRegister(unsigned int reg, unsigned char val);
			int write(unsigned char val);
			~I2C_IO();
		protected:
			unsigned char _bus;
			unsigned char _addr;
			int _error_code;	// will be used for error checking
		private:
			int _file;
	};
	
}

#endif
