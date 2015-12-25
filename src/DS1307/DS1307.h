#ifndef __DS1307_H
#define __DS1307_H

#include "../lib/I2C_IO_Singleton.h"
#include <cstdio>

namespace PatricksDrivers {
	
	#define DS1307_DEV_ADDR 0x68
	
	typedef enum
	{
		AM = 0,
		PM = 1
	} meridian_t;
	
	typedef enum
	{
		twentyFour = 0,
		twelve = 1
	} hourMode_t;
	
	typedef enum
	{
		ONE_HZ = 0x0,
		FOUR_KHZ = 0x1,
		EIGHT_KHZ = 0x2,
		THIRTYTWO_KHZ = 0x3
	} rateSelect_t;
	
	typedef enum
	{
		LOW = 0,
		HIGH = 1
	} out_t;
	
	class DS1307 {
		private:
			I2C_IO_Singleton* Device;
			unsigned char _bus;
		public:
			DS1307(unsigned char bus);
			~DS1307();
			
			struct tm read_time();
			void set_time (struct tm newDateTime);
			void toggle_CH();
			bool CH_enabled();
			void toggle_mode();
			hourMode_t read_mode();
			void toggle_out();
			out_t read_out();
			void set_rate(rateSelect_t freq);
			rateSelect_t read_rate();
			void toggle_SQWE();
			bool SQWE_enabled();
			
			/* REQUIRED FUNCTIONS
			 * Funtions to read/write from the 56 bytes of battery backed RAM on the chip
			 * 		The RAM is available in addresses 0x08 - 0x3F.
			 * 		I'll want to be able to write any C++ standard data type to the RAM.
			 * 		I want to accept a register address, and a variable. Evaluate the variable
			 * 		to determine it's type, and figure out how many bytes to write. Make sure my
			 * 		first byte is a valid register to write to, make sure I won't write to an address
			 * 		beyond 0x3F, and write the bytes to the registers.
			 * 		I'll need to see if there's some function parameter type witchery that will
			 * 		allow a function to accept an argument of an arbitrary type. I'll want to use
			 * 		function templates for this.
			 * 		For reading functions I'll want to know what type of a variable I'll be
			 * 		getting from the RAM so I can cast the result before returning it. I'll
			 * 		accept an address, a type, make sure I don't try to read past 0x3F, cast
			 * 		the result to the proper type, and retun it.
			 * 		I'm not going to worry about memory management other than making sure I'm
			 * 		reading/writing from valid register addresses.
			 */
			 
	}; // class
	
} // namespace

#endif
