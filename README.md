**I MAKE NO GUARANTEES OR WARRANTIES THAT THIS CODE WILL WORK FOR YOU! USE THIS CODE AT YOUR OWN RISK! USING THIS CODE MEANS THAT YOU WILL NOT HOLD THE AUTHOR RESPONSIBLE FOR ANY DAMAGE THAT MAY BE CAUSED TO ANY EQUIPMENT, PERSONS, OR INFORMATION.**

Author: Patrick Sanford<br>
Email: PatrickSanford@live.com<br>
Date: Dec 14, 2015

This repository is for my attempts to write drivers for the Beagle Bone Black to allow the microcontroller to interact with breakout boards sold by Adafruit.

You may need to make obj and bin directories.

All of the files are named *_Singleton.[h/cpp], but this is a styling convention. The only real singleton in the system is the I2C_IO_Singleton. All of the other classes rely on this. You can create multiple objects for the remaining classes, but I wouldn't advise it unless you're dealing with multiple real physical boards. When you instantiate an object you'll need to provide the I2C bus, and device address (if it's selectable). Don't connect multiple devices with the same address to the same bus or you'll have a bad time. By default the BBB only has one I2C bus available to the user. There is a bus that's used by the HDMI output that you don't want to mess with unless you're willing to disable the video.

On the BeagleBoneBlack P9 header you can use four pins to communicate and power I2C breakout boards. My code is written assuming the boards are being powered by 5v.

P9, 01 => DGND<br>
P9, 07 => SYS_5V<br>
P9, 19 => I2C2_SCL<br>
P9, 20 => I2C2_SDA<br>

I have used these pin configurations to succesfully test my code using PCA9685, LSM9DS0, ADS1015, and ADS1115 breakout boards. Your results may vary. I have been able to test this code with multiple breakout boards connected to the same I2C bus successfully.

References and Credits:

Derek Molloy<br>
	* Exploring BeagleBone (http://www.exploringbeaglebone.com/)

Bill Earl<br>
	* Adafruit 16-Channel Servo Drive with Arduino (https://learn.adafruit.com/16-channel-pwm-servo-driver)<br>
	* Adafruit 4-Channel ADC Breakouts (https://learn.adafruit.com/adafruit-4-channel-adc-breakouts)

Kevin Townsend<br>
	* Adafruit LSM9DS0 Accelerometer + Gyro + Magnetometer 9-DOF Breakouts (https://learn.adafruit.com/adafruit-lsm9ds0-accelerometer-gyro-magnetometer-9-dof-breakouts)
