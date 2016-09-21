#include "DataBase.h"

void DataBase::createTable(string tableName, vector<pair<string, int>> attributes, vector<string> primaryKeys)
{
	unordered_map<string, Table>::const_iterator checkIfNameUniq = dataBaseHashTable.find(tableName);
	if(checkIfNameUniq == dataBaseHashTable.end())
	{
		Table newTable = Table(tableName, attributes, primaryKeys);
		dataBaseHashTable[tableName] = newTable;
	}
	else
		throw "Table name already in use.";
}