#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "dimanet.h"

#define MAX_ITERATIONS 200000
#define TARGET_ERROR 0.01
#define RESTART_EVERY 1000

static const double inputs[4][2] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
static const double expected[4] = {0, 1, 1, 0};

/* Sum of squared errors over the four XOR patterns. */
static double total_error(dimanet *ann) {
    double err = 0.0;
    for (int i = 0; i < 4; ++i) {
        const double d = *dimanet_run(ann, inputs[i]) - expected[i];
        err += d * d;
    }
    return err;
}

int main(void) {
    printf("DimaNet example 2: learn the XOR function with random search.\n");

    /* Random search perturbs every weight and keeps any change that reduces
     * the error. It is much slower than backpropagation but requires no
     * gradient information at all. */
    dimanet_seed((uint32_t)time(NULL));
    srand((unsigned)time(NULL));

    dimanet *ann = dimanet_init(2, 1, 2, 1);
    if (!ann) {
        fprintf(stderr, "failed to allocate the network\n");
        return 1;
    }

    double best_error = total_error(ann);
    int iterations = 0;

    while (iterations < MAX_ITERATIONS && best_error > TARGET_ERROR) {
        ++iterations;

        /* If we haven't improved in a while, restart with fresh weights. */
        if (iterations % RESTART_EVERY == 0) {
            dimanet_randomize(ann);
            best_error = total_error(ann);
        }

        dimanet *candidate = dimanet_copy(ann);
        for (int i = 0; i < candidate->total_weights; ++i)
            candidate->weight[i] += (double)rand() / RAND_MAX - 0.5;

        const double error = total_error(candidate);
        if (error < best_error) {
            best_error = error;
            dimanet_free(ann);
            ann = candidate;
        } else {
            dimanet_free(candidate);
        }
    }

    printf("Finished after %d iterations (error %.4f).\n", iterations,
           best_error);
    for (int i = 0; i < 4; ++i) {
        const double out = *dimanet_run(ann, inputs[i]);
        printf("xor(%d, %d) = %.3f  (expected %d)\n",
               (int)inputs[i][0], (int)inputs[i][1], out,
               (int)expected[i]);
    }

    dimanet_free(ann);
    return 0;
}
