
#ifndef __LIGHTSENSOR_H
#define __LIGHTSENSOR_H

#include "../BBIO_Lite/ADC.h"

namespace PatricksDrivers {
	
	class LightSensor {
		private:
			BBIO::ADC Pin;
		public:
			LightSensor(const char* key);
			int value();
	}; // class
	
} // namespace

#endif
