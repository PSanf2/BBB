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

Controlling PWM with the config-pin utility is the way to go.
It will possibly also work for GPIOs.
The source is in /opt/source/beaglebone-universal-io.
It looks like the program has been into the system path, so it's available from everywhere.
I'm having an issue unloading the device tree overlay. When I try to unload an overlay it causes a kernel panic that kills the terminal session. According to Adafruit this is a bug, but should have already been corrected. The recommended workaround is rebooting the BBB instead of attempting to remove the overlay.

The bottom line is config-pin in pretty awesome, and will probably work for all of my pin configuration needs. Instead of fucking around with device tree overlays I should just use config-pin, and the overlays it comes with.
I need to see if Malloy's method of getting around using the root user works.
If it does, then I just need to set up the cape-universaln overlay to deploy at boot.
From there, my programs will be able to export pins as needed by writing to files.
Screwing with the MUX settings may involve system calls to config-pin, or writing to files.
If Malloy's methods don't work then I may be able to get around the root user issue by screwing with the permissions on various files.
You can call config-pin without needing to be root, but you need to use the full path to call the program. You still can't export, or unexport anything without being root.

Why do you need to be root to do fun stuff?
You need to be root to load a device tree overlay because it's something that's screwing with the way the system is configured to interact with hardware. This is sane from the OS perspective.
All of the directories that deal with exporting and unexporting pins are created under the /sys/class directory, and inherit the permissions of their parent directory. This is sane from the OS perspective. When working in Linux, everything is a file. This is sane because it makes an OS a lot easier to program, and works well.
Linux was programmed, and intended to be, a multi-user system. That's where thing start failing to be sane.
On the BBB, most people are interacting with the system through SSH, and using root. When the DTO system was implemented on the BBB it was decided that the programmers would interact with pins by writing to the "files" in the /sys/class/<blah>/ because that's where the system was going to mount them anyway. The fact that people were using root didn't matter because nobody was really thinking about needing to use other accounts. The intention is that you'll SSH into the BBB, configure the DTOs to load at boot time, and start whatever programs you want to run as root. When the BBB is intended to be left chugging away on its own without human supervision for long periods this is an acceptable method to do things. If you're actually wanting to follow good practices, and write a program that will work properly within the OS security constraints, you're pretty much fucked.
After looking at nearly every way I can think of, there really isn't a good method to get around the issue.
Ultimatly, apart from GPIOs, attempting to get around this is going to be a giant waste of time.
If you write your own device tree overlay then you still need to be root to deploy it. Once it's deployed you need to be root to write to the /sys/class/<blah>/ files. It looks like the exceptions are the items in /dev/, which includes I2C.

The Adafruit Python bullshit does have the saving grace is needing to rely on C++ in order to actually fucking work. This means I can fork their library, #include the relevant header files, have an easy time getting a lot of pins set up very quickly at run time, and it will work as long as it's being run by root.
SOMEHOW that shit actually manages to load device tree overlays if you run a python script with sudo.
That's well and good, but I still want to avoid getting python involved with my programs because of the performance issues scripting languages bring in to the picture.
The I2C functionality in the Adafruit BBIO library is probably shit. Their documentation states that it's only compatable with python2. Their code is actually a fork of code that was written for the Raspberry Pi, and adapted for the BBB. It looks like I should be able to rip out the relevant C portions of the code, and simplify a lot of it. From there, I'll just need to run my programs as root.

If I want to be very clever I'll set up some interprocess communication bullshit.
Set up some files in a very specific directory.
Create a group that will give users permissions to edit files in that directory.
Add my user account to that group.
Create a daemon that starts at run time under root.
Have the daemon check the files in the directory for parameters to control it.
Edit the files from the user accound to control the daemon.
Ta-da! You can do a programmed set of tasks as root from a user account.
I'll need to test this to see what's possible. If I can do this, then I'd just need to make an interface that would get the pins set up, and cleaned up as needed.

------------------------------------------------------------------------

