#!/bin/bash

echo "Building DS1307 main"
g++ -o obj/I2C_IO_Singleton.o -c src/lib/I2C_IO_Singleton.cpp
g++ -o obj/DS1307.o -c src/DS1307/DS1307.cpp
g++ -o obj/DS1307_CLI_UI_main.o -c src/DS1307/DS1307_CLI_UI_main.cpp
g++ -o bin/DS1307 obj/I2C_IO_Singleton.o obj/DS1307*
echo "Done"
