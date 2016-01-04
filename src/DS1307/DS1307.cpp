#include "DS1307.h"
#include <ctime>	// pulls in date/time library

#include <cstdio>	// pulls in printf()

using namespace std;

namespace PatricksDrivers {
	
	DS1307::DS1307(unsigned char bus) {
		Device = I2C_IO_Singleton::getInstance();
		_bus = bus;
	}
	
	struct tm* DS1307::read_time(struct tm* result) {
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
} // namespace
