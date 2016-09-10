#include "Servo.h"

#include <cstdio>		// pulls in printf()

using namespace std;

namespace PatricksDrivers {
	
	Servo::Servo(
		const char* servo_key
	) : PWMObj(servo_key) {
		PWMObj.polarity(1);
		start();
	}
	
	Servo::~Servo() {
		
	}
	
	void Servo::start() {
		PWMObj.start();
	}
	
	void Servo::stop() {
		PWMObj.stop();
	}
	
	void Servo::duty(int n) {
		PWMObj.duty(n);
	}
	
} // namespace PatricksDrivers
