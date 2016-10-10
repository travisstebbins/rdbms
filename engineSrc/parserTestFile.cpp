#define CATCH_CONFIG_MAIN
#include "../Catch/include/catch.hpp"
#include "DataBase.h"
#include "Parser.h"
#include <iostream>
#include <vector>

TEST_CASE("Parser", "[Parser]")
{
	SECTION("Initialize Parser")
	{
		Parser argParser;
		
	}
	
	Parser argParser;
	string instruction = "";
	
	pair<string, int> p1 {"name", 20};
	pair<string, int> p2 {"kind", 8};
	pair<string, int> p3 {"years", -1};
	vector<pair<string, int>> attributes1 = {p1, p2, p3};
	vector<string> primaryKeys1 = {"name", "kind"};
	
	Table testTable("animals", attributes1, primaryKeys1);
	vector<string> v1 = {"Joe", "cat", "4"};
	vector<string> v2 = {"Spot", "dog", "10"};
	vector<string> v3 = {"Snoopy", "dog", "3"};
	vector<string> v4 = {"Tweety", "bird", "1"};
	vector<string> v5 = {"Joe", "bird", "2"};

	SECTION("Create Table")
	{
		instruction = "CREATE TABLE animals (name VARCHAR(20), kind VARCHAR(8), years INTEGER) PRIMARY KEY (name, kind);";
		argParser.commandOrQuery(instruction);
	}
	
	SECTION("Insert Table")
	{
		instruction = "CREATE TABLE animals (name VARCHAR(20), kind VARCHAR(8), years INTEGER) PRIMARY KEY (name, kind);";
		argParser.commandOrQuery(instruction);
		instruction = "INSERT INTO animals VALUES FROM (\"Joe\", \"cat\", 4);";
		argParser.commandOrQuery(instruction);
		instruction = "INSERT INTO animals VALUES FROM (\"Spot\", \"dog\", 10);";
		argParser.commandOrQuery(instruction);
		instruction = "INSERT INTO animals VALUES FROM (\"Snoopy\", \"dog\", 3);";
		argParser.commandOrQuery(instruction);
		instruction = "INSERT INTO animals VALUES FROM (\"Tweety\", \"bird\", 1);";
		argParser.commandOrQuery(instruction);
		instruction = "INSERT INTO animals VALUES FROM (\"Joe\", \"bird\", 2);";
		argParser.commandOrQuery(instruction);
		instruction = "SHOW animals;";
		string retn = argParser.commandOrQuery(instruction);

		testTable.insertRecord(v1);
		testTable.insertRecord(v2);
		testTable.insertRecord(v3);
		testTable.insertRecord(v4);
		testTable.insertRecord(v5);

		REQUIRE(testTable.show() == retn);
	}
	
	instruction = "CREATE TABLE animals (name VARCHAR(20), kind VARCHAR(8), years INTEGER) PRIMARY KEY (name, kind);";
	argParser.commandOrQuery(instruction);
	instruction = "INSERT INTO animals VALUES FROM (\"Joe\", \"cat\", 4);";
	argParser.commandOrQuery(instruction);
	instruction = "INSERT INTO animals VALUES FROM (\"Spot\", \"dog\", 10);";
	argParser.commandOrQuery(instruction);
	instruction = "INSERT INTO animals VALUES FROM (\"Snoopy\", \"dog\", 3);";
	argParser.commandOrQuery(instruction);
	instruction = "INSERT INTO animals VALUES FROM (\"Tweety\", \"bird\", 1);";
	argParser.commandOrQuery(instruction);
	instruction = "INSERT INTO animals VALUES FROM (\"Joe\", \"bird\", 2);";
	argParser.commandOrQuery(instruction);
	
	SECTION("Show")
	{
		instruction = "SHOW animals;";
		string retn = argParser.commandOrQuery(instruction);

		testTable.insertRecord(v1);
		testTable.insertRecord(v2);
		testTable.insertRecord(v3);
		testTable.insertRecord(v4);
		testTable.insertRecord(v5);

		REQUIRE(testTable.show() == retn);
	}
	
	SECTION("Select")
	{
		string retn = "";

		instruction = "dogs <- select (kind == \"dog\") animals;";
		argParser.commandOrQuery(instruction);
		instruction = instruction = "SHOW dogs;";
		retn = argParser.commandOrQuery(instruction);

		Table testTable2("dogs", attributes1, primaryKeys1);
		testTable2.insertRecord(v3);
		testTable2.insertRecord(v2);

		REQUIRE(testTable2.show() == retn);

		instruction = "old_dogs <- select (years > 9) dogs;";
		argParser.commandOrQuery(instruction);
		instruction = instruction = "SHOW old_dogs;";
		retn = argParser.commandOrQuery(instruction);

		Table testTable3("old_dogs", attributes1, primaryKeys1);
		testTable3.insertRecord(v2);

		REQUIRE(testTable3.show() == retn);

		instruction = "cats_or_dogs <- dogs + (select (kind == \"cat\") animals);";
		argParser.commandOrQuery(instruction);
		instruction = instruction = "SHOW cats_or_dogs;";
		retn = argParser.commandOrQuery(instruction);

		Table testTable4("cats_or_dogs", attributes1, primaryKeys1);
		testTable4.insertRecord(v3);
		testTable4.insertRecord(v2);
		testTable4.insertRecord(v1);

		REQUIRE(testTable4.show() == retn);
	}
	
	SECTION("Project")
	{
		pair<string, int> p2 {"kind", 8};
		vector<pair<string, int>> attributes2 = {p1};
		vector<string> primaryKeys2 = {"kind"};

		vector<string> v6 = {"cat"};
		vector<string> v7 = {"dog"};
		vector<string> v8 = {"dog"};
		vector<string> v9 = {"bird"};
		vector<string> v10 = {"bird"};

		Table testTable2("species", attributes2, primaryKeys2);
		testTable2.insertRecord(v10);
		testTable2.insertRecord(v9);
		testTable2.insertRecord(v8);
		testTable2.insertRecord(v7);
		testTable2.insertRecord(v6);

		instruction = "CREATE TABLE species (kind VARCHAR(10)) PRIMARY KEY (kind);";
		argParser.commandOrQuery(instruction);
		instruction = "INSERT INTO species VALUES FROM RELATION project (kind) animals;";
		argParser.commandOrQuery(instruction);
		instruction = instruction = "SHOW species;";
		string retn = argParser.commandOrQuery(instruction);

		REQUIRE(testTable2.show() == retn);
	}
	
	SECTION("Rename")
	{
		instruction = "a <- rename (aname, akind) (project (name, kind) animals);";
		argParser.commandOrQuery(instruction);
		instruction = "common_names <- project (name) (select (aname == name && akind != kind) (a * animals));";
		argParser.commandOrQuery(instruction);
		instruction = "answer <- common_names;";
		argParser.commandOrQuery(instruction);
		instruction = "SHOW answer;";
		argParser.commandOrQuery(instruction);
	}
	
	SECTION("Write")
	{
		instruction = "WRITE animals;";
		argParser.commandOrQuery(instruction);
		string animalTable = "name: animals\nattributes: name 20,kind 8,years -1,\nprimary keys: name,kind,\ndata: Joe,bird,2,\ndata: Tweety,bird,1,\ndata: Snoopy,dog,3,\ndata: Spot,dog,10,\ndata: Joe,cat,4,\n";
		
		string fileName = "tableFiles/animals.table";
		ifstream ifs(fileName);
		string fileAnimals( (std::istreambuf_iterator<char>(ifs) ), (std::istreambuf_iterator<char>()) );
		
		REQUIRE(animalTable == fileAnimals);
	
	}
	
	SECTION("Close")
	{
		instruction = "CLOSE animals;";
		argParser.commandOrQuery(instruction);
	}
	
	SECTION("Delete")
	{
		instruction = "DELETE FROM animals Where (kind == \"bird\");";
		argParser.commandOrQuery(instruction);
		instruction = instruction = "SHOW animals;";
		string retn = argParser.commandOrQuery(instruction);
	}	
	
}
