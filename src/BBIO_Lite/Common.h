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

namespace BBIO {
	
	// This is where all of the common stuff will go.
	// I'll be using the C++ standard IO libraries.
	// I'll be making sparing use of C POSIX libraries where required.
	// Anything that deals with deploying/undeploying a DTO goes in the common file.
	// Anything that deals with reading/writing to files goes in the common file.
	
	#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]));
	
	int _export();
	int _unexport();
	
} // namespace BBIO

#endif
