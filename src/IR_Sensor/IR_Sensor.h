#ifndef __IR_SENSOR_H
#define __IR_SENSOR_H

#include "../BBIO_Lite/GPIO.h"

namespace PatricksDrivers {
	
	class IR_Sensor {
		private:
			BBIO::GPIO Pin;
		public:
			IR_Sensor(const char* key);
			void run(BBIO::GPIO::CallbackType callback);
			void stop();
	}; // class IR_Sensor
	
} // namespace

#endif
