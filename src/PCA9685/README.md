This board is to be connected to the Beagle Bone Black in the following
manner:

P9_01 GND -> GND

P9_07 SYS_5V -> VCC

P9_19 I2C2_SCL -> SCL

P9_20 I2C2_SDA -> SDA

Apply appropriate external power for the servos as documented by Adafruit. 
Connect the servos as documented by Adafruit. 
From the terminal run i2cdetect -l to make sure the bus is available. 
From the terminal run i2cdetect -y -r 1 to verify the device address. 
From the repository main directory run ./bash/link_PCA9685_CLI_main.sh. 
From the repository main directory run ./bin/PCA9685_CLI. 


I've found that 75Mhz is a good frequency. 
You need to set the refresh rate before controlling the servos. 
You need to set the refresh rate after a reset. 
You need to wake the board up before anything will happen. 
The ON setting will almost always be 0, and the OFF setting will determing 
what position the servo will move to. 
Understand what you're doing with the servos before trying to use the program. 

I successfully tested this on Oct 30, 2015 with my Beagle Bone Black 
running Debian Image 2015-03-01. I've only tested this using one PWM 
board at this time.
