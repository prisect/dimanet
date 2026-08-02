# DimaNet build system (make)
#
# Convenience targets:
#   all       - build the static and shared libraries
#   test      - build and run the test suite
#   examples  - build the example programs into build/
#   run       - build the examples and launch the interactive runner
#   valgrind  - run the test suite under valgrind (if installed)
#   install   - install the library and header under $(PREFIX)
#   uninstall - remove installed files
#   clean     - remove the build directory

CC      ?= cc
AR      ?= ar
CFLAGS  ?= -O2
CFLAGS  += -std=c11 -Wall -Wextra -Wshadow -Wstrict-prototypes \
           -Wmissing-prototypes -Wpointer-arith -pedantic
CPPFLAGS += -I.
LDLIBS   = -lm

BUILD_DIR    = build
EXAMPLES_DIR = examples
TESTS_DIR    = tests

LIB_STATIC = $(BUILD_DIR)/libdimanet.a
LIB_SHARED = $(BUILD_DIR)/libdimanet.so
TEST_BIN   = $(BUILD_DIR)/dimanet_test
EXAMPLES   = $(addprefix $(BUILD_DIR)/,example_1 example_2 example_3 \
                                              example_4 example_5)

PREFIX ?= /usr/local

.PHONY: all lib test check examples run valgrind install uninstall clean help

all: lib

lib: $(LIB_STATIC) $(LIB_SHARED)

$(BUILD_DIR):
	@mkdir -p $@

$(BUILD_DIR)/dimanet.o: dimanet.c dimanet.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(LIB_STATIC): $(BUILD_DIR)/dimanet.o
	$(AR) rcs $@ $<

$(LIB_SHARED): dimanet.c dimanet.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -fPIC -shared -o $@ $< $(LDLIBS)

test: $(TEST_BIN)
	./$(TEST_BIN)

check: test

$(TEST_BIN): dimanet.c dimanet.h $(TESTS_DIR)/dimanet_test.c \
             $(TESTS_DIR)/libdntest.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $(TESTS_DIR)/dimanet_test.c \
	    dimanet.c $(LDLIBS)

examples: $(EXAMPLES)

$(BUILD_DIR)/example_%: $(EXAMPLES_DIR)/example_%.c dimanet.c dimanet.h \
                        | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $< dimanet.c $(LDLIBS)

run: examples
	./exlaunch.sh

valgrind: $(TEST_BIN)
	@command -v valgrind >/dev/null || { \
	    echo "valgrind is not installed."; exit 1; }
	valgrind --leak-check=full --error-exitcode=1 ./$(TEST_BIN)

install: $(LIB_STATIC) dimanet.h
	install -d $(DESTDIR)$(PREFIX)/lib $(DESTDIR)$(PREFIX)/include
	install -m 644 $(LIB_STATIC) $(DESTDIR)$(PREFIX)/lib/
	install -m 644 dimanet.h $(DESTDIR)$(PREFIX)/include/

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/lib/libdimanet.a
	rm -f $(DESTDIR)$(PREFIX)/include/dimanet.h

clean:
	rm -rf $(BUILD_DIR)

help:
	@echo "DimaNet targets:"
	@echo "  all       build the static and shared libraries"
	@echo "  test      build and run the test suite"
	@echo "  examples  build the example programs"
	@echo "  run       build examples and launch the interactive runner"
	@echo "  valgrind  run the test suite under valgrind"
	@echo "  install   install the library and header (PREFIX=$(PREFIX))"
	@echo "  uninstall remove installed files"
	@echo "  clean     remove the build directory"