SET UP SLOTS AND PINS FIRST!
It is possible to load device tree overlays directly into /sys/devices/bone_capemgr.9/slots.
debian@beaglebone:~/git-repos/BBB$ export SLOTS=/sys/devices/bone_capemgr.9/slots
debian@beaglebone:~/git-repos/BBB$ export PINS=/sys/kernel/debug/pinctrl/44e10800.pinmux/pins
debian@beaglebone:~/git-repos/BBB$ cat $SLOTS
 0: 54:PF--- 
 1: 55:PF--- 
 2: 56:PF--- 
 3: 57:PF--- 
 4: ff:P-O-L Bone-LT-eMMC-2G,00A0,Texas Instrument,BB-BONE-EMMC-2G
 5: ff:P-O-L Bone-Black-HDMI,00A0,Texas Instrument,BB-BONELT-HDMI
debian@beaglebone:~/git-repos/BBB$ cat $PINS
cat: /sys/kernel/debug/pinctrl/44e10800.pinmux/pins: Permission denied
debian@beaglebone:~/git-repos/BBB$ sudo cat $PINS
registered pins: 142
pin 0 (44e10800) 00000031 pinctrl-single 
pin 1 (44e10804) 00000031 pinctrl-single 
pin 2 (44e10808) 00000031 pinctrl-single 
...

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

------------------------------------------------------------------------

CONTROLLING A PWM OUTPUT AS ROOT FROM THE TERMINAL AS A PROGRAM WILL DO
In a program you load the am33xx_pwm device tree overlay, and then load an overlay fragment for each pin.
The overlay for each pin will be named similar to bone_pwm_P9_14. The last part changes.
Once that's done, everything gets manipulated through the /sys/devices/ocp.*/pwm_test_P9_14.*/ files.
When you're done you can unload the DTO fragment without an issue, but if you try to unload the am33xx DTO you'll get a kernel panic. This is why the system is left with a hanging DTO in slots. This issue is due to a known bug with the operating system that was mentioned elsewhere in this file.
With this method of doing thigs, there will never be a chage to the items in /sys/class/pwm. The programmer needs to keep track of which pins have been "exported."

debian@beaglebone:~/git-repos/BBB$ sudo su
root@beaglebone:/home/debian/git-repos/BBB# echo am33xx_pwm > /sys/devices/bone_capemgr.*/slots
root@beaglebone:/home/debian/git-repos/BBB# echo bone_pwm_P9_14 > /sys/devices/bone_capemgr.*/slots
root@beaglebone:/home/debian/git-repos/BBB# cat /sys/devices/bone_capemgr.*/slots
 0: 54:PF--- 
 1: 55:PF--- 
 2: 56:PF--- 
 3: 57:PF--- 
 4: ff:P-O-L Bone-LT-eMMC-2G,00A0,Texas Instrument,BB-BONE-EMMC-2G
 5: ff:P-O-L Bone-Black-HDMI,00A0,Texas Instrument,BB-BONELT-HDMI
 7: ff:P-O-L Override Board Name,00A0,Override Manuf,am33xx_pwm
 8: ff:P-O-L Override Board Name,00A0,Override Manuf,bone_pwm_P9_14
root@beaglebone:/home/debian/git-repos/BBB# ls /sys/devices/ocp.*/pwm_test_P9_14.*
driver	duty  modalias	period	polarity  power  run  subsystem  uevent
root@beaglebone:/home/debian/git-repos/BBB# echo 10000 > /sys/devices/ocp.*/pwm_test_P9_14.*/period
root@beaglebone:/home/debian/git-repos/BBB# echo 5000 > /sys/devices/ocp.*/pwm_test_P9_14.*/duty
root@beaglebone:/home/debian/git-repos/BBB# echo 1 > /sys/devices/ocp.*/pwm_test_P9_14.*/run
root@beaglebone:/home/debian/git-repos/BBB# echo 1000 > /sys/devices/ocp.*/pwm_test_P9_14.*/duty
root@beaglebone:/home/debian/git-repos/BBB# echo 7500 > /sys/devices/ocp.*/pwm_test_P9_14.*/duty
root@beaglebone:/home/debian/git-repos/BBB# echo 0 > /sys/devices/ocp.*/pwm_test_P9_14.*/run
root@beaglebone:/home/debian/git-repos/BBB# echo -8 > /sys/devices/bone_capemgr.*/slots
root@beaglebone:/home/debian/git-repos/BBB# cat /sys/devices/bone_capemgr.*/slots
 0: 54:PF--- 
 1: 55:PF--- 
 2: 56:PF--- 
 3: 57:PF--- 
 4: ff:P-O-L Bone-LT-eMMC-2G,00A0,Texas Instrument,BB-BONE-EMMC-2G
 5: ff:P-O-L Bone-Black-HDMI,00A0,Texas Instrument,BB-BONELT-HDMI
 7: ff:P-O-L Override Board Name,00A0,Override Manuf,am33xx_pwm
