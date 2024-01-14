#include "dimanet.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h> // Include this header for RAND_MAX

#define NUM_SAMPLES 100
#define LEARNING_RATE 0.01
#define EPOCHS 10000

// Function to approximate: y = sin(x) + cos(2x)
double target_function(double x) { return sin(x) + cos(2 * x); }

// Function to generate training data
void generate_data(double *inputs, double *targets, int num_samples) {
  for (int i = 0; i < num_samples; ++i) {
    double x = ((double)rand() / RAND_MAX) * 10.0; // Random x in [0, 10]
    inputs[i] = x;
    targets[i] = target_function(x);
  }
}

// Function to visualize the results
void visualize_results(dimanet *ann) {
  printf("DimaNet example 5.\n");
  printf("Visualization of Neural Network Approximation:\n");
  printf(" x.x  |  Target  | Predicted\n");
  printf("----------------------------\n");

  for (double x = 0.0; x <= 10.0; x += 0.5) {
    double predicted = *dimanet_run(ann, &x);
    double target = target_function(x);
    printf("%4.1f  |  %6.3f  |  %6.3f\n", x, target, predicted);
  }
}

int main() {
  // Initialize neural network with one hidden layer
  dimanet *ann = dimanet_init(1, 1, 5, 1);

  if (!ann) {
    fprintf(stderr, "Error: Unable to initialize the neural network.\n");
    return 1;
  }

  // Generate training data
  double inputs[NUM_SAMPLES];
  double targets[NUM_SAMPLES];
  generate_data(inputs, targets, NUM_SAMPLES);

  // Train the neural network
  for (int epoch = 0; epoch < EPOCHS; ++epoch) {
    for (int i = 0; i < NUM_SAMPLES; ++i) {
      dimanet_train(ann, &inputs[i], &targets[i], LEARNING_RATE);
    }
  }

  // Visualize the results
  visualize_results(ann);

  // Free the neural network
  dimanet_free(ann);

  return 0;
}
