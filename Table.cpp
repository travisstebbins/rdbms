#include "Table.h"

using namespace std;

// evaluates an inndividual boolean expression, given an entry and a string
//  boolean expression
int Table::evaluateHelper (vector<Container> entry, string boolExpression)
{
	int currentIndex = 0;
	string operand1 = "";
	string op = "";
	string operand2 = "";
	// remove new lines, tabs, and spaces
	boolExpression.erase(remove(boolExpression.begin(), boolExpression.end(), '\r'), boolExpression.end());
	boolExpression.erase(remove(boolExpression.begin(), boolExpression.end(), '\n'), boolExpression.end());
	boolExpression.erase(remove(boolExpression.begin(), boolExpression.end(), '\t'), boolExpression.end());
	boolExpression.erase(remove(boolExpression.begin(), boolExpression.end(), ' '), boolExpression.end());
	// extract operand 1
	while(isalnum(boolExpression[currentIndex]))
	{
		operand1 += boolExpression[currentIndex++];
	}
	// extractor operator
	while(!isalnum(boolExpression[currentIndex]))
	{
		op += boolExpression[currentIndex++];
	}
	// extract operand 2
	while(currentIndex < boolExpression.size())
	{
		operand2 += boolExpression[currentIndex++];
	}
	// loop through possible attributes
	for (int i = 0; i < attributes.size(); ++i)
	{
		// if the current attribute is the one being compared
		if (attributes[i].first == operand1)
		{
			// if the data type is a VARCHAR
			if(attributes[i].second > -1)
			{
				// get the value in the entry
				string value = entry[i].getVarchar().getString();
				// make the appropriate comparison
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
			// else if the data type is an INTEGER
			else
			{
				// convert string value and operand2 to integer
				int value = entry[i].getInt();
				int operand2_int = strtol(operand2.c_str(), NULL, 10);
				// make the appropriate comparison
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

// evaluates an entire boolean expression
bool Table::evaluate(vector<Container> entry, vector<string> boolExpressions)
{
	// stack used for evaluating expression
	stack<int> evalStack;
	for (int i = 0; i < boolExpressions.size(); ++i)
	{
		// if element is an expression, not a connector (|| and &&),
		//  push it on the stack
		if (boolExpressions[i] != "||" && boolExpressions[i] != "&&")
		{
			evalStack.push(evaluateHelper(entry, boolExpressions[i]));
		}
		// if element is a connector (|| and &&), pop the last 2 values
		//  and evaluate the result
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

// inserts a record into the table from a vector of Container objects
void Table::insertRecord(vector<Container> entry)
{
	// compute the entry's hash key based on its primary keys
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

	// check if the entry is already in the table
	unordered_map<size_t, vector<Container>>::const_iterator checkEntryUniq = data.find(hash);
	// if not, add to the table
	if(checkEntryUniq == data.end())
	{
		data[hash] = entry;
	}
	else
		throw "Entry already exists.";
}

// constructor
Table::Table(string _name, vector<pair<string, int>> _attributes, vector<string> _primaryKeys)
{
	name = _name;
	attributes = _attributes;
	primaryKeys = _primaryKeys;
	// store primary key indices
	for (int i = 0; i < primaryKeys.size(); ++i)
	{
		for (int j = 0; j < attributes.size(); ++j)
		{
			if (primaryKeys[i] == attributes[j].first)
			{
				primaryKeyIndices.push_back(j);
			}
		}
	}
}

// select entries from the table and return as a new Table object
Table Table::select(string _name, vector<string> boolExpressions)
{
	Table view(_name, attributes, primaryKeys);
	for (auto it = data.begin(); it != data.end(); ++it)
	{
		// if the entry satisfies the boolean expression,
		//  add it to the table
		if (evaluate(it->second, boolExpressions))
		{
			view.insertRecord(it->second);
		}
	}
	return view;
}

// select a subset of the table's attributes and return as a new
//  Table object, with associated entry values included
Table Table::project(string _name, vector<string> desiredAttributes)
{
	// check to ensure at least one primary key is included
	//  in the prjection
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
	// loop through attributes and desired attributes to
	//  find matches
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
	// loop through table, inserting desired attributes of
	//  all entries into new table
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
// rename all of the table's attributes with new names and
//  return as a new table
Table Table::rename(string _name, vector<string> newNames)
{
	vector<pair<string, int>> newAttributes = attributes;
	vector<string> newPrimaryKeys = primaryKeys;
	// loop through attributes to set new primary key names
	//  and new attribute names
	for (int i = 0; i < newAttributes.size(); ++i)
	{
		for (int j = 0; j < newPrimaryKeys.size(); ++j)
		{
			if (newAttributes[i].first == newPrimaryKeys[j])
			{
				newPrimaryKeys[j] = newNames[i];
			}
		}
		newAttributes[i].first = newNames[i];
	}
	Table view(_name, newAttributes, newPrimaryKeys);
	// copy over all of the entries
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
	return view;
}

// return the table in human-readable format as a string
string Table::show()
{
	string s = "Table: " + name + "\n";
	// loop and print attributes
	for (int i = 0; i < attributes.size(); ++i)
	{
		s += attributes[i].first + "\t\t";
	}
	s += "\n";
	// loop and print data
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

string Table::getTableName()
{
	return name;
}

// insert a record given a vector of strings
void Table::insertRecord(vector<string> entry)
{	
	vector<Container> newEntry;
	// loop through entry
	for (int i = 0; i < entry.size(); ++i)
	{
		// if the value data type is a VARCHAR
		if (attributes[i].second > -1)
		{
			varchar vc(attributes[i].second);
			vc.setString(entry[i]);
			Container c(Container::VARCHAR, vc);
			newEntry.push_back(c);
		}
		// else if the value data type is an INTEGER
		else
		{
			int value = strtol(entry[i].c_str(), NULL, 10);
			Container c(Container::INTEGER, value);
			newEntry.push_back(c);
		}
	}
	insertRecord(newEntry);
}

// insert an entire existing table's entries into this table
void Table::insertRecord(Table relationship)
{
	unordered_map<size_t, vector<Container>> newData = relationship.data;
	// loop through table, inserting records
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
}

// delete all records that satisfy the boolean expression
void Table::deleteRecord(vector<string> boolExpressions)
{
	// loop through table
	for (auto it = data.begin(); it != data.end(); ++it)
	{
		// evaluate entry with expression
		if (evaluate(it->second, boolExpressions))
		{
			data.erase(it);
		}
	}
}

// update the desired attributes with the given new values for all entries that satisfy
//  the boolean expression
void Table::updateRecord(vector<string> desiredAttributes, vector<string> values, vector<string> boolExpressions)
{
	// loop through the table
	for (auto it = data.begin(); it != data.end(); ++it)
	{
		// if the entry satisfies the boolean expression
		if (evaluate(it->second, boolExpressions))
		{
			// loop through the table's attributes
			for (int i = 0; i < attributes.size(); ++i)
			{
				// loop through the desired to change atatributes
				for (int j = 0; j < desiredAttributes.size(); ++j)
				{
					// if they match
					if (attributes[i].first == desiredAttributes[j])
					{
						// if the value data type is a VARCHAR
						if (attributes[i].second > -1)
						{
							varchar vc(attributes[i].second);
							vc.setString(values[j]);
							(it->second)[i].setVarchar(vc);
						}
						// else if the value data type is an INTEGER
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

// overloaded = operator
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

// write the table to a file on the disk
void Table::writeToDisk()
{
	string fName = name + ".table";
	ofstream ofs(fName, ofstream::out);// creates the output file object, that outputs to a text file, will change name

	ofs <<  "name: " + name + "\n";//first line of table chunk, stores table name
	ofs << "attributes: " ;//second line of table chunk, stores table attributes
	for (int i = 0; i < attributes.size(); ++i)
	{
		int atb_num = attributes[i].second;
		//logs all atribute pairs on second line of .table file
		ofs << attributes[i].first + " " + to_string(atb_num) + ",";
	}
	ofs << "\n";//end of attributess
    
    
	ofs << "primary keys: ";//start of primary keys
	for (int i = 0; i < primaryKeys.size(); ++i)
	{
		ofs << primaryKeys[i] << ",";
	}
	ofs << "\n";//end of primary keys
    
    string s = "";
	
	// loop through table and print all data
	for (auto it = data.begin(); it != data.end(); ++it)
	{
		ofs << "data: ";
		for (int i = 0; i < it->second.size(); ++i)
		{
			if ((it->second)[i].getType() == Container::VARCHAR)
			{
				ofs << (it->second)[i].getVarchar().getString() + ",";
			}
			else
			{
				ofs << to_string((it->second)[i].getInt()) + ",";
			}
			
		}
		ofs << "\n";
	}
	ofs.close();
}