#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <stack>
#include <cctype>
#include <iostream>
#include <sstream>

#include "Container.h"

using namespace std;


class Table
{
	private:
		// table name
		string name;
		// key -> entry
		unordered_map<size_t, vector<Container>> data;
		// n = VARCHAR, -1 = INTEGER
		vector<pair<string, int>> attributes;
		// stores the table's primary keys
		vector<string> primaryKeys;
		// used to easily access the table's primary keys
		vector<int> primaryKeyIndices;

		// PRIVATE FUNCTIONS
		bool evaluate(vector<Container>& entry, vector<string> boolExpressions);
		int evaluateHelper (vector<Container>& entry, string boolExpression);
		
	public:
		// CONSTRUCTORS
		Table() {}		
		// creates a new table object based off of passed data
		Table(string _name, vector<pair<string, int>> _attributes, vector<string> _primaryKeys);		
		// copy constructor
		Table(const Table& other);

		// DESTRUCTOR
		~Table() {};

		// DATA MANIPULATION FUNCTIONS		
		// creates a new Table object (to be stored as a View) with the user’s desired conditions
		Table* select(string _name, vector<string> boolExpressions);		
		// creates a new Table object (to be stored as a View) with the user’s desired subset of attributes 
		Table* project(string _name, vector<string> desiredAttributes);	
		// written assuming attributes and newNames have same size
		// creates a new Table object (to be stored as a View) with the user's desired new names
		Table* rename(string _name, vector<string> newNames);		
		// displays the table in a human readable format (returns string representation)
		string show();
		// inserts a record into the table based on a given list of items (calls container version)
		void insertRecord(vector<string> entry);		
		// inserts a record into the table based on a given entry (vector<Container>)
		void insertRecord(vector<Container> entry);
		// inserts a record into the table based on an existing table
		void insertRecord(Table *relationship);	
		// deletes a record from the table based on the given boolean expression
		void deleteRecord(vector<string> boolExpressions);		
		// deletes a record from the table based on the given key
		void deleteRecord(size_t key);		
		// changes the value of the records in the table based on the following parameters:
		// 	list of attributes to update (vector<string>), new values for the attributes (vector<string>),
		// 	boolean expression of records to search and update (vector<string>)
		void updateRecord(vector<string> desiredAttributes, vector<string> values, vector<string> boolExpressions);

		// GETTER FUNCTIONS		
		// returns table's name
		string getTableName() { return name; }		
		// returns table's attributes
		vector<pair<string, int>> getAttributes() { return attributes; }		
		// returns table's primary keys
		vector<string> getPrimaryKeys() { return primaryKeys; }		
		// returns table's data hash table
		const unordered_map<size_t, vector<Container>>& getData() {return data; }

		// SETTER FUNCTIONS
		// changes the table's name
		void setTableName (string tableName) { name = tableName; }
		
		// HELPER FUNCTIONS
		// writes the table to disk every time the table is modified or created
		void writeToDisk();

		// OVERLOADED OPERATORS
		// overloaded assignment operator for table
		Table& operator=(const Table& other);		
		// overloaded comparison operator for table
		inline bool operator==(const Table& rhs) const;
		// overloaded assignment operator for attribute vector (vector<pair<string, int>>)
		inline vector<pair<string, int>> operator=(const vector<pair<string, int>> &vec);
};

