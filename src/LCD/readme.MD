This is my attempt to create an LCD drive for the BeagleBone Black.
I'll be working with LCDs using the Hitachi HD44780 controller.
I should be able to work with any LCD using a compatable controller.

Using an LCD requires four or eight data lines, an enable line, and an RS line.
There will also be a power and ground line.
I may need to support a tri-color LED if one is being used for the backlight. This will require three PWM channels.

Controlling an LCD is a matter of controlling six to ten GPIO pins, and possibly as many as three PWM pins. In order to do this I will need to alter the device tree in user space. This is possible via the command line, but I need to create a proper device tree overlay. In order to get started I'll be creating a shell script that will run the required commands to set up each of the required pins individually so my program can run. I'll eventually want to create a complete device tree overlay that my program can dynamically load at runtime that won't clobber overlays which may already be loaded.

The process for controlling an LCD is pretty simple.
The four to eight data lines are used to communicate a nibble or byte of information to the LCD. The RS line is used to indicate if the data lines are sending information relating to a command or data. The enable line is used as a clock to tell the LCD when it may read the values that have been set on the other five to nine lines. If I'm wanting to be able to control the backlight then I'll need to use PWM channels to essentially control the brightness of an LED. One very important item of consideration is the timing of events. The LCD needs to be provided a minimum amount of time to respond to certain events. It's also possible to perform some read operations on the LCD, but these are not important.

Getting the device tree set up will be a matter of running the proper commands.
I'll need to determine which pins I can use for GPIO and PWM, figure out the proper commands to run, document them, and create a script to automate the process. I'll need to go through a few revisions of the script before I'll find something I'm happy with. Later version should make a point of asking the user which pins will be used, and for which purpose.

For the first stage of my script I'll be using LEDs to test the functionality of each pin. This will be good because an LED will tell me the GPIO output state, and can be used to guage the performance of a PWM pin within reason. Using a tri-color LED will also be a good exercize to get all of the PWMs working together.

If I'm clever, and seperate the LCD and LED functionality I'll get a better program. The LCD code should probably lean on some underlying LED code to drive the backlight, and make things easier on the programmer.

It would probably be better if I created a library that would allow any program to dynamically specifiy which pins are being used. Python has a library called Adafruit_BBIO that can do this. Python sucks so I'll probably figure out how to reproduce this functionality purely in C++.

Python once again proves itself to be shit. The Adafruit_BBIO python library only works if you run it as root unless you jump through some hoops to install it properly. The upshot is Python ends up eventually needing to rely on C++ in order to actually get anything done, so there are some fair examples of how to manipulate the device tree. They're mainly doing some fancy shit with writing, reading, and deleting select lines from specific files. The downside is that Python's cleanup routine doesn't work, and it leave the system with hanging device tree overlays. The programmer can partially mediate this by calling cleanup functions, but certain overlays still get left behind. It looks like ine overlay per pin used will be left hanging if it's not cleaned up by the programmer. After that, an overlay named am33xx_pwm is left behind, and cannot be unloaded through the terminal. The Adafruit code has a function to unload device tree overlays, but it's never called for that one. The Adafruit code demonstrates a method that can be used to dynamically manage the device tree. The Adafruit_BBIO C++ code looks to be alright, and function properly. The Python usage of it is shit because Python is shit. A scripting language should have proper garbage collection functionality to ensure nothing gets left hanging. It could be that this is not an issue if the library is properly installed to run in userspace without needing to be root. Either way, I'll want my code to be stand alone, and clean up properly after itself.

