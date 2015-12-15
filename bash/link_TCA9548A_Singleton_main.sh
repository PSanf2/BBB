#!/bin/bash

echo "Building TCA9548A_Singleton main"
g++ -o obj/I2C_IO_Singleton.o -c src/lib/I2C_IO_Singleton.cpp
g++ -o obj/TCA9548A_Singleton.o -c src/TCA9548A_Singleton/TCA9548A_Singleton.cpp
g++ -o obj/TCA9548A_Singleton_CLI_UI_main.o -c src/TCA9548A_Singleton/TCA9548A_Singleton_CLI_UI_main.cpp
g++ -o bin/TCA9548A_Singleton obj/I2C_IO_Singleton.o obj/TCA9548A*
echo "Done"
