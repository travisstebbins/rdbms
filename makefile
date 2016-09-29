all: Table.o DataBase.o Container.o Varchar.o testerfile.o
	g++ -std=c++11 -g -o dataBaseTests Table.o DataBase.o Container.o Varchar.o testerfile.o
#main

testerfile.o: Table.h DataBase.h Container.h Varchar.h testerfile.cpp
	g++ -std=c++11 -g -c testerfile.cpp 

main: Table.o DataBase.o main.cpp
	g++ -std=c++11 -o main main.cpp Table.o DataBase.o Container.o Varchar.o

DataBase.o: Table.o DataBase.h DataBase.cpp
	g++ -std=c++11 -c -g DataBase.cpp

Table.o: Container.o Table.h Table.cpp
	g++ -std=c++11 -c -g Table.cpp

Container.o: Varchar.o Container.h Container.cpp
	g++ -std=c++11 -c -g Container.cpp

Varchar.o: Varchar.h Varchar.cpp
	g++ -std=c++11 -c -g Varchar.cpp
	
clean:
	rm *.o testTables
