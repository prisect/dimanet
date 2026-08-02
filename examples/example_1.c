#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include "dimanet.h"

#define EPOCHS 500
#define LEARNING_RATE 1.0
#define MAX_ATTEMPTS 8

static const double inputs[4][2] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
static const double expected[4] = {0, 1, 1, 0};

/* Returns 1 if the network classifies all four XOR patterns correctly. */
static int solved(dimanet *ann) {
    for (int i = 0; i < 4; ++i) {
        const double out = *dimanet_run(ann, inputs[i]);
        if ((out > 0.5) != (expected[i] > 0.5))
            return 0;
    }
    return 1;
}

int main(void) {
    printf("DimaNet example 1: learn the XOR function with backpropagation.\n");

    dimanet_seed((uint32_t)time(NULL));

    /* A 2-2-1 network with tanh activations. XOR is a hard problem for a
     * small network and plain SGD, so if a run stalls we restart with fresh
     * random weights (a standard trick in practice). */
    dimanet *ann = NULL;
    for (int attempt = 0; attempt < MAX_ATTEMPTS; ++attempt) {
        ann = dimanet_init(2, 1, 2, 1);
        if (!ann) {
            fprintf(stderr, "failed to allocate the network\n");
            return 1;
        }
        dimanet_set_activation_functions(ann, dimanet_act_tanh,
                                         dimanet_act_tanh);

        for (int e = 0; e < EPOCHS; ++e)
            for (int i = 0; i < 4; ++i)
                dimanet_train(ann, inputs[i], expected + i, LEARNING_RATE);

        if (solved(ann))
            break;

        if (attempt + 1 < MAX_ATTEMPTS) {
            dimanet_free(ann);
            ann = NULL;
        }
    }

    for (int i = 0; i < 4; ++i) {
        const double out = *dimanet_run(ann, inputs[i]);
        printf("xor(%d, %d) = %.3f  (expected %d)\n",
               (int)inputs[i][0], (int)inputs[i][1], out,
               (int)expected[i]);
    }

    dimanet_free(ann);
    return 0;
}
