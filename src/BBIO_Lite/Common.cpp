/*
 * Mod By:	Patrick Sanford
 * Version:	0.0
 * Date:	Feb 12, 2016
 * GitHub:	https://github.com/PSanf2/BBB
 * 			https://github.com/adafruit
 * Desc:	This is my modification of the Adafruit Beaglebone IO Python
 * 			library.
 */

#include "Common.h"

#include <cstdio>	// pulls in printf() for debugging
#include <cstring>	// pulls in strcmp() and strstr()
#include <string>	// pulls in string type
#include <dirent.h>	// C POSIX Library object. Lets me go over directory contents.
#include <fstream>	// ofstream class/type
#include <sstream>	// stringstream class/type
#include <time.h>	// pulls in nanosleep()

using namespace std;

namespace BBIO {
	
	int build_path(
		const char* partial_path,
		const char* prefix,
		char* full_path
	) {
		// setup vars
		int result = 0;
		DIR *dp;
		struct dirent *ep;
		// open directory
		dp = opendir(partial_path);
		// if it opened...
		if (dp != NULL) {
			// while we can read lines of output
			while (ep = readdir(dp)) {
				// see if the string i'm searching for is contained in what i've read
				char* found_string = strstr(ep->d_name, prefix);
				// if it is
				if ((found_string != NULL) && (strcmp(ep->d_name, found_string) == 0)) {
					// build up the full path
					full_path = strcpy(full_path, "");
					full_path = strcat(full_path, partial_path);
					full_path = strcat(full_path, "/");
					full_path = strcat(full_path, ep->d_name);
					// report a search hit as the result
					result = 1;
					// get out of the while loop
					break;
				} // if
			} // while
			// close the directory
			closedir(dp);
		} // if (dp != NULL)
		return result;
	} // build_path
	
	int load_device_tree(const char *name) {
		char* slots = new char[35];
		int result = 0;
		// get the full path to slots
		build_path("/sys/devices", "bone_capemgr", slots);
		slots = strcat(slots, "/slots");
		// make sure it isn't already loaded
		ifstream inf;
		inf.open(slots);
		if (inf.is_open()) {
			string line;
			while (getline(inf, line)) {
				if (strstr(line.c_str(), name) != 0) {
					inf.close();
					delete slots;
					result = 1;
					return result;
				}
			} // while
			inf.close();
		} // if
		// open the file
		ofstream fs;
		fs.open(slots);
		if (fs.is_open()) {
			// write to it
			fs << name;
			// close it
			fs.close();
			//0.2 second delay
			nanosleep((struct timespec[]){{0, 200000000}}, NULL);
			result = 1;
		}
		// clean up
		delete slots;
		return result;
	}
	
	int unload_device_tree(const char *name) {
		char* slots = new char[35];
		int result = 0;
		// get the full path to slots
		build_path("/sys/devices", "bone_capemgr", slots);
		slots = strcat(slots, "/slots");
		// see if it is loaded
		bool found = false;
		string line;
		ifstream inf;
		inf.open(slots);
		
		if (inf.is_open()) {
			while (getline(inf, line)) {
				if (strstr(line.c_str(), name) != 0) {
					found = true;
					break;
				}
			} // while
			inf.close();
		} // if
		
		if (found) {
			// i need to do some string manipulations
			// get rid of everything after the first :
			line = line.substr(0, line.find_first_of(":"));
			// get rid of any leading spaces
			if (line.substr(0, 1) == " ") {
				line = line.substr(line.find_last_of(" ")+1);
			}
			// add a hyphen to the front.
			line = "-" + line;
			// open the file
			ofstream fs;
			fs.open(slots);
			if (fs.is_open()) {
				// write to it
				fs << line.c_str();
				// close it
				fs.close();
				//0.2 second delay
				nanosleep((struct timespec[]){{0, 200000000}}, NULL);
				result = 1;
			}
		} else {
			// if it's not loaded there's nothing to do, and i can report success
			result = 1;
		}
		delete slots;
		return result;
	}
	
} // namespace BBIO
