all: main

main: Table.o main.cpp
	g++ -std=c++11 -o main main.cpp Table.o Container.o Varchar.o

Table.o: Container.o Table.h Table.cpp
	g++ -std=c++11 -c -g Table.cpp

Container.o: Varchar.o Container.h Container.cpp
	g++ -std=c++11 -c -g Container.cpp

Varchar.o: Varchar.h Varchar.cpp
	g++ -std=c++11 -c -g Varchar.cpp

clean:
	rm *.o main