root@beaglebone:/home/debian/git-repos/BBB# exit
exit
debian@beaglebone:~/git-repos/BBB$ 


------------------------------------------------------------------------

CONTROLLING A PWM OUTPUT AS ROOT FROM THE TERMINAL WITH EXPORT
This isn't the way you'll do it in a program.

EXPORT #	PIN NAME	PINS
	0		EHRPWM0A	P9.22,P9.31
	1		EHRPWM0B	P9.21,P9.29
	2		ECAPPWM0	P9.42
	3		EHRPWM1A	P9.14,P8.36
	4		EHRPWM1B	P9.16,P8.34
	5		EHRPWM2A	P8.19,P9.45
	6		EHRPWM2B	P8.13,P8.46
	7		ECAPPWM2	P9.28

In this example I'm attempting to use PWM to control an LED on P9_14.

debian@beaglebone:~$ sudo su
root@beaglebone:/home/debian# cat /sys/devices/bone_capemgr.9/slots
 0: 54:PF--- 
 1: 55:PF--- 
 2: 56:PF--- 
 3: 57:PF--- 
 4: ff:P-O-L Bone-LT-eMMC-2G,00A0,Texas Instrument,BB-BONE-EMMC-2G
 5: ff:P-O-L Bone-Black-HDMI,00A0,Texas Instrument,BB-BONELT-HDMI
root@beaglebone:/home/debian# echo cape-universaln > /sys/devices/bone_capemgr.9/slots
root@beaglebone:/home/debian# cat /sys/devices/bone_capemgr.9/slots
 0: 54:PF--- 
 1: 55:PF--- 
 2: 56:PF--- 
 3: 57:PF--- 
 4: ff:P-O-L Bone-LT-eMMC-2G,00A0,Texas Instrument,BB-BONE-EMMC-2G
 5: ff:P-O-L Bone-Black-HDMI,00A0,Texas Instrument,BB-BONELT-HDMI
 7: ff:P-O-L Override Board Name,00A0,Override Manuf,cape-universaln
root@beaglebone:/home/debian# config-pin -l P9.14
default gpio gpio_pu gpio_pd pwm
root@beaglebone:/home/debian# cat /sys/devices/ocp.*/P9_14_pinmux.*/state
default
root@beaglebone:/home/debian# config-pin P9.14 pwm
root@beaglebone:/home/debian# echo 3 > /sys/class/pwm/export 
root@beaglebone:/home/debian# ls /sys/class/pwm
export	pwm3  pwmchip0	pwmchip2  pwmchip3  pwmchip5  pwmchip7	unexport
root@beaglebone:/home/debian# ls /sys/class/pwm/pwm3
device	duty_ns  period_ns  polarity  power  run  subsystem  uevent
root@beaglebone:/home/debian# echo 10000 > /sys/class/pwm/pwm3/period_ns 
root@beaglebone:/home/debian# echo 5000 > /sys/class/pwm/pwm3/duty_ns 
root@beaglebone:/home/debian# echo 1 > /sys/class/pwm/pwm3/run
root@beaglebone:/home/debian# echo 1000 > /sys/class/pwm/pwm3/duty_ns 
root@beaglebone:/home/debian# echo 0 > /sys/class/pwm/pwm3/run
root@beaglebone:/home/debian# echo 3 > /sys/class/pwm/unexport 
root@beaglebone:/home/debian# ls /sys/class/pwm
export	pwmchip0  pwmchip2  pwmchip3  pwmchip5	pwmchip7  unexport
root@beaglebone:/home/debian# echo -7 /sys/devices/bone_capemgr.9/slots
-7 /sys/devices/bone_capemgr.9/slots
root@beaglebone:/home/debian# echo -7 > /sys/devices/bone_capemgr.9/slots

