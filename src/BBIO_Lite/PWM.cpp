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
#include <string>	// pulls in string class/type
#include <fstream>	// ofstream class/type
#include <sstream>	// pulls in ostringstream class/type

using namespace std;

namespace BBIO {
	
	bool PWM::_initialized = false;
	
	PWM::PWM(const char* key) {
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
		load_device_tree(dto.c_str());
		// determine the ocp path for the PWM pin
		char* ocp = new char[50];
		build_path("/sys/devices", "ocp", ocp);
		string pwm_test = "pwm_test_";
		pwm_test.append(_info.key);
		char* ocp2 = new char[50];
		build_path(ocp, pwm_test.c_str(), ocp2);
		delete ocp;
		_info.ocp_path = ocp2;
		// set the default values for the PWM pin
		//printf("\nBBIO::PWM says SET THE DEFAULT PWM VALUES!");
		duty(0);
		period(500000);
		start();
	}
	
	PWM::~PWM() {
		// make sure the constructor loaded valid values
		if (_info.key != NULL) {
			// figure out the name of the DTO fragment to unload
			string bone_pwm = "bone_pwm_";
			bone_pwm.append(_info.key);
			// unload the DTO fragment
			unload_device_tree(bone_pwm.c_str());
		}
		// cleanup
		delete _info.ocp_path;
	}
	
	void PWM::start() {
		// my path should be _info.ocp_path + "/" + run
		// i need to write "1" to the file
		stringstream s;
		s << _info.ocp_path;
		string path = s.str() + "/";
		string filename = "run";
		string value = "1";
		write(path, filename, value);
	}
	
	void PWM::stop() {
		// my path should be _info.ocp_path + run
		// i need to write "0" to the file
		stringstream s;
		s << _info.ocp_path;
		string path = s.str() + "/";
		string filename = "run";
		string value = "0";
		write(path, filename, value);
	}
	
	void PWM::period(int val) {
		stringstream s;
		s << _info.ocp_path;
		string path = s.str() + "/";
		string filename = "period";
		write(path, filename, val);
	}
	
	void PWM::duty(int val) {
		stringstream s;
		s << _info.ocp_path;
		string path = s.str() + "/";
		string filename = "duty";
		write(path, filename, val);
	}
	
} // namespace BBIO
