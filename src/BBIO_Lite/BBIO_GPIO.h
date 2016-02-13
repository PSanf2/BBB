/*
 * Mod By:	Patrick Sanford
 * Version:	0.0
 * Date:	Feb 12, 2016
 * GitHub:	https://github.com/PSanf2/BBB
 * 			https://github.com/adafruit
 * Desc:	This is my modification of the Adafruit Beaglebone IO Python
 * 			library.
 */

#ifndef __BBIO_GPIO_H
#define __BBIO_GPIO_H

namespace BBIO {
	
	// Most of the GPIO stuff in the Adafruit library is using Python, and no 
	// This is because using the GPIO's doesn't require deploying a device tree overlay. 
	class GPIO {
		public:
			GPIO();
			// setup()
			// cleanup()
			// set_output()
			// get_input()
			// I also need to have functions to take care of events
			// add event detect()
			// remove event detect()
			// add event callback()
			// wait for edge()
			~GPIO();
		private:
			// export()
			// unexport()
	}; // class GPIO
	
} // namespace BBIO

#endif
