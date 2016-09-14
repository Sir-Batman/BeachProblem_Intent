CC=g++
CFLAGS=--std=c++11 -lfann
EXE=beach_sim.out

.PHONY: debug debug-flags default all fast fast-flags

default: all

fast: | fast-flags all

fast-flags:
	$(eval CFLAGS += -O3)

debug-flags: 
	$(eval CFLAGS += -D DEBUG -Wall -g)

debug: | debug-flags all

all: agent.o 
	$(CC) $(CFLAGS) agent.o -o $(EXE)

agent.o: agent.cpp agent.h state.h
	$(CC) $(CFLAGS) -c agent.cpp -o agent.o

clean:
	rm -f *.o $(EXE)
