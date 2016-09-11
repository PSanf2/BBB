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
	
	// This should be accepting a parameter for the period.
	// The period isn't supposed to be changed at runtime.
	// It's supposed to be set before you start the PWM generator running, and then left alone.
	PWM::PWM(const char* key) {
		_info.name = NULL;
		_info.key = NULL;
		_info.ocp_path = NULL;
		// search for my info
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
		duty(0);
		period(500000);
	}
	
	PWM::PWM(const char* key, const int per) {
		_info.name = NULL;
		_info.key = NULL;
		_info.ocp_path = NULL;
		// search for my info
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
		duty(0);
		//period(20000000); // 2 ms. Good for servos.
		period(per);
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
	
	// The period on a PWM is not supposed to be altered during run time.
	// It's suppsed to be set once, and then left alone.
	// Create the object, set the period, set the duty to 0, start it, change the duty as needed.
	// If you're using two PWM pins that connect to the same controller on the board
	// then you won't be able to set the period on either pin w/o unloading the DTO fragment
	// for the blocking pin.
	// It would be possible to change some logic to make this possible, but it would also mean
	// cutting a PWM signal on two pins to change the period on one of them.
	// Because of this, it's best to just let the period default to 500000.
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
	
	// If polarity is 1 then higher duty values give a longer duty cycle.
	// If polarity is 0 then higher duty values give a shorter duty cycle.
	void PWM::polarity(int val) {
		stringstream s;
		s << _info.ocp_path;
		string path = s.str() + "/";
		string filename = "polarity";
		write(path, filename, val);
	}
	
} // namespace BBIO
