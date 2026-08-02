#include <stdio.h>
#include <stdlib.h>

#include "dimanet.h"

static const char *default_path = "examples/xor.ann";

static const double inputs[4][2] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};

int main(int argc, char *argv[]) {
    const char *path = argc > 1 ? argv[1] : default_path;

    printf("DimaNet example 3: load and run a network saved by "
           "dimanet_write.\n");

    FILE *in = fopen(path, "r");
    if (!in) {
        fprintf(stderr, "could not open %s\n", path);
        return 1;
    }

    dimanet *ann = dimanet_read(in);
    fclose(in);
    if (!ann) {
        fprintf(stderr, "failed to parse %s\n", path);
        return 1;
    }

    for (int i = 0; i < 4; ++i) {
        const double out = *dimanet_run(ann, inputs[i]);
        printf("xor(%d, %d) = %.3f\n", (int)inputs[i][0],
               (int)inputs[i][1], out);
    }

    dimanet_free(ann);
    return 0;
}
