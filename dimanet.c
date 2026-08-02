#include "dimanet.h"

#include <assert.h>
#include <limits.h>

/* DimaNet is derived from GENANN (http://codeplea.com/genann),
 * Copyright (c) 2015-2016 Lewis Van Winkle, distributed under the MIT
 * license. See the header comment in dimanet.h. */
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define LOOKUP_SIZE 4096
#define SIGMOID_DOM_MIN -15.0
#define SIGMOID_DOM_MAX 15.0
#define DIMANET_PI 3.14159265358979323846

/* ------------------------------------------------------------------ */
/* Internal PRNG (xorshift32)                                          */
/* ------------------------------------------------------------------ */

static uint32_t rng_state = 1u;

void dimanet_seed(uint32_t seed) {
    rng_state = seed ? seed : 1u;
}

static uint32_t next_u32(void) {
    uint32_t x = rng_state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    return rng_state = x;
}

static double next_double(void) {
    return (double)next_u32() / (double)UINT32_MAX;
}

/* ------------------------------------------------------------------ */
/* Cached sigmoid lookup table                                         */
/* ------------------------------------------------------------------ */

static double sigmoid_lookup[LOOKUP_SIZE];
static double sigmoid_interval;
static int sigmoid_ready;

static void ensure_sigmoid_lookup(void) {
    if (sigmoid_ready)
        return;

    const double step = (SIGMOID_DOM_MAX - SIGMOID_DOM_MIN) / LOOKUP_SIZE;
    sigmoid_interval = LOOKUP_SIZE / (SIGMOID_DOM_MAX - SIGMOID_DOM_MIN);
    for (int i = 0; i < LOOKUP_SIZE; ++i)
        sigmoid_lookup[i] = dimanet_act_sigmoid(NULL,
                                                SIGMOID_DOM_MIN + step * i);
    sigmoid_ready = 1;
}

/* ------------------------------------------------------------------ */
/* Activation functions                                                */
/* ------------------------------------------------------------------ */

double dimanet_act_linear(const dimanet *ann, double x) {
    (void)ann;
    return x;
}

double dimanet_act_threshold(const dimanet *ann, double x) {
    (void)ann;
    return x > 0 ? 1.0 : 0.0;
}

double dimanet_act_sigmoid(const dimanet *ann, double x) {
    (void)ann;
    if (x < -45.0)
        return 0.0;
    if (x > 45.0)
        return 1.0;
    return 1.0 / (1.0 + exp(-x));
}

double dimanet_act_sigmoid_cached(const dimanet *ann, double x) {
    (void)ann;
    assert(!isnan(x));

    ensure_sigmoid_lookup();

    if (x < SIGMOID_DOM_MIN)
        return sigmoid_lookup[0];
    if (x >= SIGMOID_DOM_MAX)
        return sigmoid_lookup[LOOKUP_SIZE - 1];

    size_t idx = (size_t)((x - SIGMOID_DOM_MIN) * sigmoid_interval + 0.5);
    if (idx >= LOOKUP_SIZE)
        return sigmoid_lookup[LOOKUP_SIZE - 1];
    return sigmoid_lookup[idx];
}

double dimanet_act_tanh(const dimanet *ann, double x) {
    (void)ann;
    return tanh(x);
}

double dimanet_act_relu(const dimanet *ann, double x) {
    (void)ann;
    return x > 0 ? x : 0.0;
}

double dimanet_act_leaky_relu(const dimanet *ann, double x) {
    return x > 0 ? x : ann->leaky_relu_alpha * x;
}

double dimanet_act_elu(const dimanet *ann, double x) {
    return x > 0 ? x : ann->elu_alpha * (exp(x) - 1.0);
}

double dimanet_act_softplus(const dimanet *ann, double x) {
    (void)ann;
    if (x > 30.0)
        return x;
    return log1p(exp(x));
}

double dimanet_act_silu(const dimanet *ann, double x) {
    (void)ann;
    return x / (1.0 + exp(-x));
}

double dimanet_act_gelu(const dimanet *ann, double x) {
    (void)ann;
    const double c = sqrt(2.0 / DIMANET_PI);
    return 0.5 * x * (1.0 + tanh(c * (x + 0.044715 * x * x * x)));
}

/* Derivative of an activation function evaluated at a neuron whose
 * pre-activation sum is `sum` and post-activation output is `out`.
 *
 * Custom activation functions are not recognized; the sigmoid
 * derivative is used as a fallback. */
