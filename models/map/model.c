#include "model.h"
#include <string.h>
#include <stdlib.h>

#define INPUT_SIZE 10
#define OUTPUT_SIZE 10

chatbot_model *chatbot_model_init() {
    chatbot_model *model = malloc(sizeof(chatbot_model));
    model->ann = dimanet_init(INPUT_SIZE, 1, 8, OUTPUT_SIZE);
    return model;
}

void chatbot_model_train(chatbot_model *model, const double *inputs, const double *desired_outputs) {
    dimanet_train(model->ann, inputs, desired_outputs, 0.1);
}

const char *chatbot_model_generate_response(chatbot_model *model, const double *inputs) {
    const double *outputs = dimanet_run(model->ann, inputs);

    // For simplicity, assume the response is a fixed set of strings
    const char *responses[] = {
        "Hello!",
        "How are you?",
        "Good to see you!",
        "I'm a learning chatbot!",
        "Ask me anything.",
        // Add more responses as needed
    };

    // Find the index with the highest output
    int max_index = 0;
    for (int i = 1; i < OUTPUT_SIZE; ++i) {
        if (outputs[i] > outputs[max_index]) {
            max_index = i;
        }
    }

    // Return the corresponding response
    return responses[max_index];
}

void chatbot_model_free(chatbot_model *model) {
    dimanet_free(model->ann);
    free(model);
}

