This is my attempt to create an LCD drive for the BeagleBone Black.
I'll be working with LCDs using the Hitachi HD44780 controller.
I should be able to work with any LCD using a compatable controller.

Using an LCD requires four or eight data lines, an enable line, and an RS line.
There will also be a power and ground line.
I may need to support a tri-color LED if one is being used for the backlight. This will require three PWM channels.

Controlling an LCD is a matter of controlling six to ten GPIO pins, and possibly as many as three PWM pins. In order to do this I will need to alter the device tree in user space. This is possible via the command line, but I need to create a proper device tree overlay. In order to get started I'll be creating a shell script that will run the required commands to set up each of the required pins individually so my program can run. I'll eventually want to create a complete device tree overlay that my program can dynamically load at runtime that won't clobber overlays which may already be loaded.

The process for controlling an LCD is pretty simply
The four to eight data lines are used to communicate a nibble or byte of information to the LCD. The RS line is used to indicate if the data lines are sending information relating to a command or data. The enable line is used as a clock to tell the LCD when it may read the values that have been set on the other five to nine lines. If I'm wanting to be able to control the backlight then I'll need to use PWM channels to essentially control the brightness of an LED. One very important item of consideration is the timing of events. The LCD needs to be provided a minimum amount of time to respond to certain events. It's also possible to perform some read operations on the LCD, but these are not important.

------------------------------------------------------------------------

Getting the device tree set up will be a matter of running the proper commands.
I'll need to determine which pins I can use for GPIO and PWM, figure out the proper commands to run, document them, and create a script to automate the process. I'll need to go through a few revisions of the script before I'll find something I'm happy with. Later version should make a point of asking the user which pins will be used, and for which purpose.

For the first stage of my script I'll be using LEDs to test the functionality of each pin. This will be good because an LED will tell me the GPIO output state, and can be used to guage the performance of a PWM pin within reason. Using a tri-color LED will also be a good exercize to get all of the PWMs working together.

If I'm clever, and seperate the LCD and LED functionality I'll get a better program. The LCD code should probably lean on some underlying LED code to drive the backlight, and make things easier on the programmer.

It would probably be better if I created a library that would allow any program to dynamically specifiy which pins are being used. Python has a library called Adafruit_BBIO that can do this. Python sucks so I'll probably figure out how to reproduce this functionality purely in C++.
