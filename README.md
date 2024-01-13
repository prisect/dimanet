# DimaNet

DimaNet serves as a compact, yet powerful library designed for the implementation and training of feedforward artificial neural networks (ANN) in the C programming language. Striving for simplicity, speed, reliability, and customization, DimaNet offers a lean set of essential functions, ensuring a clutter-free and efficient user experience.

## Building

To use DimaNet, you have to include: `dimanet.c` and `dimanet.h` as DimaNet is locally contained. Include the following files with 
```c
#include <dimanet.c> // Main DimaNet
#include <dimanet.h> // Header
```

## Examples

In the [`examples`](./examples/) folder, there are various examples you can experiment with. Including:

- [`example_1.c`](./examples/example_1.c) - Trains an ANN on the XOR function using backpropagation.
- [`example_2.c`](./examples/example_2.c) - Trains an ANN on the XOR function using random search.
- [`example_3.c`](./examples/example_3.c) - Loads and runs an ANN from a file.
- [`example_4.c`](./examples/example_4.c) - Trains an ANN on the [IRIS data-set](https://archive.ics.uci.edu/ml/datasets/Iris) using backpropagation.

I would be happy to review your code :)

## How it works

Artificial Neural Networks (ANNs) are computing systems inspired by the biological neural networks that constitute animal brains. They are composed of interconnected nodes, or “neurons”, which mimic the neurons in a biological brain. Each connection between neurons can transmit a signal from one to another. The receiving neuron processes the signal and signals downstream neurons connected to it.

See this visual reprensentation:

<img src="https://github.com/dimaaac/DimaNet/blob/main/misc/layerann.png">

In a three-layer neural network, there are three layers of neurons: the input layer, the hidden layer, and the output layer. The input layer receives input patterns and passes them to the neurons in the hidden layer. Each neuron in the hidden layer applies a set of weights to the inputs, sums them, applies an activation function, and passes the result to the neurons in the output layer. The weights represent the strength or intensity of the input for the features being processed.

The output layer receives the signals from the hidden layer, applies another set of weights, and sums them. The final output is then computed using another activation function. The weights in the network are adjusted based on the error of the final output through a process called backpropagation. In backpropagation, the error is calculated by comparing the predicted output with the expected output, and this error is propagated back through the network, adjusting the weights for each neuron. This process is repeated many times, and the network learns to make more accurate predictions. The three-layer neural network is a fundamental architecture in deep learning, and understanding it is key to understanding more complex neural networks.

We then train it on a set of labeled data using backpropagation and ask it to predict on a test data point:

```c
#include "dimanet.h"

/* Not shown, loading your training and test data. */
double **training_data_input, **training_data_output, **test_data_input;

/* New network with 2 inputs,
 * 1 hidden layer of 3 neurons each,
 * and 2 outputs. */
dimanet *ann = dimanet_init(2, 1, 3, 2);

/* Learn on the training set. */
for (i = 0; i < 300; ++i) {
    for (j = 0; j < 100; ++j)
        dimanet_train(ann, training_data_input[j], training_data_output[j], 0.1);
}

/* Run the network and see what it predicts. */
double const *prediction = dimanet_run(ann, test_data_input[0]);
printf("Output for the first test data point is: %f, %f\n", prediction[0], prediction[1]);

dimanet_free(ann);
```

This example is to show API usage, it is not showing good machine learning techniques. In a real application you would likely want to learn on the test data in a random order. You would also want to monitor the learning to prevent over-fitting.

## Usage

### Creating and Freeing ANNs
```C
dimanet *dimanet_init(int inputs, int hidden_layers, int hidden, int outputs);
dimanet *dimanet_copy(dimanet const *ann);
void dimanet_free(dimanet *ann);
```

Creating a new ANN is done with the `dimanet_init()` function. Its arguments
are the number of inputs, the number of hidden layers, the number of neurons in
each hidden layer, and the number of outputs. It returns a `dimanet` struct pointer.

Calling `dimanet_copy()` will create a deep-copy of an existing `dimanet` struct.

Call `dimanet_free()` when you're finished with an ANN returned by `dimanet_init()`.


### Training ANNs
```C
void dimanet_train(dimanet const *ann, double const *inputs,
        double const *desired_outputs, double learning_rate);
```

`dimanet_train()` will preform one update using standard backpropogation. It
should be called by passing in an array of inputs, an array of expected outputs,
and a learning rate.

A primary design goal of dimanet was to store all the network weights in one
contigious block of memory. This makes it easy and efficient to train the
network weights using direct-search numeric optimization algorthims,
such as [Hill Climbing](https://en.wikipedia.org/wiki/Hill_climbing),
[the Genetic Algorithm](https://en.wikipedia.org/wiki/Genetic_algorithm), [Simulated
Annealing](https://en.wikipedia.org/wiki/Simulated_annealing), etc.
These methods can be used by searching on the ANN's weights directly.
Every `dimanet` struct contains the members `int total_weights;` and
`double *weight;`.  `*weight` points to an array of `total_weights`
size which contains all weights used by the ANN.

### Saving and Loading ANNs

```C
dimanet *dimanet_read(FILE *in);
void dimanet_write(dimanet const *ann, FILE *out);
```
 
DimaNet provides the `dimanet_read()` and `dimanet_write()` functions for loading or saving an ANN in a text-based format.

### Evaluating

```C
double const *dimanet_run(dimanet const *ann, double const *inputs);
```

Call `dimanet_run()` on a trained ANN to run a feed-forward pass on a given set of inputs. `dimanet_run()`
will provide a pointer to the array of predicted outputs (of `ann->outputs` length).

## Hints

- Every functions start with `dimanet_`.
- Feel free to modify the code to your liking.

## Extra Resources

The [comp.ai.neural-nets
FAQ](http://www.faqs.org/faqs/ai-faq/neural-nets/part1/) is an excellent
resource for an introduction to artificial neural networks.

If you need an even smaller neural network library, check out the excellent single-hidden-layer library [tinn](https://github.com/glouw/tinn).

If you're looking for a heavier, more opinionated neural network library in C,
I recommend the [FANN library](http://leenissen.dk/fann/wp/). Another
good library is Peter van Rossum's [Lightweight Neural
Network](http://lwneuralnet.sourceforge.net/), which despite its name, is
heavier and has more features than dimanet.

## Yeah
That's all... for now.
