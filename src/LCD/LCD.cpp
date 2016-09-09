/*
 *				PIN CHART
 *LCD Pin	|		Name				|	BBB Pin			|
 *----------+---------------------------+-------------------+
 *	1		|	VSS (Ground)			|	P8_02 DGND		|
 *	2		|	VDD (+ve)				|	P9_07 VDD_5V	|
 *	3		|	VE (Contrast Voltage)	|	POT CTR PIN		|
 *	4		|	Register Select (RS)	|	P8_08 GPIO_67	|
 *	5		|	Read/Write (r/w)		|	P8_02 DGND		|
 *	6		|	Enable (EN)				|	P8_10 GPIO_68	|
 *	7		|	Data 0					|	-----			| *Pins for the low nibble aren't needed.
 *	8		|	Data 1					|	-----			|
 *	9		|	Data 2					|	-----			|
 *	10		|	Data 3					|	-----			|
 *	11		|	Data 4					|	P8_18 GPIO_65	|
 *	12		|	Data 5					|	P8_16 GPIO_46	|
 *	13		|	Data 6					|	P8_14 GPIO_26	|
 *	14		|	Data 7					|	P8_12 GPIO_44	|
 *	15		|	Backlight Anode (+ve)	|	P9_07 VDD_5V	|
 *	16		|	Red Cathode (-R/red)	|	P9_16 EHRPWM1A	| *Only cathode on monochrome LCD.
 *	17		|	Green Cathode (-G/green)|	P9_14 EHRPWM1B	| *Only on RGB backlit LCD.
 *	18		|	Blue Cathode (-B/blue)	|	P8_13 EHRPWM2B	| *Only on RGB backlit LCD.
 * 
 * NOTE: The potentiometer needs to have one outter pin connected to P9_07 VDD_5V, and the
 * other connected to P8_02 DGND. The potentiometer controls the contrast for the LCD characters.
 * The RGB cathodes are controlled with PWM, and govern the color and brightness of the backlight.
 * The BBB does have enough GPIO pins available to accomidate LCD pins 7-10, but this would just
 * allow me to halve the number of write operations required to control the device. This is something
 * I could do in order to allow me to code for it, but isn't required with compatable LCD modules.
 * I'm coding for HD44780 compatible LCD modules. I have three I can play with, and an I2C breakout
 * board as well. I have one monochrome 16x2 LCD, one 16x2 LCD with tri-color backlight, and one
 * monochrome 20x4 LCD. I'm wanting to write a driver that will support all three devices. All three
 * devices support the same protocol. Everybody else (Adafruit, who else?) has written their code to
 * support an 8 bit, and 4 bit parralel bus. The I2C backback does use all the LCD pins, but that's a
 * totally different story. Keep in mind that the PWM pins are not the same as GPIO pins. They can
 * probably be used for digital output if needed (full on/full off), but if you want to code for
 * a GPIO controlled LED backlight then you need to use a GPIO pin (Unless you want to get crazy).
 */

#include "LCD.h"

#include <cstdio>		// pulls in printf()
#include <cstring>

using namespace std;

namespace PatricksDrivers {
	
	/* Class LCD */
	
	LCD::LCD(
		const char* rs,
		const char* en,
		const char* data4,
		const char* data5,
		const char* data6,
		const char* data7,
		unsigned int rows,
		unsigned int cols
	) {
		
		_rows = rows;
		_cols = cols;
		
		_rs = new BBIO::GPIO(rs);
		_en = new BBIO::GPIO(en);
		_data[0] = new BBIO::GPIO(data4);
		_data[1] = new BBIO::GPIO(data5);
		_data[2] = new BBIO::GPIO(data6);
		_data[3] = new BBIO::GPIO(data7);
		
		_rs->direction(BBIO::OUTPUT);
		_en->direction(BBIO::OUTPUT);
		_data[0]->direction(BBIO::OUTPUT);
		_data[1]->direction(BBIO::OUTPUT);
		_data[2]->direction(BBIO::OUTPUT);
		_data[3]->direction(BBIO::OUTPUT);
		
		_rs->value(BBIO::LOW);
		_en->value(BBIO::HIGH); // en is to be pulled high
		_data[0]->value(BBIO::LOW);
		_data[1]->value(BBIO::LOW);
		_data[2]->value(BBIO::LOW);
		_data[3]->value(BBIO::LOW);
		
		/*
		 * I now need to bit bang the LCD a series of commands to initalize it.
		 * Once that's done I'll be able to write to it, and send it other commands.
		 * 
		 * Nibble sequence
		 * rs	7654	0xN
		 * 0	0010	0x20	// 1 nibble operation
		 * 0	0010	0x20	// function set
		 * 0	1000	0x80
		 * 0	0000	0x00	// display on/off
		 * 0	1111	0xF0
		 * 0	0000	0x00	// entry mode
		 * 0	0110	0x60
		 * Byte sequence
		 * rs	7654 3210	0xN
		 * 00	0010 ----	0x2-	// 1 nibble operation
		 * 0	0010 1000	0x28	// function set
		 * 0	0000 1111	0x0F	// display on/off
		 * 0	0000 0110	0x06	// entry mode
		 * 
		 * I'll need to pulse en after each nibble.
		 * 
		 * I'll need to send the first nibble from here, and pulse the en pin the first time.
		 * After that, I should be able to send everything through command and write
		 */

		_data[3]->value(BBIO::LOW); // data7
		_data[2]->value(BBIO::LOW); // data6
		_data[1]->value(BBIO::HIGH);// data5
		_data[0]->value(BBIO::LOW); // data4
		
		pulse_en();
		
		command(0x28);
		command(0x0F);
		command(0x06);
		
		/* Bit banging works! I can now use the LCD.
		write(0x48); // H
		write(0x65); // e
		write(0x6C); // l
		write(0x6C); // l
		write(0x6F); // o
		write(0x20); // space
		write(0x57); // W
		write(0x6F); // o
		write(0x72); // r
		write(0x6C); // l
		write(0x64); // d
		write(0x21); // !
		*/
	}
	
