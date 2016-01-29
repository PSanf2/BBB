#include "GPIO_Singleton.h"

#include <cstdio>	// pulls in printf()
#include <string>	// string class/type
#include <cstring>	// pulls in strcmp for const char* comparisons, other string related functions.
#include <fstream>	// ofstream class/type
#include <sstream>	// stringstream class/type

using namespace std;

namespace PatricksDrivers {
	
	// GPIO_Pin methods
	GPIO_Singleton::GPIO_Pin::GPIO_Pin(gpio_pin_t info) {
		_info = info;
	}
	
	GPIO_Singleton::GPIO_Pin::~GPIO_Pin() {
		
	}
	
	int GPIO_Singleton::GPIO_Pin::setDirection(GPIO_DIRECTION dir) {
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
			printf("\nExport failed: Unable to open file.");
			return -1;
		}
		
		// write the file
		fs << value;
		
		// close the file
		fs.close();
		
		return 0;
	}
	
	int GPIO_Singleton::GPIO_Pin::setValue(GPIO_VALUE val) {
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
			printf("\nExport failed: Unable to open file.");
			return -1;
		}
		
		// write the file
		fs << value;
		
		// close the file
		fs.close();
		
		return 0;
	}
	
	GPIO_DIRECTION GPIO_Singleton::GPIO_Pin::getDirection() {
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
		if (!fs.is_open()) {
			printf("\ngetDirection failed: Failed to open file");
		}
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
	
	GPIO_VALUE GPIO_Singleton::GPIO_Pin::getValue() {
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
		if (!fs.is_open()) {
			printf("\ngetValue failed: Failed to open file");
		}
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
	
	const char* GPIO_Singleton::GPIO_Pin::getName() {
		return _info.name;
	}
	
	const char* GPIO_Singleton::GPIO_Pin::getKey() {
		return _info.key;
	}
	
	int GPIO_Singleton::GPIO_Pin::getGpio() {
		return _info.gpio;
	}
	
	// GPIO_Singleton methods
	static GPIO_Singleton* GPIO_Singleton_instance;
	
	GPIO_Singleton* GPIO_Singleton::getInstance() {
		if (!GPIO_Singleton_instance)
			GPIO_Singleton_instance = new GPIO_Singleton();
		return GPIO_Singleton_instance;
	}
	
	GPIO_Singleton::GPIO_Singleton() {
		
	}
	
	GPIO_Singleton::~GPIO_Singleton() {
		
	}
	
	int GPIO_Singleton::exportGPIO(int gpio) {
		// figure out the path and file name
		string path = GPIO_PATH;
		string filename = "export";
		
		// convert the int to a string
		stringstream s;
		s << gpio;
		string value = s.str();
		
		// open the file
		ofstream fs;
		fs.open((path + filename).c_str());
		if (!fs.is_open()) {
			printf("\nExport failed: Unable to open file.");
			return -1;
		}
		
		// write the file
		fs << value;
		
		// close the file
		fs.close();
		
		return 0;
	}
	
	int GPIO_Singleton::unexportGPIO(int gpio) {
		// figure out the path and file name
		string path = GPIO_PATH;
		string filename = "unexport";
		
		// convert the int to a string
		stringstream s;
		s << gpio;
		string value = s.str();
		
		// open the file
		ofstream fs;
		fs.open((path + filename).c_str());
		if (!fs.is_open()) {
			printf("\nUnexport failed: Unable to open file.");
			return -1;
		}
		
		// write the file
		fs << value;
		
		// close the file
		fs.close();
		
		return 0;
	}
	
	GPIO_Singleton::GPIO_Pin* GPIO_Singleton::setup(const char* key) {
		// find the element on pinInfo that matches key
		int idx = -1;
		int sz = ARRAY_SIZE(pinInfo);
				
		for (int i = 0; i < sz; i++)
			if (strcmp(key, pinInfo[i].key) == 0) {
				idx = i;
				break;
			}
		
		// if not found, then we're not working w/ a GPIO pin I support
		if (idx == -1)
			return 0;
		
		// make sure there isn't already a GPIO_Pin object on the vector
			// if there is, just return that
		if (_pin.size() > 0) {
			for (int i = 0; i < _pin.size(); i++) {
				if (_pin[i].getGpio() == pinInfo[i].gpio) {
					return &_pin[i];
				}
			}
		}
		
		// create a new GPIO_Pin object
		GPIO_Pin* newObj = new GPIO_Pin(pinInfo[idx]);
		
		// export the pin
		exportGPIO(newObj->getGpio());
		
		// put it on the vector
		_pin.push_back(*newObj);

		// return a pointer to the GPIO_Pin object
		return newObj;
		
	}
	
	void GPIO_Singleton::cleanup(const char* key) {
		printf("\nGPIO_Singleton::cleanup called!");
		
		// find the element on pinInfo that matches key
		int idx = -1;
		int sz = ARRAY_SIZE(pinInfo);
				
		for (int i = 0; i < sz; i++)
			if (strcmp(key, pinInfo[i].key) == 0) {
				idx = i;
				break;
			}
		
		// if one was found, unexport it
		if (idx != -1)
			unexportGPIO(pinInfo[idx].gpio);
		
		// look for an object on the vector matching the pin
		if (_pin.size() > 0) {
			for (int i = 0; i < _pin.size(); i++) {
				if (_pin[i].getGpio() == pinInfo[i].gpio) {
					// destroy the object
					// remove the element from the vector
					delete &_pin[i];
					_pin.erase(_pin.begin() + i);
				}
			}
		}
		
	}
} // namespace PatricksDrivers
