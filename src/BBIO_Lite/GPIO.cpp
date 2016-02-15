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
#include <cstring>	// pulls in strcmp for const char* comparisons, other string related functions.
#include <string>	// string class/type
#include <fstream>	// ofstream class/type
#include <sstream>	// stringstream class/type

using namespace std;

namespace BBIO {
	
	GPIO::GPIO(const char* key) {
		// setup
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
		
		if (fs.is_open()) {
			// write to the file
			fs << val;
			// close the file
			fs.close();
		}
	}
	
	GPIO::~GPIO() {
		// cleanup
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
	
	int GPIO::direction(GPIO_DIRECTION dir) {
		string path = GPIO_PATH;
		stringstream s;
		s << _info.gpio;
		string path2 = "gpio" + s.str() + "/";
		string filename = "direction";
		string full_path = path + path2 + filename;
		string value;
		if (dir == INPUT)
			value = "in";
		else
			value = "out";
		// open the file
		ofstream fs;
		fs.open(full_path.c_str());
		if (!fs.is_open()) {
			return -1;
		}
		// write the file
		fs << value;
		// close the file
		fs.close();
		return 0;
	}
	
	int GPIO::value(GPIO_VALUE val) {
		string path = GPIO_PATH;
		stringstream s;
		s << _info.gpio;
		string path2 = "gpio" + s.str() + "/";
		string filename = "value";
		string full_path = path + path2 + filename;
		string value;
		if (val == LOW)
			value = "0";
		else
			value = "1";
		// open the file
		ofstream fs;
		fs.open(full_path.c_str());
		if (!fs.is_open()) {
			return -1;
		}
		// write the file
		fs << value;
		// close the file
		fs.close();
		return 0;
	}
	
	GPIO_DIRECTION GPIO::direction() {
		// build full path to file
		string path = GPIO_PATH;
		stringstream s;
		s << _info.gpio;
		string path2 = "gpio" + s.str() + "/";
		string filename = "direction";
		string full_path = path + path2 + filename;
		// open the file
		ifstream fs;
		fs.open(full_path.c_str());
		if (fs.is_open()) {
			string val;
			// read file
			getline(fs, val);
			// close file
			fs.close();
			
			// evaluate and return
			if (val == "in")
				return INPUT;
			else
				return OUTPUT;
		}
		return DIR_ERR;
	}
	
	GPIO_VALUE GPIO::value() {
		// build full path to file
		string path = GPIO_PATH;
		stringstream s;
		s << _info.gpio;
		string path2 = "gpio" + s.str() + "/";
		string filename = "value";
		string full_path = path + path2 + filename;
		// open the file
		ifstream fs;
		fs.open(full_path.c_str());
		if (fs.is_open()) {
			string val;
			// read file
			getline(fs, val);
			// close file
			fs.close();
			// evaluate and return
			if (val == "1")
				return HIGH;
			else
				return LOW;
		}
		return VAL_ERR;
	}
	
	const char* GPIO::name() {
		return _info.name;
	}
	
	const char* GPIO::key() {
		return _info.key;
	}
	
	int GPIO::gpio() {
		return _info.gpio;
	}
	
} // namespace BBIO