	void LCD::send(unsigned char data_byte, unsigned char rs_val) {
		if (rs_val == 0)
			// rs needs to be low for a command
			_rs->value(BBIO::LOW);
		else
			// rs needs to be high
			_rs->value(BBIO::HIGH);
		
		// this is where i need to set the values on each of the GPIO lines based
		// off the value of data_bits.
		// i'll need to write the first four bits, pulse en, write the last four, and pulse again.
		
		unsigned char high_nibble = (data_byte >> 4);
		unsigned char low_nibble = (data_byte & 0x0F);
		
		set4bits(high_nibble);
		pulse_en();
		set4bits(low_nibble);
		pulse_en();
	}
	
	void LCD::set4bits(unsigned char data_bits) {
		//_data[3]->value(BBIO::LOW);	// data7
		//_data[2]->value(BBIO::LOW);	// data6
		//_data[1]->value(BBIO::HIGH);	// data5
		//_data[0]->value(BBIO::LOW);	// data4
		// an input of 0x2 will be evaluated as 0100
		for (int i = 3; i >= 0; i--) {
			bool bit = ((data_bits >> i) & 0x01);
			if (bit)
				_data[i]->value(BBIO::HIGH);
			else
				_data[i]->value(BBIO::LOW);
		}
	}
	
	void LCD::pulse_en() {
		/*
		 * The en line is supposed to be pulled high.
		 * To pulse the en line I need to...
		 * Pull the line low
		 * Wait >450ns
		 * Pull the line high
		 * wait >37us for things to settle
		 * 1us = 1000ns (microsecond to nanosecond)
		 */
		_en->value(BBIO::LOW);
		usleep(1);
		_en->value(BBIO::HIGH);
		usleep(50);
	}
	
	/*
	 * There's some funky things to know about the way an LCD displays to the screen.
	 * The maximum sized LCD that the controller will support is 20x4.
	 * Think of the LCD as having a little memory on it (because it does).
	 * This memory takes the form of an array.
	 * The upper left character on the LCD is at position 0 of the array.
	 * If you were using a 20x4 LCD then the lower right character would be at the last position.
	 * When you write to a 16x2 LCD then things get funny.
	 * The first row can display the first 16 positons of the array, but consists of the first 40.
	 * The second row displays the last 40 positions on the aray.
	 * Text will wrap, but you'll need to put in >40 characters to fill the first row. When you
	 * do this you will only be able to see the first 16 characters on the LCD.
	 * The solution is for the user to be aware of this behavior, and take advantage of the
	 * methods that have been defined so text displays where they want it to.
	 */
	void LCD::print(const char* val) {
		for (int i = 0; i < strlen(val); i++)
			write(val[i]);
	}
	
	void LCD::clear() {
		command(0x01);
	}
	
	void LCD::home() {
		command(0x02);
	}
	
	void LCD::onOff(bool display, bool cursor, bool blink) {
		// use ternary operator for cleaner code.
		// variable = (condition) ? true : false;
		unsigned char val = 0x08;
		val = (display) ? (val | 0x04) : (val & ~0x04);
		val = (cursor) ? (val | 0x02) : (val & ~0x02);
		val = (blink) ? (val | 0x01) : (val & ~0x01);
		command(val);
	}
	
	void LCD::curPos(unsigned int col, unsigned int row) {
		unsigned int offsets[4] = { 0x00, 0x40, 0x00 + _cols, 0x40 + _cols };
		const unsigned int max_lines = sizeof(offsets) / sizeof(*offsets);
		
		if (row >= max_lines)
			row = max_lines - 1;
		
		if (row >= _rows)
			row = _rows - 1;
		
		command(0x80 | (col + offsets[row]));
	}
	
	void LCD::scroll(bool scrollLock, bool leftRight) {
		unsigned char val = 0x04;
		val = (scrollLock) ? (val | 0x01) : (val & ~0x01);
		val = (leftRight) ? (val | 0x02) : (val & ~0x02);
		printf("\nval = 0x%X", val);
		command(val);
	}
	
	void LCD::scrollDisplay(bool right) {
		// comes out ot 0x18. example does stupid shit like this.
		// it was done to create mode #defines
		unsigned char val = 0x10 | 0x08;
		val = (right) ? (val | 0x04) : (val & ~0x04);
		command(val);
	}
	
} // namespace
