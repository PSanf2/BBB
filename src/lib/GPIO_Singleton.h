/*
 *
 */

#ifndef __GPIO_SINGLETON_H
#define __GPIO_SINGLETON_H

#include <vector>	// dynamically sized array

namespace PatricksDrivers {
	
	#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]));
	
	#define GPIO_PATH "/sys/class/gpio/"
	
	typedef struct gpio_pin_t {
		const char* name;
		const char* key;
		int gpio;
	} gpio_pin_t;
	
	const gpio_pin_t pinInfo[] = {
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
	
	enum GPIO_DIRECTION { INPUT, OUTPUT };
	enum GPIO_VALUE { LOW, HIGH };
	
	class GPIO_Singleton {
		public:
			// nested class
			class GPIO_Pin {
				public:
					GPIO_Pin(gpio_pin_t info);
					~GPIO_Pin();
					int setDirection(GPIO_DIRECTION dir);
					int setValue(GPIO_VALUE val);
					GPIO_DIRECTION getDirection();
					GPIO_VALUE getValue();
					const char* getName();
					const char* getKey();
					int getGpio();
				private:
					GPIO_Singleton* _gpio;
					gpio_pin_t _info;
			}; // class GPIO_Pin
			
			static GPIO_Singleton* getInstance();
			GPIO_Pin* setup(const char* key);
			void cleanup(const char* key);
		private:
			// variables
			std::vector<GPIO_Pin> _pin;	// a place to store instance GPIO_Pin with dynamic memory
			// methods
			GPIO_Singleton();
			GPIO_Singleton(const GPIO_Singleton&);
			GPIO_Singleton& operator= (const GPIO_Singleton&);
			~GPIO_Singleton();
			int exportGPIO(int gpio);
			int unexportGPIO(int gpio);
	}; // class GPIO_Singleton
	
} // namespace PatricksDrivers

#endif
