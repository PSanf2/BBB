#!/bin/bash

echo "Building Singleton"
g++ -o obj/I2C_IO_Singleton.o -c src/lib/I2C_IO_Singleton.cpp
g++ -o obj/LSM9DS0_Singleton.o -c src/LSM9DS0_Singleton/LSM9DS0_Singleton.cpp
g++ -o obj/Singleton_CLI_UI_main.o -c src/LSM9DS0_Singleton/LSM9DS0_Singleton_CLI_UI_main.cpp
g++ -o bin/Singleton_LSM9DS0_CLI obj/*
echo "Done"
