#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dimanet.h"
#include "libdntest.h"

static const double xor_input[4][2] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
static const double xor_output[4] = {0, 1, 1, 0};

static double squared_error(dimanet *ann, const double *in,
                            const double *target) {
    const double pred = *dimanet_run(ann, in);
    const double err = pred - target[0];
    return 0.5 * err * err;
}

/* ------------------------------------------------------------------ */

static void test_basic(void) {
    dimanet *ann = dimanet_init(1, 0, 0, 1);
    lequal(ann->total_weights, 2);

    ann->weight[0] = 0;
    ann->weight[1] = 0;
    lfequal(0.5, *dimanet_run(ann, &(double){0}));
    lfequal(0.5, *dimanet_run(ann, &(double){11}));

    ann->weight[0] = 1;
    ann->weight[1] = 1;
    lfequal(1.0, *dimanet_run(ann, &(double){10}));
    lfequal(0.0, *dimanet_run(ann, &(double){-10}));

    dimanet_free(ann);
}

static void test_xor_handcrafted(void) {
    dimanet *ann = dimanet_init(2, 1, 2, 1);
    dimanet_set_activation_functions(ann, dimanet_act_threshold,
                                     dimanet_act_threshold);
    lequal(ann->total_weights, 9);

    /* First hidden neuron. */
    ann->weight[0] = 0.5;
    ann->weight[1] = 1.0;
    ann->weight[2] = 1.0;
    /* Second hidden neuron. */
    ann->weight[3] = 1.0;
    ann->weight[4] = 1.0;
    ann->weight[5] = 1.0;
    /* Output. */
    ann->weight[6] = 0.5;
    ann->weight[7] = 1.0;
    ann->weight[8] = -1.0;

    for (int i = 0; i < 4; ++i)
        lfequal(xor_output[i], *dimanet_run(ann, xor_input[i]));

    dimanet_free(ann);
}

static void test_forward_linear(void) {
    /* A linear single-layer network: output = -bias + w0*x0 + w1*x1. */
    dimanet *ann = dimanet_init(2, 0, 0, 1);
    dimanet_set_activation_functions(ann, dimanet_act_linear,
                                     dimanet_act_linear);

    ann->weight[0] = 0.5;
    ann->weight[1] = 1.0;
    ann->weight[2] = 2.0;

    const double in[2] = {0.3, -0.7};
    lfequal(-0.5 + 0.3 + 2.0 * -0.7, *dimanet_run(ann, in));

    dimanet_free(ann);
}

static void test_backprop(void) {
    dimanet_seed(1);
    dimanet *ann = dimanet_init(1, 0, 0, 1);

    const double input = 0.5;
    const double target = 1.0;

    const double before = fabs(*dimanet_run(ann, &input) - target);
    dimanet_train(ann, &input, &target, 0.5);
    const double after = fabs(*dimanet_run(ann, &input) - target);

    lok(before > after);

    dimanet_free(ann);
}

static void train_logic_gate(dimanet *ann, const double *truth_table,
                             int epochs, double lr) {
    for (int e = 0; e < epochs; ++e)
        for (int i = 0; i < 4; ++i)
            dimanet_train(ann, xor_input[i], truth_table + i, lr);
}

static void test_train_and(void) {
    static const double output[4] = {0, 0, 0, 1};
    dimanet_seed(2);
    dimanet *ann = dimanet_init(2, 0, 0, 1);
    train_logic_gate(ann, output, 50, 0.8);
    dimanet_set_activation_functions(ann, dimanet_act_linear,
                                     dimanet_act_threshold);
    for (int i = 0; i < 4; ++i)
        lfequal(output[i], *dimanet_run(ann, xor_input[i]));
    dimanet_free(ann);
}

static void test_train_or(void) {
    static const double output[4] = {0, 1, 1, 1};
    dimanet_seed(3);
    dimanet *ann = dimanet_init(2, 0, 0, 1);
    dimanet_randomize(ann);
    train_logic_gate(ann, output, 50, 0.8);
    dimanet_set_activation_functions(ann, dimanet_act_linear,
                                     dimanet_act_threshold);
    for (int i = 0; i < 4; ++i)
        lfequal(output[i], *dimanet_run(ann, xor_input[i]));
    dimanet_free(ann);
}

