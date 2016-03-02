#!/bin/bash

echo "Building LED main..."
./bash/compile_BBIO_Lite.sh
g++ -o obj/LED.o -c src/LED/LED.cpp
g++ -o obj/LED_CLI_UI_main.o -c src/LED/LED_CLI_UI_main.cpp
g++ -o bin/LED obj/BBIO_Lite* obj/LED* -lpthread
echo "Done Building LED main"
