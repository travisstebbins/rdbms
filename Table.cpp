#include <iostream>

#include "Table.h"

using namespace std;

bool Table::evaluate(vector<Container> entry, vector<string> boolExpressions)
{

}

void Table::insertRecord(vector<Container> entry)
{
	string hashString = "";
	for (int i = 0; i < primaryKeyIndices.size(); ++i)
	{
		if (entry[primaryKeyIndices[i]].getType() == Container::VARCHAR)
		{
			hashString += entry[primaryKeyIndices[i]].getVarchar().getString();
		}
		else
		{
			hashString += entry[primaryKeyIndices[i]].getInt();
		}		
	}
	hash<string> str_hash;
	size_t hash = str_hash(hashString);

	unordered_map<size_t, vector<Container>>::const_iterator checkEntryUniq = data.find(hash);	
	if(checkEntryUniq == data.end())
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
	for (int i = 0; i < primaryKeys.size(); ++i)
	{
		bool foundPrimaryKey = false;
		for (int j = 0; j < desiredAttributes.size(); ++j)
		{
			if (primaryKeys[i] == desiredAttributes[j])
			{
				foundPrimaryKey = true;
			}
		}
		if (foundPrimaryKey)
		{
			foundPrimaryKey = false;
		}
		else
		{
			throw "At least all of the table's primary keys must be projected";
		}
	}
	vector<pair<string, int>> newAttributes;
	vector<int> newAttributeIndices;
	for (int i = 0; i < attributes.size(); ++i)
	{
		for (int j = 0; j < desiredAttributes.size(); ++j)
		{
			if (attributes[i].first == desiredAttributes[j])
			{
				newAttributes.push_back(attributes[i]);
				newAttributeIndices.push_back(i);
			}
		}
	}
	Table view(_name, newAttributes, primaryKeys);
	for (auto it = data.begin(); it != data.end(); ++it)
	{
		vector<Container> newEntry;
		for (int i = 0; i < newAttributeIndices.size(); ++i)
		{
			newEntry.push_back((it->second)[newAttributeIndices[i]]);
		}
		view.insertRecord(newEntry);
	}
	return view;
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
	for (int i = 0; i < attributes.size(); ++i)
	{
		s += attributes[i].first + "\t";
	}
	s += "\n";
	for (auto it = data.begin(); it != data.end(); ++it)
	{
		for (int i = 0; i < it->second.size(); ++i)
		{
			if ((it->second)[i].getType() == Container::VARCHAR)
			{
				s += (it->second)[i].getVarchar().getString() + "\t";
			}
			else
			{
				s += (it->second)[i].getInt() + "\t";
			}
			
		}
		s += "\n";
	}
	return s;
}

void Table::insertRecord(vector<string> entry)
{	
	vector<Container> newEntry;
	for (int i = 0; i < entry.size(); ++i)
	{
		if (attributes[i].second > -1)
		{
			varchar vc(attributes[i].second);
			vc.setString(entry[i]);
			Container c(Container::VARCHAR, vc);
			newEntry.push_back(c);
		}
		else
		{
			int value = strtol(entry[i].c_str(), NULL, 10);
			Container c(Container::INTEGER, value);
			newEntry.push_back(c);
		}
	}
	insertRecord(newEntry);
}

void Table::insertRecord(Table relationship)
{
	unordered_map<size_t, vector<Container>> newData = relationship.data;
	for (auto it = newData.begin(); it != newData.end(); ++it)
	{
		insertRecord(it->second);
	}
}

void Table::deleteRecord(vector<string> boolExpressions)
{
	for (auto it = data.begin(); it != data.end(); ++it)
	{
		if (evaluate(it->second, boolExpressions))
		{
			data.erase(it);
		}
	}
}

void Table::updateRecord(vector<string> desiredAttributes, vector<string> values, vector<string> boolExpressions)
{
	for (auto it = data.begin(); it != data.end(); ++it)
	{
		if (evaluate(it->second, boolExpressions))
		{
			for (int i = 0; i < attributes.size(); ++i)
			{
				for (int j = 0; j < desiredAttributes.size(); ++j)
				{
					if (attributes[i].first == desiredAttributes[j])
					{
						if (attributes[i].second > -1)
						{
							varchar vc(attributes[i].second);
							vc.setString(values[j]);
							(it->second)[i].setVarchar(vc);
						}
						else
						{
							int newInt = strtol(values[j].c_str(), NULL, 10);
							(it->second)[i].setInt(newInt);
						}
					}
				}
			}
		}
	}
}

void Table::writeToDisk()
{

}