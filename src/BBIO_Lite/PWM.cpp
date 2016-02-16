/*
 * Mod By:	Patrick Sanford
 * Version:	0.0
 * Date:	Feb 12, 2016
 * GitHub:	https://github.com/PSanf2/BBB
 * 			https://github.com/adafruit
 * Desc:	This is my modification of the Adafruit Beaglebone IO Python
 * 			library.
 */

#include "PWM.h"

#include <cstdio>	// pulls in printf() for debugging

using namespace std;

namespace BBIO {
	
	bool PWM::_initialized = false;
	
	PWM::PWM(const char* key) {
		// if !_initialized
			// load the am33xx_pwm DTO
			// toggle _initialized
		// load the bone_pwm_<key> DTO fragment
		// determine the ocp path for the PWM pin
		// set the default values for the PWM pin
		printf("\nBBIO::PWM Called!");
		printf("\n_initialized = %i", _initialized);
		printf("\nkey = %s", key);
		
		if (!_initialized) {
			load_device_tree("am33xx_pwm");
			_initialized = !_initialized;
		}
		
		printf("\nBBIO::PWM Done!");
	}
	
	PWM::~PWM() {
		// unload my DTO fragment
		printf("\nBBIO::~PWM Called!");
	}
	
} // namespace BBIO
