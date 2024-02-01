#include "dimanet.h"
#include <stdio.h>
#include <string.h>

#define MAX_INPUT_LENGTH 100

// Function to train the chatbot model
void trainChatbotModel(dimanet *model) {
    // Example training data
    const char *inputData[] = {"How are you?", "What's your name?", "Tell me a joke"};
    const char *outputData[] = {"I'm fine.", "I'm a chatbot.", "Why don't scientists trust atoms? Because they make up everything."};

    // Training loop
    for (int epoch = 0; epoch < 10000; ++epoch) {
        for (int i = 0; i < sizeof(inputData) / sizeof(inputData[0]); ++i) {
            // Convert input and output strings to arrays of doubles (for simplicity, just use ASCII values)
            double inputs[MAX_INPUT_LENGTH];
            double outputs[MAX_INPUT_LENGTH];
            for (int j = 0; j < strlen(inputData[i]); ++j) {
                inputs[j] = (double)inputData[i][j];
            }
            for (int j = 0; j < strlen(outputData[i]); ++j) {
                outputs[j] = (double)outputData[i][j];
            }

            // Train the model
            dimanet_train(model, inputs, outputs, 0.01);
        }
    }

    printf("Training complete!\n");
}

// Function to interact with the chatbot
void interactWithChatbot(dimanet *model) {
    char userInput[MAX_INPUT_LENGTH];

    printf("Chatbot: Hello! Ask me something or say 'exit' to end.\n");

    while (1) {
        printf("User: ");
        fgets(userInput, MAX_INPUT_LENGTH, stdin);

        // Remove newline character
        userInput[strcspn(userInput, "\n")] = 0;

        if (strcmp(userInput, "exit") == 0) {
            printf("Chatbot: Goodbye!\n");
            break;
        }

        // Convert user input to array of doubles
        double input[MAX_INPUT_LENGTH];
        for (int i = 0; i < strlen(userInput); ++i) {
            input[i] = (double)userInput[i];
        }

        // Run the model to get the chatbot's response
        const double *output = dimanet_run(model, input);

        // Convert output array to string
        char response[MAX_INPUT_LENGTH];
        for (int i = 0; i < MAX_INPUT_LENGTH; ++i) {
            response[i] = (char)output[i];
        }

        printf("Chatbot: %s\n", response);
    }
}

int main() {
    // Initialize the chatbot model (adjust parameters as needed)
    dimanet *chatbotModel = dimanet_init(MAX_INPUT_LENGTH, 1, 64, MAX_INPUT_LENGTH);

    // Train the chatbot model
    trainChatbotModel(chatbotModel);

    // Interact with the trained chatbot
    interactWithChatbot(chatbotModel);

    // Free memory
    dimanet_free(chatbotModel);

    return 0;
}

