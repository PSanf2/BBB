#!/bin/bash

echo "Building Button main..."
./bash/compile_BBIO_Lite.sh
g++ -o obj/LED.o -c src/Button_LED/LED.cpp
g++ -o obj/Button.o -c src/Button_LED/Button.cpp
g++ -o obj/Button_LED_CLI_UI_main.o -c src/Button_LED/Button_LED_CLI_UI_main.cpp
g++ -o bin/Button_LED obj/BBIO_Lite* obj/LED.o obj/Button* -lpthread
echo "Done Building LED main"
