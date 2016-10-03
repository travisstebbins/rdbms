#define CATCH_CONFIG_MAIN
#include "Catch/include/catch.hpp"
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
		argParser.commandOrQuery(instruction);
	}
	
	SECTION("Select")
	{
		instruction = "dogs <- select (kind == \"dog\") animals;";
		argParser.commandOrQuery(instruction);
		instruction = "old_dogs <- select (years > 10) dogs;";
		argParser.commandOrQuery(instruction);
		instruction = "cats_or_dogs <- dogs + (select (kind == \"cat\") animals);";
		argParser.commandOrQuery(instruction);
	}
	
	SECTION("Project")
	{
		instruction = "CREATE TABLE species (kind VARCHAR(10)) PRIMARY KEY (kind);";
		argParser.commandOrQuery(instruction);
		instruction = "INSERT INTO species VALUES FROM RELATION project (kind) animals;";
		argParser.commandOrQuery(instruction);
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
	}
	
	SECTION("Close")
	{
		instruction = "CLOSE animals;";
		argParser.commandOrQuery(instruction);
	}
	
	
}