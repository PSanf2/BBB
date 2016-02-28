#ifndef __BUTTON_H
#define __BUTTON_H

#include "../BBIO_Lite/GPIO.h"

namespace PatricksDrivers {
	
	class Button {
		private:
			BBIO::GPIO Pin;
		public:
			Button(const char* key);
			void wait_for();
	}; // class button
	
} // namespace

#endif
