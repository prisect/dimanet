#include "dimanet.h"
#include "libdntest.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

/* Function prototypes for the new test functions */
void test_relu(void);
void test_tanh(void);
void test_leaky_relu(void);
void test_activation_setters(void);
void test_training_with_modern_activations(void);

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

/* New tests for modern activation functions */

void test_relu(void) {
    
    // Test ReLU function directly
    lfequal(0.0, dimanet_act_relu(NULL, -5.0));
    lfequal(0.0, dimanet_act_relu(NULL, -0.1));
    lfequal(0.0, dimanet_act_relu(NULL, 0.0));
    lfequal(0.1, dimanet_act_relu(NULL, 0.1));
    lfequal(5.0, dimanet_act_relu(NULL, 5.0));
    
    // Test with a simple network
    dimanet *ann = dimanet_init(1, 0, 0, 1);
    dimanet_set_activation_functions(ann, dimanet_act_relu, dimanet_act_relu);
    
    // Set weights to test positive and negative cases
    // Weight structure: [bias_weight, input_weight]
    // Network calculation: sum = bias_weight * -1 + input_weight * input
    ann->weight[0] = 1.0;  // bias weight (will be multiplied by -1)
    ann->weight[1] = 1.0;  // input weight
    
    double input;
    
    // Positive input: input = 2.0
    // Calculation: sum = 1.0 * -1 + 1.0 * 2.0 = -1.0 + 2.0 = 1.0
    // ReLU(1.0) = 1.0
    input = 2.0;
    double result = *dimanet_run(ann, &input);
    lfequal(1.0, result);
    
    // Negative input: input = -3.0  
    // Calculation: sum = 1.0 * -1 + 1.0 * -3.0 = -1.0 - 3.0 = -4.0
    // ReLU(-4.0) = 0.0
    input = -3.0;
    result = *dimanet_run(ann, &input);
    lfequal(0.0, result);
    
    dimanet_free(ann);
}

void test_tanh(void) {
    
    // Test tanh function directly
    lfequal(-1.0, dimanet_act_tanh(NULL, -10.0));
    lfequal(-0.7615941559557649, dimanet_act_tanh(NULL, -1.0));
    lfequal(0.0, dimanet_act_tanh(NULL, 0.0));
    lfequal(0.7615941559557649, dimanet_act_tanh(NULL, 1.0));
    lfequal(1.0, dimanet_act_tanh(NULL, 10.0));
    
    // Test with a simple network
    dimanet *ann = dimanet_init(1, 0, 0, 1);
    dimanet_set_activation_functions(ann, dimanet_act_tanh, dimanet_act_tanh);
    
    // Set specific weights for predictable output
    ann->weight[0] = 0.0;  // bias
    ann->weight[1] = 1.0;  // weight
    
    double input = 1.0;
    double result = *dimanet_run(ann, &input);
    lfequal(tanh(1.0), result);
    
    dimanet_free(ann);
}

void test_leaky_relu(void) {
    
    // Test Leaky ReLU function directly with default alpha (0.01)
    dimanet ann_default;
    ann_default.leaky_relu_alpha = 0.01;
    
    lfequal(-0.05, dimanet_act_leaky_relu(&ann_default, -5.0));
    lfequal(-0.001, dimanet_act_leaky_relu(&ann_default, -0.1));
    lfequal(0.0, dimanet_act_leaky_relu(&ann_default, 0.0));
    lfequal(0.1, dimanet_act_leaky_relu(&ann_default, 0.1));
    lfequal(5.0, dimanet_act_leaky_relu(&ann_default, 5.0));
    
    // Test with custom alpha
    dimanet ann_custom;
    ann_custom.leaky_relu_alpha = 0.1;
    
    lfequal(-0.5, dimanet_act_leaky_relu(&ann_custom, -5.0));
    lfequal(-0.01, dimanet_act_leaky_relu(&ann_custom, -0.1));
    
    // Test with a network
    dimanet *ann = dimanet_init(1, 0, 0, 1);
    dimanet_set_activation_functions(ann, dimanet_act_leaky_relu, dimanet_act_leaky_relu);
    dimanet_set_leaky_relu_alpha(ann, 0.1);
    
    ann->weight[0] = 1.0;  // bias
    ann->weight[1] = 1.0;  // weight
    
    double input = -3.0;
    double result = *dimanet_run(ann, &input);
    // Calculation: 1.0 * -1 + 1.0 * -3.0 = -4.0, LeakyReLU(-4.0) with alpha=0.1 = -0.4
    lfequal(-0.4, result);
    
    dimanet_free(ann);
}

void test_activation_setters(void) {
    
    dimanet *ann = dimanet_init(2, 1, 2, 1);
    
    // Test setting different combinations
    dimanet_set_activation_functions(ann, dimanet_act_relu, dimanet_act_linear);
    lok(ann->activation_hidden == dimanet_act_relu);
    lok(ann->activation_output == dimanet_act_linear);
    
    dimanet_set_activation_functions(ann, dimanet_act_tanh, dimanet_act_sigmoid);
    lok(ann->activation_hidden == dimanet_act_tanh);
    lok(ann->activation_output == dimanet_act_sigmoid);
    
    dimanet_set_activation_functions(ann, dimanet_act_leaky_relu, dimanet_act_tanh);
    lok(ann->activation_hidden == dimanet_act_leaky_relu);
    lok(ann->activation_output == dimanet_act_tanh);
    
    // Test Leaky ReLU alpha setting
    dimanet_set_leaky_relu_alpha(ann, 0.5);
    lfequal(0.5, ann->leaky_relu_alpha);
    
    dimanet_free(ann);
}

void test_training_with_modern_activations(void) {
    
    // Test training with ReLU
    dimanet *ann_relu = dimanet_init(2, 1, 3, 1);
    dimanet_set_activation_functions(ann_relu, dimanet_act_relu, dimanet_act_linear);
    
    double input[4][2] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
    double output[4] = {0, 1, 1, 0}; // XOR problem
    
    // Train for a few iterations
    for (int i = 0; i < 100; ++i) {
        for (int j = 0; j < 4; ++j) {
            dimanet_train(ann_relu, input[j], output + j, 0.1);
        }
    }
    
    // Network should still produce outputs (not NaN or infinity)
    for (int j = 0; j < 4; ++j) {
        double result = *dimanet_run(ann_relu, input[j]);
        lok(!isnan(result));
        lok(!isinf(result));
    }
    
    dimanet_free(ann_relu);
    
    // Test training with tanh
    dimanet *ann_tanh = dimanet_init(2, 1, 3, 1);
    dimanet_set_activation_functions(ann_tanh, dimanet_act_tanh, dimanet_act_tanh);
    
    // Train for a few iterations
    for (int i = 0; i < 100; ++i) {
        for (int j = 0; j < 4; ++j) {
            dimanet_train(ann_tanh, input[j], output + j, 0.1);
        }
    }
    
    // Outputs should be in tanh range [-1, 1]
    for (int j = 0; j < 4; ++j) {
        double result = *dimanet_run(ann_tanh, input[j]);
        lok(result >= -1.0 && result <= 1.0);
        lok(!isnan(result));
    }
    
    dimanet_free(ann_tanh);
}

int main(int argc, char *argv[])
{
    printf("- DIMANET TEST SUITE -\n");

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
    
    /* New tests for modern activation functions */
    lrun("relu", test_relu);
    lrun("tanh", test_tanh);
    lrun("leaky relu", test_leaky_relu);
    lrun("setters", test_activation_setters);
    lrun("activations", test_training_with_modern_activations);

    lresults();

    return lfails != 0;
}