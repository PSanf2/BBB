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
		/*
		printf("\nLCD called!");
		printf("\nrs = %s", rs);
		printf("\nen = %s", en);
		printf("\ndata4 = %s", data4);
		printf("\ndata5 = %s", data5);
		printf("\ndata6 = %s", data6);
		printf("\ndata7 = %s", data7);
		printf("\nrows = %i", rows);
		printf("\ncols = %i", cols);
		*/
		
		// I need to create a BBIO::GPIO object for each of the inputs.
		// I need to set the relevant member variables to store pointers to those objects.
		// I need to set the direction on each of the GPIO objects.
		// I need to set the initial value for each of the GPIO objets.
		
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
		 * 0	0010	0x20
		 * 0	1000	0x80
		 * 0	0000	0x00
		 * 0	1111	0xF0
		 * 0	0000	0x00
		 * 0	0110	0x60
		 * Byte sequence
		 * rs	7654 3210	0xN
		 * 00	0010 ----	0x2-	// 1 nibble operation
		 * 0	0010 1000	0x28
		 * 0	0000 1111	0x0F
		 * 0	0000 0110	0x06
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
		
		/* Bit banging works!
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
	
	LCD::~LCD() {
		// I don't need to delete anything becuase garbage collection will take care of it.
		// When an object goes out of scope the destructors are automatically called.
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
		
	}
	
	void LCD::scrollDisplayLeft() {
		
	}
	
	void LCD::scrollDisplayRight() {
		
	}
	
	void LCD::leftToRight() {
		
	}
	
	void LCD::rightToLeft() {
		
	}
	
	void LCD::autoScrollOn() {
		
	}
	
	void LCD::autoScrollOff() {
		
	}
	
} // namespace
