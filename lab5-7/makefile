CC = g++
FLAGS = -Wall -Wextra -ggdb
ZMQ =-L/usr/local/lib -lzmq

main: main.cpp tree.h calc
	$(CC) $(FLAGS) -o main.out main.cpp tree.h $(ZMQ)

calc: calc.cpp
	$(CC) $(FLAGS) -o calc.out calc.cpp $(ZMQ)

clean:
	rm -rf *.out