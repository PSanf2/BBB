/*
 * Mod By:	Patrick Sanford
 * Version:	0.0
 * Date:	Feb 12, 2016
 * GitHub:	https://github.com/PSanf2/BBB
 * 			https://github.com/adafruit
 * Desc:	This is my modification of the Adafruit Beaglebone IO Python
 * 			library.
 */

#ifndef __BBIO_GPIO_H
#define __BBIO_GPIO_H

#include "Common.h"	// Inclue BBIO_Lite Common library.

namespace BBIO {
	
	#define GPIO_PATH "/sys/class/gpio/"
	
	typedef struct gpio_pin_t {
		const char* name;
		const char* key;
		int gpio;
	} gpio_pin_t;
	
	const gpio_pin_t GPIO_Info[] = {
		{"TIMER4",		"P8_07",	66},
		{"TIMER7",		"P8_08",	67},
		{"TIMER5",		"P8_09",	69},
		{"TIMER6",		"P8_10",	68},
		{"GPIO1_13",	"P8_11",	45},
		{"GPIO1_12",	"P8_12",	44},
		{"GPIO0_26",	"P8_14",	26},
		{"GPIO1_15",	"P8_15",	47},
		{"GPIO1_14",	"P8_16",	46},
		{"GPIO0_27",	"P8_17",	27},
		{"GPIO2_1",		"P8_18",	65},
		{"GPIO1_29",	"P8_26",	61},
		{"GPIO1_28",	"P9_12",	60},
		{"GPIO1_16",	"P9_15",	48},
		{"GPIO1_17",	"P9_23",	49},
		{"GPIO3_21",	"P9_25",	117},
		{"GPIO3_19",	"P9_27",	115},
		{"SPI1_D1",		"P9_30",	112},
		{"CLKOUT2",		"P9_41",	20}
	};
	
	enum GPIO_DIRECTION { INPUT, OUTPUT, DIR_ERR };
	enum GPIO_VALUE { LOW, HIGH, VAL_ERR };
	enum GPIO_EDGE { NONE, RISING, FALLING, BOTH, EDG_ERR };
	
	class GPIO {
		public:
			// de/constructor
			GPIO(const char* key);
			~GPIO();
			// setters
			int direction(GPIO_DIRECTION dir);
			int value(GPIO_VALUE val);
			int edge(GPIO_EDGE val);
			// getters
			GPIO_DIRECTION direction();
			GPIO_VALUE value();
			GPIO_EDGE edge();
			
			const char* name();
			const char* key();
			int gpio();
			// I also need to have functions to take care of events
			int waitForEdge(); // waits forever
			// wait for edge()
		private:
			gpio_pin_t _info;
	}; // class GPIO
	
} // namespace BBIO

#endif
