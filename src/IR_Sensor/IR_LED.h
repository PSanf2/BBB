#ifndef __IR_LED_H
#define __IR_LED_H

#include "../BBIO_Lite/PWM.h"

namespace PatricksDrivers {
	
	class IR_LED {
		private:
			BBIO::PWM Pin;
			bool threadRunning;
			pthread_t thread;
			friend void* threadedFunction(void* value);
		public:
			IR_LED(const char* key);
			int run();
			void stop();
	}; // class IR_LED
	
} // namespace PatricksDrivers

#endif
