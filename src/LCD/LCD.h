#ifndef __LCD_H
#define __LCD_H

#include "../BBIO_Lite/PWM.h"
#include "../BBIO_Lite/GPIO.h"

namespace PatricksDrivers {
	
	class LCD {
		protected:
			BBIO::GPIO* _rs;
			BBIO::GPIO* _en;
			BBIO::GPIO* _data[4];
		public:
			LCD(
				const char* rs,
				const char* en,
				const char* data4,
				const char* data5,
				const char* data6,
				const char* data7
			);
			~LCD();
	}; // class LCD
	
	class LCD_RGB_PWM : public LCD {
		private:
			BBIO::PWM* _red;
			BBIO::PWM* _green;
			BBIO::PWM* _blue;
		public:
			LCD_RGB_PWM(
				const char* rs,
				const char* en,
				const char* data4,
				const char* data5,
				const char* data6,
				const char* data7,
				const char* red,
				const char* green,
				const char* blue
			);
			~LCD_RGB_PWM();
	}; // class LCD_RGB_PWM
	
	class LCD_GPIO_Backlight : public LCD {
		private:
			BBIO::GPIO* _backlight;
		public:
			LCD_GPIO_Backlight(
				const char* rs,
				const char* en,
				const char* data4,
				const char* data5,
				const char* data6,
				const char* data7,
				const char* backlight
			);
			~LCD_GPIO_Backlight();
	}; // class LCD_GPIO_Backlight
	
} // namespace

#endif
