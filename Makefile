# License-Identifier: GPL-3.0
# VERSION: 1.2
# NAME: DimaNet

# DOCUMENTATION
# ----------------------------------------
# To see the list of typical targets, or and to view the library's
# info, execute "make help". Comments in this file are targeted
# only to the developer, do not expect to learn how to build or to
# use the library.
# More info can be located in the ./README file.

CFLAGS = -Wall -Wshadow -O3 -g -march=native
LDLIBS = -lm
EXAMPLES_DIR = examples

EXAMPLES = $(EXAMPLES_DIR)/example1 $(EXAMPLES_DIR)/example2 $(EXAMPLES_DIR)/example3 $(EXAMPLES_DIR)/example4 $(EXAMPLES_DIR)/example5

all: build

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

help:
	@echo "Cleaning targets:"
	@echo "  clean    | Remove all compiled and generated files from the entire directory"
	@echo "  exclean  | Remove only examples generated files"
	@echo ""
	@echo "Make targets:"
	@echo "  examples | Make all the examples"
	@echo "  build    | Build the main file, E.G: main.c"
	@echo ""
	@echo "Documentation targets:"
	@echo "  help     | You should know what this does, you ran it to display this message idiot"

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

exclean:
	$(RM) $(EXAMPLES_DIR)/*.o

clean:
	$(RM) *.o
	$(RM) persist.txt
	$(RM) main
	$(RM) $(EXAMPLES_DIR)/*.o

.PHONY: sigmoid threshold linear clean build examples help
