#include "Table.h"

using namespace std;

// PRIVATE FUNCTIONS
// evaluates an entire boolean expression and returns if it is true or false
bool Table::evaluate(vector<Container>& entry, vector<string> boolExpressions)
{
	// stack used for evaluating expression
	stack<int> evalStack;
	// iterate through expresssion
	for (int i = 0; i < boolExpressions.size(); ++i)
	{
		// if element is a condition, not a connector (|| and &&),
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
	// return the result of the boolean expression
	return (evalStack.top());
}

// evaluates an individual boolean condition, given an entry and a string
//  boolean condition
int Table::evaluateHelper(vector<Container>& entry, string boolExpression)
{
	// used for extracting components
	int currentIndex = 0;
	// first operand
	string operand1 = "";
	// operator
	string op = "";
	// second operand
	string operand2 = "";
	// stores if the second operand is a literal or the name of an attribute
	bool operand2IsAttribute = true;
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
	// extract operator
	while(!isalnum(boolExpression[currentIndex]) && boolExpression[currentIndex] != '\"')
	{
		op += boolExpression[currentIndex++];
	}
	// extract operand 2
	while(currentIndex < boolExpression.size())
	{
		// if the operand begins with ", it is a literal; skip the "
		if (boolExpression[currentIndex] == '\"')
		{
			operand2IsAttribute = false;
			currentIndex++;
		}
		// if the operand begins with a digit, it is a literal
		else if (isdigit(boolExpression[currentIndex]))
		{
			operand2IsAttribute = false;
			operand2 += boolExpression[currentIndex++];
		}
		else
		{
			operand2 += boolExpression[currentIndex++];
		}		
	}
	// if the second operand is another attribute (not a literal)
	if (operand2IsAttribute)
	{
		// stores if the second attribute has been found
		bool foundAttribute = false;
		// iterate through all attributes
		for (int i = 0; i < attributes.size(); ++i)
		{
			// if the attribute has been found
			if (attributes[i].first == operand2)
			{
				foundAttribute = true;
				// set operand2 to be the value of the attribute
				if (attributes[i].second > -1)
				{
					operand2 = entry[i].getVarchar().getString();
				}
				else
				{
					int operand2Int = entry[i].getInt();
					operand2 = to_string(operand2Int);
				}
			}
		}
		// if the second attribute does not exist, throw an error
		if (!foundAttribute)
		{
			throw "attribute \"" + operand2 + "\" does not exist"; 
		}
	}
	// iterate through possible attributes
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

// CONSTRUCTORS
// creates a new table object based off of passed data
Table::Table(string _name, vector<pair<string, int>> _attributes, vector<string> _primaryKeys)
{
	// copy variables
	name = _name;
	vector<pair<string, int>> copyAttributes = _attributes;
	attributes = copyAttributes;
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
	data = {};
	writeToDisk();
}

// copy constructor
Table::Table(const Table& other)
{
	// copy variables
	name = other.name;
	data = other.data;
	attributes = other.attributes;
	primaryKeys = other.primaryKeys;
	primaryKeyIndices = other.primaryKeyIndices;
}

// DATA MANIPULATION FUNCTIONS
// creates a new Table object (to be stored as a View) with the user’s desired conditions
Table* Table::select(string _name, vector<string> boolExpressions)
{
	// copy the table attributes
	vector<pair<string, int>> copyAttributes;
	copy(attributes.begin(), attributes.end(), back_inserter(copyAttributes));
	// create a new view table
	Table* view = new Table(_name, copyAttributes, primaryKeys);
	// iterate through data, copying entries that satisfy the boolean expression
	for (auto it = data.begin(); it != data.end(); ++it)
	{
		// if the entry satisfies the boolean expression,
		//  add it to the table
		if (evaluate(it->second, boolExpressions))
		{
			view->insertRecord(it->second);
		}
	}
	return view;
}

// creates a new Table object (to be stored as a View) with the user’s desired subset of attributes
Table* Table::project(string _name, vector<string> desiredAttributes)
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
	// if no primary keys are included, throw an error
	if (primaryKeyMatches.size() == 0)
	{
		throw "At least one primary key must be projected";
	}
	vector<pair<string, int>> newAttributes;
	vector<int> newAttributeIndices;
	// iterate through attributes and desired attributes to
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
	// create a new view table with the new name, attributes, and primary keys
	Table* view = new Table(_name, newAttributes, primaryKeyMatches);
	// iterate through table, inserting desired attributes of
	//  all entries into new table
	for (auto it = data.begin(); it != data.end(); ++it)
	{
		vector<Container> newEntry;
		// iterate through and add all desired attributes
		for (int i = 0; i < newAttributeIndices.size(); ++i)
		{
			newEntry.push_back((it->second)[newAttributeIndices[i]]);
		}
		// add the new entry to the view table
		try
		{
			view->insertRecord(newEntry);
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
	writeToDisk();
	return view;
}

// written assuming attributes and newNames have same size
// creates a new Table object (to be stored as a View) with the user's desired new names
Table* Table::rename(string _name, vector<string> newNames)
{
	vector<pair<string, int>> newAttributes = attributes;
	vector<string> newPrimaryKeys = primaryKeys;
	// iterate through attributes to set new primary key names
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
	// create new view table
	Table* view = new Table(_name, newAttributes, newPrimaryKeys);
	// copy over all of the entries
	for (auto it = data.begin(); it != data.end(); ++it)
	{
		try
		{
			view->insertRecord(it->second);
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

// displays the table in a human readable format (returns string representation)// return the table in human-readable format as a string
string Table::show()
{
	string s = "TABLE: " + name + "\n";
	// iterate through attributes
	for (int i = 0; i < attributes.size(); ++i)
	{
		s += attributes[i].first + "\t\t";
	}
	s += "\n";
	// add seperator line
	for (int i = 0; i < attributes.size(); ++i)
	{
		s += "---------------";
	}
	s += "\n";
	// iterate through data
	for (auto it = data.begin(); it != data.end(); ++it)
	{
		// iterate through all entries
		for (int i = 0; i < it->second.size(); ++i)
		{
			// iterate through all attributes in the entry
			// if the attribute is a varchar
			if ((it->second)[i].getType() == Container::VARCHAR)
			{
				s += (it->second)[i].getVarchar().getString() + "\t\t";
			}
			// else if the attribute is an integer
			else
			{
				s += to_string((it->second)[i].getInt()) + "\t\t";
			}
			
		}
		s += "\n";
	}
	return s;
}

// inserts a record into the table based on a given list of items (calls container version)
void Table::insertRecord(vector<string> entry)
{
	// create new entry (vector<Container>) tol hold data
	vector<Container> newEntry;
	// iterate through string
	for (int i = 0; i < entry.size(); ++i)
	{
		// if the value data type is a VARCHAR
		if (attributes[i].second > -1)
		{
			// create new varchar and add to entry
			varchar vc(attributes[i].second);
			vc.setString(entry[i]);
			Container c(Container::VARCHAR, vc);
			newEntry.push_back(c);
		}
		// else if the value data type is an INTEGER
		else
		{
			// create new integer and add to entry
			int value = strtol(entry[i].c_str(), NULL, 10);
			Container c(Container::INTEGER, value);
			newEntry.push_back(c);
		}
	}
	// call insertRecord(vector<Container> entry)
	insertRecord(newEntry);
	writeToDisk();
}

// inserts a record into the table based on a given entry (vector<Container>)
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
	// computation
	hash<string> str_hash;
	size_t hash = str_hash(hashString);

	// check if the entry is already in the table
	unordered_map<size_t, vector<Container>>::const_iterator checkEntryUniq = data.find(hash);
	// if not, add to the table
	if(checkEntryUniq == data.end())
	{
		data[hash] = entry;
		writeToDisk();
	}
	writeToDisk();
}

// inserts a record into the table based on an existing table
void Table::insertRecord(Table* relationship)
{
	// copy the data from the existing table
	unordered_map<size_t, vector<Container>> newData = relationship->data;
	// iterate through table, inserting records
	for (auto it = newData.begin(); it != newData.end(); ++it)
	{
		try
		{
			// calls insertRecord(vector<Container> entry)
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

// deletes a record from the table based on the given boolean expression
void Table::deleteRecord(vector<string> boolExpressions)
{
	// iterate through table
	auto it = data.begin();
	while(it != data.end())
	{
		auto temp = it;
		it++;
		// if the entry satisfies the boolean expression, erase it
		if (evaluate(temp->second, boolExpressions))
		{
			data.erase(temp);
		}	
	}
	writeToDisk();
}

// deletes a record from the table based on the given key
void Table::deleteRecord(size_t key)
{
	// iterate through table
	auto it = data.begin();
	while(it != data.end())
	{
		auto temp = it;
		it++;
		// if the keys match, erase record
		if(temp->first == key)
			data.erase(temp);
	}
	writeToDisk();
}

// changes the value of the records in the table based on the following parameters:
// 	list of attributes to update (vector<string>), new values for the attributes (vector<string>),
// 	boolean expression of records to search and update (vector<string>)
void Table::updateRecord(vector<string> desiredAttributes, vector<string> values, vector<string> boolExpressions)
{
	// iterate through the table
	for (auto it = data.begin(); it != data.end(); ++it)
	{
		// if the entry satisfies the boolean expression
		if (evaluate(it->second, boolExpressions))
		{
			// iterate through the table's attributes
			for (int i = 0; i < attributes.size(); ++i)
			{
				// iterate through the attributes desired to change
				for (int j = 0; j < desiredAttributes.size(); ++j)
				{
					// if they match
					if (attributes[i].first == desiredAttributes[j])
					{
						// if the value data type is a VARCHAR
						if (attributes[i].second > -1)
						{
							// create new varchar and update entry
							varchar vc(attributes[i].second);
							vc.setString(values[j]);
							(it->second)[i].setVarchar(vc);
						}
						// else if the value data type is an INTEGER
						else
						{
							// create new integer and update entry
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

// HELPER FUNCTIONS
// writes the table to disk every time the table is modified or created
void Table::writeToDisk()//writes a table to a .table file
{
	string path = "tableFiles/" + name + ".table";
	ofstream ofs(path, ofstream::out);// creates the output file object, that outputs to a .table file

	ofs <<  "name: " + name + "\n";//first line of table chunk, stores table name
	ofs << "attributes: " ;//second line of table chunk, stores table attributes
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

// OVERLOADED OPERATORS
// overloaded assignment operator for table
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

// overloaded comparison operator for table
inline bool Table::operator==(const Table& rhs) const
{
	if(!(this->attributes == rhs.attributes))
		return false;
	if(!(this->primaryKeys == rhs.primaryKeys))
		return false;
	if(!(this->data == rhs.data))
		return false;
	return true;
}	

// overloaded assignment operator for attribute vector (vector<pair<string, int>>)
inline vector<pair<string, int>> Table::operator=(const vector<pair<string, int>> &vec)
{
	vector<pair<string, int>> newVec;
	for (int i = 0; i < vec.size(); ++i)
	{
		pair<string, int> p;
		p.first = vec[i].first;
		p.second = vec[i].second;
		newVec.push_back(p);
	}
	return newVec;
}