#include "Parser.h"
//should we include DataBase.h here or in Parser.h?

using namespace std;

vector<string> Parser::stringToTokens (string boolExpression) 
{
	// result
	vector<string> tokens;
	// remove new lines, tabs, and spaces
	boolExpression.erase(remove(boolExpression.begin(), boolExpression.end(), '\r'), boolExpression.end());
	boolExpression.erase(remove(boolExpression.begin(), boolExpression.end(), '\n'), boolExpression.end());
	boolExpression.erase(remove(boolExpression.begin(), boolExpression.end(), '\t'), boolExpression.end());
	boolExpression.erase(remove(boolExpression.begin(), boolExpression.end(), ' '), boolExpression.end());
	// convert string to tokens
	int current = 0;
	string token = "";
	while (current != boolExpression.length()) {
		 if (boolExpression[current] != '&' && boolExpression[current] != '|'
		 	&& boolExpression[current] != '(' && boolExpression[current] != ')')
		 {
		 	token += boolExpression[current++];
		 }
		 else
		 {
		 	if (token.length() > 0)
		 	{
		 		tokens.push_back(token);
		 	}
		 	token = "";
		 	if (boolExpression[current] == '(' || boolExpression[current] == ')')
		 	{
		 		token += boolExpression[current++];
		 		tokens.push_back(token);
		 		token = "";
		 	}
		 	else 
		 	{
		 		token += boolExpression[current++];
		 		token += boolExpression[current++];
		 		tokens.push_back(token);
		 		token = "";
		 	}
		 }
	}
	if (token.length() > 0)
	{
		tokens.push_back(token);
	}
	return tokens;
}

vector<string> Parser::convertBoolExpression (string boolExpression)
{
	vector<string> tokens = stringToTokens(boolExpression);
	vector<string> postfix;
	stack<string> opStack;
	for (int i = 0; i < tokens.size(); ++i)
	{
		if (tokens[i][0] != '&' && tokens[i][0] != '|'
			&& tokens[i][0] != '(' && tokens[i][0] != ')')
		{
			postfix.push_back(tokens[i]);
		}
		else {
			if (tokens[i] == "(")
			{
				opStack.push(tokens[i]);
			}
			else if (tokens[i] == ")")
			{
				while (!opStack.empty() && opStack.top() != "(")
				{
					string op = opStack.top();
					opStack.pop();
					postfix.push_back(op);
				}
				if (!opStack.empty())
				{
					opStack.pop();
				}				
			}
			else if (!opStack.empty() && opStack.top() != "(")
			{
				string op = opStack.top();
				opStack.pop();
				opStack.push(tokens[i]);
				postfix.push_back(op);
			}
			else
			{
				opStack.push(tokens[i]);
			}
		}
	}
	// check if any operators are left on the opStack
	while (!opStack.empty())
	{
		postfix.push_back(opStack.top());
		opStack.pop();
	}
	return postfix;
}

void Parser::commandOrQuery(string instruction)
{
	if(instruction.find("<-") == 0)// <- found 
	{
		if(instruction.find("INSERT") == 0)// <- found whatever command could use a query
		{
			commandParse(instruction);
		}
		else
		{
			//put query parse here
		}
	}
	else
	{
		commandParse(instruction);
	}
}	


