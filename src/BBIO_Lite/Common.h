/*
 * Mod By:	Patrick Sanford
 * Version:	0.0
 * Date:	Feb 12, 2016
 * GitHub:	https://github.com/PSanf2/BBB
 * 			https://github.com/adafruit
 * Desc:	This is my modification of the Adafruit Beaglebone IO Python
 * 			library.
 */

#ifndef __BBIO_H
#define __BBIO_H

#include <string>

using std::string;

namespace BBIO {
	
	// This is where all of the common stuff will go.
	// I'll be using the C++ standard IO libraries.
	// I'll be making sparing use of C POSIX libraries where required.
	// Anything that deals with deploying/undeploying a DTO goes in the common file.
	// Anything that deals with reading/writing to files goes in the common file.
	
	#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]));
	
	// this does not put a forward slash on the end of the returned value.
	// if have partial path /some/dir that contains a file named abc123
	// then a prefix value of abc will result in a full path of /some/dir/abc123
	// if /some/dir contains /some/dir/subdir then a prefix value of sub will get
	// a full back of /some/dir/subdir
	int build_path(
		const char* partial_path,
		const char* prefix,
		char* full_path
	);
	
	int load_device_tree(const char* name);
	int unload_device_tree(const char* name);
	
	int write(string path, string filename, string value);
	int write(string path, string filename, int value);
	string read(string path, string filename);
	
} // namespace BBIO

#endif
