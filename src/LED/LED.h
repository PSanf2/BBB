#ifndef __LED_H
#define __LED_H

#include "../BBIO_Lite/GPIO.h"

namespace PatricksDrivers {
	
	class LED {
		private:
			BBIO::GPIO Pin;
		public:
			LED(const char* key);
			void on();
			void off();
	}; // class LED
	
} // namespace PatricksDrivers

#endif
