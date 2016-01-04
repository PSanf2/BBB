#ifndef __DS1307_H
#define __DS1307_H

#include "../lib/I2C_IO_Singleton.h"
#include <ctime>	// pulls in date/time library

#include <cstdio>	// pulls in printf()

namespace PatricksDrivers {
	
	#define DS1307_DEV_ADDR 0x68
	
	typedef enum
	{
		AM = 0,
		PM = 1
	} meridian_t;
	
	typedef enum
	{
		TWENTYFOURHOURMODE = 0,
		TWELVEHOURMODE = 1
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
			struct tm* read_time(struct tm* result);
			void write_time (struct tm* newDateTime);
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
			template <class T> void write_RAM(unsigned char start_addr, T val);
			template <class T> void read_RAM(unsigned char start_addr, T result);
			 
	}; // class
	
	/* REQUIRED FUNCTION TEMPLATES
	 * 
	 * Function templates needs to be defined in the header file.
	 * Trying to define them in the cpp file won't compile.
	 * 
	 * Funtions to read/write from the 56 bytes of battery backed RAM on the chip
	 * 		The RAM is available in addresses 0x08 - 0x3F.
	 * 		I'll want to be able to write any C++ standard data type to the RAM.
	 * 		I want to accept a register address, and a variable. Evaluate the variable
	 * 		to determine it's type, and figure out how many bytes to write. Make sure my
	 * 		first byte is a valid register to write to, make sure I won't write to an address
	 * 		beyond 0x3F, and write the bytes to the registers.
	 * 		I'll need to see if there's some function parameter type witchery that will
	 * 		allow a function to accept an argument of an arbitrary type. I'll want to use
	 * 		function templates for this. I may end up using a sizeof function on the
	 * 		parameter to determine how many bytes to write. This would allow for a function
	 * 		that doesn't need to use a case statement, and I'd be able to write non-basic
	 * 		variable types.
	 * 		For reading functions I'll want to know what type of a variable I'll be
	 * 		getting from the RAM so I can cast the result before returning it. I'll
	 * 		accept an address, a type, make sure I don't try to read past 0x3F, cast
	 * 		the result to the proper type, and retun it.
	 * 		I'm not going to worry about memory management other than making sure I'm
	 * 		reading/writing from valid register addresses.
	 */
	
	template <class T> void DS1307::write_RAM(unsigned char start_addr, T val) {
		// make sure my starting address is valid
		if (start_addr < 0x08)
			return;
		
		// make sure the end address is valid.
		if ((start_addr + sizeof(*val) - 1) > 0x3F)
			return;
		
		// create an array for what will be written
		unsigned char* newVals = new unsigned char[sizeof(*val) + 1];
		
		// populate the starting address
		newVals[0] = start_addr;
		
		// populate the rest of the array
		for (int i = 1; i < sizeof(*val) + 1; i++) {
			newVals[i] = (*val >> (8 * (i - 1)));
		}
		
		// write the values
		Device->write(_bus, DS1307_DEV_ADDR, sizeof(*val) + 1, newVals);
		
		// clean up
		delete newVals;
	}
	
	template <class T> void DS1307::read_RAM(unsigned char start_addr, T result) {
		// make sure my starting address is valid
		if (start_addr < 0x08)
			return;
		
		// make sure the end address is valid
		if ((start_addr + sizeof(*result) - 1) > 0x3F)
			return;
		
		// clear the current value in *result
		*result = 0;
		
		// create an array to hold the results off the chip
		unsigned char* vals = new unsigned char[sizeof(*result)];
		
		// read the proper number of bytes off the chip
		Device->read(_bus, DS1307_DEV_ADDR, start_addr, sizeof(*result), vals);
		
		// or shift each byte of the read values into the result variable
		for (int i = 0; i < sizeof(*result); i++) {
			*result |= (vals[i] << (8 * i));
		}
		
		// clean up
		delete vals;
	}
	
} // namespace

#endif
