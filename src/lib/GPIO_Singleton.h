/*
 *
 */

#ifndef __GPIO_SINGLETON_H
#define __GPIO_SINGLETON_H

namespace PatricksDrivers {
	
	// I'll be using polymorphism to manage my pins, so I'll need a class.
	class GPIO_Pin {
		public:
			GPIO_Pin(unsigned int pinNum);
			~GPIO_Pin();
	}; // class GPIO_Pin
	
	class GPIO_Singleton {
		public:
			static GPIO_Singleton* getInstance();
		private:
			// variables
			GPIO_Pin* pins[];	// a place to hold all of my GPIO_Pin objets
			// functions
			GPIO_Singleton();
			GPIO_Singleton(const GPIO_Singleton&);
			GPIO_Singleton& operator= (const GPIO_Singleton&);
			~GPIO_Singleton();
	}; // class GPIO_Singleton
	
} // namespace PatricksDrivers

#endif
