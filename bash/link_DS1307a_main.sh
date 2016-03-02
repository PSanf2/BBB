#!/bin/bash

echo "Building DS1307a main..."
./bash/compile_BBIO_Lite.sh
g++ -o obj/DS1307a.o -c src/DS1307a/DS1307a.cpp
g++ -o obj/DS1307a_CLI_UI_main.o -c src/DS1307a/DS1307a_CLI_UI_main.cpp
g++ -o bin/DS1307a obj/BBIO_Lite* obj/DS1307a* -lpthread
echo "Done Building DS1307a main"