static void test_train_xor(void) {
    /* XOR is a genuinely hard problem for plain SGD on a small network;
     * like any real training run, restart with fresh weights until it
     * learns. tanh outputs are thresholded at 0.5. */
    int learned = 0;
    for (int attempt = 0; attempt < 8 && !learned; ++attempt) {
        dimanet_seed(100 + (uint32_t)attempt);
        dimanet *ann = dimanet_init(2, 1, 2, 1);
        dimanet_set_activation_functions(ann, dimanet_act_tanh, dimanet_act_tanh);
        for (int e = 0; e < 500; ++e)
            for (int i = 0; i < 4; ++i)
                dimanet_train(ann, xor_input[i], xor_output + i, 1.0);

        learned = 1;
        for (int i = 0; i < 4; ++i) {
            const double r = *dimanet_run(ann, xor_input[i]);
            if ((r > 0.5) != (xor_output[i] > 0.5)) {
                learned = 0;
                break;
            }
        }
        dimanet_free(ann);
    }
    lok(learned);
}

static void test_copy(void) {
    dimanet *first = dimanet_init(1000, 5, 50, 10);
    dimanet *second = dimanet_copy(first);

    lequal(first->inputs, second->inputs);
    lequal(first->hidden_layers, second->hidden_layers);
    lequal(first->hidden, second->hidden);
    lequal(first->outputs, second->outputs);
    lequal(first->total_weights, second->total_weights);

    for (int i = 0; i < first->total_weights; ++i)
        lfequal(first->weight[i], second->weight[i]);

    /* Copy must be independent. */
    second->weight[0] += 1.0;
    lok(first->weight[0] != second->weight[0]);

    dimanet_free(first);
    dimanet_free(second);
}

static void test_persist(void) {
    dimanet *first = dimanet_init(1000, 5, 50, 10);

    FILE *out = tmpfile();
    lequal(dimanet_write(first, out), 0);

    rewind(out);
    dimanet *second = dimanet_read(out);
    fclose(out);

    lok(second != NULL);
    if (!second) {
        dimanet_free(first);
        return;
    }

    lequal(first->inputs, second->inputs);
    lequal(first->hidden_layers, second->hidden_layers);
    lequal(first->hidden, second->hidden);
    lequal(first->outputs, second->outputs);
    lequal(first->total_weights, second->total_weights);

    for (int i = 0; i < first->total_weights; ++i)
        lok(first->weight[i] == second->weight[i]);

    dimanet_free(first);
    dimanet_free(second);
}

static void test_persist_metadata(void) {
    dimanet *first = dimanet_init(3, 2, 4, 2);
    dimanet_set_activation_functions(first, dimanet_act_tanh,
                                     dimanet_act_relu);
    dimanet_set_leaky_relu_alpha(first, 0.2);
    dimanet_set_elu_alpha(first, 1.5);
    dimanet_set_momentum(first, 0.9);
    dimanet_set_gradient_clip(first, 5.0);

    FILE *out = tmpfile();
    lequal(dimanet_write(first, out), 0);

    rewind(out);
    dimanet *second = dimanet_read(out);
    fclose(out);

    lok(second != NULL);
    if (!second) {
        dimanet_free(first);
        return;
    }

    lok(second->activation_hidden == dimanet_act_tanh);
    lok(second->activation_output == dimanet_act_relu);
    lfequal(second->leaky_relu_alpha, 0.2);
    lfequal(second->elu_alpha, 1.5);
    lfequal(second->momentum, 0.9);
    lfequal(second->gradient_clip, 5.0);

    for (int i = 0; i < first->total_weights; ++i)
        lok(first->weight[i] == second->weight[i]);

    dimanet_free(first);
    dimanet_free(second);
}

static void test_read_garbage(void) {
    FILE *bad = tmpfile();
    fputs("not a dimanet file\n", bad);
    rewind(bad);
    lok(dimanet_read(bad) == NULL);
    fclose(bad);
}

static void test_init_invalid(void) {
    lok(dimanet_init(0, 0, 0, 1) == NULL);
    lok(dimanet_init(1, 0, 0, 0) == NULL);
    lok(dimanet_init(1, -1, 0, 1) == NULL);
    lok(dimanet_init(1, 1, 0, 1) == NULL);

    dimanet *ann = dimanet_init(1, 1, 2, 1);
    lok(ann != NULL);
    dimanet_free(ann);
}

static void test_sigmoid(void) {
    for (double x = -20.0; x < 20.0; x += 0.01)
        lfequal(dimanet_act_sigmoid(NULL, x),
                dimanet_act_sigmoid_cached(NULL, x));
}

