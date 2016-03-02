#ifndef __BUTTON_H
#define __BUTTON_H

#include "../BBIO_Lite/GPIO.h"

namespace PatricksDrivers {
	
	class Button {
		private:
			BBIO::GPIO Pin;
		public:
			Button(const char* key);
			void wait();
			void wait(BBIO::GPIO::CallbackType callback);
	}; // class button
	
} // namespace

#endif
