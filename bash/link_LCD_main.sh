#!/bin/bash

echo "Building LCD main..."
./bash/compile_BBIO_Lite.sh
g++ -o obj/LCD.o -c src/LCD/LCD.cpp
g++ -o obj/LCD_CLI_UI_main.o -c src/LCD/LCD_CLI_UI_main.cpp
g++ -o bin/LCD obj/BBIO_Lite* obj/LCD* -lpthread
echo "Done Building LCD main"
