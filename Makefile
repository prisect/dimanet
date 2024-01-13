CFLAGS = -Wall -Wshadow -O3 -g -march=native
LDLIBS = -lm

all: check example_1 example_2 example_3 example_4

sigmoid: CFLAGS += -Ddimanet_act=dimanet_act_sigmoid_cached
sigmoid: all

threshold: CFLAGS += -Ddimanet_act=dimanet_act_threshold
threshold: all

linear: CFLAGS += -Ddimanet_act=dimanet_act_linear
linear: all

test: test.o dimanet.o
check: test
	./$^

example1: examples/example_1.o dimanet.o
example2: examples/example_2.o dimanet.o
example3: examples/example_3.o dimanet.o
example4: examples/example_4.o dimanet.o

clean:
	$(RM) *.o
	$(RM) test example_1 example_2 example_3 example_4 *.exe
	$(RM) persist.txt

.PHONY: sigmoid threshold linear clean
