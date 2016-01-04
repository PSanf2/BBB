#!/bin/bash

echo "Building TCA9548A main"
g++ -o obj/I2C_IO_Singleton.o -c src/lib/I2C_IO_Singleton.cpp
g++ -o obj/TCA9548A.o -c src/TCA9548A/TCA9548A.cpp
g++ -o obj/TCA9548A_CLI_UI_main.o -c src/TCA9548A/TCA9548A_CLI_UI_main.cpp
g++ -o bin/TCA9548A obj/I2C_IO_Singleton.o obj/TCA9548A*
echo "Done"
