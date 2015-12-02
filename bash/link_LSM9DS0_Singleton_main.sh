#!/bin/bash

echo "Building Singleton"
g++ -o obj/I2C_IO_Singleton.o -c src/lib/I2C_IO_Singleton.cpp
g++ -o obj/LSM9DS0_Singleton.o -c src/LSM9DS0_Singleton/LSM9DS0_Singleton.cpp
g++ -o obj/LSM9DS0_Singleton_CLI_UI_main.o -c src/LSM9DS0_Singleton/LSM9DS0_Singleton_CLI_UI_main.cpp
g++ -o bin/LSM9DS0_Singleton obj/I2C_IO_Singleton.o obj/LSM9DS0*
echo "Done"
