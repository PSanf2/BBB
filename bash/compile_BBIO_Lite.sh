#!/bin/bash

echo "Compiling BBIO Lite library..."
g++ -o obj/BBIO_Lite_Common.o -c src/BBIO_Lite/Common.cpp
g++ -o obj/BBIO_Lite_GPIO.o -c src/BBIO_Lite/GPIO.cpp
g++ -o obj/BBIO_Lite_PWM.o -c src/BBIO_Lite/PWM.cpp
echo "Done Compiling BBIO Lite"
