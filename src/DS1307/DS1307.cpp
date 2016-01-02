#include "DS1307.h"

#include <cstdio>	// pulls in printf()
#include <ctime>	// pulls in date/time library

using namespace std;

namespace PatricksDrivers {
	
	DS1307::DS1307(unsigned char bus) {
		Device = I2C_IO_Singleton::getInstance();
		_bus = bus;
	}
	
	struct tm* DS1307::read_time(struct tm* result) {
		/*
		 * The struct tm type is mostly compatable with the registers on
		 * the chip. One item to question is how the tm_year member is
		 * being handled by conversion functions in the ctime library.
		 * The docs say the member holds the years since 1900. The chip
		 * counts the years since 2000 in the year register. Trying to
		 * input values assuming the year register starts from 1900 seems
		 * to cause some sort of issue. Either way, in order to proper
		 * read the date off the chip I'll need to evaulate all the
		 * appropriate registers.
		 */
		
		 /*
		  * Zero out all of the values received in result.
		  * Get as much data as possible from the chip.
		  * Convert the data into information, populate as many members of result as possible.
		  * //Run result rhough mktime() to populate any missing members.
		  * //Immediatly run that result throuhg gmtime to get a properly populated tm.
		  * //Use the values in that tm to populate result.
		  * Don't do those last three steps. I'm returning what's on the chip, not what
		  * the system thinks of what's on the chip. If the calling function wants the returned
		  * struct to have all possible values populated then let the programmer worry about it.
		  */
		
		result->tm_sec = 0;
		result->tm_min = 0;
		result->tm_hour = 0;
		result->tm_mday = 0;
		result->tm_mon = 0;
		result->tm_year = 0;
		result->tm_wday = 0;
		result->tm_yday = 0;
		result->tm_isdst = 0;
		
		// read six bytes from the device starting at 0x0.
		unsigned char* rawData = new unsigned char[7];
		Device->read(_bus, DS1307_DEV_ADDR, 0x00, 7, rawData);
		
		// this is for debugging
		//for (int i = 0; i < 7; i++)
		//	printf("\n0x%X = 0x%X", i, rawData[i]);
		
		// turn the data into information
		unsigned char info = 0;
		
		// seconds
		info =  (10 * ((rawData[0] & 0x70) >> 4)) + (rawData[0] & 0x0F);
		result->tm_sec = info;
		
		// minutes
		info =  (10 * ((rawData[1] & 0x70) >> 4)) + (rawData[1] & 0x0F);
		result->tm_min = info;
		
		// hours
		if (rawData[2] & (1 << 6)) { // 12 hour mode
			// needs to be read, and converted into 24 hour mode for the result
			info =  (10 * ((rawData[2] & 0x10) >> 4)) + (rawData[2] & 0x0F);
			if (rawData[2] & (1 << 5))
				info += 12;
		} else { // 24 hour mode
			info =  (10 * ((rawData[2] & 0x30) >> 4)) + (rawData[2] & 0x0F);
		}
		result->tm_hour = info;
		
		// day of the week
		// on the chip 1 = sunday. in the result 0 = sunday
		info = rawData[3] - 1;
		result->tm_wday = info;
		
		// day of the month
		info =  (10 * ((rawData[4] & 0x30) >> 4)) + (rawData[4] & 0x0F);
		result->tm_mday = info;
		
		// month
		// on the chip 1 = jan. in the result 0 = jan
		info =  ((10 * ((rawData[5] & 0x10) >> 4)) + (rawData[5] & 0x0F)) - 1;
		result->tm_mon = info;
		
		// year
		// on the chip it's the years since 1900, and same on the result
		// not entirely sure if this is 100% accurate.
		// there seems to be some odd behavior when linux sets the year.
		info =  100 + (10 * ((rawData[6] & 0xF0) >> 4)) + (rawData[6] & 0x0F);
		result->tm_year = info;
		
		/*
		// print some things for debugging
		printf("\nresult->tm_sec =\t%i", result->tm_sec);
		printf("\nresult->tm_min =\t%i", result->tm_min);
		printf("\nresult->tm_hour =\t%i", result->tm_hour);
		printf("\nresult->tm_mday =\t%i", result->tm_mday);
		printf("\nresult->tm_mon =\t%i", result->tm_mon);
		printf("\nresult->tm_year =\t%i", result->tm_year);
		printf("\nresult->tm_wday =\t%i", result->tm_wday);
		printf("\nresult->tm_yday =\t%i", result->tm_yday);
		printf("\nresult->tm_isdst =\t%i", result->tm_isdst);
		*/
		
