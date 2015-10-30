#ifndef _PWM_BOARD_H
#define __PWM_BOARD_H

#include "../lib/I2C_IO.h"

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

namespace PatricksDrivers {
	
	class PWM_Board  : public I2C_IO {
		public:
			PWM_Board(unsigned char bus, unsigned char addr);
			void setRefresh(unsigned char rate);
			void wake();
			void sleep();
			void reset();
			void setPWM(unsigned char channel, unsigned int on, unsigned int off);
			void setAllPWM(unsigned int on, unsigned int off);
			unsigned char get_bus();
			unsigned char get_addr();
			int get_error();
			~PWM_Board();
	};
	
}

#endif
