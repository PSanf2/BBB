#include "LCD.h"

#include <cstdio>		// pulls in printf()

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
		printf("\nLCD called!");
		printf("\nrs = %s", rs);
		printf("\nen = %s", en);
		printf("\ndata4 = %s", data4);
		printf("\ndata5 = %s", data5);
		printf("\ndata6 = %s", data6);
		printf("\ndata7 = %s", data7);
		printf("\nrows = %i", rows);
		printf("\ncols = %i", cols);
		
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
		 
		 _data[3]->value(BBIO::LOW);	// data7
		 _data[2]->value(BBIO::LOW);	// data6
		 _data[1]->value(BBIO::HIGH);	// data5
		 _data[0]->value(BBIO::LOW);	// data4
		 
		 pulse_en();
		 
		 command(0x28);
		 command(0x0F);
		 command(0x06);
	}
	
	LCD::~LCD() {
		// I don't need to delete anything becuase garbage collection will take care of it.
		// When an object goes out of scope the destructors are automatically called.
	}
	
	void LCD::send(unsigned char data_bits, unsigned char rs_val) {
		printf("\nsend called!");
		if (rs_val == 0) {
			// rs needs to be low for a command
			_rs->value(BBIO::LOW);
		} else {
			// rs needs to be high
			_rs->value(BBIO::HIGH);
		}
		
		// this is where i need to set the values on each of the GPIO lines based
		// off the value of data_bits.
		// i'll need to write the first four bits, pulse en, write the last four, and pulse again.
		
	}
	
	void LCD::pulse_en() {
		printf("\npulse_en called!");
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
		
	}
	
	void LCD::clear() {
		
	}
	
	void LCD::home() {
		
	}
	
	void LCD::curPos(unsigned int col, unsigned int row) {
		
	}
	
	void LCD::dispOn() {
		
	}
	
	void LCD::dispOff() {
		
	}
	
	void LCD::curOn() {
		
	}
	
	void LCD::curOff() {
		
	}
	
	void LCD::curBlinkOn() {
		
	}
	
	void LCD::curBlinkOff() {
		
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
