# Examples

Small, self-contained programs that show how to use DimaNet. Each example is a
single file and links directly against `dimanet.c`.

| File | Description |
| ---- | ----------- |
| [`example_1.c`](example_1.c) | Learns the XOR function with backpropagation. |
| [`example_2.c`](example_2.c) | Learns the XOR function with random search (no gradients). |
| [`example_3.c`](example_3.c) | Loads and runs a network saved with `dimanet_write`. |
| [`example_4.c`](example_4.c) | Trains a classifier on the IRIS data set. |
| [`example_5.c`](example_5.c) | Approximates `sin(x) + cos(2x)` (regression). |

`data/iris.data` is the classic IRIS flower data set and `xor.ann` is a
pre-trained XOR network used by example 3.

## Running

With make:

```
make examples
make run        # interactive launcher
```

With CMake:

```
cmake -B build
cmake --build build
./build/example_1
```

Each example can also be run directly, e.g. `./build/example_4`. Example 3
accepts a path to a `.ann` file as an optional argument.