void Parser::commandParse(string instruction)//parses a command
{
	if(instruction.find("OPEN") == 0)// <- found 
	{
		instruction = instruction.erase(0,5);
		commandOpen(instruction);// may change if the "OPEN " part of the string needs to be removed
	}
	else if(instruction.find("CLOSE") == 0)
	{
		instruction = instruction.erase(0,6);
		commandClose(instruction);// may change if the "CLOSE " part of the string needs to be removed
	}
	else if(instruction.find("WRITE") == 0)
	{
		instruction = instruction.erase(0,6);
		commandWrite(instruction);// may change if the "WRITE " part of the string needs to be removed
	}
	else if(instruction.find("SHOW") == 0)
	{
		instruction = instruction.erase(0,5);
		commandShow(instruction);// may change if the "SHOW " part of the string needs to be removed
	}
	else if(instruction.find("EXIT") == 0)
	{
		instruction = instruction.erase(0,5);
		commandExit();// may change if the "EXIT " part of the string needs to be removed
	}
	else if(instruction.find("CREATE") == 0)
	{
		instruction = instruction.erase(0,6);
		commandCreate(instruction);
	}
	else if(instruction.find("UPDATE") == 0)
	{
		instruction = instruction.erase(0,6);
		commandUpdate(instruction);
	}
	else if(instruction.find("INSERTINTO") == 0)//since whitespace will be eliminated
	{
		instruction = instruction.erase(0,10);//removes INSERT INTO
		commandInsert(instruction);
	}
	else if(instruction.find("DROP") == 0)
	{
		instruction = instruction.erase(0,5);
		commandDrop(instruction);// may change if the "DROP " part of the string needs to be removed
	}
	else if(instruction.find("DELETE") == 0)
	{
		instruction = instruction.erase(0,6);
		commandDelete(instruction);
	}
	else
	{
		cout << "Not a valid command\n";
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////
////////////////COMMAND FUNCTIONS////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
void Parser::commandOpen(string filename)
{
	filename = filename.substr(0, filename.size()-1);//eliminates semicolon at end of command
	db.readTableFromDisk(filename);//under the impression that the filename string has no ""
}

void Parser::commandClose(string tablename)
{
	tablename = tablename.substr(0, tablename.size()-1);//eliminates semicolon at end of command
	db.dropTable(tablename);//works the exact same way as commandDrop()
}

void  Parser::commandWrite(string tableName)
{
	db.writeTableToDisk(tableName);
}

void Parser::commandExit()//For Travis
{
	int success = db.exit();
	if (success != -1)
	{
		cout << "Saved " << success << " database tables" << endl;
		exit(EXIT_SUCCESS);
	}
	else
	{
		cout << "Failed to save database tables" << endl;
		exit(EXIT_FAILURE);
	}
}

void Parser::commandShow(string tablename)
{
	tablename = tablename.substr(0, tablename.size()-1);//eliminates semicolon at end of command
	db.showTable(tablename);
}

void Parser::commandCreate(string instr)// We'll need 
{
	
}

void Parser::commandUpdate(string instr)
{
	
}

// split function from here: http://code.runnable.com/VHb0hWMZp-ws1gAr/splitting-a-string-into-a-vector-for-c%2B%2B
vector<string> split(string str, char delimiter) {
  vector<string> internal;
  stringstream ss(str); // Turn the string into a stream.
  string tok;
  
  while(getline(ss, tok, delimiter)) {
    internal.push_back(tok);
  }
  
  return internal;
}

vector<string> Parser::extractAttributes (string attributeList)
{
	attributeList.erase(remove(attributeList.begin(), attributeList.end(), '\r'), attributeList.end());
	attributeList.erase(remove(attributeList.begin(), attributeList.end(), '\n'), attributeList.end()); 
	attributeList.erase(remove(attributeList.begin(), attributeList.end(), '\t'), attributeList.end()); 
	attributeList.erase(remove(attributeList.begin(), attributeList.end(), ' '), attributeList.end()); 
	attributeList.erase(remove(attributeList.begin(), attributeList.end(), '('), attributeList.end()); 
	attributeList.erase(remove(attributeList.begin(), attributeList.end(), ')'), attributeList.end());
	vector<string> attributes = split(attributeList, ',');
	return attributes;
}

void Parser::commandInsert(string instr)
{
	string name;
	vector<string> attributes;
	int nameIndex = instr.find("INSERTINTO");
	if (nameIndex == string::npos)
	{
		throw "INSERTINTO not found";
	}
	nameIndex += 10;
	int attributesIndex = instr.find("VALUESFROMRELATION");
	if (attributesIndex != string::npos)
	{
		name = instr.substr(nameIndex, (attributesIndex - nameIndex));
		attributesIndex += 18;
		string expression = instr.substr(attributesIndex);
		Table result = queryParse("tmp", expression);
		db.getTable(name).insertRecord(result);
	}
	else
	{
		attributesIndex = instr.find("VALUESFROM");
		if (attributesIndex == string::npos)
		{
			throw "VALUESFROM not found";
		}
		else
		{
			name = instr.substr(nameIndex, (attributesIndex - nameIndex));
			attributesIndex += 10;
			attributes = extractAttributes(instr.substr(attributesIndex));
			db.getTable(name).insertRecord(attributes);
		}
	}
}

void Parser::commandDrop(string tablename)
{
	tablename = tablename.substr(0, tablename.size()-1);//eliminates semicolon at end of command
	db.dropTable(tablename);
}

void Parser::commandDelete(string instr)
{

}

