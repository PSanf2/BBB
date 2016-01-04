**I MAKE NO GUARANTEES OR WARRANTIES THAT THIS CODE WILL WORK FOR YOU! USE THIS CODE AT YOUR OWN RISK! USING THIS CODE MEANS THAT YOU WILL NOT HOLD THE AUTHOR RESPONSIBLE FOR ANY DAMAGE THAT MAY BE CAUSED TO ANY EQUIPMENT, PERSONS, OR INFORMATION.**

Author: Patrick Sanford<br>
Email: PatrickSanford@live.com<br>
Date: Jan 3, 2016

This repository is for my attempts to write I2C drivers for the BeagleBone Black. On the BeagleBoneBlack P9 header you can use four pins to power communicate with I2C devices. My code is written assuming the boards are being powered by 5v. Different I2C devices use differnt logic voltage levels, and you must consult with manufacturer provided documentation to determine the proper voltage to use with your device. When connecting an I2C device to the BeagleBone Black you should use pull-up resistors on the SCL and SDA lines. The I2C specification states that I2C devices may pull the lines to low, but not to high. I've had success using pull-up resistor values of 1k and 2.2k for this purpose. You must consult with the manufacturer provided documentation to determine the proper pull-up resistor values to use for a specific device. My code uses a common singleton object along with the "everything is a file" concept to read and write data via an I2C bus to a variety of devices. My I2C devices have primaily been sourced from Adafruit Industries in the form of breakout boards. My documentation is patchy, and incomplete. Where possible I've attempted to include links to relevant information in another file.

P9, 01 => DGND<br>
P9, 07 => SYS_5V<br>
P9, 19 => I2C2_SCL<br>
P9, 20 => I2C2_SDA<br>

I have used these pin configurations to succesfully test my code using PCA9685, LSM9DS0, ADS1015, ADS1115, and TCS9548A breakout boards from Adafruit. I have also tested my code agaist a DS1307 chip on a breadboard. Your results may vary. I have been able to test this code with multiple breakout boards connected to the same I2C bus successfully.<br>