static void test_activation_values(void) {
    dimanet dummy;
    memset(&dummy, 0, sizeof dummy);
    dummy.leaky_relu_alpha = 0.01;
    dummy.elu_alpha = 1.0;

    lfequal(2.5, dimanet_act_linear(&dummy, 2.5));
    lfequal(1.0, dimanet_act_threshold(&dummy, 1e-6));
    lfequal(0.0, dimanet_act_threshold(&dummy, -1e-6));
    lfequal(0.5, dimanet_act_sigmoid(&dummy, 0.0));
    lfequal(tanh(1.0), dimanet_act_tanh(&dummy, 1.0));
    lfequal(5.0, dimanet_act_relu(&dummy, 5.0));
    lfequal(0.0, dimanet_act_relu(&dummy, -5.0));
    lfequal(-0.05, dimanet_act_leaky_relu(&dummy, -5.0));
    lfequal(5.0, dimanet_act_leaky_relu(&dummy, 5.0));
    lfequal(exp(-5.0) - 1.0, dimanet_act_elu(&dummy, -5.0));
    lfequal(5.0, dimanet_act_elu(&dummy, 5.0));
    lfequal(log(2.0), dimanet_act_softplus(&dummy, 0.0));
    lfequal(0.0, dimanet_act_silu(&dummy, 0.0));
    lfequal(0.0, dimanet_act_gelu(&dummy, 0.0));
    lfequal(100.0, dimanet_act_gelu(&dummy, 100.0));
    lfequal(0.0, dimanet_act_gelu(&dummy, -100.0));

    /* ELU alpha should be configurable. */
    dummy.elu_alpha = 0.5;
    lfequal(-0.5 * 0.6321, dimanet_act_elu(&dummy, -1.0));
}

static void test_setters(void) {
    dimanet *ann = dimanet_init(2, 1, 2, 1);

    dimanet_set_activation_functions(ann, dimanet_act_relu, dimanet_act_linear);
    lok(ann->activation_hidden == dimanet_act_relu);
    lok(ann->activation_output == dimanet_act_linear);

    dimanet_set_leaky_relu_alpha(ann, 0.5);
    lfequal(0.5, ann->leaky_relu_alpha);
    dimanet_set_elu_alpha(ann, 1.25);
    lfequal(1.25, ann->elu_alpha);
    dimanet_set_momentum(ann, 0.9);
    lfequal(0.9, ann->momentum);
    dimanet_set_gradient_clip(ann, 2.0);
    lfequal(2.0, ann->gradient_clip);

    dimanet_free(ann);
}

static void test_train_modern_activations(void) {
    /* ReLU + linear output must stay finite while learning XOR. */
    dimanet_seed(5);
    dimanet *ann = dimanet_init(2, 1, 3, 1);
    dimanet_set_activation_functions(ann, dimanet_act_relu,
                                     dimanet_act_linear);
    dimanet_set_gradient_clip(ann, 1.0);
    train_logic_gate(ann, xor_output, 100, 0.05);
    for (int i = 0; i < 4; ++i) {
        const double r = *dimanet_run(ann, xor_input[i]);
        lok(!isnan(r) && !isinf(r));
    }
    dimanet_free(ann);

    /* Tanh outputs must stay in [-1, 1]. */
    ann = dimanet_init(2, 1, 3, 1);
    dimanet_set_activation_functions(ann, dimanet_act_tanh, dimanet_act_tanh);
    train_logic_gate(ann, xor_output, 100, 0.05);
    for (int i = 0; i < 4; ++i) {
        const double r = *dimanet_run(ann, xor_input[i]);
        lok(!isnan(r) && r >= -1.0 && r <= 1.0);
    }
    dimanet_free(ann);
}

static void test_seed(void) {
    dimanet_seed(42);
    dimanet *a = dimanet_init(4, 2, 5, 3);
    dimanet_seed(42);
    dimanet *b = dimanet_init(4, 2, 5, 3);
    dimanet_seed(43);
    dimanet *c = dimanet_init(4, 2, 5, 3);

    for (int i = 0; i < a->total_weights; ++i) {
        lok(a->weight[i] == b->weight[i]);
        lok(a->weight[i] != c->weight[i]);
    }

    dimanet_free(a);
    dimanet_free(b);
    dimanet_free(c);
}

static void test_initializers(void) {
    dimanet_seed(7);

    dimanet *ann = dimanet_init(4, 2, 5, 3);

    dimanet_init_xavier(ann);
    for (int i = 0; i < ann->total_weights; ++i)
        lok(fabs(ann->weight[i]) <= sqrt(6.0 / 8.0) + 1e-9);

    dimanet_init_he(ann);
    for (int i = 0; i < ann->total_weights; ++i)
        lok(fabs(ann->weight[i]) <= sqrt(6.0 / 4.0) + 1e-9);

    dimanet_init_lecun(ann);
    for (int i = 0; i < ann->total_weights; ++i)
        lok(fabs(ann->weight[i]) <= sqrt(3.0 / 4.0) + 1e-9);

    /* Biases must be exactly zero; first weight of every block. */
    dimanet_init_xavier(ann);
    lequal(ann->weight[0], 0.0);
    lequal(ann->weight[5], 0.0);
    lequal(ann->weight[10], 0.0);
    lequal(ann->weight[15], 0.0);
    lequal(ann->weight[20], 0.0);
    lequal(ann->weight[25], 0.0);
    lequal(ann->weight[31], 0.0);
    lequal(ann->weight[37], 0.0);
    lequal(ann->weight[43], 0.0);
    lequal(ann->weight[49], 0.0);
    lequal(ann->weight[55], 0.0);
    lequal(ann->weight[61], 0.0);
    lequal(ann->weight[67], 0.0);

    dimanet_free(ann);
}

