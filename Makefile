# Makefile - dimanet main makefile
# License-Identifier: GPL-3.0
# Latest updated version: v1.3

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
MODELS_DIR = models
BUILD_DIR = build
EXBUILD_DIR = $(BUILD_DIR)/examples
MOBUILD_DIR = $(BUILD_DIR)/models
STRINGSLIB = stringslib

M1 = map
M2 = wheel
M3 = book

EXAMPLES = $(EXAMPLES_DIR)/example1 $(EXAMPLES_DIR)/example2 $(EXAMPLES_DIR)/example3 $(EXAMPLES_DIR)/example4 $(EXAMPLES_DIR)/example5

INSTALL_DIR = /usr/local
LIB_DIR = $(INSTALL_DIR)/lib
INC_DIR = $(INSTALL_DIR)/include

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

PHONY += valgrind
valgrind: valgrind
    valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./$(BUILD_DIR)/valgrind

PHONY += gen
gen: gen.c dimanet.o gen.o
    $(CC) $(CFLAGS) -c $< -o $@

PHONY += main
main: main.c $(STRINGSLIB)/strlib.c dimanet.o
    $(CC) $(CFLAGS) -o $(BUILD_DIR)/main main.c $(STRINGSLIB)/strlib.c dimanet.o $(LDLIBS) -I$(STRINGSLIB)

PHONY += compile
compile: debug.o dimanet.o
    $(CC) $(CFLAGS) -o $(BUILD_DIR)/debug $^ $(LDLIBS)
    @./$(BUILD_DIR)/debug

PHONY += strings
strings: $(STRINGSLIB)/strlib.o $(STRINGSLIB)/strlibtest.o
    $(CC) $(CFLAGS) -o $(BUILD_DIR)/strings $(STRINGSLIB)/strlib.o $(STRINGSLIB)/strlibtest.o $(LDLIBS)
    @echo ""
    @echo "Test output:"
    @./$(BUILD_DIR)/strings

PHONY += examples
examples: $(EXAMPLES)
    @echo ""
    @echo "Notice: For now, the generated files are in the examples/ directory."

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

PHONY += $(M1)
$(M1): $(MODELS_DIR)/$(M1)/main.o dimanet.o $(MODELS_DIR)/$(M1)/model.o
    $(CC) $(CFLAGS) -o $(MOBUILD_DIR)/$(M1) $< dimanet.o $(MODELS_DIR)/$(M1)/model.o $(LDLIBS)
    @echo ""
    @echo "Successfully compiled '$(M1)'."
    @echo "Running $(M1)"
    @echo ""
    @./$(MOBUILD_DIR)/$(M1)

PHONY += $(M2)
$(M2): $(MODELS_DIR)/$(M2)/main.o dimanet.o $(MODELS_DIR)/$(M2)/model.o
    $(CC) $(CFLAGS) -o $(MOBUILD_DIR)/$(M2) $< dimanet.o $(MODELS_DIR)/$(M2)/model.o $(LDLIBS)
    @echo ""
    @echo "Successfully compiled '$(M2)'."
    @echo "Running $(M2)"
    @echo ""
    @./$(MOBUILD_DIR)/$(M2)

PHONY += $(M3)
$(M3): $(MODELS_DIR)/$(M3)/main.o dimanet.o $(MODELS_DIR)/$(M3)/model.o
    $(CC) $(CFLAGS) -o $(MOBUILD_DIR)/$(M3) $< dimanet.o $(MODELS_DIR)/$(M3)/model.o $(LDLIBS)
    @echo ""
    @echo "Successfully compiled '$(M3)'."
    @echo "Running $(M3)"
    @echo ""
    @./$(MOBUILD_DIR)/$(M3)

PHONY += install
install:
    install -D -m 0644 dimanet.o $(LIB_DIR)/dimanet.o
    install -D -m 0644 dimanet.h $(INC_DIR)/dimanet.h

PHONY += uninstall
uninstall:
    rm -f $(LIB_DIR)/dimanet.o
    rm -f $(INC_DIR)/dimanet.h

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
    $(RM) gen
    $(RM) dimanet
    $(RM) compile
    $(RM) debug
    $(RM) $(EXBUILD_DIR)/*.o
    $(RM) $(MODELS_DIR)/*.o
PHONY += start
start:
	@echo ""
	@echo "  To view all the DimaNet controller script commands, run:"
	@echo "    make help"
	@echo ""

PHONY += help
help:
	@echo ""
	@echo "  Cleaning targets:"
	@echo "    clean    | Remove all compiled and generated files from the entire directory"
	@echo "    xclean   | Remove only examples generated files"
	@echo "    bclean   | Remove only build generated files"
	@echo "    mclean   | Remove only build generated files"
	@echo ""
	@echo "  Make targets:"
	@echo "    examples | Compile all examples"
	@echo "    build    | Compile dimanet"
	@echo "    strings  | Compile stringslib"
	@echo "    gen      | Compile gen"
	@echo "    compile  | Build the main file"
	@echo "    <model>  | Build the model, E.G: map, wheel, book"
	@echo ""
	@echo "  Documentation targets:"
	@echo "    help     | A command for dummies"
	@echo ""

.PHONY: $(PHONY)
