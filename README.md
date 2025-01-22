
# DimaNet <img alt="DimaNet Logo didn't load :(" width="390" height="270" src="misc/logo.png" align="right" />

![LICENSE](https://img.shields.io/github/license/dimalmfao/dimanet)
![GitHub Release](https://img.shields.io/github/v/release/dimalmfao/dimanet)
![GitHub last commit](https://img.shields.io/github/last-commit/dimalmfao/dimanet)


DimaNet, just a bad integration of AI serves as a compact, yet powerful library API designed for the implementation and training of feedforward artificial neural networks (ANN) in the classic *ANSI* C programming language. Striving for ~~simplicity~~, speed, ~~reliability~~, and customization, DimaNet offers a lean set of essential functions, ~~ensuring a clutter-free and efficient user experience~~.

ⓘ | This library is primarily developed and maintained by hobbyists and is intended for non-commercial or educational use. It is not recommended for deployment within official organizational settings or professional environments. If so, we welcome collaboration and contributions to elevate its suitability for professional use and organizational deployment.
:---: | :---

ⓘ | To use this library, extract the main two files, dimanet.h and dimanet.c. The rest is unnecessary for building your own projects.
:---: | :---

#### Notice

> *If you have issues with the permission, or the make file, please report it at the ***Issues*** tab. (make sure to see if the project successfully built on the top of this readme file before reporting anything. if not, it might probably be an issue with your system or the make command it self)*
> *Also, search errors up instead of commiting issues concerning the commands, etc...*

## Deployments

Here are the list of the workflow statuses of github actions deployments.

|  Name | Status |
| -------------- | ------------------------------------------------------- |
| Debug   | ![MAKE DEBUG](https://img.shields.io/github/actions/workflow/status/dimalmfao/dimanet/makedebug.yml) |
| Gen    | ![MAKE GEN](https://img.shields.io/github/actions/workflow/status/dimalmfao/dimanet/makegen.yml) |
| Strings | ![MAKE STRINGS](https://img.shields.io/github/actions/workflow/status/dimalmfao/dimanet/makestrings.yml) |
| Valgrind | ![MAKE GEN](https://img.shields.io/github/actions/workflow/status/dimalmfao/dimanet/makevalgrind.yml) |

> *Note that the main deployment that is required for the project to run is `debug`. Others can be ignored.*

## Automated Installation (Arch Only)
If you're using [Arch Linux](https://archlinux.org/), you can easily install the [dimanet library](https://aur.archlinux.org/packages/dimanet) through the [AUR](https://aur.archlinux.org/). Use the [yay](https://aur.archlinux.org/packages/yay) AUR helper by running the following command:
```
$ yay -S dimanet
```
> *Note that this will only build and install the libraries on your system.*

## Cloning

Alternativly, to clone DimaNet, run:
```
$ git clone https://github.com/dimalmfao/dimanet --recursive --depth=1
```
`--recursive` to clone dependencies.
`--depth=1` to only clone the latest commit. (time saver)

## Building

To build, just run:
```bash
$ make
```
## Adding DimaNet
To use DimaNet, you have to include: `dimanet.c` and `dimanet.h` as **DimaNet is locally contained**. Include the following files with 
```c
#include <dimanet.c> // Main DimaNet
#include <dimanet.h> // Header
```

## Dependencies

To use the Makefile to run tests and debug scripts, DimaNet requires:

- `make`
- `gcc`
- `valgrind`

## Examples

In the [`examples`](./examples/) folder, there are various examples you can experiment with. Including:

- [`example_1.c`](./examples/example_1.c) - Trains an ANN on the XOR function using backpropagation.
- [`example_2.c`](./examples/example_2.c) - Trains an ANN on the XOR function using random search.
- [`example_3.c`](./examples/example_3.c) - Loads and runs an ANN from a file.
- [`example_4.c`](./examples/example_4.c) - Trains an ANN on the [IRIS data-set](https://archive.ics.uci.edu/ml/datasets/Iris) using backpropagation.
- [`example_5.c`](./examples/example_5.c) - Visualization of Neural Network Approximation.

`persist.txt` is basically last saved training.
> *It would be really appreciated to contribute and to add example files :)*

## How it works

Artificial Neural Networks (ANNs) are computing systems inspired by the biological neural networks that constitute animal brains. They are composed of interconnected nodes, or “neurons”, which mimic the neurons in a biological brain. Each connection between neurons can transmit a signal from one to another. The receiving neuron processes the signal and signals downstream neurons connected to it.

See this visual representation:

<img src="https://raw.githubusercontent.com/dimalmfao/dimanet/master/misc/visualann.png">

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

### 1. **Activation Functions**
The code references several activation functions, which are crucial in neural networks. Here are the mathematical representations:

- **Sigmoid Activation Function:**
  ![formula](http://latex.codecogs.com/svg.latex?\sigma(a)%20=%20\frac{1}{1%20+%20e^{-a}})

  This function is used to introduce non-linearity into the network.

- **Threshold Activation Function:**
  ![formula](http://latex.codecogs.com/svg.latex?f(a)%20=%20\begin{cases}%201%20&%20\text{if%20}%20a%20\geq%200%20\\%200%20&%20\text{if%20}%20a%20<%200%20\end{cases})
  
  This is a binary step function that outputs 1 if the input is greater than or equal to zero, otherwise 0.

- **Linear Activation Function:**
  ![formula](http://latex.codecogs.com/svg.latex?f(a)%20=%20a)
  
  This function simply returns the input as the output, providing no transformation.

---

### 2. **Feedforward Calculation**
The feedforward process calculates the output of the neural network given an input. For a single neuron, the output \( y \) is calculated as:
![formula](http://latex.codecogs.com/svg.latex?y%20=%20f\left(\sum_{i=1}^{n}%20w_i%20x_i%20+%20b\right))

where:
- \( f \) is the activation function,
- \( w_i \) are the weights,
- \( x_i \) are the inputs,
- \( b \) is the bias term.

---

### 3. **Backpropagation**
Backpropagation is used to update the weights of the network during training. The weight update rule is:
![formula](http://latex.codecogs.com/svg.latex?w_i%20%3D%20w_i%20-%20%5Ceta%20%5Cfrac%7B%5Cpartial%20E%7D%7B%5Cpartial%20w_i%7D)

where:
- \( \eta \) is the learning rate,
- \( \frac{\partial E}{\partial w_i} \) is the gradient of the error with respect to the weight \( w_i \).

---

### 4. **Random Weight Initialization**
The weights are initialized randomly, typically using a uniform distribution between 0 and 1:
![formula](http://latex.codecogs.com/svg.latex?w_i%20=%20\text{random}(0,%201))

This helps in breaking symmetry and ensuring that different neurons learn different features.

---

### 5. **Delta Calculation**
The delta for each neuron is calculated as:
![formula](http://latex.codecogs.com/svg.latex?\delta_j%20=%20f'(a_j)%20\sum_{k}%20\delta_k%20w_{jk})

where:
- \( \delta_j \) is the delta for neuron \( j \),
- \( f'(a_j) \) is the derivative of the activation function,
- \( \delta_k \) is the delta for the next layer's neuron \( k \),
- \( w_{jk} \) is the weight connecting neuron \( j \) to neuron \( k \).

---

### 6. **Error Calculation**
The error \( E \) for a single training example is often calculated using Mean Squared Error (MSE):
![formula](http://latex.codecogs.com/svg.latex?E%20=%20\frac{1}{2}%20\sum_{k}%20(y_k%20-%20t_k)^2)

where:
- \( y_k \) is the predicted output,
- \( t_k \) is the target output.

---

### 7. **Weight Update Rule**
The weights are updated using the gradient descent rule:
![formula](http://latex.codecogs.com/svg.latex?w_{ij}%20=%20w_{ij}%20-%20\eta%20\delta_j%20x_i)

where:
- \( \eta \) is the learning rate,
- \( \delta_j \) is the delta for neuron \( j \),
- \( x_i \) is the input to the weight \( w_{ij} \).

---

### 8. **Total Weights Calculation**
The total number of weights in the network is calculated as:
![formula](http://latex.codecogs.com/svg.latex?\text{total\_weights}%20=%20(\text{inputs}%20\times%20\text{hidden})%20+%20(\text{hidden}%20\times%20\text{hidden})%20\times%20(\text{hidden\_layers}%20-%201)%20+%20(\text{hidden}%20\times%20\text{outputs}))

This formula accounts for the weights between the input layer and the first hidden layer, between hidden layers, and between the last hidden layer and the output layer.

---

### 9. **Total Neurons Calculation**
The total number of neurons in the network is calculated as:
![formula](http://latex.codecogs.com/svg.latex?\text{total\_neurons}%20=%20\text{inputs}%20+%20(\text{hidden}%20\times%20\text{hidden\_layers})%20+%20\text{outputs})

This includes the input neurons, hidden neurons, and output neurons.

---

### 10. **Random Number Generation**
The code uses a uniform random number generator to initialize weights:
![formula](http://latex.codecogs.com/svg.latex?\text{dimanet\_RANDOM()}%20=%20\frac{\text{rand()}}{\text{RAND\_MAX}})
This generates a random number between 0 and 1.

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
void dimanet_train(dimanet const *ann,
                   double const *inputs,
                   double const *desired_outputs,
                   double learning_rate);
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
double const *dimanet_run(dimanet const *ann,
                          double const *inputs);
```

Call `dimanet_run()` on a trained ANN to run a feed-forward pass on a given set of inputs. `dimanet_run()`
will provide a pointer to the array of predicted outputs (of `ann->outputs` length).

## Hints

- Every functions start with `dimanet_`.
- Feel free to modify the code to your liking.

> *Add more Hints here by pull requesting!*

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

- [comp.ai.neural-nets
FAQ](http://www.faqs.org/faqs/ai-faq/neural-nets/part1/)
- [TINN](https://github.com/glouw/tinn)
- [FANNlib](http://leenissen.dk/fann/wp/)
- [Lightweight Neural
Network](http://lwneuralnet.sourceforge.net/)
- [DimaNet](https://github.com/dimalmfao/dimanet/)

> *Add more Extra Resources here by pull requesting!*

## Strings

Simple static string library
See more at https://github.com/dimalmfao/stringslib/

``` bash
echo "made by unsrced with love, too official to be true."
```
