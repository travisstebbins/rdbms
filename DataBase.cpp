/****************************************************************************************************************
*Filename: DataBase.cpp 	  										*
*Initial Author: Andrew Meserole										*
*Project: RDBMS													*
*Description:													*
*Source file for DataBase.h 											*
*A decription of the class and functions can be found in the header file.					*
*****************************************************************************************************************/

#include "DataBase.h"

DataBase::DataBase()
{
	dataBaseHashTable = {};
	viewHashTable = {};
}

void DataBase::createTable(string tableName, vector<pair<string, int>> attributes, vector<string> primaryKeys)
{
	unordered_map<string, Table>::const_iterator checkNameUniq = dataBaseHashTable.find(tableName);
	
	if(checkNameUniq == dataBaseHashTable.end())
	{
		Table newTable = Table(tableName, attributes, primaryKeys);
		dataBaseHashTable[tableName] = newTable;
		newTable.writeToDisk();
	}
	else
		throw "Table name already in use.";
}

void DataBase::createTable(Table newTable)
{
	string tableName = newTable.getTableName();
	auto checkNameUniq = dataBaseHashTable.find(tableName);
	if(checkNameUniq == dataBaseHashTable.end())
	{
		dataBaseHashTable[tableName] = newTable;
	}
	else 
		throw "Table already exists";
}

Table DataBase::projectTable(string tableName, string _name, vector<string> desiredAttributes)
{
	auto getTable = dataBaseHashTable.find(tableName);
	if (getTable != dataBaseHashTable.end())
	{
		return getTable->second.project(_name, desiredAttributes);
	}
	else
		throw "Table does not exist";
}

Table DataBase::selectTable(string tableName, string _name, vector<string> boolExpressions)
{
	auto getTable = dataBaseHashTable.find(tableName);
	if(getTable != dataBaseHashTable.end())
	{
		return getTable->second.select(_name, boolExpressions);
	}
	else
		throw "Table does not exist";
}

void DataBase::dropTable(string tableName)
{
	unordered_map<string, Table>::const_iterator checkNameExist = dataBaseHashTable.find(tableName);
	
	if(checkNameExist != dataBaseHashTable.end())
	{
		dataBaseHashTable.erase(checkNameExist);
	}
	else
		throw "Table does not exist.";
}

Table DataBase::getTable(string tableName)
{
	unordered_map<string, Table>::const_iterator getTable = dataBaseHashTable.find(tableName);
	
	if(getTable != dataBaseHashTable.end())
	{
		return getTable->second;
	}
	else
		throw "Table does not exist.";
}

void DataBase::insertIntoTable(string tableName, vector<string> entry)
{
	auto getTable = dataBaseHashTable.find(tableName);
	if(getTable != dataBaseHashTable.end())
	{
		getTable->second.insertRecord(entry);
	}
	else
		throw "Table could not be found";
	
}

void DataBase::insertIntoTable(string tableName, Table relationships)
{
	auto getTable = dataBaseHashTable.find(tableName);
	if(getTable != dataBaseHashTable.end())
	{
		getTable->second.insertRecord(relationships);
	}
	else
		throw "Table could not be found";
}

string DataBase::showTable(string tableName)
{
	auto getTable = dataBaseHashTable.find(tableName);
	if(getTable != dataBaseHashTable.end())
	{
		return getTable->second.show();
	}
	else
		throw "Table could not be found";
}

Table DataBase::setUnion(string tableName1, string tableName2)
{
	
	auto getTable1 = dataBaseHashTable.find(tableName1);
	auto getTable2 = dataBaseHashTable.find(tableName2);
	if(getTable1 != dataBaseHashTable.end() && getTable2 != dataBaseHashTable.end())
	{
		Table tableUnion = Table(getTable1->second);
		// tableUnion.insertRecord(getTable1->second);
		tableUnion.insertRecord(getTable2->second);
		return tableUnion;
	}
	else
		throw "One of the tables could not be found";
}

Table DataBase::setDifference(string tableName1, string tableName2)
{
	Table t = Table();	//Not how function will work just for compilation purposes
	return t;	
}

Table DataBase::cossProduct(string tableName1, string tableName2)
{
	Table t = Table();	//Not how function will work just for compilation purposes
	return t;
}

void readTableFromDisk(string fileName)
{
	
}
