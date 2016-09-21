all: main

main: Table.o
	g++ -std=c++11 -o main main.cpp Table.o

Table.o: Table.h Table.cpp
	g++ -std=c++11 -c -g Table.cpp

clean:
	rm *.o main