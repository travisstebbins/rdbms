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
		string name;
		// key -> entry
		unordered_map<size_t, vector<Container>> data;
		// n = VARCHAR, -1 = INTEGER
		vector<pair<string, int>> attributes;
		// stores the table's primary keys
		vector<string> primaryKeys;
		// used to easily access the table's primary keys
		vector<int> primaryKeyIndices;

		// private functions
		bool evaluate(vector<Container>& entry, vector<string> boolExpressions);
		int evaluateHelper (vector<Container>& entry, string boolExpression);
		
	public:
		// constructors
		Table() {}
		
		// creates a new table object based off of passed data
		Table(string _name, vector<pair<string, int>> _attributes, vector<string> _primaryKeys);
		
		// copy constructor
		Table(const Table& other);
		~Table();
		// data manipulation functions
		
		//Creates a new Table object (to be stored as a View) with the user’s desired conditions
		Table* select(string _name, vector<string> boolExpressions);
		
		//Creates a new Table object (to be stored as a View) with the user’s desired attributes 
		Table* project(string _name, vector<string> desiredAttributes);
		
		// 
		Table* rename(string _name, vector<string> newNames);
		
		// Display the table in a human readable format
		string show();
		
		// Return tables name
		string getTableName();
		
		//Change the tables name
		void setTableName (string tableName) { name = tableName; }
		
		// Returns tables attribues
		vector<pair<string, int>> getAttributes();
		
		// Returns tables primary keys
		vector<string> getPrimaryKeys();
		
		// Returns tables data hash table
		const unordered_map<size_t, vector<Container>>& getData();
		
		// Inserts a record into the data table based on a given list of items
		void insertRecord(vector<string> entry);
		
		// inserts a record into the data table based on an existing table
		void insertRecord(Table *relationship);

		// Inserts a record into the data table based on a given list of items
		void insertRecord(vector<Container> entry);
		
		// Delete a record from table using the bool expressions for comparisons
		void deleteRecord(vector<string> boolExpressions);
		
		// Delete a record from table based on key passed
		void deleteRecord(size_t key);
		
		// Changes the value of the record in the table
		// List of attributes to update (vector<string>), New values for the attributes (vector<string>), Boolean expression of records to search and update (vector<string>)
		void updateRecord(vector<string> desiredAttributes, vector<string> values, vector<string> boolExpressions);
		
		// write the table to disk every time the table is modified or created
		void writeToDisk();

		// Overloaded assignment operator for table
		Table& operator=(const Table& other);
		
		// Overloaded comparison operator for table
		inline bool operator==(const Table& rhs) const
		{
			if(!(this->attributes == rhs.attributes))
				return false;
			if(!(this->primaryKeys == rhs.primaryKeys))
				return false;
			if(!(this->data == rhs.data))
				return false;
			return true;
		}
		

		inline vector<pair<string, int>> operator=(const vector<pair<string, int>> &vec);
};

