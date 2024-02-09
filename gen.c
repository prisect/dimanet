#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "dimanet.h"

/* Generate a random number between min and max */
double random_double(double min, double max) {
    return min + ((double)rand() / RAND_MAX) * (max - min);
}

/* Generate a random number from a Gaussian distribution with mean mu and standard deviation sigma */
double random_gaussian(double mu, double sigma) {
    double u1 = random_double(0, 1);
    double u2 = random_double(0, 1);
    double z = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
    return mu + sigma * z;
}

/* Initialize weights with random values from a Gaussian distribution */
void dimanet_randomize_gaussian(dimanet *ann, double mu, double sigma) {
    srand(time(NULL));  // Seed the random number generator
    int i;
    for (i = 0; i < ann->total_weights; ++i) {
        ann->weight[i] = random_gaussian(mu, sigma);
    }
}

/* Initialize weights with random values between min and max */
void dimanet_randomize_uniform(dimanet *ann, double min, double max) {
    srand(time(NULL));  // Seed the random number generator
    int i;
    for (i = 0; i < ann->total_weights; ++i) {
        ann->weight[i] = random_double(min, max);
    }
}

/* Initialize weights with Xavier initialization */
void dimanet_randomize_xavier(dimanet *ann) {
    const double scale = sqrt(2.0 / (ann->inputs + ann->outputs));
    dimanet_randomize_uniform(ann, -scale, scale);
}

