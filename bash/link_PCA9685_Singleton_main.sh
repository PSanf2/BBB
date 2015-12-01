#!/bin/bash

echo "Building PCA9685_Singleton main"
g++ -o obj/I2C_IO_Singleton.o -c src/lib/I2C_IO_Singleton.cpp
g++ -o obj/PCA9685_Singleton.o -c src/PCA9685_Singleton/PCA9685_Singleton.cpp
g++ -o obj/PCA9685_Singleton_CLI_UI_main.o -c src/PCA9685_Singleton/PCA9685_Singleton_CLI_UI_main.cpp
g++ -o bin/PCA9685_Singleton obj/*
echo "Done"
