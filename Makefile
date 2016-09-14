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

all: agent.o beach.o
	$(CC) $(CFLAGS) agent.o -o $(EXE)

test-compile: test.o
	$(CC) $(CFLAGS) test.o -o test.out

agent.o: agent.cpp 
	$(CC) $(CFLAGS) -c agent.cpp -o agent.o

beach.o: beach.cpp
	$(CC) $(CFLAGS) -c beach.cpp -o beach.o

clean:
	rm -f *.o $(EXE)
