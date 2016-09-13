#!/bin/bash

echo "Building IR_LED_Sensor main..."
./bash/compile_BBIO_Lite.sh
g++ -o obj/IR_LED_Sensor.o -c src/IR_LED_Sensor/IR_LED_Sensor.cpp
g++ -o obj/IR_LED_Sensor_CLI_UI_main.o -c src/IR_LED_Sensor/IR_LED_Sensor_CLI_UI_main.cpp
g++ -o bin/IR_LED_Sensor obj/BBIO_Lite* obj/IR* -lpthread
echo "Done Building IR_Sensor main"
