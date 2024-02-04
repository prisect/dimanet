// --------------------------------------------------
// This file contains a basic implementation of a
// neural network in C.
//                      ~by @dimaaac
//
// This neural network is a feedforward network with
// a single hidden layer using a sigmoid activation
// function. This example is intended for educational
// purposes and serves as a minimal illustration of a
// neural network structure in vannila C.
//
// (got a no third party libraries achivement here!)
// --------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define INPUT_SIZE 2
#define HIDDEN_SIZE 3 // Change this to increase/decrease the hidden layer
#define OUTPUT_SIZE 1

// Sigmoid activation function
double sigmoid(double x) {
    return 1 / (1 + exp(-x));
}

// Forward pass of the neural network
void forward(double input[INPUT_SIZE], double weights_ih[INPUT_SIZE][HIDDEN_SIZE], double bias_h[HIDDEN_SIZE],
             double weights_ho[HIDDEN_SIZE][OUTPUT_SIZE], double bias_o[OUTPUT_SIZE], double output[OUTPUT_SIZE]) {
    // Hidden layer
    double hidden[HIDDEN_SIZE];
    for (int i = 0; i < HIDDEN_SIZE; i++) {
        hidden[i] = 0;
        for (int j = 0; j < INPUT_SIZE; j++) {
            hidden[i] += input[j] * weights_ih[j][i];
        }
        hidden[i] += bias_h[i];
        hidden[i] = sigmoid(hidden[i]);
    }

    // Output layer
    for (int i = 0; i < OUTPUT_SIZE; i++) {
        output[i] = 0;
        for (int j = 0; j < HIDDEN_SIZE; j++) {
            output[i] += hidden[j] * weights_ho[j][i];
        }
        output[i] += bias_o[i];
        output[i] = sigmoid(output[i]);
    }
}

int main() {
    // Initialize input
    double input[INPUT_SIZE] = {0.5, 0.7};
    double weights_ih[INPUT_SIZE][HIDDEN_SIZE] = {{0.1, 0.4, 0.7},
                                                  {0.2, 0.5, 0.8}};
    double bias_h[HIDDEN_SIZE] = {0.3, 0.6, 0.9};
    double weights_ho[HIDDEN_SIZE][OUTPUT_SIZE] = {{0.2},
                                                   {0.5},
                                                   {0.8}};
    double bias_o[OUTPUT_SIZE] = {0.4};
    double output[OUTPUT_SIZE]; // Initialize output

    // Perform forward pass
    forward(input, weights_ih, bias_h, weights_ho, bias_o, output);

    printf("Output: %lf\n", output[0]);

    return 0;
}
