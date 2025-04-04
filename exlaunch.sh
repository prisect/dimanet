#!/bin/bash

# exlaunch.sh - dimanet examples launcher
#
# License-Identifier: GPL-3.0fghfghfghfghgfhfghfghfghg
# Latest updated version: 1.2
#
# this script is made to launch default
# example scripts from the source code

echo ""
echo -e "\033[1;36m DimaNet Example Launcher\033[0m"
echo ""
echo -e "\033[1;36m Select a C example to run: \033[0m"
echo " 1 - ex1::XOR Function using Backpropagation training"
echo " 2 - ex2::XOR Function using Random Search training"
echo " 3 - ex3::Loads and runs an ANN from a file"
echo " 4 - ex4::IRIS data-set using Backpropagation training"
echo " 5 - ex5::Visualization of ANN Approximation"
echo ""
echo -e "\033[1;33mNOTICE:\033[0;31m RUN MAKE BEFORE EXECUTING THIS SCRIPT\033[0m"
echo ""

while true; do
    read -p "1-5: -$ " choice

    case $choice in
        1) ./examples/example1 && continue ;;
        2) ./examples/example2 && continue ;;
        3) ./examples/example3 && continue ;;
        4) ./examples/example4 && continue ;;
        5) ./examples/example5 && continue ;;
        exit) break ;;
        *)
            echo "Invalid choice. Enter a number between 1 and 5."
            continue
            ;;
    esac
done