static void test_momentum(void) {
    dimanet_seed(6);
    dimanet *ann = dimanet_init(2, 1, 3, 1);
    dimanet_set_activation_functions(ann, dimanet_act_tanh, dimanet_act_tanh);
    dimanet_set_momentum(ann, 0.9);

    const double before = squared_error(ann, xor_input[1], &(double){1.0});
    for (int e = 0; e < 200; ++e)
        for (int i = 0; i < 4; ++i)
            dimanet_train(ann, xor_input[i], xor_output + i, 0.1);

    const double after = squared_error(ann, xor_input[1], &(double){1.0});

    lok(before > after);
    lok(!isnan(after) && !isinf(after));

    /* The velocity buffer should be in use. */
    int used = 0;
    for (int i = 0; i < ann->total_weights; ++i)
        if (ann->velocity[i] != 0.0)
            used = 1;
    lok(used);

    dimanet_free(ann);
}

static void test_gradient_clip(void) {
    /* A large learning rate with ReLU would normally explode. */
    dimanet_seed(8);
    dimanet *ann = dimanet_init(2, 1, 3, 1);
    dimanet_set_activation_functions(ann, dimanet_act_relu, dimanet_act_linear);
    dimanet_set_gradient_clip(ann, 1.0);

    for (int e = 0; e < 20; ++e)
        for (int i = 0; i < 4; ++i)
            dimanet_train(ann, xor_input[i], xor_output + i, 100.0);

    for (int i = 0; i < ann->total_weights; ++i)
        lok(isfinite(ann->weight[i]));

    dimanet_free(ann);
}

static void test_gradient(void) {
    /* Compare the analytic gradient computed by dimanet_train against a
     * finite-difference estimate. */
    dimanet_seed(7);
    dimanet *ann = dimanet_init(2, 1, 3, 1);
    dimanet_set_activation_functions(ann, dimanet_act_sigmoid,
                                     dimanet_act_sigmoid);

    const double in[2] = {0.3, -0.7};
    const double target[1] = {0.9};

    dimanet *probe = dimanet_copy(ann);
    dimanet_train(probe, in, target, 1.0);

    const double eps = 1e-4;
    int failures = 0;
    for (int i = 0; i < ann->total_weights; ++i) {
        const double orig = ann->weight[i];

        ann->weight[i] = orig + eps;
        const double up = squared_error(ann, in, target);
        ann->weight[i] = orig - eps;
        const double dn = squared_error(ann, in, target);
        const double numeric = (up - dn) / (2.0 * eps);

        ann->weight[i] = orig;

        /* The stored gradient is the descent direction, i.e. -dE/dw. */
        const double analytic = -(probe->weight[i] - ann->weight[i]);

        const double tol = 1e-3 * (fabs(numeric) + fabs(analytic) + 1e-6);
        if (fabs(numeric - analytic) > tol)
            ++failures;
    }

    lok(failures == 0);

    dimanet_free(probe);
    dimanet_free(ann);
}

/* ------------------------------------------------------------------ */

int main(void) {
    printf("- DIMANET TEST SUITE -\n");

    dimanet_seed(100);

    lrun("basic", test_basic);
    lrun("forward linear", test_forward_linear);
    lrun("xor (handcrafted)", test_xor_handcrafted);
    lrun("backprop", test_backprop);
    lrun("train and", test_train_and);
    lrun("train or", test_train_or);
    lrun("train xor", test_train_xor);
    lrun("copy", test_copy);
    lrun("persist", test_persist);
    lrun("persist metadata", test_persist_metadata);
    lrun("read garbage", test_read_garbage);
    lrun("init invalid", test_init_invalid);
    lrun("sigmoid vs cached", test_sigmoid);
    lrun("activation values", test_activation_values);
    lrun("setters", test_setters);
    lrun("modern activations", test_train_modern_activations);
    lrun("seed", test_seed);
    lrun("initializers", test_initializers);
    lrun("momentum", test_momentum);
    lrun("gradient clip", test_gradient_clip);
    lrun("gradient (finite diff)", test_gradient);

    lresults();

    return lfails != 0;
}
