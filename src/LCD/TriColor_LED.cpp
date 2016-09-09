#include "TriColor_LED.h"

#include <cstdio>		// pulls in printf()

using namespace std;

namespace PatricksDrivers {
	
	TriColor_LED::TriColor_LED(
		const char* red_key,
		const char* green_key,
		const char* blue_key
	) : Red(red_key), Green(green_key), Blue(blue_key) {
		start();
	}
	
	TriColor_LED::~TriColor_LED() {
		
	}
	
	void TriColor_LED::start() {
		Red.polarity(1);
		Green.polarity(1);
		Blue.polarity(1);
		Red.start();
		Green.start();
		Blue.start();
	}
	
	void TriColor_LED::stop() {
		Red.stop();
		Green.stop();
		Blue.stop();
	}
	
	void TriColor_LED::duty(int r, int g, int b) {
		Red.duty(r);
		Green.duty(g);
		Blue.duty(b);
	}
	
} // namespace PatricksDrivers
