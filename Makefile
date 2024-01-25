CFLAGS = -Wall -Wshadow -O3 -g -march=native
LDLIBS = -lm

all: test build

sigmoid: CFLAGS += -Ddimanet_act=dimanet_act_sigmoid_cached
sigmoid: all

threshold: CFLAGS += -Ddimanet_act=dimanet_act_threshold
threshold: all

linear: CFLAGS += -Ddimanet_act=dimanet_act_linear
linear: all

debug: debug.o dimanet.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)
	./$@


test: debug
	./$<

build: compile

main.o: main.c
	$(CC) $(CFLAGS) -c -o $@ $<

compile: main.o dimanet.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

clean:
	$(RM) *.o
	$(RM) debug
	$(RM) persist.txt
	$(RM) compile

.PHONY: sigmoid threshold linear clean compile test
