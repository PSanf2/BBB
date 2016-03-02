#!/bin/bash

echo "Compiling BBIO Lite library..."
g++ -o obj/BBIO_Lite_Common.o -c src/BBIO_Lite/Common.cpp
g++ -o obj/BBIO_Lite_GPIO.o -c src/BBIO_Lite/GPIO.cpp
g++ -o obj/BBIO_Lite_PWM.o -c src/BBIO_Lite/PWM.cpp
g++ -o obj/BBIO_Lite_I2C.o -c src/BBIO_Lite/I2C.cpp
g++ -o obj/BBIO_Lite_ADC.o -c src/BBIO_Lite/ADC.cpp
echo "Done Compiling BBIO Lite"
