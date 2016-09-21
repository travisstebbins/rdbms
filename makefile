all: test

test: Table.o test.cpp
	g++ -std=c++11 -c -g test.cpp -o test Table.o

Table: Table.h Table.cpp
	g++ -std=c++11 -c -g Table.cpp

clean:
	rm *.o test