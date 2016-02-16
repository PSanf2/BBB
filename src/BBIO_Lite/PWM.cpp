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
#include <cstring>	// pulls in strcmp() and strstr()
#include <string>

using namespace std;

namespace BBIO {
	
	bool PWM::_initialized = false;
	
	PWM::PWM(const char* key) {
		printf("\nBBIO::PWM Called!");
		printf("\n_initialized = %i", _initialized);
		printf("\nkey = %s", key);
		
		_info.name = NULL;
		_info.key = NULL;
		_info.ocp_path = NULL;
		
		int idx = -1;
		int sz = ARRAY_SIZE(PWM_Info);
		for (int i = 0; i < sz; i++)
			if (strcmp(key, PWM_Info[i].key) == 0) {
				idx = i;
				break;
			}
		// if not found, then we're not working w/ a PWM pin I support
		if (idx == -1)
			return;
		// populate _info
		_info = PWM_Info[idx];
		
		printf("\n_info.name = %s", _info.name);
		printf("\n_info.key = %s", _info.key);
		
		// if !_initialized
		if (!_initialized) {
			// load the am33xx_pwm DTO
			load_device_tree("am33xx_pwm");
			// toggle _initialized
			_initialized = !_initialized;
		}
		
		// load the bone_pwm_<key> DTO fragment
		string dto = "bone_pwm_";
		dto.append(_info.key);
		printf("\ndto = %s", dto.c_str());
		load_device_tree(dto.c_str());
		printf("\nloaded!");
		
		// determine the ocp path for the PWM pin
		
		char* ocp = new char[50];
		build_path("/sys/devices", "ocp", ocp);
		printf("\nocp = %s", ocp);
		
		string pwm_test = "pwm_test_";
		pwm_test.append(_info.key);
		printf("\npwm_test = %s", pwm_test.c_str());
		
		char* ocp2 = new char[50];
		
		build_path(ocp, pwm_test.c_str(), ocp2);
		printf("\nocp2 = %s", ocp2);
		delete ocp;
		_info.ocp_path = ocp2;
		printf("\n_info.ocp_path = %s", _info.ocp_path);
		
		// set the default values for the PWM pin
		
		printf("\nBBIO::PWM Done!");
	}
	
	PWM::~PWM() {
		// unload my DTO fragment
		printf("\nBBIO::~PWM Called!");
		delete _info.ocp_path;
	}
	
} // namespace BBIO