Message from syslogd@beaglebone at Feb 10 17:33:38 ...
 kernel:[  672.745394] Internal error: Oops: 17 [#1] SMP THUMB2

Message from syslogd@beaglebone at Feb 10 17:33:38 ...
 kernel:[  672.823814] Process bash (pid: 1522, stack limit = 0xdaa6c240)

Message from syslogd@beaglebone at Feb 10 17:33:38 ...
 kernel:[  672.829991] Stack: (0xdaa6ddd8 to 0xdaa6e000)

Message from syslogd@beaglebone at Feb 10 17:33:38 ...
 kernel:[  672.844734] ddc0:                                                       dac20210 c0021789

Message from syslogd@beaglebone at Feb 10 17:33:38 ...
 kernel:[  672.854478] dde0: dac20210 c02ccf59 dac20210 dabd2a10 00000000 c02cdab7 00000000 dead4ead

Message from syslogd@beaglebone at Feb 10 17:33:38 ...
 kernel:[  672.863258] de00: df465100 dac20244 40000013 dac20210 dac20280 dac20210 40000013 00000004

Message from syslogd@beaglebone at Feb 10 17:33:38 ...
 kernel:[  672.876856] de20: 00000001 daa646e0 daca99d8 c02cde95 df465100 dac20210 c0899080 c0898090

Message from syslogd@beaglebone at Feb 10 17:33:38 ...
 kernel:[  672.885828] de40: 00000001 c02c73d5 df465100 dac20244 dac20210 c02c744d 00000000 df04943c

Message from syslogd@beaglebone at Feb 10 17:33:38 ...
 kernel:[  672.894592] de60: dac20210 c02c710f dac20210 dabd2a10 00000000 c02c5acf dac20200 dabd2a00

Message from syslogd@beaglebone at Feb 10 17:33:38 ...
 kernel:[  672.903446] de80: 00000000 c02c8507 dac20200 c02c86b1 dab5ca80 c03db207 dab5ca80 daa64718

Message from syslogd@beaglebone at Feb 10 17:33:38 ...
 kernel:[  672.912518] dea0: 00200200 daa64718 00100100 c03db2b5 df465100 daa646e0 00000015 daa646e8

Message from syslogd@beaglebone at Feb 10 17:33:38 ...
 kernel:[  672.921396] dec0: df0d9810 daa6e000 df0d9818 c03db56b de34cc10 df0d9800 00000000 c02d8337

Message from syslogd@beaglebone at Feb 10 17:33:38 ...
 kernel:[  672.930156] dee0: de0a6c20 00000007 de0a8b88 c02d9585 000aa630 daca99ec c08c09c0 22222222

Message from syslogd@beaglebone at Feb 10 17:33:38 ...
 kernel:[  672.946059] df00: 22222222 22222222 00000000 c02d934d c02c5121 de0a8b88 c051ff50 00000003

Message from syslogd@beaglebone at Feb 10 17:33:38 ...
 kernel:[  672.991239] df20: df0d9818 daca99d8 000b6c58 c02c512b daa6df80 c00fe2ad daa6c000 00000003

Message from syslogd@beaglebone at Feb 10 17:33:38 ...
 kernel:[  673.051376] df40: daa28200 c00fe205 000b2c08 daa6df80 00000000 c00bc323 da83acc0 c00c5fe5

Message from syslogd@beaglebone at Feb 10 17:33:38 ...
 kernel:[  673.097120] df60: daa28200 daa28200 00000000 000b2c08 00000003 00000000 00000000 c00bc521

Message from syslogd@beaglebone at Feb 10 17:33:38 ...
 kernel:[  673.124512] df80: 00000000 00000000 00000000 b6ecd5e0 00000003 000b2c08 00000004 c000c9e4

Message from syslogd@beaglebone at Feb 10 17:33:38 ...
 kernel:[  673.145767] dfa0: daa6c000 c000c841 b6ecd5e0 00000003 00000001 000b2c08 00000003 00000000

Message from syslogd@beaglebone at Feb 10 17:33:38 ...
 kernel:[  673.176075] dfc0: b6ecd5e0 00000003 000b2c08 00000004 bef0e4ac 000ad06c 000b6c59 000b6c58

Message from syslogd@beaglebone at Feb 10 17:33:38 ...
 kernel:[  673.209601] dfe0: 00000003 bef0e430 b6e3b905 b6e7506c 40000010 00000001 00000000 00000000

Message from syslogd@beaglebone at Feb 10 17:33:38 ...
 kernel:[  673.450671] Code: 4604 b108 f8d0 41c8 (7ee3) 2b01 
debian@beaglebone:~$ cat /sys/devices/bone_capemgr.9/slots
^C^K^X

--------------------------------------------------------------------------------
WHAT TO DO

Make sure I have the Adafruit library installed, and working properly in Python (I do).
	This is mainly to make sure the Adafruit Device Tree Overlays are available, and working.
Get the Adafruit BBIO Python library (Got it).
Rip all of the Python shit out of the Adafruit code.
	Strip it down so only C functions are left, and what's left is all that's needed for the job.
Apply some good programming principles to what's left.
	Object oriented.
	Have a class for each type of pin setup I'll work with.
		I2C, GPIO, ADC, PWM (DAC), etc.
	Stop using singletons.
Port my LED code over to the new system.

--------------------------------------------------------------------------------
ANALOG INPUTS

From the terminal as a program would
You load the device tree, and then get your readings from /sys/devices/ocp.*/helper.*/AIN#
As with GPIOs, don't unload the device tree. If you attempt to do so you'll cause a kernel panic.

debian@beaglebone:~$ cat /sys/devices/bone_capemgr.9/slots
 0: 54:PF--- 
 1: 55:PF--- 
 2: 56:PF--- 
 3: 57:PF--- 
 4: ff:P-O-L Bone-LT-eMMC-2G,00A0,Texas Instrument,BB-BONE-EMMC-2G
 5: ff:P-O-L Bone-Black-HDMI,00A0,Texas Instrument,BB-BONELT-HDMI
debian@beaglebone:~$ sudo su
root@beaglebone:/home/debian# echo cape-bone-iio > /sys/devices/bone_capemgr.9/slots
root@beaglebone:/home/debian# exit
exit
debian@beaglebone:~$ cat /sys/devices/bone_capemgr.9/slots
 0: 54:PF--- 
 1: 55:PF--- 
 2: 56:PF--- 
 3: 57:PF--- 
 4: ff:P-O-L Bone-LT-eMMC-2G,00A0,Texas Instrument,BB-BONE-EMMC-2G
 5: ff:P-O-L Bone-Black-HDMI,00A0,Texas Instrument,BB-BONELT-HDMI
 7: ff:P-O-L Override Board Name,00A0,Override Manuf,cape-bone-iio
debian@beaglebone:~$ ls /sys/devices/ocp.3/
44e07000.gpio     44e35000.wdt    48044000.timer  4819c000.i2c                   48310000.rng       56000000.sgx  mmc.5        power
44e09000.serial   44e3e000.rtc    48046000.timer  481ac000.gpio                  49000000.edma      gpio-leds.8   modalias     rstctl.4
44e0b000.i2c      47400000.usb    48048000.timer  481ae000.gpio                  4a100000.ethernet  hdmi.13       nop-phy.6    sound.14
44e0d000.tscadc   48038000.mcasp  4804a000.timer  48200000.interrupt-controller  53100000.sham      helper.15     nop-phy.7    subsystem
44e10448.bandgap  48042000.timer  4804c000.gpio   4830e000.fb                    53500000.aes       mmc.11        nxptda@0.12  uevent
debian@beaglebone:~$ ls /sys/devices/ocp.3/helper.15/
AIN0  AIN1  AIN2  AIN3  AIN4  AIN5  AIN6  AIN7  driver  modalias  power  subsystem  uevent
debian@beaglebone:~$ cat /sys/devices/ocp.3/helper.15/AIN1
727
debian@beaglebone:~$ cat /sys/devices/ocp.3/helper.15/AIN1
719
debian@beaglebone:~$ cat /sys/devices/ocp.3/helper.15/AIN1
196
debian@beaglebone:~$ cat /sys/devices/ocp.3/helper.15/AIN1
1367
debian@beaglebone:~$ cat /sys/devices/ocp.3/helper.15/AIN1
1760
debian@beaglebone:~$ cat /sys/devices/ocp.3/helper.15/AIN1
1750
debian@beaglebone:~$ cat /sys/devices/ocp.3/helper.15/AIN1
195

================================================================================

GETTING DOWN TO ACTUALLY WRITING AN LCD DRIVER

https://learn.adafruit.com/character-lcd-with-raspberry-pi-or-beaglebone-black
See the above for the wiring.
I used those instructions to connect an LCD w/ a tri-color backlight to my BBB.
I've tested the LCD with the Adafruit code, and it works fine.
I was able to modify my TriColor_LED code to work the backlight on the LCD.
I also learned that there is some funny behavior with PWM periods due to a kernel bug.
If you're using two PWM pins that are governed by the same controller on the board then you will not be able to change the period on either of them. This is due to a locking condition involving the controller.
There's a resolution that involves recombiling the BBB Linux PWM driver.
A workaround involves unloading the DTO fragment of the blocking pin so you can change the period on the other.
The smart thing to do is work with the default period of 500000. If you want to be really clever, then when you try to change the period or duty values, you'd read in the existing period, and scale the duty to match, storing a "virtual" period in a member variable to take into account. It'd be a nifty way to seemingly correct the issue for most practacle purposes.

For my LCD driver, I'm finding that the PWM pins on the P9 header won't let me change the period value because they're being managed by the same controller on the BBB. No matter what I do, the period is stuck at the default value of 500000. I'll just use that. I've found that I can control the color of the backlight without an issue when I do.

The process for controlling an LCD is pretty simple.
The four to eight data lines are used to communicate a nibble or byte of information to the LCD. The RS line is used to indicate if the data lines are sending information relating to a command or data. The enable line is used as a clock to tell the LCD when it may read the values that have been set on the other five to nine lines. If I'm wanting to be able to control the backlight then I'll need to use PWM channels to essentially control the brightness of an LED. One very important item of consideration is the timing of events. The LCD needs to be provided a minimum amount of time to respond to certain events. It's also possible to perform some read operations on the LCD, but these are not important.

				PIN CHART
LCD Pin	|		Name				|	BBB Pin		|
--------+---------------------------+---------------+
	1	|	VSS (Ground)			|	P8_02 DGND	|
	2	|	VDD (+ve)				|	P9_07 VDD_5V|
	3	|	VE (Contrast Voltage)	|	POT CTR PIN	|
	4	|	Register Select			|	P8_08		|
	5	|	Read/Write (r/w)		|	P8_02 DGND	|
	6	|	Enable (EN)				|	P8_10		|
	7	|	Data 0					|	-----		|
	8	|	Data 1					|	-----		|
	9	|	Data 2					|	-----		|
	10	|	Data 3					|	-----		|
	11	|	Data 4					|	P8_18		|
	12	|	Data 5					|	P8_16		|
	13	|	Data 6					|	P8_14		|
	14	|	Data 7					|	P8_12		|
	15	|	Backlight Anode (+ve)	|	P9_07 VDD_5V|
	16	|	Red Cathode (-R/red)	|	P9_16		| *Only cathode on monochrome LCD
	17	|	Green Cathode (-G/green)|	P9_14		| *Only on RGB backlit LCD
	18	|	Blue Cathode (-B/blue)	|	P8_13		| *Only on RGB backlit LCD
	
	NOTE: The potentiometer needs to have one outter pin connected to P9_07 VDD_5V, and the other connected to P8_02 DGND. The potentiometer controls the contrast for the LCD characters. The RGB cathodes are controlled with PWM, and govern the color and brightness of the backlight. The BBB does have enough GPIO pins available to accomidate LCD pins 7-10, but this would just allow me to halve the number of write operations required to control the device. This is something I could do in order to allow me to code for it, but isn't required with compatable LCD modules. I'm coding for HD44780 compatible LCD modules. I have three I can play with, and an I2C breakout board as well. I have one monochrome 16x2 LCD, one 16x2 LCD with tri-color backlight, and one monochrome 20x4 LCD. I'm wanting to write a driver that will support all three devices. All three devices support the same protocol. Everybody else (Adafruit, who else?) has written their code to support an 8 bit, and 4 bit parralel bus. The I2C backback does use all the LCD pins, but that's a totally different story. 
