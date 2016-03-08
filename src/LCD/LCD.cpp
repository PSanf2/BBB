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
	
	/* Class LCD_RGB_PWM */
	
	LCD_RGB_PWM::LCD_RGB_PWM(
		const char* rs,
		const char* en,
		const char* data4,
		const char* data5,
		const char* data6,
		const char* data7,
		const char* red,
		const char* green,
		const char* blue
	) : LCD(rs, en, data4, data5, data6, data7) {
		printf("\nLCD_RGB_PWM called!");
		printf("\nred = %s", red);
		printf("\ngreen = %s", green);
		printf("\nblue = %s", blue);
		
		_red = new BBIO::PWM(red);
		_green = new BBIO::PWM(green);
		_blue = new BBIO::PWM(blue);
		
		_red->start();
		_green->start();
		_blue->start();
		
		// If you pass an signed int then 0 is full on, and higher values are lower.
		// If duty == period then the LED is off.
		// If you pass an unsigned int then things get a little whackier.
		_red->duty(0);
		_green->duty(0);
		_blue->duty(0);
	}
	
	LCD_RGB_PWM::~LCD_RGB_PWM() {
		
	}
	
	/* Class LCD_GPIO_Backlight */
	
	LCD_GPIO_Backlight::LCD_GPIO_Backlight(
		const char* rs,
		const char* en,
		const char* data4,
		const char* data5,
		const char* data6,
		const char* data7,
		const char* backlight
	) : LCD(rs, en, data4, data5, data6, data7) {
		printf("\nLCD_GPIO_Backlight called!");
		printf("\nbacklight = %s", backlight);
	}
	
	LCD_GPIO_Backlight::~LCD_GPIO_Backlight() {
		
	}
	
} // namespace
