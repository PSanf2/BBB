#ifndef __DS1307_H
#define __DS1307_H

#include "../lib/I2C_IO_Singleton.h"
#include <ctime>	// pulls in date/time library

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
	
	// Template function members must be defined in the header file.
	// If you attempt to define them in the .cpp file it won't compile.
	
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
		
		// create an array to hold the bytes off the chip
		unsigned char* vals = new unsigned char[sizeof(*result)];
		
		// read the proper number of bytes off the chip
		Device->read(_bus, DS1307_DEV_ADDR, start_addr, sizeof(*result), vals);
		
		// shift and OR each byte into the result
		for (int i = 0; i < sizeof(*result); i++) {
			*result |= (vals[i] << (8 * i));
		}
		
		// clean up
		delete vals;
	}
	
} // namespace

#endif
