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

CONTROLLING A PWM OUTPIT AS ROOT FROM THE TERMINAL
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
