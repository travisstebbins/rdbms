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
		vector<pair<string, int> > table1Attr = getTable1->second.getAttributes();
		vector<pair<string, int> > table2Attr = getTable2->second.getAttributes();

		if(table1Attr == table2Attr)
		{
			Table tableUnion = Table(getTable1->second);
			tableUnion.insertRecord(getTable2->second);
			return tableUnion;
		}
		else
			throw "Tables are not union compatible"
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

void DataBase::readTableFromDisk(string fileName)
{
	ifstream ifs (fileName, ifstream::in);//inputfile creator
	
	string tName;//name to be used as arg
	vector<pair<string, int> > tAttributes;//atribute vector to be used as arg
	vector<string> tPrimKeys;
	vector<vector<string>> tData;
	
	string line;//most recent line received from ifstream
	if (ifs.is_open())
	{
		while (getline(ifs,line))
		{
			if(line.find("name: ") == 0)
			{
				tName = line.erase(0,5);
				cout << tName + "\n";
			}
			else if(line.find("attributes: ") == 0)
			{
				line = line.erase(0,11);//erase "attributes: "
				string s_one;
				stringstream ssline_one(line);
				
				while(getline(ssline_one,s_one,','))//eliminates 
				{
					stringstream ssline_two(s_one);
					string s_two;
					int i = 0;
					string a;
					int b;
					
					
					while(getline(ssline_two,s_two,' '))// I can assume that itw will be <string int>
					{
						if(i == 0)
						{
							a = s_two;
							i++;
						}
						else
						{
							b = stoi(s_two);
						}
					
					}
					pair<string, int> tempPair = make_pair(a,b);
					tAttributes.push_back(tempPair);
				}
				//add code here
			}
			else if(line.find("primary keys: ") == 0)
			{
				line = line.erase(0,13);
				stringstream ssline(line);//stringstream line
				string s;
				while(getline(ssline,s,','))
				{
					tPrimKeys.push_back(s);
				}
			}
			else if(line.find("data: ") == 0)
			{
				vector<string> tempVect;//used to store within tData
				line = line.erase(0,5);//erases "data: ""
				stringstream ssline(line);//stringstream line
				string s;//used with while loop
				while(getline(ssline,s,','))//while line isn't empty
				{
					tempVect.push_back(s);//push string back into 
				}
				tData.push_back(tempVect);
			}
					
		}
		ifs.close();
		createTable(tName, tAttributes, tPrimKeys);//creates table
		for(int i = 0; i < tData.size(); i++)
		{
			insertIntoTable(tName, tData[i]);//adds entries to table
		}
	}
}
