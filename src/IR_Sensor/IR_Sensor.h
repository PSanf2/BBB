#ifndef __IR_SENSOR_H
#define __IR_SENSOR_H

#include "../BBIO_Lite/GPIO.h"

namespace PatricksDrivers {
	
	class IR_Sensor {
		private:
			BBIO::GPIO Pin;
		public:
			IR_Sensor(const char* key);
			void wait(BBIO::GPIO::CallbackType callback);
	}; // class IR_Sensor
	
} // namespace

#endif
