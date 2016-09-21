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

#include <unordered_map>
#include "Table.h"
#include <string>
#include <vector>
#include <utility>
#include <iostream>

using namespace std;

class DataBase 
{
	unordered_map<string, Table> dataBaseHashTable;
	unordered_map<string, Table> viewHashTable;
	
public:

	void createTable(string tableName, vector<pair<string, int>> attributes, vector<string> primaryKeys);
	void dropTable(string tableName);
	Table setUnion(string tableName1, string tableName2);
	Table setDifference(string tableName1, string tableName2);
	Table cossProduct(string tableName1, string tableName2);
	Table getTable(string tableName);
	void readTableFromDisk(string fileName);
};