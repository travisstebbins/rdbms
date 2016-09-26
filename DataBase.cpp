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
		cout << "made it into show\n";
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

void DataBase::readTableFromDisk(string fileName)//Reads a .table file and creates a table from it
{
	ifstream ifs (fileName, ifstream::in);//inputfile creator
	
	string tName;//name to be used as arg
	vector<pair<string, int> > tAttributes;//atribute vector to be used as arg
	vector<string> tPrimKeys;//Primary keys of table
	vector<vector<string>> tData;//The data that will be logged into the table
	
	string line;//most recent line received from ifstream
	if (ifs.is_open())
	{
		while (getline(ifs,line))//while there is still stuff in the file
		{
			if(line.find("name: ") == 0)//name found
			{
				tName = line.erase(0,6);//erase "name: "
			}
			else if(line.find("attributes: ") == 0)//attributes found
			{
				line = line.erase(0,12);//erase "attributes: "
				string s_one;
				stringstream ssline_one(line);
				
				while(getline(ssline_one,s_one,','))//separates the line by commas
				{
					stringstream ssline_two(s_one);
					string s_two;
					int i = 0;//used in if statement
					string a;//second element of pair
					int b;//second element of pair
					while(getline(ssline_two,s_two,' '))// I can assume that it will be <string, int>
					{
						if(i == 0)//is first half of split
						{
							a = s_two;
							i++;
						}
						else//second half of splt
						{
							b = stoi(s_two);
						}
					
					}
					pair<string, int> tempPair = make_pair(a,b);
					tAttributes.push_back(tempPair);
				}
			}
			else if(line.find("primary keys: ") == 0)//found primary keys
			{
				line = line.erase(0,14);//erase "primary keys: "
				stringstream ssline(line);//stringstream line
				string s;//holds getline
				while(getline(ssline,s,','))//split on commas
				{
					tPrimKeys.push_back(s);//add to key vector
				}
			}
			else if(line.find("data: ") == 0)//data is found
			{
				vector<string> tempVect;//used to store within tData
				line = line.erase(0,6);//erases "data: ""
				
				stringstream ssline(line);//stringstream line
				string s;//used with while loop
				
				while(getline(ssline,s,','))//while line isn't empty
				{
					tempVect.push_back(s);//push string back into 
				}
				
				tData.push_back(tempVect);//pushes string vector into vector vector
			}
					
		}
		ifs.close();//closes filestream
		Table t = Table(tName, tAttributes, tPrimKeys);//creates table
		
		for(int i = 0; i < tData.size(); i++)
		{
			t.insertRecord(tData[i]);//adds entries to table
		}
		createTable(t);
		t.writeToDisk();//saves again for consistency
	}
}

/*
Used these resources to help understand how to implement my readfrom function.
http://stackoverflow.com/questions/19912751/how-to-initialize-a-vector-of-vector-of-strings-using-for-loop-in-c
http://www.cplusplus.com/forum/beginner/85227/
http://www.cplusplus.com/forum/beginner/85227/
*/
