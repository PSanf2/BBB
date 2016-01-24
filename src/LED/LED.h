#ifndef __LED_H
#define __LED_H

#include "../lib/GPIO_Singleton.h"

namespace PatricksDrivers {
	
	class LED {
		private:
			GPIO_Singleton* Gpio;
			GPIO_Singleton::GPIO_Pin* Pin;
		public:
			LED(const char* key);
			~LED();
			void on();
			void off();
	}; // class LED
	
} // namespace PatricksDrivers

#endif
