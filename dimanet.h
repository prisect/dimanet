#ifdef __cplusplus
#ifndef dimanet_H
#define dimanet_H

#include <stdio.h>

extern "C" {
#endif

#ifndef dimanet_RANDOM
// using function to pick random numbers between 0 and 1
#define dimanet_RANDOM() (((double)rand())/RAND_MAX)
#endif

struct dimanet;
typedef double (*dimanet_actfun)(const struct dimanet *ann, double a);

typedef struct dimanet {

    int inputs, hidden_layers, hidden, outputs;

    dimanet_actfun activation_hidden;
    dimanet_actfun activation_output;

    // total number of weights and size of weights buffer
    int total_weights;

    // total number of neurons + inputs and size of output buffer
    int total_neurons;
    double *weight;

    // stores input array and output of each neuron 
    double *output;

    // stores delta of each hidden and output neuron
    double *delta;

} dimanet;

dimanet *dimanet_init(int inputs, int hidden_layers, int hidden, int outputs);
dimanet *dimanet_read(FILE *in);

void dimanet_randomize(dimanet *ann);
dimanet *dimanet_copy(dimanet const *ann);
void dimanet_free(dimanet *ann);

// algorithm that calculates the ouput of ann
double const *dimanet_run(dimanet const *ann, double const *inputs);

void dimanet_train(dimanet const *ann, double const *inputs, double const *desired_outputs, double learning_rate);
void dimanet_write(dimanet const *ann, FILE *out);

void dimanet_init_sigmoid_lookup(const dimanet *ann);
double dimanet_act_sigmoid(const dimanet *ann, double a);
double dimanet_act_sigmoid_cached(const dimanet *ann, double a);
double dimanet_act_threshold(const dimanet *ann, double a);
double dimanet_act_linear(const dimanet *ann, double a);

#ifdef __cplusplus
}
#endif
#endif // back to dimanet_H ez
