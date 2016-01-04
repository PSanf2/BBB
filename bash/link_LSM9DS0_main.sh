#!/bin/bash

echo "Building LSM9DS0 main"
g++ -o obj/I2C_IO_Singleton.o -c src/lib/I2C_IO_Singleton.cpp
g++ -o obj/LSM9DS0.o -c src/LSM9DS0/LSM9DS0.cpp
g++ -o obj/LSM9DS0_CLI_UI_main.o -c src/LSM9DS0/LSM9DS0_CLI_UI_main.cpp
g++ -o bin/LSM9DS0 obj/I2C_IO_Singleton.o obj/LSM9DS0*
echo "Done"
