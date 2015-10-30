#!/bin/bash

echo "Build script running!"
echo "Deleting previous build."
rm bin/*
rm build/*
echo "Building objects."
g++ -o build/CLI_UI_main.o -c src/CLI_UI_main.cpp
g++ -o build/I2C_IO.o -c src/I2C_IO.cpp
g++ -o build/PWM_Board.o -c src/PWM_Board.cpp
echo "Linking."
g++ -o bin/PatricksProgram build/*
echo "Build script done."
