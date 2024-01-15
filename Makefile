# dont judge how cool this looks

CFLAGS = -Wall -Wshadow -O3 -g -march=native
LDLIBS = -lm
EXAMPLES_DIR = examples

EXAMPLES = $(EXAMPLES_DIR)/example1 $(EXAMPLES_DIR)/example2 $(EXAMPLES_DIR)/example3 $(EXAMPLES_DIR)/example4
all: check $(EXAMPLES)

sigmoid: CFLAGS += -Ddimanet_act=dimanet_act_sigmoid_cached
sigmoid: all

threshold: CFLAGS += -Ddimanet_act=dimanet_act_threshold
threshold: all

linear: CFLAGS += -Ddimanet_act=dimanet_act_linear
linear: all

debug: debug.o dimanet.o
check: debug
	./$^


$(EXAMPLES_DIR)/example1: $(EXAMPLES_DIR)/example_1.o dimanet.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)
$(EXAMPLES_DIR)/example2: $(EXAMPLES_DIR)/example_2.o dimanet.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)
$(EXAMPLES_DIR)/example3: $(EXAMPLES_DIR)/example_3.o dimanet.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)
$(EXAMPLES_DIR)/example4: $(EXAMPLES_DIR)/example_4.o dimanet.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)
$(EXAMPLES_DIR)/example5: $(EXAMPLES_DIR)/example_5.o dimanet.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)
	
clean:
	$(RM) *.o
	$(RM) debug $(EXAMPLES) *.exe
	$(RM) persist.txt

.PHONY: sigmoid threshold linear clean
