#include "DS1307.h"

#include <cstdio>	// pulls in printf()
#include <ctime>	// pulls in date/time library

using namespace std;

namespace PatricksDrivers {
	
	DS1307::DS1307(unsigned char bus) {
		Device = I2C_IO_Singleton::getInstance();
		_bus = bus;
	}
	
	struct tm* DS1307::read_time() {
		struct tm* result;
		return result;
	}
	
	void DS1307::write_time (struct tm* newDateTime) {
		
	}
	
	void DS1307::toggle_CH() {
		
	}
	
	bool DS1307::CH_enabled() {
		return false;
	}
	
	void DS1307::toggle_mode() {
		
	}
	
	hourMode_t DS1307::read_mode() {
		hourMode_t result;
		return result;
	}
	
	void DS1307::toggle_out() {
		
	}
	
	out_t DS1307::read_out() {
		out_t result;
		return result;
	}
	
	void DS1307::set_rate(rateSelect_t freq) {
		
	}
	
	rateSelect_t DS1307::read_rate() {
		rateSelect_t result;
		return result;
	}
	
	void DS1307::toggle_SQWE() {
		
	}
	
	bool DS1307::SQWE_enabled() {
		return false;
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
