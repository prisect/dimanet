# Makefile - dimanet main makefile
# License-Identifier: GPL-3.0
# Latest updated version: 1.2

# Contributions-welcome!

# DOCUMENTATION
# ---------------------------------------
# To see the list of typical targets, or
# and to view the library's info, execute
# "make help". Comments in this file are
# targeted only to the developer, do not
# expect to learn how to build or to use
# the library.
#
# More info can be located in the ./README

# TODO:
# Redirect all files, targets to the root
# directory, in other words, replace the
# files to the correct path they are in.

CFLAGS = -Wall -Wshadow -O3 -g -march=native
LDLIBS = -lm

EXAMPLES_DIR = examples
MODELS_DIR = models
BUILD_DIR = build
EXBUILD_DIR = $(BUILD_DIR)/examples
MOBUILD_DIR = $(BUILD_DIR)/models

EXAMPLES = $(EXAMPLES_DIR)/example1 $(EXAMPLES_DIR)/example2 $(EXAMPLES_DIR)/example3 $(EXAMPLES_DIR)/example4 $(EXAMPLES_DIR)/example5

all: start

PHONY += sigmoid
sigmoid: CFLAGS += -Ddimanet_act=dimanet_act_sigmoid_cached
sigmoid: all

PHONY += threshold
threshold: CFLAGS += -Ddimanet_act=dimanet_act_threshold
threshold: all

PHONY += linear
linear: CFLAGS += -Ddimanet_act=dimanet_act_linear
linear: all

PHONY += debug
debug: debug.o dimanet.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)
	@./debug

PHONY += examples
examples: $(EXAMPLES)

$(EXBUILD_DIR)/example1: $(EXBUILD_DIR)/example_1.o dimanet.o
	$(CC) $(CFLAGS) -o $(EXBUILD_DIR)/$@.o $^ $(LDLIBS)
$(EXBUILD_DIR)/example2: $(EXBUILD_DIR)/example_2.o dimanet.o
	$(CC) $(CFLAGS) -o $(EXBUILD_DIR)/$@.o $^ $(LDLIBS)
$(EXBUILD_DIR)/example3: $(EXBUILD_DIR)/example_3.o dimanet.o
	$(CC) $(CFLAGS) -o $(EXBUILD_DIR)/$@.o $^ $(LDLIBS)
$(EXBUILD_DIR)/example4: $(EXBUILD_DIR)/example_4.o dimanet.o
	$(CC) $(CFLAGS) -o $(EXBUILD_DIR)/$@.o $^ $(LDLIBS)
$(EXBUILD_DIR)/example5: $(EXBUILD_DIR)/example_5.o dimanet.o
	$(CC) $(CFLAGS) -o $(EXBUILD_DIR)/$@.o $^ $(LDLIBS)

PHONY += map
map: $(MODELS_DIR)/map/main.o dimanet.o
	$(CC) $(CFLAGS) -o $(MOBUILD_DIR)/map $< dimanet.o $(LDLIBS)
	@echo "Succesfully compiled 'map'."
PHONY += wheel
wheel: $(MODELS_DIR)/wheel/main.o dimanet.o
	$(CC) $(CFLAGS) -o $(MOBUILD_DIR)/wheel $< dimanet.o $(LDLIBS)
	@echo "Succesfully compiled 'wheel'."
PHONY += book
book: $(MODELS_DIR)/book/main.o dimanet.o
	$(CC) $(CFLAGS) -o $(MOBUILD_DIR)/book $< dimanet.o $(LDLIBS)
	@echo "Succesfully compiled 'book'."

PHONY += xclean
xclean:
	$(RM) $(EXBUILD_DIR)/*.o
	@echo "Succesfully cleaned examples build files."
PHONY += mclean
mclean:
	$(RM) $(MOBUILD_DIR)/*.o
	@echo "Succesfully cleaned model build files."
PHONY += bclean
bclean:
	$(RM) $(EXBUILD_DIR)/*.o
	$(RM) $(MOBUILD_DIR)/*.o
	@echo "Succesfully cleaned all build files."

PHONY += clean
clean:
	$(RM) *.o
	$(RM) persist.txt
	$(RM) main
	$(RM) debug
	$(RM) $(EXBUILD_DIR)/*.o
	$(RM) $(MODELS_DIR)/*.o
	@echo "Succesfully cleaned everything."

PHONY += start
start:
	@echo ""
	@echo "  To view all the DimaNet controller script commands, run:"
	@echo "    \033[0;32mmake help"
	@echo ""

PHONY += help
help:
	@echo ""
	@echo "  \033[1;30mCleaning targets:\033[1;37m"
	@echo "    clean    | Remove all compiled and generated files from the entire directory"
	@echo "    xclean   | Remove only examples generated files"
	@echo "    bclean   | Remove only build generated files"
	@echo "    mclean   | Remove only build generated files"
	@echo ""
	@echo "  \033[1;30mMake targets:\033[1;37m"
	@echo "    examples | Make all the examples"
#	@echo "    build    | Build the main file, E.G: main.c" <-- disabled for now
	@echo "    debug    | debug, test dimanet"
	@echo "    <model>  | Build the model, E.G: map, wheel, book"
	@echo ""
	@echo "  \033[1;30mDocumentation targets:\033[1;37m"
	@echo "    help     | You should know what this does, you ran it to output this message idiot"
	@echo ""

.PHONY: $(PHONY)