		delete rawData;
		
		return result;
	}
	
	void DS1307::write_time (struct tm* newDateTime) {
		
	}
	
	void DS1307::toggle_CH() {
		unsigned char* val = new unsigned char[1];
		Device->readRegister(_bus, DS1307_DEV_ADDR, 0x00, val);
		unsigned char result = val[0];
		delete val;
		result ^= (1 << 7);
		Device->writeRegister(_bus, DS1307_DEV_ADDR, 0x00, result);
	}
	
	bool DS1307::CH_enabled() {
		unsigned char* val = new unsigned char[1];
		Device->read(_bus, DS1307_DEV_ADDR, 0x00, 1, val);
		unsigned char result = val[0];
		delete val;
		return (result & (1 << 7));
	}
	
	void DS1307::toggle_mode() {
		unsigned char* val = new unsigned char[1];
		Device->readRegister(_bus, DS1307_DEV_ADDR, 0x02, val);
		unsigned char result = val[0];
		delete val;
		result ^= (1 << 6);
		Device->writeRegister(_bus, DS1307_DEV_ADDR, 0x02, result);
	}
	
	hourMode_t DS1307::read_mode() {
		unsigned char* val = new unsigned char[1];
		Device->readRegister(_bus, DS1307_DEV_ADDR, 0x02, val);
		unsigned char result = val[0];
		delete val;
		// (condition) ? (if_true) : (if_flase)
		return (result & (1 << 6)) ? TWELVEHOURMODE : TWENTYFOURHOURMODE;
	}
	
	void DS1307::toggle_out() {
		unsigned char* val = new unsigned char[1];
		Device->readRegister(_bus, DS1307_DEV_ADDR, 0x07, val);
		unsigned char result = val[0];
		delete val;
		result ^= (1 << 7);
		Device->writeRegister(_bus, DS1307_DEV_ADDR, 0x07, result);
	}
	
	out_t DS1307::read_out() {
		unsigned char* val = new unsigned char[1];
		Device->readRegister(_bus, DS1307_DEV_ADDR, 0x07, val);
		unsigned char result = val[0];
		delete val;
		return (result & (1 << 7)) ? HIGH : LOW;
	}
	
	void DS1307::set_rate(rateSelect_t freq) {
		unsigned char* val = new unsigned char[1];
		Device->readRegister(_bus, DS1307_DEV_ADDR, 0x07, val);
		unsigned char result = val[0];
		delete val;
		result &= ~(1 << 0);
		result &= ~(1 << 1);
		switch (freq) {
			case ONE_HZ:
				;
			break;
			case FOUR_KHZ:
				result |= (1 << 0);
			break;
			case EIGHT_KHZ:
				result |= (1 << 1);
			break;
			case THIRTYTWO_KHZ:
				result |= (1 << 0);
				result |= (1 << 1);
			break;
		}
		Device->writeRegister(_bus, DS1307_DEV_ADDR, 0x07, result);
	}
	
	rateSelect_t DS1307::read_rate() {
		unsigned char* val = new unsigned char[1];
		Device->readRegister(_bus, DS1307_DEV_ADDR, 0x07, val);
		unsigned char result = val[0];
		delete val;
		bool RS0 = result & (1 << 0);
		bool RS1 = result & (1 << 1);
		if (!RS1 && !RS0)
			return ONE_HZ;
		else if (!RS1 && RS0)
			return FOUR_KHZ;
		else if (RS1 && !RS0)
			return EIGHT_KHZ;
		else // (RS1 && RS0)
			return THIRTYTWO_KHZ;
	}
	
	void DS1307::toggle_SQWE() {
		unsigned char* val = new unsigned char[1];
		Device->readRegister(_bus, DS1307_DEV_ADDR, 0x07, val);
		unsigned char result = val[0];
		delete val;
		result ^= (1 << 4);
		Device->writeRegister(_bus, DS1307_DEV_ADDR, 0x07, result);
	}
	
	bool DS1307::SQWE_enabled() {
		unsigned char* val = new unsigned char[1];
		Device->readRegister(_bus, DS1307_DEV_ADDR, 0x07, val);
		unsigned char result = val[0];
		delete val;
		// (condition) ? (if_true) : (if_flase)
		return (result & (1 << 4)) ? true : false;
	}
	
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
	
	template <class T*> void DS1307::write_RAM(unsigned char start_addr, T* val) {
		
	}
	
	template <class T*> T* DS1307::read_RAM(unsigned char start_addr, T* result) {
		return result;
	}
	
} // namespace
