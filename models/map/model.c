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
        "Nice to meet you!",
        "What's on your mind?",
        "I'm here to assist you.",
        "Feel free to chat with me.",
        "I'm always ready for a conversation.",
        "Greetings!",
        "Tell me about your day.",
        "Anything specific you want to talk about?",
        "I'm here to help and learn.",
        "Let's chat and learn together!",
        "Hi there!",
        "How's your day going?",
        "It's a pleasure to chat with you!",
        "Ask away, I'm here to help.",
        "What's the latest in your world?",
        "Hello, how can I assist you today?",
        "Good day! What brings you here?",
        "Ready for a chat. What's up?",
        "Greetings! I'm at your service.",
        "How's everything on your end?",
        "I'm excited to talk with you!",
        "Hi! Tell me, how can I make your day better?",
        "What's the scoop? Let's talk!",
        "I'm here and ready to engage!",
        "Greetings! How can I be of service?",
        "Hello, friend! What's on your agenda?",
        "How's life treating you today?",
        "Ready for a chat. Shoot me a question!",
        "Hi there! What's new in your world?",
        "Good to have you here! What's on your mind?",
        "Hello! I'm here to chat and assist.",
        "Howdy! Ready for a conversation?",
        "Greetings, traveler! What brings you here?",
        "I'm all ears! What do you want to discuss?",
        "Hi! Let's make this conversation interesting!",
        "How's the day treating you so far?",
        "What's cracking? Let's chat!",
        "Greetings! How may I be of service today?"
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
