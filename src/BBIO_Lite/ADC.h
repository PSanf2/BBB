/*
 * Mod By:	Patrick Sanford
 * Version:	0.0
 * Date:	Feb 12, 2016
 * GitHub:	https://github.com/PSanf2/BBB
 * 			https://github.com/adafruit
 * Desc:	This is my modification of the Adafruit Beaglebone IO Python
 * 			library.
 */

#ifndef __BBIO_ADC_H
#define __BBIO_ADC_H

#include "Common.h"

namespace BBIO {
	
	typedef struct adc_pin_t {
		const char* name;
		const char* key;
		char* ocp_path;
	} adc_pin_t;
	
	const adc_pin_t ADC_Info[] = {
		//{"VDD_ADC", "P9_32"},
		{"AIN4", "P9_33"},
		//{"GNDA_ADC", "P9_34"},
		{"AIN6", "P9_35"},
		{"AIN5", "P9_36"},
		{"AIN2", "P9_37"},
		{"AIN3", "P9_38"},
		{"AIN0", "P9_39"},
		{"AIN1", "P9_40"}
	};
	
	class ADC {
		public:
			ADC(const char* key);
			~ADC();
			int value();
		private:
			adc_pin_t _info;
			static bool _initialized;
	}; // class ADC
	
} // namespace BBIO

#endif
