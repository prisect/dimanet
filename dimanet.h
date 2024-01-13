/*
 * dimanet - Minimal C Artificial Neural Network
 *
 * Copyright (c) 2015-2018 Lewis Van Winkle
 *
 * http://CodePlea.com
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgement in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 */


#ifndef dimanet_H
#define dimanet_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef dimanet_RANDOM
/* We use the following for uniform random numbers between 0 and 1.
 * If you have a better function, redefine this macro. */
#define dimanet_RANDOM() (((double)rand())/RAND_MAX)
#endif

struct dimanet;

typedef double (*dimanet_actfun)(const struct dimanet *ann, double a);

typedef struct dimanet {
    /* How many inputs, outputs, and hidden neurons. */
    int inputs, hidden_layers, hidden, outputs;

    /* Which activation function to use for hidden neurons. Default: gennann_act_sigmoid_cached*/
    dimanet_actfun activation_hidden;

    /* Which activation function to use for output. Default: gennann_act_sigmoid_cached*/
    dimanet_actfun activation_output;

    /* Total number of weights, and size of weights buffer. */
    int total_weights;

    /* Total number of neurons + inputs and size of output buffer. */
    int total_neurons;

    /* All weights (total_weights long). */
    double *weight;

    /* Stores input array and output of each neuron (total_neurons long). */
    double *output;

    /* Stores delta of each hidden and output neuron (total_neurons - inputs long). */
    double *delta;

} dimanet;

/* Creates and returns a new ann. */
dimanet *dimanet_init(int inputs, int hidden_layers, int hidden, int outputs);

/* Creates ANN from file saved with dimanet_write. */
dimanet *dimanet_read(FILE *in);

/* Sets weights randomly. Called by init. */
void dimanet_randomize(dimanet *ann);

/* Returns a new copy of ann. */
dimanet *dimanet_copy(dimanet const *ann);

/* Frees the memory used by an ann. */
void dimanet_free(dimanet *ann);

/* Runs the feedforward algorithm to calculate the ann's output. */
double const *dimanet_run(dimanet const *ann, double const *inputs);

/* Does a single backprop update. */
void dimanet_train(dimanet const *ann, double const *inputs, double const *desired_outputs, double learning_rate);

/* Saves the ann. */
void dimanet_write(dimanet const *ann, FILE *out);

void dimanet_init_sigmoid_lookup(const dimanet *ann);
double dimanet_act_sigmoid(const dimanet *ann, double a);
double dimanet_act_sigmoid_cached(const dimanet *ann, double a);
double dimanet_act_threshold(const dimanet *ann, double a);
double dimanet_act_linear(const dimanet *ann, double a);


#ifdef __cplusplus
}
#endif

#endif /*dimanet_H*/
