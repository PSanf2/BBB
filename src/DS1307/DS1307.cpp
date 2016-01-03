#include "DS1307.h"
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
			if ((rawData[2] & (1 << 5)) && (info < 12))
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
		delete rawData;
		return result;
	}
	
	void DS1307::write_time (struct tm* newDateTime) {
		// Writing the time to the chip will cause it to go into 24 hour mode.
		// This is because don't want to do a read to determine the current
		// mode, and make a lot of adjustments.
		// Writing the time is also going to disable to Clock Hold.
		// What's the point of setting the time if you're not going to keep up with it?
		unsigned char* newVals = new unsigned char[8];
		// starting register i'll be writing to
		newVals[0] = 0x00;
		// seconds (also disables the CH bit)
		newVals[1] = 0x00 | ((newDateTime->tm_sec / 10) << 4) | (newDateTime->tm_sec % 10);
		// minutes
		newVals[2] = 0x00 | ((newDateTime->tm_min / 10) << 4) | (newDateTime->tm_min % 10);
		// hours (switches to 24 hour mode)
		newVals[3] = 0x00 | ((newDateTime->tm_hour / 10) << 4) | (newDateTime->tm_hour % 10);
		// day of the week (accounting for funky offset)
		newVals[4] = 0x00 | (newDateTime->tm_wday + 1);
		// day of the month
		newVals[5] = 0x00 | ((newDateTime->tm_mday / 10) << 4) | (newDateTime->tm_mday % 10);
		// month of the year (accounting for offset)
		newVals[6] = 0x00 | (newDateTime->tm_mon + 1);
		// years since 1900 or 2000. whatever!
		int year = newDateTime->tm_year - 100;
		newVals[7] = 0x00 | ((year / 10) << 4) | (year % 10);
		//newVals[7] = 0x00 | (((newDateTime->tm_year - 100) / 10) << 4) | ((newDateTime->tm_mday - 100) % 10);
		Device->write(_bus, DS1307_DEV_ADDR, 8, newVals);
		delete newVals;
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
		// read in the value from 0x02.
		unsigned char* val = new unsigned char[1];
		Device->readRegister(_bus, DS1307_DEV_ADDR, 0x02, val);
		unsigned char regVal = val[0];
		delete val;
		// declare a variable for the result
		unsigned char newVal = 0x00;
		// a place to work with my hour value
		unsigned int hour;
		// determine the current mode
		hourMode_t hourMode = (regVal & (1 << 6)) ? TWELVEHOURMODE : TWENTYFOURHOURMODE;
		if (hourMode == TWELVEHOURMODE) {
			// switching to 24 hour mode
			// read the current hour
			hour = (10 * ((regVal & 0x10) >> 4)) + (regVal & 0x0F);
			// read the am/pm bit
			meridian_t am_pm = (regVal & (1 << 5)) ? PM : AM;
			if (am_pm == AM) { // (hour = 12am - 11am)
				if (hour == 12) { // (12am -> 0000)
					hour = 0;
				}
			} else { // am_pm == PM (hour = 12pm - 11pm)
				if (hour < 12) { // (hour = 1pm - 11pm -> 1300 - 2300)
					hour += 12;
				}
			}
			// write the hour to newVal, unset bit 6
			newVal = 0x00 | ((hour / 10) << 4) | (hour % 10);
		} else { // hourMode == TWENTYFOURHOURMODE
			// switching to 12 hour mode
			// read the current hour
			hour = (10 * ((regVal & 0x30) >> 4)) + (regVal & 0x0F);
			if (hour < 12) { // AM (hour = 0000 - 1100)
				if (hour == 0) {
					hour = 12;
				}
				// set newVal here w/ am/pm flag set to 0
				newVal = 0x40 | ((hour / 10) << 4) | (hour % 10);
			} else { // PM (hour = 1200 - 2300)
				if (hour > 12) { // (hour = 1300 - 2300 -> 1pm - 11pm)
					hour -= 12;
				}
				// set newVal here w/ am/pm flag set to 1
				newVal = 0x60 | ((hour / 10) << 4) | (hour % 10);
			}
		}
		// write newVal back to the register on the chip.
		Device->writeRegister(_bus, DS1307_DEV_ADDR, 0x02, newVal);
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
