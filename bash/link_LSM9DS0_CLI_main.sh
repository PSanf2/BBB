#!/bin/bash

echo "Linking."
sh ./bash/build_LSM9DS0_CLI_main.sh
g++ -o bin/LSM9DS0_CLI obj/*
echo "Done linking."