static double activation_derivative(const dimanet *ann, dimanet_actfun act,
                                    double sum, double out) {
    if (act == dimanet_act_linear)
        return 1.0;
    if (act == dimanet_act_sigmoid || act == dimanet_act_sigmoid_cached)
        return out * (1.0 - out);
    if (act == dimanet_act_tanh)
        return 1.0 - out * out;
    if (act == dimanet_act_relu)
        return sum > 0 ? 1.0 : 0.0;
    if (act == dimanet_act_leaky_relu)
        return sum > 0 ? 1.0 : ann->leaky_relu_alpha;
    if (act == dimanet_act_elu)
        return sum > 0 ? 1.0 : ann->elu_alpha * exp(sum);
    if (act == dimanet_act_softplus) {
        if (sum > 30.0)
            return 1.0;
        return 1.0 - exp(-out);
    }
    if (act == dimanet_act_silu) {
        const double s = 1.0 / (1.0 + exp(-sum));
        return s + sum * s * (1.0 - s);
    }
    if (act == dimanet_act_gelu) {
        const double c = sqrt(2.0 / DIMANET_PI);
        const double g = sum + 0.044715 * sum * sum * sum;
        const double t = tanh(c * g);
        const double dg = 1.0 + 3.0 * 0.044715 * sum * sum;
        return 0.5 * (1.0 + t) + 0.5 * sum * (1.0 - t * t) * c * dg;
    }
    return out * (1.0 - out);
}

/* ------------------------------------------------------------------ */
/* Memory layout                                                       */
/* ------------------------------------------------------------------ */

/* Every network is one contiguous allocation: the struct followed by the
 * weight, sum, output, delta, velocity and gradient buffers. */
static size_t blob_size(const dimanet *ann) {
    return sizeof(dimanet)
         + (size_t)ann->total_weights * sizeof(double)              /* weight */
         + (size_t)ann->total_neurons * sizeof(double)              /* sum */
         + (size_t)ann->total_neurons * sizeof(double)              /* output */
         + (size_t)(ann->total_neurons - ann->inputs) * sizeof(double) /* delta */
         + (size_t)ann->total_weights * sizeof(double)              /* velocity */
         + (size_t)ann->total_weights * sizeof(double);             /* gradient */
}

static void link_buffers(dimanet *ann) {
    double *p = (double *)((char *)ann + sizeof(dimanet));
    ann->weight = p; p += ann->total_weights;
    ann->sum = p;    p += ann->total_neurons;
    ann->output = p; p += ann->total_neurons;
    ann->delta = p;  p += ann->total_neurons - ann->inputs;
    ann->velocity = p; p += ann->total_weights;
    ann->gradient = p;
}

/* ------------------------------------------------------------------ */
/* Lifecycle                                                           */
/* ------------------------------------------------------------------ */

dimanet *dimanet_init(int inputs, int hidden_layers, int hidden, int outputs) {
    if (inputs < 1 || outputs < 1 || hidden_layers < 0)
        return NULL;
    if (hidden_layers > 0 && hidden < 1)
        return NULL;

    const long long hidden_weights =
        hidden_layers
            ? (long long)(inputs + 1) * hidden
              + (long long)(hidden_layers - 1) * (hidden + 1) * hidden
            : 0;
    const long long output_weights =
        (long long)(hidden_layers ? (hidden + 1) : (inputs + 1)) * outputs;
    const long long total_weights = hidden_weights + output_weights;
    const long long total_neurons =
        (long long)inputs + (long long)hidden * hidden_layers + outputs;

    if (total_weights <= 0 || total_neurons <= 0 ||
        total_weights > INT_MAX || total_neurons > INT_MAX)
        return NULL;

    dimanet proto;
    memset(&proto, 0, sizeof(proto));
    proto.inputs = inputs;
    proto.hidden_layers = hidden_layers;
    proto.hidden = hidden;
    proto.outputs = outputs;
    proto.total_weights = (int)total_weights;
    proto.total_neurons = (int)total_neurons;

    dimanet *ann = malloc(blob_size(&proto));
    if (!ann)
        return NULL;

    memcpy(ann, &proto, sizeof(proto));
    link_buffers(ann);

    ann->leaky_relu_alpha = 0.01;
    ann->elu_alpha = 1.0;
    ann->momentum = 0.0;
    ann->gradient_clip = 0.0;
    ann->activation_hidden = dimanet_act_sigmoid_cached;
    ann->activation_output = dimanet_act_sigmoid_cached;

    memset(ann->velocity, 0, sizeof(double) * (size_t)ann->total_weights);
    memset(ann->gradient, 0, sizeof(double) * (size_t)ann->total_weights);

    dimanet_randomize(ann);
    return ann;
}

