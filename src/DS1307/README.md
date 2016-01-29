THE DS1307 CODE HAS NOT BEEN TESTED ON THE ADAFRUIT DS1307 BREAKOUT BOARD!
DO NOT USE THIS CODE WITH THE ADAFRUIT DS1307 BREAKOUT BOARD!

This code was written for the DS1307 chip. I purchased the chip from Mouser.
When I wanted to add a real-time clock to my BeagleBone Black the Adafruit DS1307 Breakout Board Kit was out of stock, and I did not purchase one. I read all of the available information regarding the Adafruit breakout board, purchased components from Mouser, read the datasheet for the DS1307 chip, and assembled my components on a breadboard according to the datasheet. My breadboarded circuit has a 10K pull-up resistor that the Adafruit breakout board does not include. This is being used to enable the square wave output functionality of the chip. It is possible to enable the square wave generator on the Adafruit breakout board according to the available documentation, but since I don't have one those it means my information is second-hand.

I have assembled my circuit on a breadboard because I am interested in learing to etch my own PCBs, and prototyping my circuit before creating a layout in KiCad. I'm writing the code for the chip before creating a PCB to ensure I fully understand the functioning of the chip, have everything connected correctly, and have a working prototype to test my finished version against.

By writing drivers for Adafruit breakout boards I've been learning how to use the BeagleBone Black, learning how to use the I2C bus, and learning how to use a variety of chips with useful purposes. I intend to learn how to create my own printed circuit boards, and reverse engineering Adafruit breakout boards offers a good learning opportunity. I'm intending to create a PCB to mount the components required to support the DS1307 chip as a first attempt at etching a single sided PCB. I've chosen the DS1307 for my first PCB creation attempt because of the simple circuitry required, and the interesting functionality.

UPDATE JAN 29, 2016
I've been able to create my own breakout board based on the DS1307. I etched my own PCB, and soldered my components on to it. I've tested my self-made PCB, and it functions to my satisfaction.
