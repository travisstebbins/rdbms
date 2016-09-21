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