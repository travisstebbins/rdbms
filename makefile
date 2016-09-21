all: main

main: Table.o DataBase.o
	g++ -std=c++11 -o main main.cpp Table.o DataBase.o

Table.o: Table.h Table.cpp
	g++ -std=c++11 -c -g Table.cpp

DataBase.o: Table.h DataBase.h DataBase.cpp
	g++ -std=c++11 -c DataBase.cpp
	
clean:
	rm *.o main