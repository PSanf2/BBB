/*
* Mod By:	Patrick Sanford
* Version:	0.0
* Date:	Feb 12, 2016
* GitHub:	https://github.com/PSanf2/BBB
* 			https://github.com/adafruit
* Desc:	This is my modification of the Adafruit Beaglebone IO Python
* 			library.
*/

#include "ADC.h"

#include <cstdio>	// pulls in printf() for debugging
#include <cstring>	// pulls in strcmp() and strstr()
#include <sstream>	// stringstream class/type

using namespace std;

namespace BBIO {
 
	bool ADC::_initialized = false;
	 
	ADC::ADC(const char* key) {
		// this will need to be done like the PWM constructor.
		_info.name = NULL;
		_info.key = NULL;
		_info.ocp_path = NULL;
		// search for my info
		int idx = -1;
		int sz = ARRAY_SIZE(ADC_Info);
		for (int i = 0; i < sz; i++)
			if (strcmp(key, ADC_Info[i].key) == 0) {
				idx = i;
				break;
			}
		// if not found, then we're not working w/ a pin I support
		if (idx == -1)
			return;
		// populate _info
		_info = ADC_Info[idx];
		if (!_initialized) {
			// load the am33xx_pwm DTO
			load_device_tree("cape-bone-iio");
			// toggle _initialized
			_initialized = !_initialized;
		}
		// determine the ocp path for the PWM pin
		// target is /sys/devices/ocp.*/helper.*
		char* ocp = new char[50];
		build_path("/sys/devices", "ocp", ocp);
		char* ocp2 = new char[50];
		build_path(ocp, "helper", ocp2);
		delete ocp;
		_info.ocp_path = ocp2;
	}
	
	int ADC::value() {
		stringstream path_stream;
		path_stream << _info.ocp_path;
		string path = path_stream.str();
		path = path + "/";
		stringstream file_stream;
		file_stream << _info.name;
		string file = file_stream.str();
		string value = read(path, file);
		stringstream value_stream(value);
		int result;
		value_stream >> result;
		return result;
	}
	
	ADC::~ADC() {
		delete _info.ocp_path;
	}
	 
} // namespace
