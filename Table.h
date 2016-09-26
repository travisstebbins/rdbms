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
	// friend class DataBase;
	private:
		string name;
		// key -> entry
		unordered_map<size_t, vector<Container>> data;
		// n = VARCHAR, -1 = INTEGER
		vector<pair<string, int> > attributes;
		// stores the table's primary keys
		vector<string> primaryKeys;
		// used to easily access the table's primary keys
		vector<int> primaryKeyIndices;

		// private functions
		bool evaluate(vector<Container> entry, vector<string> boolExpressions);
		int evaluateHelper (vector<Container> entry, string boolExpression);
		void insertRecord(vector<Container> entry);
		
	public:
		// constructors
		Table() {}
		Table(string _name, vector<pair<string, int>> _attributes, vector<string> _primaryKeys);
		// data manipulation functions
		Table select(string _name, vector<string> boolExpressions);
		Table project(string _name, vector<string> desiredAttributes);
		Table rename(string _name, vector<string> newNames);
		string show();
		string getTableName();
		void insertRecord(vector<string> entry);
		void insertRecord(Table relationship);
		void deleteRecord(vector<string> boolExpressions);
		void updateRecord(vector<string> desiredAttributes, vector<string> values, vector<string> boolExpressions);
		void writeToDisk();
		Table& operator=(const Table& other);
};