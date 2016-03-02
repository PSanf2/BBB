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
#include <sys/epoll.h> // pulls in epoll stuff
#include <fcntl.h>	// pulls in open(), O_RDONLY, and O_NONBLOCK
#include <pthread.h>// pulls in stuff needed for multithreading.

using namespace std;

namespace BBIO {
	
	GPIO::GPIO(const char* key) {
		// setup
		_info.name = NULL;
		_info.key = NULL;
		_info.gpio = 0;
		_info.debounce = 0;
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
		write(GPIO_PATH, filename, val);
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
		write(GPIO_PATH, filename, val);
	}
	
	int GPIO::direction(GPIO_DIRECTION dir) {
		// convert a number to a string
		stringstream dir_num;
		dir_num << _info.gpio;
		string path = GPIO_PATH;
		path = path + "gpio" + dir_num.str() + "/";
		string filename = "direction";
		string value;
		if (dir == INPUT)
			value = "in";
		else
			value = "out";
		return write(path, filename, value);
	}
	
	int GPIO::value(GPIO_VALUE val) {
		// convert a number to a string
		stringstream dir_num;
		dir_num << _info.gpio;
		string path = GPIO_PATH;
		path = path + "gpio" + dir_num.str() + "/";
		string filename = "value";
		string value;
		if (val == LOW)
			value = "0";
		else
			value = "1";
		return write(path, filename, value);
	}
	
	// THIS NEEDS TO BE TESTED
	GPIO_DIRECTION GPIO::direction() {
		stringstream dir_num;
		dir_num << _info.gpio;
		string path = GPIO_PATH;
		path = path + "gpio" + dir_num.str() + "/";
		string filename = "direction";
		
		string value = read(path, filename);
		
		printf("\nvalue = %s", value.c_str());
		
		if (value == "in")
			return INPUT;
		else if (value == "out")
			return OUTPUT;
		else
			return DIR_ERR;
		
		/*
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
		*/
	}
	
	// THIS NEEDS TO BE TESTED
	GPIO_VALUE GPIO::value() {
		stringstream dir_num;
		dir_num << _info.gpio;
		string path = GPIO_PATH;
		path = path + "gpio" + dir_num.str() + "/";
		string filename = "value";
		
		string value = read(path, filename);
		
		printf("\nvalue = %s", value.c_str());
		
		if (value == "1")
			return HIGH;
		else if (value == "0")
			return LOW;
		else
			return VAL_ERR;
		
		/*
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
		*/
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
	
	int GPIO::edge(GPIO_EDGE val) {
		// convert a number to a string
		stringstream dir_num;
		dir_num << _info.gpio;
		string path = GPIO_PATH;
		path = path + "gpio" + dir_num.str() + "/";
		string filename = "edge";
		string value;
		if (val == RISING)
			value = "rising";
		else if (val == FALLING)
			value = "falling";
		else if (val == BOTH)
			value = "both";
		else // if value == NONE
			value = "none";
		return write(path, filename, value);
	}
	
	GPIO_EDGE GPIO::edge() {
		stringstream dir_num;
		dir_num << _info.gpio;
		string path = GPIO_PATH;
		path = path + "gpio" + dir_num.str() + "/";
		string filename = "edge";
		
		string value = read(path, filename);
		
		printf("\nvalue = %s", value.c_str());
		
		if (value == "none")
			return NONE;
		else if (value == "rising")
			return RISING;
		else if (value == "falling")
			return FALLING;
		else if (value == "both")
			return BOTH;
		else
			return EDG_ERR;
	}
	
	int GPIO::waitForEdge() {
		direction(INPUT);
		int fd, i, epollfd, count = 0;
		struct epoll_event ev;
		stringstream dir_num;
		dir_num << _info.gpio;
		string path = GPIO_PATH;
		path = path + "gpio" + dir_num.str() + "/";
		string filename = "value";
		epollfd = epoll_create(1);
		if (epollfd == -1)
			return -1;
		if ((fd = open((path + filename).c_str(), O_RDONLY | O_NONBLOCK)) == -1)
			return -1;
		ev.events = EPOLLIN | EPOLLET | EPOLLPRI;
		ev.data.fd = fd;
		if (epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev) == -1)
			return -1;
		while (count <= 1) {
			i = epoll_wait(epollfd, &ev, 1, -1);
			if (i == -1)
				count = 5;
			else
				count++;
		}
		close(fd);
		if (count == 5)
			return -1;
		return 0;
	}
	
	// This is a friend function of the class
	void* threadedPoll(void* value) {
		GPIO* gpio = static_cast<GPIO*>(value);
		while (gpio->threadRunning) {
			gpio->callbackFunction(gpio->waitForEdge());
			usleep(gpio->_info.debounce * 1000);
		}
		return 0;
	}
	
	// works!
	int GPIO::waitForEdge(CallbackType callback) {
		threadRunning = true;
		callbackFunction = callback;
		if (pthread_create(&thread, NULL, &threadedPoll, static_cast<void*>(this))) {
			threadRunning = false;
			return -1;
		}
		return 0;
	}
	
	void GPIO::waitForEdgeCancel() {
		threadRunning = false;
	}
	
	void GPIO::debounce(int time) {
		_info.debounce = time;
	}
	
} // namespace BBIO
