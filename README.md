Author: Patrick Sanford

Date: Dec 2, 2015

This repository is for my attempts to write drivers for the Beagle
Bone Black to allow the microcontroller to interact with breakout boards
sold by Adafruit.

You may need to make obj and bin directories.

I MAKE NO GUARANTEES OR WARRANTIES THAT THIS CODE WILL WORK FOR YOU!
USE THIS CODE AT YOUR OWN RISK! USING THIS CODE MEANS THAT YOU WILL
NOT HOLD THE AUTHOR RESPONSIBLE FOR ANY DAMAGE THAT MAY BE CAUSED TO
ANY EQUIPMENT, PERSONS, OR INFORMATION.

On the BeagleBoneBlack P9 header you can use four pins to communicate
and power I2C breakout boards. My code is written assuming the boards
are being powered by 5v.

P9, 01 => DGND

P9, 07 => SYS_5V

P9, 19 => I2C2_SCL

P9, 20 => I2C2_SDA

I have used these pin configurations to succesfully test my code using
PCA9685 and LSM9DS0 breakout boards. Your results may vary.

References and Credits:

Derek Molloy - Exploring BeagleBone (http://www.exploringbeaglebone.com/)

Bill Earl - Adafruit 16-Channel Servo Drive with Arduino (https://learn.adafruit.com/16-channel-pwm-servo-driver)

Kevin Townsend - Adafruit LSM9DS0 Accelerometer + Gyro + Magnetometer 9-DOF Breakouts (https://learn.adafruit.com/adafruit-lsm9ds0-accelerometer-gyro-magnetometer-9-dof-breakouts)

