# this script is made to launch default
# example scripts from the source code
#
# i wont call this a framework tho
# lmfao

#!/bin/bash

echo "DimaNet Example Launcher"
echo ""
echo "Select a C example to run:"
echo " 1 - ex1::XOR Function using Backpropagation training"
echo " 2 - ex2::XOR Function using Random Search training"
echo " 3 - ex3::Loads and runs an ANN from a file"
echo " 4 - ex4::IRIS data-set using Backpropagation training"
echo " 5 - ex5::Visualization of ANN Approximation"
echo ""
echo "WARNING: RUN `MAKE` BEFORE RUNNING THE SCRIPT!"

while true; do
    read -p "1-5: -$ " choice

    case $choice in
        1) ./examples/example_1.o && break ;;
        2) ./examples/example_2.o && break ;;
        3) ./examples/example_3.o && break ;;
        4) ./examples/example_4.o && break ;;
        5) ./examples/example_5.o && break ;;
        *)
            echo "Invalid choice. Enter a number between 1 and 5."
            continue
            ;;
    esac
done
