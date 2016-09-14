CC=g++
CFLAGS=--std=c++11 -lfann -I include/
EXE=beach_sim.out

.PHONY: debug debug-flags default all fast fast-flags test

default: all

fast: | fast-flags all

fast-flags:
	$(eval CFLAGS += -O3)

debug-flags: 
	$(eval CFLAGS += -D DEBUG -Wall -g)

debug: | debug-flags all

test: | debug-flags test-compile

all: agent.o 
	$(CC) $(CFLAGS) agent.o -o $(EXE)

test-compile: test.o
	$(CC) $(CFLAGS) test.o -o test.out

agent.o: agent.cpp include/agent.h include/state.h
	$(CC) $(CFLAGS) -c agent.cpp -o agent.o

clean:
	rm -f *.o $(EXE)