dimanet *dimanet_copy(const dimanet *ann) {
    dimanet *ret = malloc(blob_size(ann));
    if (!ret)
        return NULL;

    memcpy(ret, ann, blob_size(ann));
    link_buffers(ret);
    return ret;
}

void dimanet_free(dimanet *ann) {
    free(ann);
}

/* ------------------------------------------------------------------ */
/* Weight initialization                                               */
/* ------------------------------------------------------------------ */

void dimanet_init_uniform(dimanet *ann, double lo, double hi) {
    for (int i = 0; i < ann->total_weights; ++i)
        ann->weight[i] = lo + (hi - lo) * next_double();
}

void dimanet_init_normal(dimanet *ann, double mean, double stddev) {
    for (int i = 0; i < ann->total_weights; ++i) {
        double u1 = next_double();
        double u2 = next_double();
        if (u1 < 1e-12)
            u1 = 1e-12;
        const double z = sqrt(-2.0 * log(u1)) * cos(2.0 * DIMANET_PI * u2);
        ann->weight[i] = mean + stddev * z;
    }
}

void dimanet_randomize(dimanet *ann) {
    dimanet_init_uniform(ann, -0.5, 0.5);
}

/* Fills one fully-connected block of weights (bias + prev_size
 * connections per neuron) with uniform values in [-bound, bound].
 * Biases are initialized to zero. */
static void init_layer(double *w, int prev_size, int num_neurons,
                       double bound) {
    for (int j = 0; j < num_neurons; ++j) {
        *w++ = 0.0;
        for (int k = 0; k < prev_size; ++k)
            *w++ = (next_double() * 2.0 - 1.0) * bound;
    }
}

typedef enum {
    SCHEME_XAVIER,
    SCHEME_HE,
    SCHEME_LECUN
} init_scheme;

static double scheme_bound(init_scheme scheme, int fan_in, int fan_out) {
    switch (scheme) {
        case SCHEME_XAVIER: return sqrt(6.0 / (fan_in + fan_out));
        case SCHEME_HE:     return sqrt(6.0 / fan_in);
        case SCHEME_LECUN:  return sqrt(3.0 / fan_in);
    }
    return 0.0;
}

static void init_by_scheme(dimanet *ann, init_scheme scheme) {
    double *w = ann->weight;
    int prev = ann->inputs;

    for (int h = 0; h < ann->hidden_layers; ++h) {
        const double bound = scheme_bound(scheme, prev, ann->hidden);
        init_layer(w, prev, ann->hidden, bound);
        w += (prev + 1) * ann->hidden;
        prev = ann->hidden;
    }

    const double bound = scheme_bound(scheme, prev, ann->outputs);
    init_layer(w, prev, ann->outputs, bound);
    w += (prev + 1) * ann->outputs;

    assert(w - ann->weight == ann->total_weights);
}

void dimanet_init_xavier(dimanet *ann) { init_by_scheme(ann, SCHEME_XAVIER); }
void dimanet_init_he(dimanet *ann)     { init_by_scheme(ann, SCHEME_HE); }
void dimanet_init_lecun(dimanet *ann)  { init_by_scheme(ann, SCHEME_LECUN); }

/* ------------------------------------------------------------------ */
/* Forward pass                                                        */
/* ------------------------------------------------------------------ */

const double *dimanet_run(dimanet *ann, const double *inputs) {
    memcpy(ann->output, inputs, sizeof(double) * (size_t)ann->inputs);

    const double *w = ann->weight;
    double *o = ann->output + ann->inputs;
    double *s = ann->sum + ann->inputs;
    int prev = ann->inputs;

    for (int h = 0; h < ann->hidden_layers; ++h) {
        const double *src = h == 0 ? ann->output : o - ann->hidden;
        for (int j = 0; j < ann->hidden; ++j) {
            double sum = *w++ * -1.0;
            for (int k = 0; k < prev; ++k)
                sum += *w++ * src[k];
            *s++ = sum;
            *o++ = ann->activation_hidden(ann, sum);
        }
        prev = ann->hidden;
    }

    const double *src = ann->hidden_layers ? o - ann->hidden : ann->output;
    const double *ret = o;
    for (int j = 0; j < ann->outputs; ++j) {
        double sum = *w++ * -1.0;
        for (int k = 0; k < prev; ++k)
            sum += *w++ * src[k];
        *s++ = sum;
        *o++ = ann->activation_output(ann, sum);
    }

    assert(w - ann->weight == ann->total_weights);
    assert(o - ann->output == ann->total_neurons);

    return ret;
}

