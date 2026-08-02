#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "dimanet.h"

#define NUM_FEATURES 4
#define NUM_CLASSES 3
#define EPOCHS 2000
#define LEARNING_RATE 0.05

static const char *default_path = "examples/data/iris.data";

static const char *class_names[NUM_CLASSES] = {
    "Iris-setosa", "Iris-versicolor", "Iris-virginica"
};

static double *features;
static double *labels;
static int samples;

static int load_data(const char *path) {
    FILE *in = fopen(path, "r");
    if (!in)
        return -1;

    /* Count the samples first so we can allocate the buffers. */
    char line[256];
    int count = 0;
    while (fgets(line, sizeof line, in))
        if (line[0] != '\n')
            ++count;

    rewind(in);

    features = malloc(sizeof(double) * (size_t)count * NUM_FEATURES);
    labels = malloc(sizeof(double) * (size_t)count * NUM_CLASSES);
    if (!features || !labels) {
        fclose(in);
        return -1;
    }

    samples = 0;
    while (fgets(line, sizeof line, in)) {
        if (line[0] == '\n')
            continue;

        double f[NUM_FEATURES];
        char name[32];
        if (sscanf(line, "%lf,%lf,%lf,%lf,%31s", &f[0], &f[1], &f[2], &f[3],
                   name) != NUM_FEATURES + 1) {
            fclose(in);
            return -1;
        }

        int cls = -1;
        for (int i = 0; i < NUM_CLASSES; ++i)
            if (strcmp(name, class_names[i]) == 0)
                cls = i;
        if (cls < 0) {
            fclose(in);
            return -1;
        }

        double *x = features + (size_t)samples * NUM_FEATURES;
        double *y = labels + (size_t)samples * NUM_CLASSES;
        memcpy(x, f, sizeof f);
        memset(y, 0, sizeof(double) * NUM_CLASSES);
        y[cls] = 1.0;

        ++samples;
    }

    fclose(in);
    return 0;
}

int main(int argc, char *argv[]) {
    const char *path = argc > 1 ? argv[1] : default_path;

    printf("DimaNet example 4: train on the IRIS data set.\n");

    if (load_data(path) != 0) {
        fprintf(stderr, "failed to read %s\n", path);
        return 1;
    }
    printf("Loaded %d samples.\n", samples);

    dimanet_seed((uint32_t)time(NULL));

    dimanet *ann = dimanet_init(NUM_FEATURES, 1, 4, NUM_CLASSES);
    if (!ann) {
        fprintf(stderr, "failed to allocate the network\n");
        return 1;
    }
    dimanet_set_activation_functions(ann, dimanet_act_sigmoid,
                                     dimanet_act_sigmoid);

    for (int e = 0; e < EPOCHS; ++e)
        for (int i = 0; i < samples; ++i)
            dimanet_train(ann, features + (size_t)i * NUM_FEATURES,
                          labels + (size_t)i * NUM_CLASSES, LEARNING_RATE);

    int correct = 0;
    for (int i = 0; i < samples; ++i) {
        const double *pred =
            dimanet_run(ann, features + (size_t)i * NUM_FEATURES);
        int guess = 0;
        for (int c = 1; c < NUM_CLASSES; ++c)
            if (pred[c] > pred[guess])
                guess = c;

        const double *truth = labels + (size_t)i * NUM_CLASSES;
        if (truth[guess] == 1.0)
            ++correct;
    }

    printf("Accuracy: %d/%d (%.1f%%)\n", correct, samples,
           (double)correct / samples * 100.0);

    dimanet_free(ann);
    free(features);
    free(labels);
    return 0;
}
