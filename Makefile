CFLAGS = -Wall -Wshadow -O3 -g -march=native
LDLIBS = -lm
EXAMPLES_DIR = examples

EXAMPLES = $(EXAMPLES_DIR)/example1 $(EXAMPLES_DIR)/example2 $(EXAMPLES_DIR)/example3 $(EXAMPLES_DIR)/example4 $(EXAMPLES_DIR)/example5

all: test build

sigmoid: CFLAGS += -Ddimanet_act=dimanet_act_sigmoid_cached
sigmoid: all

threshold: CFLAGS += -Ddimanet_act=dimanet_act_threshold
threshold: all

linear: CFLAGS += -Ddimanet_act=dimanet_act_linear
linear: all

debug: debug.o dimanet.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

test: debug
	./debug
build: main
examples: $(EXAMPLES)

$(EXAMPLES_DIR)/example1: $(EXAMPLES_DIR)/example_1.o dimanet.o
	$(CC) $(CFLAGS) -o $@.o $^ $(LDLIBS)
$(EXAMPLES_DIR)/example2: $(EXAMPLES_DIR)/example_2.o dimanet.o
	$(CC) $(CFLAGS) -o $@.o $^ $(LDLIBS)
$(EXAMPLES_DIR)/example3: $(EXAMPLES_DIR)/example_3.o dimanet.o
	$(CC) $(CFLAGS) -o $@.o $^ $(LDLIBS)
$(EXAMPLES_DIR)/example4: $(EXAMPLES_DIR)/example_4.o dimanet.o
	$(CC) $(CFLAGS) -o $@.o $^ $(LDLIBS)
$(EXAMPLES_DIR)/example5: $(EXAMPLES_DIR)/example_5.o dimanet.o
	$(CC) $(CFLAGS) -o $@.o $^ $(LDLIBS)

main.o: main.c
	$(CC) $(CFLAGS) -c -o main.o main.c

main: main.o dimanet.o
	$(CC) $(CFLAGS) -o main main.o dimanet.o $(LDLIBS)

clean:
	$(RM) *.o
	$(RM) $(EXAMPLES_DIR)/*.o
	$(RM) persist.txt
	$(RM) main

.PHONY: sigmoid threshold linear clean compile examples