Turns out the stupid reason that stupid Python does everything as stupid root is because the kernel doesn't support fucking with the device tree as a user by default. Molloy covers what needs to be done to address this on page 247 of his book. The Adafruit_BBIO library doesn't attempt to address this because Python is shit. Page 223 has information about setting up PINS and SLOTS.
The way Python is doing things is just fucking wrong.
Malloy's solution is to set up SLOTS, and PINS to get set up at boot, then fix the security issue to allow users to manipulate the device tree. Malloy is relying heavily on screwing with the device tree, but it may be possible to use the pins in a program without doing so. Using the commands below you can use a GPIO pin by reading/writing to the proper files. I believe it's possible to do something similar w/ the PWM controls. This relys on the "everything is a file" concept of Linux. The commands below are actaully piping desired values into files to enable, manipulate, and disable the pins i'm wanting to use. The code I've done for the I2C bus work on a similar prinicple, but just happens to work with files in other locations. Malloy wants to fuck with the device tree for analog and PWM stuff. It's probably not needed because there are pins available for this by default. Nope. You need to be able to use the device tree, and there's no way around it. Rip off what you need from the available open source code, improve it, write a little script to properly install the library, and make sure I'm doing actualy to goodness garbage collection so I'm not screwing everything over for the next guy. All of the search results I find keep coming back to the Adafruit_BBIO Python library. Looks like they were the first to publish code for hipsters, and they've meme'ed the shit out of it. The upshot is I can probably test Molloy's permissions fix, and get it to run on the proper user account. The main thing wrong with the Adafruit code is the fact that it was written for Python.

This project is probably going to end up adding more singletons to my lib folder.
I'll end up with one for analog pins, one for GPIO, and one for PWM

GPIO's don't use the device tree. analog and PWM do.

Life is a learning experience.
My code is suffering from some of the same flaws that Python code has.
You have to do sudo su before you can run anything. I believe this can be overcome. The Adafruit code doesn't mention trying to get around this, and was only developer tested with root. Molloy addresses the shortcoming, noting that it's due to Linux permissions, and offers a possible workaround. I'll need to test this.
There are memory leaks when the program terminates. With the Python code it's leaving things hanging on the device tree, and failing to clean up after itself. My code looks like an improperly implemented singleton template that results in the singleton object destructor never being called (and thus never deleting the instance from memory). Some of this can be mitigated by the programmer being smart about his own garbage collection from the calling code. The ultimate solution will be to redo the way I have the static pointer instance variable set up, and altering getInstance. I'm using a Gamma Singleton pattern. What I'm looking at as a memory leak is being cleaned up by the OS when the program quits. That's why the destructors are never called. The alternative pattern is the Meyers singleton, which I'll need to examine. What I have is not a memory leak. The OS will clean up the instances that have been allocated, but I need to explicitly clean up after myself. If I really wanted to get stupid with making sure a single destructor is called then there are some wonkly things I can do with friend classes that involve explicitly destroy the singleton when the friend goes out of scope. The Meyers singleton has been documented as acutally not being thread safe, and possibly causing real memory leaks.


------------------------------------------------------------------------

THIS IS HOW YOU CONTROL A SINGLE GPIO PIN AS ROOT FROM THE TERMINAL
In this example I'm controlling an LED on P8_09.
I get the pin set up, turn the LED on, off, then clean up.

debian@beaglebone:~$ sudo su
root@beaglebone:/home/debian# cd /sys/class/gpio
root@beaglebone:/sys/class/gpio# ls
export	gpiochip0  gpiochip32  gpiochip64  gpiochip96  unexport
root@beaglebone:/sys/class/gpio# echo 69 > export
root@beaglebone:/sys/class/gpio# ls
export	gpio69	gpiochip0  gpiochip32  gpiochip64  gpiochip96  unexport
root@beaglebone:/sys/class/gpio# cd gpio69
root@beaglebone:/sys/class/gpio/gpio69# ls
active_low  direction  edge  power  subsystem  uevent  value
root@beaglebone:/sys/class/gpio/gpio69# echo out > direction
root@beaglebone:/sys/class/gpio/gpio69# cat value
0
root@beaglebone:/sys/class/gpio/gpio69# echo 1 > value
root@beaglebone:/sys/class/gpio/gpio69# echo 0 > value
root@beaglebone:/sys/class/gpio/gpio69# cd ..
root@beaglebone:/sys/class/gpio# echo 69 > unexport
root@beaglebone:/sys/class/gpio# ls
export	gpiochip0  gpiochip32  gpiochip64  gpiochip96  unexport
root@beaglebone:/sys/class/gpio# exit
exit
debian@beaglebone:~$ 
