#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "dimanet.h"

#define NUM_SAMPLES 100
#define EPOCHS 20000
#define LEARNING_RATE 0.01

/* Function to approximate: y = sin(x) + cos(2x) */
static double target_function(double x) {
    return sin(x) + cos(2 * x);
}

static void generate_data(double *inputs, double *targets, int n) {
    for (int i = 0; i < n; ++i) {
        const double x = ((double)rand() / RAND_MAX) * 10.0;
        inputs[i] = x;
        targets[i] = target_function(x);
    }
}

static void visualize_results(dimanet *ann) {
    printf(" x    |  target  |  predicted\n");
    printf("------+----------+-----------\n");
    for (double x = 0.0; x <= 10.0; x += 0.5) {
        const double predicted = *dimanet_run(ann, &x);
        printf("%4.1f  |  %6.3f   |  %6.3f\n", x, target_function(x),
               predicted);
    }
}

int main(void) {
    printf("DimaNet example 5: approximate sin(x) + cos(2x).\n");

    dimanet_seed((uint32_t)time(NULL));
    srand((unsigned)time(NULL));

    /* tanh hidden units with a linear output layer; a natural choice for
     * real-valued regression (the target ranges over [-1.5, 2]). */
    dimanet *ann = dimanet_init(1, 1, 10, 1);
    if (!ann) {
        fprintf(stderr, "failed to allocate the network\n");
        return 1;
    }
    dimanet_set_activation_functions(ann, dimanet_act_tanh,
                                     dimanet_act_linear);
    dimanet_init_lecun(ann);

    double inputs[NUM_SAMPLES];
    double targets[NUM_SAMPLES];
    generate_data(inputs, targets, NUM_SAMPLES);

    for (int e = 0; e < EPOCHS; ++e)
        for (int i = 0; i < NUM_SAMPLES; ++i)
            dimanet_train(ann, &inputs[i], &targets[i], LEARNING_RATE);

    visualize_results(ann);

    dimanet_free(ann);
    return 0;
}
