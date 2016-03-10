#ifndef __LCD_H
#define __LCD_H

#include "../BBIO_Lite/PWM.h"
#include "../BBIO_Lite/GPIO.h"

#include <string>

namespace PatricksDrivers {
	
	/* I could support all eight data pins, but screw it.
	// It would be extra code that wouldn't be used. If I'm hooking up
	// an LCD then I'm not going to use more wires than I need.
	*/
	
	/*
	 * Don't waste time writing a bunch of child classes just for the backlight controls.
	 * That's dumb. Let the programmer worry about it. The backlight controls on an LCD
	 * are just like an LED, and the code to control them can be the same.
	 */
	
	class LCD {
		private:
			BBIO::GPIO* _rs;
			BBIO::GPIO* _en;
			BBIO::GPIO* _data[4];
			void send(unsigned char data_byte, unsigned char rs_val);
			void set4bits(unsigned char data_bits);
			void command(unsigned char data_bits) { send(data_bits, 0); }
			void write(unsigned char data_bits) { send(data_bits, 1); }
			void pulse_en();
		public:
			LCD(
				const char* rs,
				const char* en,
				const char* data4,
				const char* data5,
				const char* data6,
				const char* data7,
				unsigned int rows = 2,
				unsigned int cols = 16
			);
			~LCD();
			void print(const char* val);
			void clear();
			void home();
			void curPos(unsigned int col, unsigned int row);
			void dispOn();
			void dispOff();
			void curOn();
			void curOff();
			void curBlinkOn();
			void curBlinkOff();
			void scrollDisplayLeft();
			void scrollDisplayRight();
			void leftToRight();
			void rightToLeft();
			void autoScrollOn();
			void autoScrollOff();
	}; // class LCD
	
} // namespace

#endif
