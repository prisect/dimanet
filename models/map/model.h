#ifndef MODEL_H
#define MODEL_H

#include "../../dimanet.h"

typedef struct {
    dimanet *ann;
} chatbot_model;

chatbot_model *chatbot_model_init();
void chatbot_model_train(chatbot_model *model, const double *inputs, const double *desired_outputs);
const char *chatbot_model_generate_response(chatbot_model *model, const double *inputs);
void chatbot_model_free(chatbot_model *model);

#endif // MODEL_H

