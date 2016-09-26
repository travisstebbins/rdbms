#include "Table.h"

using namespace std;

int Table::evaluateHelper (vector<Container> entry, string boolExpression)
{
	int currentIndex = 0;
	string operand1 = "";
	string op = "";
	string operand2 = "";
	boolExpression.erase(remove(boolExpression.begin(), boolExpression.end(), '\r'), boolExpression.end());
	boolExpression.erase(remove(boolExpression.begin(), boolExpression.end(), '\n'), boolExpression.end());
	boolExpression.erase(remove(boolExpression.begin(), boolExpression.end(), '\t'), boolExpression.end());
	boolExpression.erase(remove(boolExpression.begin(), boolExpression.end(), ' '), boolExpression.end());
	while(isalnum(boolExpression[currentIndex]))
	{
		operand1 += boolExpression[currentIndex++];
	}
	while(!isalnum(boolExpression[currentIndex]))
	{
		op += boolExpression[currentIndex++];
	}
	while(currentIndex < boolExpression.size())
	{
		operand2 += boolExpression[currentIndex++];
	}
	for (int i = 0; i < attributes.size(); ++i)
	{
		if (attributes[i].first == operand1)
		{
			if(attributes[i].second > -1)
			{
				string value = entry[i].getVarchar().getString();
				if (op == "==")
				{
					return (value == operand2) ? 1 : 0;
				}
				else if (op == "!=")
				{
					return (value != operand2) ? 1 : 0;
				}
				else if (op == "<")
				{
					return (value < operand2) ? 1 : 0;
				}
				else if (op == ">")
				{
					return (value > operand2) ? 1 : 0;
				}
				else if (op == "<=")
				{
					return (value <= operand2) ? 1 : 0;
				}
				else if (op == ">=")
				{
					return (value >= operand2) ? 1 : 0;
				}
				else
				{
					throw "Invalid operator";
				}
			}
			else
			{
				int value = entry[i].getInt();
				int operand2_int = strtol(operand2.c_str(), NULL, 10);
				if (op == "==")
				{
					return (value == operand2_int) ? 1 : 0;
				}
				else if (op == "!=")
				{
					return (value != operand2_int) ? 1 : 0;
				}
				else if (op == "<")
				{
					return (value < operand2_int) ? 1 : 0;
				}
				else if (op == ">")
				{
					return (value > operand2_int) ? 1 : 0;
				}
				else if (op == "<=")
				{
					return (value <= operand2_int) ? 1 : 0;
				}
				else if (op == ">=")
				{
					return (value >= operand2_int) ? 1 : 0;
				}
				else
				{
					throw "Invalid operator";
				}
			}
		}
	}
}

