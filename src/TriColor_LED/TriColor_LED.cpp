#include "TriColor_LED.h"

using namespace std;

namespace PatricksDrivers {
	
	TriColor_LED::TriColor_LED(
		const char* red_key,
		const char* green_key,
		const char* blue_key
	) : Red(red_key), Blue(blue_key), Green(green_key) {
		
	}
	
	TriColor_LED::~TriColor_LED() {
		
	}
	
} // namespace PatricksDrivers
