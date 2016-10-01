#include "Parser.h"
//should we include DataBase.h here or in Parser.h?

using namespace std;

vector<string> Parser::stringToTokens (string boolExpression) {
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

string print (vector<string> vec)
{
	string result = "{ ";
	for (int i = 0; i < vec.size(); ++i)
	{
		if (i != vec.size() - 1)
		{
			result += vec[i] + " , "; 
		}
		else
		{
			result += vec[i] + " }";
		}
	}
	return result;
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
	else if(instruction.find("DROP") == 0)
	{
		instruction = instruction.erase(0,5);
		commandDrop(instruction);// may change if the "DROP " part of the string needs to be removed
	}
	else if(instruction.find("EXIT") == 0)
	{
		instruction = instruction.erase(0,5);
		commandExit();// may change if the "EXIT " part of the string needs to be removed
	}
	else
	{
		cout << "Not a valid command\n";
	}
}

void Parser::commandShow(string tablename)
{
	tablename = tablename.substr(0, tablename.size()-1);
	db.showTable(tablename);
}

void Parser::commandDrop(string tablename)
{
	tablename = tablename.substr(0, tablename.size()-1);
	db.dropTable(tablename);
}

void Parser::commandCreate(string tablename)
{
	
}

vector<string> convertBoolExpression (string boolExpression) {
	// data structures
	vector<string> tokens;
	vector<string> postfix;
	stack<string> opStack;
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
		 	tokens.push_back(token);
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
}