#ifndef __TRICOLOR_LED_H
#define __TRICOLOR_LED_H

#include "../BBIO_Lite/PWM.h"

namespace PatricksDrivers {
	
	class TriColor_LED {
		private:
			BBIO::PWM Red, Green, Blue;
		public:
			TriColor_LED(
				const char* red_key,
				const char* green_key,
				const char* blue_key
			);
			
			~TriColor_LED();
	}; // class TriColor_LED
	
} // namespace PatricksDrivers

#endif
