#!/bin/bash

echo "Building PCA9685 with CLI."

sh ./bash/build_lib.sh

g++ -o obj/PWM_Board.o -c src/PCA9685/PWM_Board.cpp
g++ -o obj/PCA9685_CLI_UI_main.o -c src/PCA9685/PCA9685_CLI_UI_main.cpp
