#!/bin/bash

echo "Building ADS1X15 main"
g++ -o obj/I2C_IO_Singleton.o -c src/lib/I2C_IO_Singleton.cpp
g++ -o obj/ADS1X15.o -c src/ADS1X15/ADS1X15.cpp
g++ -o obj/ADS1X15_CLI_UI_main.o -c src/ADS1X15/ADS1X15_CLI_UI_main.cpp
g++ -o bin/ADS1X15 obj/I2C_IO_Singleton.o obj/ADS1X15*
echo "Done"
