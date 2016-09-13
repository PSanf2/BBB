#ifndef __IR_LED_SENSOR_H
#define __IR_LED_SENSOR_H

#include "../BBIO_Lite/PWM.h"
#include "../BBIO_Lite/GPIO.h"

namespace PatricksDrivers {
	
	class IR_LED_Sensor {
		private:
			BBIO::PWM LED_pin;
			BBIO::GPIO Sensor_pin;
			bool threadRunning;
			pthread_t thread;
			friend void* threadedFunction(void* value);
		public:
			IR_LED_Sensor(const char* led_key, const char* sensor_key);
			int run(BBIO::GPIO::CallbackType callback);
			void stop();
	}; // class IR_LED_Sensor
	
} // namespace PatricksDrivers

#endif
