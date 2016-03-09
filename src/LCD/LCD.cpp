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
		const char* data7
	) {
		printf("\nLCD called!");
		printf("\nrs = %s", rs);
		printf("\nen = %s", en);
		printf("\ndata4 = %s", data4);
		printf("\ndata5 = %s", data5);
		printf("\ndata6 = %s", data6);
		printf("\ndata7 = %s", data7);
		
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
		_en->value(BBIO::LOW);
		_data[0]->value(BBIO::LOW);
		_data[1]->value(BBIO::LOW);
		_data[2]->value(BBIO::LOW);
		_data[3]->value(BBIO::LOW);
	}
	
	LCD::~LCD() {
		// I don't need to delete anything becuase garbage collection will take care of it.
		// When an object goes out of scope the destructors are automatically called.
	}
	
} // namespace
