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
			template <class T*> void write_RAM(unsigned char start_addr, T* val);
			template <class T*> T* read_RAM(unsigned char start_addr, T* result);
			 
	}; // class
	
} // namespace

#endif
