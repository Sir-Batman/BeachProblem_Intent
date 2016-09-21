CC=g++
CFLAGS=--std=c++11 -lfann -I include/ -I .
EXE=beach_sim.out

.PHONY: debug debug-flags default all fast fast-flags test

default: all

fast: | fast-flags all

fast-flags:
	$(eval CFLAGS += -O3)

debug-flags: 
	$(eval CFLAGS += -D DEBUG -Wall -g -pg)

debug: | debug-flags all

test: | debug-flags test-compile

all: agent.o beach.o controller.o
	$(CC) $(CFLAGS) agent.o beach.o controller.o -o $(EXE)

test-compile: test.o beach.o agent.o
	$(CC) $(CFLAGS) test.o beach.o agent.o -o test.out

agent.o: agent.cpp config.h
	$(CC) $(CFLAGS) -c agent.cpp -o agent.o

beach.o: beach.cpp config.h
	$(CC) $(CFLAGS) -c beach.cpp -o beach.o

test.o: test.cpp config.h
	$(CC) $(CFLAGS) -c test.cpp -o test.o

controller.o: controller.cpp config.h
	$(CC) $(CFLAGS) -c controller.cpp -o controller.o

clean:
	rm -f *.o $(EXE)
