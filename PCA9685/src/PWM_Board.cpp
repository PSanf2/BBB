#include "PWM_Board.h"

#include <cstdio>	// pulls in read()
#include <math.h>	// pulls in round()
#include <unistd.h> // pulls in usleep()
#include <cstdio>	// pulls in printf()

using namespace std;

namespace PatricksDrivers {
	
	// I'm using the funky constructor because I'm inheriting off the I2C_IO class.
	// All of the public members are available in the main file
	// All of the protected members are available here
	// All of the private members are still private.
	PWM_Board::PWM_Board(unsigned char bus, unsigned char addr) : I2C_IO(bus, addr) {
		
	}
	
	void PWM_Board::setRefresh(unsigned char rate) {
		
		// calculate the new prescale value
		// convert it to the proper data type
		unsigned char pre_val = (unsigned char) (round(OSC_CLOCK / (4096 * rate)) - 1);
		
		// i need to put the board in to sleep mode
		sleep();
		
		// i need to write to the proper register to update the refresh rate
		writeRegister(PRE_SCALE, pre_val);
		
		
		
	}
	
	void PWM_Board::wake() {
		// clear the sleep bit (4)
		// writing a 0 to the restart bit (7) does not affect it.
		unsigned char mode = readRegister(MODE1);
		mode = mode & 0x6F;
		writeRegister(MODE1, mode);
		// wait 500 us
		usleep(500);
		// maybe hit the restart bit (bit 7).
		// when waking the chip you may need to hit the restart bit to get
		// the chip to start sending the on and off signal out the channel
		// leads. this is because we must assume that the user did not
		// do a graceful shutdown of all the PWM channels. hitting the
		// restart bit with a 1 will actually trigger the chip to toggle
		// the value that's in it. We want it to be a 0
		mode = readRegister(MODE1);
		if ((mode & 0x80) == 0x80) {
			mode = mode & 0xEF;
			writeRegister(MODE1, mode);
		}
		
	}
	
	void PWM_Board::sleep() {
		// I'm not going to worry about a graceful shutdown to prevent
		// the reset bit from being set. when the chip is put into sleep
		// mode without clearing the PWM bits the registers will hold
		// their state, and the reset bit will be set. this means you
		// can put the board to sleep, and the register settings on the
		// PWM channels won't change. When you wake the board up you
		// need to clear the reset bit by sending it a 1.
		unsigned char mode = readRegister(MODE1);
		mode = mode | 0x10;
		writeRegister(MODE1, mode);
	}
	
	void PWM_Board::reset() {
		// I want this function to perform a software reset of the board.
		// According to the datasheet I need to make a some special
		// writes onto the I2C bus. I'll need to be able to send the
		// start condition (somehow doing this already)
		// write a bite to the I2C bus indicating i'm writing to an
			// all call device address.
		// send a specific byte: 0000 0110. That's the SWRST
		// send the stop condition.
		// I'll also need to be able to read acks from the slave device
		
		// THIS IS HOW RESETS WORK W/ I2C
		// There's a "virtual" device on the I2C bus at address 0x00
		// that all devices respond to for very specific calls. You can
		// address every I2C device on this bus with this General Call
		// address. Once you've made a call to that "device" you send a
		// byte that every device will do something with. If the
		// if (incoming byte) == some value logic holds true then the
		// device will do something. For my device, if the byte is 0x06
		// then the device will perform a software reset.
		
		I2C_IO *General = new I2C_IO(bus, 0x00);
		
		if (!General->is_open()) {
			printf("\nFailed to open General device");
			return;
		}
		printf("\nGeneral device open");
		
		General->write(0x00); // You need to write to register 0x00.
		// for some reason that does not seem to be documented.
		
		General->write(0x06);
		
		General->close();
		
		delete General;
	}
	
	void PWM_Board::setPWM(unsigned char channel, unsigned int on, unsigned int off) {
		unsigned int ledN_on_l = LED0_ON_L+4*channel;
		writeRegister(ledN_on_l, on);
		writeRegister(ledN_on_l + 1, on >> 8);
		writeRegister(ledN_on_l + 2, off);
		writeRegister(ledN_on_l + 3, off >> 8);
	}
	
	void PWM_Board::setAllPWM(unsigned int on, unsigned int off) {
		writeRegister(ALL_LED_ON_L, on);
		writeRegister(ALL_LED_ON_H, on >> 8);
		writeRegister(ALL_LED_OFF_L, off);
		writeRegister(ALL_LED_OFF_H, off >> 8);
	}
	
	unsigned char PWM_Board::get_bus() {
		return bus;
	}
	
	unsigned char PWM_Board::get_addr() {
		return addr;
	}
	
	int PWM_Board::get_error() {
		return error_code;
	}
	
	PWM_Board::~PWM_Board() {
		
	}

}