bool Table::evaluate(vector<Container> entry, vector<string> boolExpressions)
{
	stack<int> evalStack;
	for (int i = 0; i < boolExpressions.size(); ++i)
	{
		if (boolExpressions[i] != "||" && boolExpressions[i] != "&&")
		{
			evalStack.push(evaluateHelper(entry, boolExpressions[i]));
		}
		else
		{
			int val1 = evalStack.top();
			evalStack.pop();
			int val2 = evalStack.top();
			evalStack.pop();
			if (boolExpressions[i] == "||")
			{
				evalStack.push(max(val1, val2));
			}
			else
			{
				evalStack.push(min(val1, val2));
			}
		}
	}
	return (evalStack.top());
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
		writeToDisk();
	}
	else
		throw "Entry already exists.";
	writeToDisk();
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
			if (primaryKeys[i] == attributes[j].first)
			{
				// cout << "logged primary key index" << endl;
				primaryKeyIndices.push_back(j);
			}
		}
	}
	writeToDisk();
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
	vector<string> primaryKeyMatches;
	for (int i = 0; i < primaryKeys.size(); ++i)
	{
		for (int j = 0; j < desiredAttributes.size(); ++j)
		{
			if (primaryKeys[i] == desiredAttributes[j])
			{
				primaryKeyMatches.push_back(primaryKeys[i]);
			}
		}
	}
	if (primaryKeyMatches.size() == 0)
	{
		throw "At least one primary key must be projected";
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
	Table view(_name, newAttributes, primaryKeyMatches);
	for (auto it = data.begin(); it != data.end(); ++it)
	{
		vector<Container> newEntry;
		for (int i = 0; i < newAttributeIndices.size(); ++i)
		{
			newEntry.push_back((it->second)[newAttributeIndices[i]]);
		}
		try
		{
			view.insertRecord(newEntry);
		}
		catch(char const* c)
		{
			cout << c << endl;
		}
		catch(...)
		{
			cout << "Unknown exception in project function" << endl;
		}
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
	cout << "primary key indices size: " << view.primaryKeyIndices.size() << endl;
	for (int i = 0; i < view.primaryKeyIndices.size(); ++i)
	{
		cout << "Primary key kindex " << i << ": " << view.primaryKeyIndices[i] << endl;
	}
	for (auto it = data.begin(); it != data.end(); ++it)
	{
		try
		{
			view.insertRecord(it->second);
		}		
		catch(char const* c)
		{
			cout << c << endl;
		}
		catch(...)
		{
			cout << "Unknown exception in rename function" << endl;
		}
	}
	writeToDisk();
	return view;
}

string Table::show()
{
	string s = "Table: " + name + "\n";
	for (int i = 0; i < attributes.size(); ++i)
	{
		s += attributes[i].first + "\t\t";
	}
	s += "\n";
	for (auto it = data.begin(); it != data.end(); ++it)
	{
		for (int i = 0; i < it->second.size(); ++i)
		{
			if ((it->second)[i].getType() == Container::VARCHAR)
			{
				s += (it->second)[i].getVarchar().getString() + "\t\t";
			}
			else
			{
				s += to_string((it->second)[i].getInt()) + "\t\t";
			}
			
		}
		s += "\n";
	}
	return s;
}

vector<pair<string, int> > Table::getAttributes()
{
	return attributes;
}

string Table::getTableName()
{
	return name;
}

const unordered_map<size_t, vector<Container>>& Table::getData()
{
	return data;
}

vector<string> Table::getPrimaryKeys()
{
	return primaryKeys;
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
	writeToDisk();
}

void Table::insertRecord(Table relationship)
{
	unordered_map<size_t, vector<Container>> newData = relationship.data;
	for (auto it = newData.begin(); it != newData.end(); ++it)
	{
		try
		{
			insertRecord(it->second);
		}
		catch(char const* c)
		{
			cout << c << endl;
		}
		catch(...)
		{
			cout << "Unknown error in insertRecord(Table relationship)" << endl;
		}
	}
	writeToDisk();
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
	writeToDisk();
}

void Table::deleteRecord(size_t key)
{
	for (auto it = data.begin(); it != data.end(); ++it)
	{
		if(it->first == key)
			data.erase(it);
	}
	writeToDisk();
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
	writeToDisk();
}

Table& Table::operator=(const Table& other)
{
	if(&other == this)
		return *this;
	this->name = other.name;
	this->data = other.data;
	this->attributes = other.attributes;
	this->primaryKeys = other.primaryKeys;
	this->primaryKeyIndices = other.primaryKeyIndices;
	return *this;
}

void Table::writeToDisk()//writes a table to a .table file
{
	string fName = name + ".table";
	ofstream ofs(fName, ofstream::out);// creates the output file object, that outputs to a .table file

	ofs <<  "name: " + name + "\n";//first line of table chunk
	ofs << "attributes: " ;//first line of table chunk
	for (int i = 0; i < attributes.size(); ++i)
	{
		int atb_num = attributes[i].second;
		
		
		ofs << attributes[i].first + " " + to_string(atb_num) + ",";//logs attr. pairs below name in file
	}
	ofs << "\n";//end of attributes
    
    
	ofs << "primary keys: ";//start of primary keys
	for (int i = 0; i < primaryKeys.size(); ++i)
	{
		ofs << primaryKeys[i] << ",";
	}
	ofs << "\n";//end of primary keys
	
	
	
	/*
		did data logging differently using string s due to weird problem I had with just
		outputting to ofs. Also done this ways so I could test if data was outputting how I
		Wanted it to
	*/
	string s = "";//Data string
	for (auto it = data.begin(); it != data.end(); ++it)
	{
		s += "data: ";
		for (int i = 0; i < it->second.size(); ++i)
		{
			if ((it->second)[i].getType() == Container::VARCHAR)
			{
				s += (it->second)[i].getVarchar().getString() + ",";
			}
			else
			{
				s += to_string((it->second)[i].getInt()) + ",";
			}
			
		}
		s += "\n";
	}
	ofs << s;//
	
	ofs.close();//close file
}