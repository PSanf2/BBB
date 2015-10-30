#!/bin/bash

echo "Building LSM9DS0 with CLI."

sh ./bash/build_lib.sh

g++ -o obj/LSM9DS0.o -c src/LSM9DS0/LSM9DS0.cpp
g++ -o obj/CLI_UI_main.o -c src/LSM9DS0/LSM9DS0_CLI_UI_main.cpp
