/****************************************************************************************************************
*Filename: DataBase.h 												*
*Initial Author: Andrew Meserole										*
*Project: RDBMS													*
*Description:													*
*This class holds all of the tables (relationships) and views and contains the functions used to create,	*
*delete, modify, and query tables and views. The parser interfaces with the database class in the main 		*
*function by parsing the input and calling the correct functions on the database class with the correct		*
*arguments. Views are exactly the same as tables, except they are stored in a separate data structure 		*
*from the tables in the Database class.										*
*****************************************************************************************************************/

#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <utility>
#include <iostream>
#include "Table.h"

using namespace std;

class DataBase 
{
private:
	//This is used to store all of the tables that are created
	//The key used will be unique and based on the table name
	unordered_map<string, Table> dataBaseHashTable;
	unordered_map<string, Table> viewHashTable;
	
public:
	
	//Initialize empty unordered_map
	DataBase();
	
	//Creates a new Table object and inserts it into the hash table of tables.
	//Table name (string), list of attribute names and types (vector<pair<string, int>>), and list of primary key(s) (vector<string>)
	void createTable(string tableName, vector<pair<string, int>> attributes, vector<string> primaryKeys);
	
	//Creates a new Table object and inserts it into the hash table of tables
	//Table newTable table object to be inserted into databse
	void createTable(Table newTable);

	
	void createView(Table newView);
	
	//Delete the table from the hash table of tables 
	//Table name (string)
	void dropTable(string tableName);
	
	//Wrapper function for Table insertRecord(vector<string> entry)
	//Calls insertRecord on Table tableName
	void insertIntoTable(string tableName, vector<string> entry);
		
	//Wrapper function for Table insertRecord(Table relationships)
	//Calls insertRecord on Table tableName
	void insertIntoTable(string tableName, Table relationships);
	
	//Wrapper function for Table deleteRecord(vector<string> boolExpressions)
	//Calls deleteRecord on Table tableName
	void deleteFromTable(string name, vector<string> boolExpressions);
	
	//Wrapper function for Table show()
	//Calls show on Table tableName
	string showTable(string tableName);
	
	//Wrapper function for Table project(string _name, vector<string> desiredAttributes)
	//Calls project on Table tableName
	Table projectTable(string tableName, string _name, vector<string> desiredAttributes);
	
	//Wrapper function for Table select(string _name, vector<string> boolExpressions
	//Calls select on Table tableName
	Table selectTable(string tableName, string _name, vector<string> boolExpressions);
	
	//Return a view that contains the set union of two tables
	Table setUnion(string tableName1, string tableName2);
	
	//Return a view that contains the set difference product of two tables
	Table setDifference(string tableName1, string tableName2);
	
	//Return a view that contains the cross product of two tables
	Table crossProduct(string tableName1, string tableName2);
	
	//This is used to access and return a table based on the given name
	Table getTable(string tableName);
	
	Table getView(string viewName);
	
	//This reads a database file from the disk and loads it into memory, 
	//recreating the Table object and inserting it into the hash table of tables
	void readTableFromDisk(string fileName);
};
