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
//#include "json/src/json.hpp"

using namespace std;

class DataBase 
{
	// friend class Table;
private:
	unordered_map<string, Table> dataBaseHashTable;
	unordered_map<string, Table> viewHashTable;
	
public:

	DataBase();
	void createTable(string tableName, vector<pair<string, int>> attributes, vector<string> primaryKeys);
	void createTable(Table newTable);
	void dropTable(string tableName);
	void insertIntoTable(string tableName, vector<string> entry);
	string showTable(string tableName);
	Table setUnion(string tableName1, string tableName2);
	Table setDifference(string tableName1, string tableName2);
	Table cossProduct(string tableName1, string tableName2);
	Table getTable(string tableName);
	Table selectTable(string tableName, string _name, vector<string> boolExpressions);
	void readTableFromDisk(string fileName);
};
