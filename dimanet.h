#ifdef __cplusplus
#ifndef DIMANET_H
#define DIMANET_H

#include <stdio.h>

extern "C" {
#endif

#ifndef DIMANET_RANDOM
// using function to pick random numbers between 0 and 1
#define DIMANET_RANDOM() (((double)rand())/RAND_MAX)
#endif

struct DIMANET;
typedef double (*DIMANET_actfun)(const struct DIMANET *ann, double a);

typedef struct DIMANET {

    int inputs, hidden_layers, hidden, outputs;

    DIMANET_actfun activation_hidden;
    DIMANET_actfun activation_output;

    // total number of weights and size of weights buffer
    int total_weights;

    // total number of neurons + inputs and size of output buffer
    int total_neurons;
    double *weight;

    // stores input array and output of each neuron 
    double *output;

    // stores delta of each hidden and output neuron
    double *delta;

} DIMANET;

DIMANET *DIMANET_init(int inputs, int hidden_layers, int hidden, int outputs);
DIMANET *DIMANET_read(FILE *in);

void DIMANET_randomize(DIMANET *ann);
DIMANET *DIMANET_copy(DIMANET const *ann);
void DIMANET_free(DIMANET *ann);

// algorithm that calculates the ouput of ann
double const *DIMANET_run(DIMANET const *ann, double const *inputs);

void DIMANET_train(DIMANET const *ann, double const *inputs, double const *desired_outputs, double learning_rate);
void DIMANET_write(DIMANET const *ann, FILE *out);

void DIMANET_init_sigmoid_lookup(const DIMANET *ann);
double DIMANET_act_sigmoid(const DIMANET *ann, double a);
double DIMANET_act_sigmoid_cached(const DIMANET *ann, double a);
double DIMANET_act_threshold(const DIMANET *ann, double a);
double DIMANET_act_linear(const DIMANET *ann, double a);

#ifdef __cplusplus
}
#endif
#endif // back to DIMANET_H ez
