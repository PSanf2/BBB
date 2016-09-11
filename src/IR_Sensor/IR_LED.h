#ifndef __IR_LED_H
#define __IR_LED_H

#include "../BBIO_Lite/GPIO.h"

namespace PatricksDrivers {
	
	class IR_LED {
		private:
			BBIO::GPIO Pin;
			bool threadRunning;
			pthread_t thread;
			friend void* threadedFunction(void* value);
		public:
			typedef int (*CallbackType) (int);
			IR_LED(const char* key);
			void on();
			void off();
			int run();
			void stop();
	}; // class IR_LED
	
} // namespace PatricksDrivers

#endif
