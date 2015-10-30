#!/bin/bash

echo "Build script running!"

echo "Deleting previous build."
rm bin/*
rm obj/*

echo "Building objects."
# I need a line like this for each file.
#g++ -o build/I2C_IO.o -c src/I2C_IO.cpp
g++ -o obj/I2C_IO.o -c src/I2C_IO.cpp
g++ -o obj/LSM9DS0.o -c src/LSM9DS0.cpp
g++ -o obj/CLI_UI_main.o -c src/CLI_UI_main.cpp

echo "Linking."
g++ -o bin/LSM9DS0_CLI obj/*

echo "Build script done."
