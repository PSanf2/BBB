#ifndef __LED_H
#define __LED_H

#include "../BBIO_Lite/GPIO.h"

namespace PatricksDrivers {
	
	class LED {
		private:
			//GPIO_Singleton* Gpio;
			//GPIO_Singleton::GPIO_Pin* Pin;
			BBIO::GPIO Pin;
		public:
			LED(const char* key);
			~LED();
			void on();
			void off();
	}; // class LED
	
} // namespace PatricksDrivers

#endif
