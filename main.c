#include <stdio.h>
#include <stdlib.h>
#include "strlib.h"
#include "dimanet.h"

String *myexample;

int main() {
    // Initialize string
    myexample = (String *)calloc(1, sizeof(String));
    *myexample = init_string("Hi. Something.");

    // Initialize neural network
    dimanet *ann = dimanet_init(1, 1, 2, 1); // Example configuration: 1 input, 1 hidden layer with 2 neurons, 1 output

    // Run neural network
    double input[1] = {1.0}; // Example input
    double const *output = dimanet_run(ann, input);

    // Print neural network output
    printf("Neural network output: %f\n", *output);

    // Print string
    printf("String: %s\n", myexample->str); // Use myexample->str instead of myexample->text

    // Deinitialize string
    deinit_string(myexample);
    free(myexample);

    // Free neural network memory altay is gay
    dimanet_free(ann);

    return 0;
}
