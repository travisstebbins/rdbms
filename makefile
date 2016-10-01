all: Table.o DataBase.o Container.o Varchar.o testerfile.o
	g++ -std=c++11 -g -o dataBaseTests Table.o DataBase.o Container.o Varchar.o testerfile.o
#main

testerfile.o: Table.h DataBase.h Container.h Varchar.h testerfile.cpp
	g++ -std=c++11 -c -g testerfile.cpp 

main: Table.o DataBase.o Parser.o main.cpp
	g++ -std=c++11 -o main main.cpp Table.o DataBase.o Container.o Varchar.o

boolConverterTester: Table.o DataBase.o Parser.o boolConverterTester.cpp
	g++ -std=c++11 -g -o boolConverterTester boolConverterTester.cpp Table.o DataBase.o Container.o Varchar.o

Parser.o: DataBase.o Parser.h Parser.cpp
	g++ -std=c++11 -c -g Parser.cpp

DataBase.o: Table.o DataBase.h DataBase.cpp
	g++ -std=c++11 -c -g DataBase.cpp

Table.o: Container.o Table.h Table.cpp
	g++ -std=c++11 -c -g Table.cpp

Container.o: Varchar.o Container.h Container.cpp
	g++ -std=c++11 -c -g Container.cpp

Varchar.o: Varchar.h Varchar.cpp
	g++ -std=c++11 -c -g Varchar.cpp
	
clean:
	rm *.o boolConverterTester dataBaseTests main
