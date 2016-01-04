#!/bin/bash

echo "Building PCA9685 main"
g++ -o obj/I2C_IO_Singleton.o -c src/lib/I2C_IO_Singleton.cpp
g++ -o obj/PCA9685.o -c src/PCA9685/PCA9685.cpp
g++ -o obj/PCA9685_CLI_UI_main.o -c src/PCA9685/PCA9685_CLI_UI_main.cpp
g++ -o bin/PCA9685 obj/I2C_IO_Singleton.o obj/PCA9685*
echo "Done"
