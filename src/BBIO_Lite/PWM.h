/*
 * Mod By:	Patrick Sanford
 * Version:	0.0
 * Date:	Feb 12, 2016
 * GitHub:	https://github.com/PSanf2/BBB
 * 			https://github.com/adafruit
 * Desc:	This is my modification of the Adafruit Beaglebone IO Python
 * 			library.
 */

#ifndef __BBIO_PWM_H
#define __BBIO_PWM_H

#include "Common.h"	// Inclue BBIO_Lite Common library.

namespace BBIO {
	
	typedef struct pwm_pin_t {
		const char* name;
		const char* key;
		char* ocp_path;
	} pwm_pin_t;
	
	const pwm_pin_t PWM_Info[] = {
		{"EHRPWM2B",	"P8_13"},
		{"EHRPWM2A",	"P8_19"},
		{"EHRPWM1A",	"P9_14"},
		{"EHRPWM1B",	"P9_16"},
		{"UART2_TXD",	"P9_21"},
		{"UART2_RXD",	"P9_22"},
		{"GPIO0_7",		"P9_42"}
	};
	
	class PWM {
		public:
			PWM(const char* key);
			~PWM();
			// run()
			// stop()
			// set_duty()
			// set_period()
			// get_duty()
			// get_period()
		private:
			pwm_pin_t _info;
			static bool _initialized;
	}; // class PWM
	
} // namespace BBIO

#endif
