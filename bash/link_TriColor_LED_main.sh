#!/bin/bash

echo "Building LED main..."
./bash/compile_BBIO_Lite.sh
g++ -o obj/TriColor_LED.o -c src/TriColor_LED/TriColor_LED.cpp
g++ -o obj/TriColor_LED_CLI_UI_main.o -c src/TriColor_LED/TriColor_LED_CLI_UI_main.cpp
g++ -o bin/TriColor_LED obj/BBIO_Lite* obj/TriColor_LED* -lpthread
echo "Done Building LED main"
