#!/bin/bash

echo "Building LightSensor main..."
./bash/compile_BBIO_Lite.sh
g++ -o obj/Button.o -c src/LightSensor/Button.cpp
g++ -o obj/LightSensor.o -c src/LightSensor/LightSensor.cpp
g++ -o obj/LightSensor_CLI_UI_main.o -c src/LightSensor/LightSensor_CLI_UI_main.cpp
g++ -o bin/LightSensor obj/BBIO_Lite* obj/LightSensor* obj/Button* -lpthread
echo "Done Building LED main"
