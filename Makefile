CFLAGS = -Wall -Wshadow -O3 -g -march=native
LDLIBS = -lm

all: check example1 example2 example3 example4

sigmoid: CFLAGS += -Ddimanet_act=dimanet_act_sigmoid_cached
sigmoid: all

threshold: CFLAGS += -Ddimanet_act=dimanet_act_threshold
threshold: all

linear: CFLAGS += -Ddimanet_act=dimanet_act_linear
linear: all

debug: debug.o dimanet.o
check: debug
	./$^

example1: examples/example_1.o dimanet.o
	chmod +x examples/example_*.o  # Add this line to set execute permission for all example_*.o files
example2: examples/example_2.o dimanet.o
example3: examples/example_3.o dimanet.o
example4: examples/example_4.o dimanet.o

clean:
	$(RM) *.o
	$(RM) debug example1 example2 example3 example4 *.exe
	$(RM) persist.txt

.PHONY: sigmoid threshold linear clean
