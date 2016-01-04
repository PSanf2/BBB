#ifndef __PCA9685_H
#define __PCA9685_H

#include "../lib/I2C_IO_Singleton.h"
#include <cstdio>

namespace PatricksDrivers {
	
	#define MODE1 0x0
	#define PRE_SCALE 0xFE
	#define OSC_CLOCK 25000000
	#define LED0_ON_L 0x6
	#define LED0_ON_H 0x7
	#define LED0_OFF_L 0x8
	#define LED0_OFF_H 0x9
	#define ALL_LED_ON_L 0xFA
	#define ALL_LED_ON_H 0xFB
	#define ALL_LED_OFF_L 0xFC
	#define ALL_LED_OFF_H 0xFD
	
	class PCA9685 {
		private:
			I2C_IO_Singleton* Device;
			unsigned char _bus;
			unsigned char _addr;
		public:
			PCA9685(unsigned char bus, unsigned char addr);
			void setRefresh(unsigned char rate);
			void wake();
			void sleep();
			void reset();
			void setPWM(unsigned char channel, unsigned int on, unsigned int off);
			void setAllPWM(unsigned int on, unsigned int off);
			~PCA9685();
	}; // class
	
} // namespace

#endif
