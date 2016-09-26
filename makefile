all: testerfile
#main

testerfile: Table.o DataBase.o testerfile.cpp
	g++ -std=c++11 -o testerfile testerfile.cpp Table.o DataBase.o Container.o Varchar.o

main: Table.o DataBase.o main.cpp
	g++ -std=c++11 -o main main.cpp Table.o DataBase.o Container.o Varchar.o

DataBase.o: Table.o DataBase.h DataBase.cpp
	g++ -std=c++11 -c DataBase.cpp

Table.o: Container.o Table.h Table.cpp
	g++ -std=c++11 -c -g Table.cpp

Container.o: Varchar.o Container.h Container.cpp
	g++ -std=c++11 -c -g Container.cpp

Varchar.o: Varchar.h Varchar.cpp
	g++ -std=c++11 -c -g Varchar.cpp
	
clean:
	rm *.o testerfile
