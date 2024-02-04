// Basic ANN XOR C++ Script
//
// Keep in mind that this is
// isn't connected with the
// main DimaNet library.
//
// Made by @dimalmfao

#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

// Activation function (sigmoid)
double sigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
}

// Derivative of the sigmoid function
double sigmoid_derivative(double x) {
    double s = sigmoid(x);
    return s * (1 - s);
}

// Neural Network class
class NeuralNetwork {
private:
    int input_size;
    int hidden_size;
    int output_size;
    
    // Weights and biases
    vector<vector<double>> weights_ih;
    vector<double> bias_h;
    vector<vector<double>> weights_ho;
    vector<double> bias_o;

public:
    NeuralNetwork(int input_size, int hidden_size, int output_size)
        : input_size(input_size), hidden_size(hidden_size), output_size(output_size) {
        
        // Initialize weights and biases randomly
        weights_ih.resize(hidden_size, vector<double>(input_size));
        for (int i = 0; i < hidden_size; ++i) {
            for (int j = 0; j < input_size; ++j) {
                weights_ih[i][j] = (rand() % 2000 - 1000) / 1000.0;  // Random values between -1 and 1
            }
        }

        bias_h.resize(hidden_size);
        for (int i = 0; i < hidden_size; ++i) {
            bias_h[i] = (rand() % 2000 - 1000) / 1000.0;
        }

        weights_ho.resize(output_size, vector<double>(hidden_size));
        for (int i = 0; i < output_size; ++i) {
            for (int j = 0; j < hidden_size; ++j) {
                weights_ho[i][j] = (rand() % 2000 - 1000) / 1000.0;
            }
        }

        bias_o.resize(output_size);
        for (int i = 0; i < output_size; ++i) {
            bias_o[i] = (rand() % 2000 - 1000) / 1000.0;
        }
    }

    // Forward pass
    vector<double> predict(const vector<double>& input) {
        // Input to hidden layer
        vector<double> hidden(hidden_size);
        for (int i = 0; i < hidden_size; ++i) {
            hidden[i] = 0;
            for (int j = 0; j < input_size; ++j) {
                hidden[i] += input[j] * weights_ih[i][j];
            }
            hidden[i] += bias_h[i];
            hidden[i] = sigmoid(hidden[i]);
        }

        // Hidden to output layer
        vector<double> output(output_size);
        for (int i = 0; i < output_size; ++i) {
            output[i] = 0;
            for (int j = 0; j < hidden_size; ++j) {
                output[i] += hidden[j] * weights_ho[i][j];
            }
            output[i] += bias_o[i];
            output[i] = sigmoid(output[i]);
        }

        return output;
    }

    // Training using backpropagation
    void train(const vector<vector<double>>& inputs, const vector<vector<double>>& targets, int epochs, double learning_rate) {
        for (int epoch = 0; epoch < epochs; ++epoch) {
            for (int i = 0; i < inputs.size(); ++i) {
                // Forward pass
                vector<double> input = inputs[i];
                vector<double> target = targets[i];

                // Input to hidden layer
                vector<double> hidden(hidden_size);
                for (int j = 0; j < hidden_size; ++j) {
                    hidden[j] = 0;
                    for (int k = 0; k < input_size; ++k) {
                        hidden[j] += input[k] * weights_ih[j][k];
                    }
                    hidden[j] += bias_h[j];
                    hidden[j] = sigmoid(hidden[j]);
                }

                // Hidden to output layer
                vector<double> output(output_size);
                for (int j = 0; j < output_size; ++j) {
                    output[j] = 0;
                    for (int k = 0; k < hidden_size; ++k) {
                        output[j] += hidden[k] * weights_ho[j][k];
                    }
                    output[j] += bias_o[j];
                    output[j] = sigmoid(output[j]);
                }

                // Backpropagation

                // Calculate output layer errors
                vector<double> output_errors(output_size);
                for (int j = 0; j < output_size; ++j) {
                    output_errors[j] = target[j] - output[j];
                }

                // Calculate output layer gradients
                vector<double> output_gradients(output_size);
                for (int j = 0; j < output_size; ++j) {
                    output_gradients[j] = output_errors[j] * sigmoid_derivative(output[j]);
                }

                // Update weights and biases for hidden to output layer
                for (int j = 0; j < output_size; ++j) {
                    for (int k = 0; k < hidden_size; ++k) {
                        weights_ho[j][k] += learning_rate * output_gradients[j] * hidden[k];
                    }
                    bias_o[j] += learning_rate * output_gradients[j];
                }

                // Calculate hidden layer errors
                vector<double> hidden_errors(hidden_size);
                for (int j = 0; j < hidden_size; ++j) {
                    hidden_errors[j] = 0;
                    for (int k = 0; k < output_size; ++k) {
                        hidden_errors[j] += output_errors[k] * weights_ho[k][j];
                    }
                }

                // Calculate hidden layer gradients
                vector<double> hidden_gradients(hidden_size);
                for (int j = 0; j < hidden_size; ++j) {
                    hidden_gradients[j] = hidden_errors[j] * sigmoid_derivative(hidden[j]);
                }

                // Update weights and biases for input to hidden layer
                for (int j = 0; j < hidden_size; ++j) {
                    for (int k = 0; k < input_size; ++k) {
                        weights_ih[j][k] += learning_rate * hidden_gradients[j] * input[k];
                    }
                    bias_h[j] += learning_rate * hidden_gradients[j];
                }
            }
        }
    }
};

int main() {
    // XOR inputs and corresponding outputs
    vector<vector<double>> inputs = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
    vector<vector<double>> targets = {{0}, {1}, {1}, {0}};

    // Create a neural network with 2 input neurons, 2 hidden neurons, and 1 output neuron
    NeuralNetwork nn(2, 2, 1);

    // Train the neural network for 10000 epochs with a learning rate of 0.1
    nn.train(inputs, targets, 10000, 0.1);

    // Test the trained network
    for (int i = 0; i < inputs.size(); ++i) {
        vector<double> input = inputs[i];
        vector<double> output = nn.predict(input);

        cout << "Input: " << input[0] << " " << input[1] << " Output: " << output[0] << endl;
    }

    return 0;
}