/* ------------------------------------------------------------------ */
/* Backpropagation                                                     */
/* ------------------------------------------------------------------ */

void dimanet_train(dimanet *ann, const double *inputs,
                   const double *desired_outputs, double learning_rate) {
    dimanet_run(ann, inputs);

    const int hidden = ann->hidden;
    const int first_output = ann->inputs + hidden * ann->hidden_layers;

    /* Output layer deltas. */
    {
        const double *out = ann->output + first_output;
        const double *sum = ann->sum + first_output;
        double *d = ann->delta + hidden * ann->hidden_layers;
        for (int j = 0; j < ann->outputs; ++j) {
            const double err = desired_outputs[j] - out[j];
            d[j] = err * activation_derivative(ann, ann->activation_output,
                                               sum[j], out[j]);
        }
    }

    /* Hidden layer deltas, last layer first. */
    for (int h = ann->hidden_layers - 1; h >= 0; --h) {
        const double *out = ann->output + ann->inputs + h * hidden;
        const double *sum = ann->sum + ann->inputs + h * hidden;
        double *d = ann->delta + h * hidden;

        const double *dd = ann->delta + (h + 1) * hidden;
        const int next_units = h == ann->hidden_layers - 1
                                   ? ann->outputs
                                   : hidden;
        const double *ww = ann->weight
                         + (ann->inputs + 1) * hidden
                         + (hidden + 1) * hidden * h;

        for (int j = 0; j < hidden; ++j) {
            double acc = 0.0;
            for (int k = 0; k < next_units; ++k)
                acc += dd[k] * ww[k * (hidden + 1) + (j + 1)];
            d[j] = acc * activation_derivative(ann, ann->activation_hidden,
                                               sum[j], out[j]);
        }
    }

    /* Build the gradient vector in weight order. */
    {
        double *g = ann->gradient;
        int prev = ann->inputs;

        for (int h = 0; h < ann->hidden_layers; ++h) {
            const double *src = h == 0
                ? ann->output
                : ann->output + ann->inputs + (h - 1) * hidden;
            const double *d = ann->delta + h * hidden;
            for (int j = 0; j < hidden; ++j) {
                *g++ = d[j] * -1.0;
                for (int k = 0; k < prev; ++k)
                    *g++ = d[j] * src[k];
            }
            prev = hidden;
        }

        const double *src = ann->hidden_layers
            ? ann->output + ann->inputs + (ann->hidden_layers - 1) * hidden
            : ann->output;
        const double *d = ann->delta + hidden * ann->hidden_layers;
        for (int j = 0; j < ann->outputs; ++j) {
            *g++ = d[j] * -1.0;
            for (int k = 0; k < prev; ++k)
                *g++ = d[j] * src[k];
        }

        assert(g - ann->gradient == ann->total_weights);
    }

    /* Optional global L2 gradient clipping. */
    if (ann->gradient_clip > 0.0) {
        double norm = 0.0;
        for (int i = 0; i < ann->total_weights; ++i)
            norm += ann->gradient[i] * ann->gradient[i];
        norm = sqrt(norm);
        if (norm > ann->gradient_clip && norm > 0.0) {
            const double scale = ann->gradient_clip / norm;
            for (int i = 0; i < ann->total_weights; ++i)
                ann->gradient[i] *= scale;
        }
    }

    /* Apply the updates, optionally with momentum. */
    if (ann->momentum > 0.0) {
        for (int i = 0; i < ann->total_weights; ++i) {
            ann->velocity[i] =
                ann->momentum * ann->velocity[i] + learning_rate * ann->gradient[i];
            ann->weight[i] += ann->velocity[i];
        }
    } else {
        for (int i = 0; i < ann->total_weights; ++i)
            ann->weight[i] += learning_rate * ann->gradient[i];
    }
}

/* ------------------------------------------------------------------ */
/* Persistence                                                         */
/* ------------------------------------------------------------------ */

