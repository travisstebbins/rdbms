#include "Table.h"

using namespace std;

bool Table::evaluate(tuple<> entry, vector<string> boolExpressions)
{

}

void Table::insertRecord(tuple<> entry)
{
	string hashString = "";
	for (int i = 0; i < primaryKeyIndices.size(); ++i)
	{
		hashString += attributes[primaryKeyIndices[i]].first;
	}
	hash<string> str_hash;
	size_t hash = str_hash(hashString);

	unordered_map<size_t, tuple<> >::const_iterator checkEntryUniq = data.find(hash);	
	if(checkEntryUniq != data.end())
	{
		data[hash] = entry;
	}
	else
		throw "Entry already exists.";
}

Table::Table(string _name, vector<pair<string, int>> _attributes, vector<string> _primaryKeys)
{
	name = _name;
	attributes = _attributes;
	primaryKeys = _primaryKeys;
	for (int i = 0; i < primaryKeys.size(); ++i)
	{
		for (int j = 0; j < attributes.size(); ++j)
		{
			if (primaryKeys[i] == attributes[i].first)
			{
				primaryKeyIndices.push_back(j);
			}
		}
	}
}

Table Table::select(string _name, vector<string> boolExpressions)
{
	Table view(_name, attributes, primaryKeys);
	for (auto it = data.begin(); it != data.end(); ++it)
	{
		if (evaluate(it->second, boolExpressions))
		{
			view.insertRecord(it->second);
		}
	}
	return view;
}

Table Table::project(string _name, vector<string> desiredAttributes)
{
	// how should this handle projections that don't include the primary key(s)?
}

// written assuming attributes and newNames have same size
Table Table::rename(string _name, vector<string> newNames)
{
	vector<pair<string, int>> newAttributes = attributes;
	vector<string> newPrimaryKeys = primaryKeys;
	for (int i = 0; i < newAttributes.size(); ++i)
	{
		for (int j = 0; j < newPrimaryKeys.size(); ++j)
		{
			if (newAttributes[i].first == newPrimaryKeys[j])
			{
				newPrimaryKeys[j] == newNames[i];
			}
		}
		newAttributes[i].first = newNames[i];
	}
	Table view(_name, newAttributes, newPrimaryKeys);
	return view;
}

string Table::show()
{
	string s = "";
}

void Table::insertRecord(vector<string> entry)
{

}

void Table::insertRecord(Table relationship)
{

}

void Table::deleteRecord(vector<string> boolExpressions)
{

}

void Table::updateRecord(vector<string> attributes, vector<string> values, vector<string> boolExpressions)
{

}

void Table::writeToDisk()
{

}