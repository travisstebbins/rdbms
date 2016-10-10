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
	// this is used to store all of the tables that are created
	// the key used will be unique and based on the table name
	unordered_map<string, Table*> dataBaseHashTable;
	// this is used to store all of the view tables that are created
	unordered_map<string, Table*> viewHashTable;

public:
	
	// CONSTRUCTOR
		DataBase();

	// TABLE CREATE/DROP FUNCTIONS

		// creates a new Table object and inserts it into the hash table of tables based
		//	on the following parameters:
		// 	table name (string), list of attribute names and types (vector<pair<string, int>>),
		// 	and list of primary key(s) (vector<string>)
		void createTable(string tableName, vector<pair<string, int>> attributes, vector<string> primaryKeys);
		
		// creates a new Table object and inserts it into the hash table of tables
		// table newTable table object to be inserted into databse
		void createTable(Table *newTable);
		
		// creates a new view Table object and inserts it into the hash table of views
		// table newView table object to be inserted into databse
		void createView(Table *newView);	
		
		// delete the table from the hash table of tables 
		// table name (string)
		void dropTable(string tableName);
		
		// delete the view from the hash table of tables 
		// table name (string)
		void dropView(string tableName);

	// TABLE WRAPPER FUNCTIONS
	
		// wrapper function for Table insertRecord(vector<string> entry)
		// calls insertRecord on Table tableName
		void insertIntoTable(string tableName, vector<string> entry);
			
		// wrapper function for Table insertRecord(Table relationships)
		// calls insertRecord on Table tableName
		void insertIntoTable(string tableName, Table *relationships);
		
		// wrapper function for Table deleteRecord(vector<string> boolExpressions)
		// calls deleteRecord on Table tableName
		void deleteFromTable(string name, vector<string> boolExpressions);
		
		// wrapper function for Table updateRecord(vector<string> desiredAttributes, vector<string> values, vector<string> boolExpressions)
		// calls updateRecord on Table tableName
		void updateTableRecord(string tableName, vector<string> desiredAttributes, vector<string> values, vector<string> boolExpressions);
		
		// wrapper function for Table show()
		// calls show on Table tableName
		string showTable(string tableName);
		
		//wrapper function for Table show()
		// calls show on Table tableName
		string showView(string viewName);
		
		// wrapper function for Table project(string _name, vector<string> desiredAttributes)
		// calls project on Table tableName
		Table* projectTable(string tableName, string _name, vector<string> desiredAttributes);
		
		// wrapper function for Table select(string _name, vector<string> boolExpressions
		// calls select on Table tableName
		Table* selectTable(string tableName, string _name, vector<string> boolExpressions);
	
	// TABLE MANIPULATION FUNCTIONS
		// returns a view that contains the set union of two tables
		Table* setUnion(string tableName1, string tableName2);		// used for existing tables
		Table* setUnion(Table *t1, Table *t2);						// used for temporary tables
		
		// returns a view that contains the set difference product of two tables
		Table* setDifference(string tableName1, string tableName2);	// used for existing tables
		Table* setDifference(Table *t1, Table *t2);					// used for temporary tables
		
		// returns a view that contains the cross product of two tables
		Table* crossProduct(string tableName1, string tableName2);	// used for existing tables
		Table* crossProduct(Table *t1, Table *t2);					// used for temporary tables
	
	// GETTER FUNCTIONS
		// this is used to access and return a table based on the given name
		Table* getTable(string tableName);
		
		// this is used to access and return a view based on the given name
		Table* getView(string viewName);

	// HELPER functions
		// returns if the database contains a table with the given name
		bool containsTable(string tableName);
		// returns if the database contains a view with the given name
		bool containsView(string viewName);
	
		//This reads a database file from the disk and loads it into memory, 
		//recreating the Table object and inserting it into the hash table of tables
		void readTableFromDisk(string fileName);
		
		// Wrapper function for table writeToDisk()
		void writeTableToDisk(string tableName);

		// This saves all tables to disk; returns number of tables that
		// were written to disk and -1 in case of error
		int exit();
};
