#!/bin/bash

echo "Building ADS1X15_Singleton main"
g++ -o obj/I2C_IO_Singleton.o -c src/lib/I2C_IO_Singleton.cpp
g++ -o obj/ADS1X15_Singleton.o -c src/ADS1X15_Singleton/ADS1X15_Singleton.cpp
g++ -o obj/ADS1X15_Singleton_CLI_UI_main.o -c src/ADS1X15_Singleton/ADS1X15_Singleton_CLI_UI_main.cpp
g++ -o bin/ADS1X15_Singleton obj/I2C_IO_Singleton.o obj/ADS1X15*
echo "Done"