static int act_id(dimanet_actfun f) {
    if (f == dimanet_act_linear)        return 1;
    if (f == dimanet_act_threshold)     return 2;
    if (f == dimanet_act_sigmoid)       return 3;
    if (f == dimanet_act_sigmoid_cached)return 4;
    if (f == dimanet_act_tanh)          return 5;
    if (f == dimanet_act_relu)          return 6;
    if (f == dimanet_act_leaky_relu)    return 7;
    if (f == dimanet_act_elu)           return 8;
    if (f == dimanet_act_softplus)      return 9;
    if (f == dimanet_act_silu)          return 10;
    if (f == dimanet_act_gelu)          return 11;
    return 0;
}

static dimanet_actfun act_from_id(int id) {
    switch (id) {
        case 1:  return dimanet_act_linear;
        case 2:  return dimanet_act_threshold;
        case 3:  return dimanet_act_sigmoid;
        case 4:  return dimanet_act_sigmoid_cached;
        case 5:  return dimanet_act_tanh;
        case 6:  return dimanet_act_relu;
        case 7:  return dimanet_act_leaky_relu;
        case 8:  return dimanet_act_elu;
        case 9:  return dimanet_act_softplus;
        case 10: return dimanet_act_silu;
        case 11: return dimanet_act_gelu;
        default: return NULL;
    }
}

int dimanet_write(const dimanet *ann, FILE *out) {
    if (!out)
        return -1;

    if (fprintf(out, "dimanet_v3 %d %d %d %d\n",
                ann->inputs, ann->hidden_layers, ann->hidden,
                ann->outputs) < 0)
        return -1;
    if (fprintf(out, "%d %d\n", act_id(ann->activation_hidden),
                act_id(ann->activation_output)) < 0)
        return -1;
    if (fprintf(out, "%.17g %.17g %.17g %.17g\n",
                ann->leaky_relu_alpha, ann->elu_alpha, ann->momentum,
                ann->gradient_clip) < 0)
        return -1;

    for (int i = 0; i < ann->total_weights; ++i)
        if (fprintf(out, "%.17g\n", ann->weight[i]) < 0)
            return -1;

    return 0;
}

dimanet *dimanet_read(FILE *in) {
    if (!in)
        return NULL;

    char tag[32];
    int inputs, hidden_layers, hidden, outputs;
    int hidden_act, output_act;
    double leaky_alpha, elu_alpha, momentum, clip;

    if (fscanf(in, "%31s", tag) != 1)
        return NULL;
    if (strcmp(tag, "dimanet_v3") != 0)
        return NULL;
    if (fscanf(in, "%d %d %d %d", &inputs, &hidden_layers, &hidden,
               &outputs) != 4)
        return NULL;
    if (fscanf(in, "%d %d", &hidden_act, &output_act) != 2)
        return NULL;
    if (fscanf(in, "%lf %lf %lf %lf", &leaky_alpha, &elu_alpha, &momentum,
               &clip) != 4)
        return NULL;

    dimanet_actfun hfun = act_from_id(hidden_act);
    dimanet_actfun ofun = act_from_id(output_act);
    if (!hfun || !ofun)
        return NULL;

    dimanet *ann = dimanet_init(inputs, hidden_layers, hidden, outputs);
    if (!ann)
        return NULL;

    ann->activation_hidden = hfun;
    ann->activation_output = ofun;
    ann->leaky_relu_alpha = leaky_alpha;
    ann->elu_alpha = elu_alpha;
    ann->momentum = momentum;
    ann->gradient_clip = clip;

    for (int i = 0; i < ann->total_weights; ++i) {
        if (fscanf(in, "%lf", &ann->weight[i]) != 1) {
            dimanet_free(ann);
            return NULL;
        }
    }

    return ann;
}

/* ------------------------------------------------------------------ */
/* Configuration                                                       */
/* ------------------------------------------------------------------ */

void dimanet_set_activation_functions(dimanet *ann, dimanet_actfun hidden,
                                      dimanet_actfun output) {
    ann->activation_hidden = hidden;
    ann->activation_output = output;
}

void dimanet_set_leaky_relu_alpha(dimanet *ann, double alpha) {
    ann->leaky_relu_alpha = alpha;
}

void dimanet_set_elu_alpha(dimanet *ann, double alpha) {
    ann->elu_alpha = alpha;
}

void dimanet_set_momentum(dimanet *ann, double momentum) {
    ann->momentum = momentum;
}

void dimanet_set_gradient_clip(dimanet *ann, double clip) {
    ann->gradient_clip = clip;
}
