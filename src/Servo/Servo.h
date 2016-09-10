#ifndef __SERVO_H
#define __SERVO_H

#include "../BBIO_Lite/PWM.h"

namespace PatricksDrivers {
	
	class Servo {
		private:
			BBIO::PWM PWMObj;
		public:
			Servo(
				const char* servo_key
			);
			~Servo();
			void start();
			void stop();
			void duty(int n);
	}; // class Servo
	
} // namespace PatricksDrivers

#endif
