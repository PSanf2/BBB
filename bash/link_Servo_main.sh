#!/bin/bash

echo "Building Servo main..."
./bash/compile_BBIO_Lite.sh
g++ -o obj/Servo.o -c src/Servo/Servo.cpp
g++ -o obj/Servo_CLI_UI_main.o -c src/Servo/Servo_CLI_UI_main.cpp
g++ -o bin/Servo obj/BBIO_Lite* obj/Servo* -lpthread
echo "Done Building Servo main"
