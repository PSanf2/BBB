/*
 * Mod By:	Patrick Sanford
 * Version:	0.0
 * Date:	Feb 12, 2016
 * GitHub:	https://github.com/PSanf2/BBB
 * 			https://github.com/adafruit
 * Desc:	This is my modification of the Adafruit Beaglebone IO Python
 * 			library.
 */

#include "GPIO.h"
#include <cstdio>	// pulls in printf() for debugging
#include <cstring>	// pulls in strcmp for const char* comparisons, other string related functions.
#include <string>	// string class/type
#include <fstream>	// ofstream class/type
#include <sstream>	// stringstream class/type

using namespace std;

namespace BBIO {
	
	GPIO::GPIO(const char* key) {
		// setup
		printf("\nBBIO::GPIO called!");
		printf("\nkey = %s", key);
		
		_info.name = "";
		_info.key = "";
		_info.gpio = -1;
		
		// search GPIO_Info to find the matching key
		int idx = -1;
		int sz = ARRAY_SIZE(GPIO_Info);
				
		for (int i = 0; i < sz; i++)
			if (strcmp(key, GPIO_Info[i].key) == 0) {
				idx = i;
				break;
			}
		
		// if not found, then we're not working w/ a GPIO pin I support
		if (idx == -1)
			return;
		
		// populate _info
		_info = GPIO_Info[idx];
		
		printf("\n_info.name = %s", _info.name);
		printf("\n_info.key = %s", _info.key);
		printf("\n_info.gpio = %i", _info.gpio);
		
		// export the pin (exporting the same pin multiple times doesn't hurt anything).
		// convert _info.gpio into a string
		stringstream s;
		s << _info.gpio;
		string val = s.str();
		
		// convert the file name into a usable string
		string filename = "export";
		
		// open the file
		ofstream fs;
		fs.open((GPIO_PATH + filename).c_str());
		
		// if the file's open
		if (fs.is_open()) {
			// write to the file
			fs << val;
			// close the file
			fs.close();
		}
	}
	
	GPIO::~GPIO() {
		// cleanup
		printf("\nBBIO::~GPIO called!");
		
		// unexport the pin (unexporting the same pin multiple times doesn't hurt anything).
		// convert _info.gpio into a string
		stringstream s;
		s << _info.gpio;
		string val = s.str();
		
		// convert the file name into a usable string
		string filename = "unexport";
		
		// open the file
		ofstream fs;
		fs.open((GPIO_PATH + filename).c_str());
		
		// if the file's open
		if (fs.is_open()) {
			// write to the file
			fs << val;
			// close the file
			fs.close();
		}
	}
	
	void GPIO::direction(GPIO_DIRECTION dir) {
		
	}
	
	void GPIO::value(GPIO_VALUE val) {
		
	}
	
	GPIO_DIRECTION GPIO::direction() {
		return INPUT;
	}
	
	GPIO_VALUE GPIO::value() {
		return LOW;
	}
	
	const char* GPIO::name() {
		return _info.name;
	}
	
	const char* GPIO::key() {
		return _info.key;
	}
	
	int GPIO::pin() {
		return _info.gpio;
	}
	
} // namespace BBIO
