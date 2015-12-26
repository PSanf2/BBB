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
