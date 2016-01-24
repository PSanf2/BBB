#!/bin/bash

echo "Building LED main"
g++ -o obj/GPIO_Singleton.o -c src/lib/GPIO_Singleton.cpp
g++ -o obj/LED.o -c src/LED/LED.cpp
g++ -o obj/LED_CLI_UI_main.o -c src/LED/LED_CLI_UI_main.cpp
g++ -o bin/LED obj/GPIO_Singleton.o obj/LED*
echo "Done"
