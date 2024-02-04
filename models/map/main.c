/* models/map/main.c - dimanet map main.c file
 *
 * License-Identifier: GPL-3.0
 * Latest updated version: 1.2 */

#include "model.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    chatbot_model *model = chatbot_model_init();

    // Example training data (inputs and corresponding desired outputs)
    double inputs[] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};
    double desired_outputs[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0};

    // Train the chatbot model
    chatbot_model_train(model, inputs, desired_outputs);

    // Example interaction with the chatbot
    double user_input[] = {0.6, 0.7, 0.8, 0.9, 1.0, 0.1, 0.2, 0.3, 0.4, 0.5};
    const char *response = chatbot_model_generate_response(model, user_input);

    printf("User: How are you?\n");
    printf("Chatbot: %s\n", response);

    // Cleanup
    chatbot_model_free(model);

    return 0;
}
