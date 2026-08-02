/* ------------------------------------------------------------------ */
/* DimaNet - a minimal C feedforward neural network library.          */
/*                                                                    */
/* Derived from GENANN (http://codeplea.com/genann):                  */
/*                                                                    */
/*   Copyright (c) 2015-2016 Lewis Van Winkle                        */
/*                                                                    */
/*   This software is provided 'as-is', without any express or        */
/*   implied warranty. In no event will the authors be held liable    */
/*   for any damages arising from the use of this software.           */
/*                                                                    */
/*   Permission is granted to anyone to use this software for any     */
/*   purpose, including commercial applications, and to alter it and  */
/*   redistribute it freely, subject to the following restrictions:   */
/*                                                                    */
/*   1. The origin of this software must not be misrepresented; you   */
/*      must not claim that you wrote the original software. If you   */
/*      use this software in a product, an acknowledgement in the     */
/*      product documentation would be appreciated but is not         */
/*      required.                                                     */
/*   2. Altered source versions must be plainly marked as such, and   */
/*      must not be misrepresented as being the original software.    */
/*   3. This notice may not be removed or altered from any source     */
/*      distribution.                                                 */
/*                                                                    */
/* ------------------------------------------------------------------ */

#ifndef DIMANET_H
#define DIMANET_H

#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#define DIMANET_VERSION "3.0.0"

typedef struct dimanet dimanet;

/* Activation function: maps a neuron's pre-activation sum to its output. */
typedef double (*dimanet_actfun)(const dimanet *ann, double x);

struct dimanet {
    /* Network topology. */
    int inputs;
    int hidden_layers;
    int hidden;
    int outputs;

    /* Activation functions. Default: cached sigmoid for both. */
    dimanet_actfun activation_hidden;
    dimanet_actfun activation_output;

    /* Total number of weights and neurons (+ inputs). */
    int total_weights;
    int total_neurons;

    /* All weights, in layer order (total_weights doubles). */
    double *weight;

    /* Pre-activation sum of every neuron (total_neurons doubles). */
    double *sum;

    /* Post-activation output of every neuron (total_neurons doubles). */
    double *output;

    /* Error of every hidden and output neuron (total_neurons - inputs doubles). */
    double *delta;

    /* Momentum buffer, one entry per weight (total_weights doubles). */
    double *velocity;

    /* Gradient buffer, one entry per weight (total_weights doubles). */
    double *gradient;

    /* Slope of leaky ReLU below zero (default 0.01). */
    double leaky_relu_alpha;

    /* Slope of ELU below zero (default 1.0). */
    double elu_alpha;

    /* Momentum factor; 0 disables momentum (default 0). */
    double momentum;

    /* Maximum L2 norm of the gradient; 0 disables clipping (default 0). */
    double gradient_clip;
};

/* ------------------------------------------------------------------ */
/* Lifecycle                                                           */
/* ------------------------------------------------------------------ */

/* Creates a new network and initializes its weights randomly. */
dimanet *dimanet_init(int inputs, int hidden_layers, int hidden, int outputs);

/* Loads a network saved with dimanet_write. Returns NULL on failure. */
dimanet *dimanet_read(FILE *in);

/* Returns a deep copy of ann. */
dimanet *dimanet_copy(const dimanet *ann);

/* Frees the memory used by ann. */
void dimanet_free(dimanet *ann);

/* ------------------------------------------------------------------ */
/* Randomness                                                          */
/* ------------------------------------------------------------------ */

/* Seeds the internal PRNG used by the initializers. */
void dimanet_seed(uint32_t seed);

/* ------------------------------------------------------------------ */
/* Weight initialization                                               */
/* ------------------------------------------------------------------ */

/* Uniform in [-0.5, 0.5]; the default initialization. */
void dimanet_randomize(dimanet *ann);

/* Uniform in [lo, hi]. */
void dimanet_init_uniform(dimanet *ann, double lo, double hi);

/* Normal with the given mean and standard deviation. */
void dimanet_init_normal(dimanet *ann, double mean, double stddev);

/* Glorot/Xavier uniform initialization. */
void dimanet_init_xavier(dimanet *ann);

/* He/Kaiming uniform initialization (suited for ReLU). */
void dimanet_init_he(dimanet *ann);

/* LeCun uniform initialization (suited for sigmoid/tanh). */
void dimanet_init_lecun(dimanet *ann);

/* ------------------------------------------------------------------ */
/* Forward pass and training                                           */
/* ------------------------------------------------------------------ */

/* Runs a forward pass; returns a pointer to the outputs array. */
const double *dimanet_run(dimanet *ann, const double *inputs);

/* Performs a single backpropagation update. */
void dimanet_train(dimanet *ann, const double *inputs,
                   const double *desired_outputs, double learning_rate);

/* ------------------------------------------------------------------ */
/* Persistence                                                         */
/* ------------------------------------------------------------------ */

/* Saves the network in a self-describing text format. Returns 0 on
 * success and -1 on failure. */
int dimanet_write(const dimanet *ann, FILE *out);

/* ------------------------------------------------------------------ */
/* Activation functions                                                */
/* ------------------------------------------------------------------ */

double dimanet_act_linear(const dimanet *ann, double x);
double dimanet_act_threshold(const dimanet *ann, double x);
double dimanet_act_sigmoid(const dimanet *ann, double x);
double dimanet_act_sigmoid_cached(const dimanet *ann, double x);
double dimanet_act_tanh(const dimanet *ann, double x);
double dimanet_act_relu(const dimanet *ann, double x);
double dimanet_act_leaky_relu(const dimanet *ann, double x);
double dimanet_act_elu(const dimanet *ann, double x);
double dimanet_act_softplus(const dimanet *ann, double x);
double dimanet_act_silu(const dimanet *ann, double x);
double dimanet_act_gelu(const dimanet *ann, double x);

/* ------------------------------------------------------------------ */
/* Configuration                                                       */
/* ------------------------------------------------------------------ */

void dimanet_set_activation_functions(dimanet *ann,
                                      dimanet_actfun hidden,
                                      dimanet_actfun output);
void dimanet_set_leaky_relu_alpha(dimanet *ann, double alpha);
void dimanet_set_elu_alpha(dimanet *ann, double alpha);
void dimanet_set_momentum(dimanet *ann, double momentum);
void dimanet_set_gradient_clip(dimanet *ann, double clip);

#ifdef __cplusplus
}
#endif

#endif /* DIMANET_H */
