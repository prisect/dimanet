#include "dimanet.h"
#include "libdntest.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

void basic() {
    dimanet *ann = dimanet_init(1, 0, 0, 1);

    lequal(ann->total_weights, 2);
    double a;
  
    a = 0;
    ann->weight[0] = 0;
    ann->weight[1] = 0;
    lfequal(0.5, *dimanet_run(ann, &a));

    a = 1;
    lfequal(0.5, *dimanet_run(ann, &a));

    a = 11;
    lfequal(0.5, *dimanet_run(ann, &a));

    a = 1;
    ann->weight[0] = 1;
    ann->weight[1] = 1;
    lfequal(0.5, *dimanet_run(ann, &a));

    a = 10;
    ann->weight[0] = 1;
    ann->weight[1] = 1;
    lfequal(1.0, *dimanet_run(ann, &a));

    a = -10;
    lfequal(0.0, *dimanet_run(ann, &a));

    dimanet_free(ann);
}

void xor() {
    dimanet *ann = dimanet_init(2, 1, 2, 1);
    ann->activation_hidden = dimanet_act_threshold;
    ann->activation_output = dimanet_act_threshold;

    lequal(ann->total_weights, 9);

    /* First hidden. */
    ann->weight[0] = .5;
    ann->weight[1] = 1;
    ann->weight[2] = 1;

    /* Second hidden. */
    ann->weight[3] = 1;
    ann->weight[4] = 1;
    ann->weight[5] = 1;

    /* Output. */
    ann->weight[6] = .5;
    ann->weight[7] = 1;
    ann->weight[8] = -1;


    double input[4][2] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
    double output[4] = {0, 1, 1, 0};

    lfequal(output[0], *dimanet_run(ann, input[0]));
    lfequal(output[1], *dimanet_run(ann, input[1]));
    lfequal(output[2], *dimanet_run(ann, input[2]));
    lfequal(output[3], *dimanet_run(ann, input[3]));

    dimanet_free(ann);
}

void backprop() {
    dimanet *ann = dimanet_init(1, 0, 0, 1);

    double input, output;
    input = .5;
    output = 1;

    double first_try = *dimanet_run(ann, &input);
    dimanet_train(ann, &input, &output, .5);
    double second_try = *dimanet_run(ann, &input);
    lok(fabs(first_try - output) > fabs(second_try - output));

    dimanet_free(ann);
}

void train_and() {
    double input[4][2] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
    double output[4] = {0, 0, 0, 1};

    dimanet *ann = dimanet_init(2, 0, 0, 1);

    int i, j;

    for (i = 0; i < 50; ++i) {
        for (j = 0; j < 4; ++j) {
            dimanet_train(ann, input[j], output + j, .8);
        }
    }

    ann->activation_output = dimanet_act_threshold;
    lfequal(output[0], *dimanet_run(ann, input[0]));
    lfequal(output[1], *dimanet_run(ann, input[1]));
    lfequal(output[2], *dimanet_run(ann, input[2]));
    lfequal(output[3], *dimanet_run(ann, input[3]));

    dimanet_free(ann);
}

void train_or() {
    double input[4][2] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
    double output[4] = {0, 1, 1, 1};

    dimanet *ann = dimanet_init(2, 0, 0, 1);
    dimanet_randomize(ann);

    int i, j;

    for (i = 0; i < 50; ++i) {
        for (j = 0; j < 4; ++j) {
            dimanet_train(ann, input[j], output + j, .8);
        }
    }

    ann->activation_output = dimanet_act_threshold;
    lfequal(output[0], *dimanet_run(ann, input[0]));
    lfequal(output[1], *dimanet_run(ann, input[1]));
    lfequal(output[2], *dimanet_run(ann, input[2]));
    lfequal(output[3], *dimanet_run(ann, input[3]));

    dimanet_free(ann);
}

void train_xor() {
    double input[4][2] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
    double output[4] = {0, 1, 1, 0};

    dimanet *ann = dimanet_init(2, 1, 2, 1);

    int i, j;

    for (i = 0; i < 500; ++i) {
        for (j = 0; j < 4; ++j) {
            dimanet_train(ann, input[j], output + j, 3);
        }
        /* printf("%1.2f ", xor_score(ann)); */
    }

    ann->activation_output = dimanet_act_threshold;
    lfequal(output[0], *dimanet_run(ann, input[0]));
    lfequal(output[1], *dimanet_run(ann, input[1]));
    lfequal(output[2], *dimanet_run(ann, input[2]));
    lfequal(output[3], *dimanet_run(ann, input[3]));

    dimanet_free(ann);
}

void persist() {
    dimanet *first = dimanet_init(1000, 5, 50, 10);

    FILE *out = fopen("persist.txt", "w");
    dimanet_write(first, out);
    fclose(out);


    FILE *in = fopen("persist.txt", "r");
    dimanet *second = dimanet_read(in);
    fclose(in);

    lequal(first->inputs, second->inputs);
    lequal(first->hidden_layers, second->hidden_layers);
    lequal(first->hidden, second->hidden);
    lequal(first->outputs, second->outputs);
    lequal(first->total_weights, second->total_weights);

    int i;
    for (i = 0; i < first->total_weights; ++i) {
        lok(first->weight[i] == second->weight[i]);
    }

    dimanet_free(first);
    dimanet_free(second);
}

void copy() {
    dimanet *first = dimanet_init(1000, 5, 50, 10);
    dimanet *second = dimanet_copy(first);

    lequal(first->inputs, second->inputs);
    lequal(first->hidden_layers, second->hidden_layers);
    lequal(first->hidden, second->hidden);
    lequal(first->outputs, second->outputs);
    lequal(first->total_weights, second->total_weights);

    int i;
    for (i = 0; i < first->total_weights; ++i) {
        lfequal(first->weight[i], second->weight[i]);
    }

    dimanet_free(first);
    dimanet_free(second);
}

void sigmoid() {
    double i = -20;
    const double max = 20;
    const double d = .0001;

    while (i < max) {
        lfequal(dimanet_act_sigmoid(NULL, i), dimanet_act_sigmoid_cached(NULL, i));
        i += d;
    }
}

int main(int argc, char *argv[])
{
    printf("DimaTest TEST SUITE\n");

    srand(100); //Repeatable test results.

    lrun("basic", basic);
    lrun("xor", xor);
    lrun("backprop", backprop);
    lrun("train and", train_and);
    lrun("train or", train_or);
    lrun("train xor", train_xor);
    lrun("persist", persist);
    lrun("copy", copy);
    lrun("sigmoid", sigmoid);

    lresults();

    return lfails != 0;
}
