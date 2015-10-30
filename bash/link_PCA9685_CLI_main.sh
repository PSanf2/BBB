#!/bin/bash

echo "Linking."
sh ./bash/build_PCA9685_CLI_main.sh
g++ -o bin/PCA9685_CLI obj/*
echo "Done linking."

