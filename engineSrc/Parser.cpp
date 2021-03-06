#include "Parser.h"

using namespace std;

void printVector (vector<string> vec)
{	
	cout << "{ ";
	for (int i = 0; i < vec.size(); ++i)
	{
		if (i != vec.size() - 1)
			cout << vec[i] << " , ";
		else
			cout << vec[i] << " }";
	}
	cout << endl;
}

void printVector (vector<pair<string, int>> vec)
{	
	cout << "{ ";
	for (int i = 0; i < vec.size(); ++i)
	{
		if (i != vec.size() - 1)
			cout << vec[i].first << " , " << vec[i].second << " | ";
		else
			cout << vec[i].first << " , " << vec[i].second << " }";
	}
	cout << endl;
}

void Parser::runOnCommandLine()
{
	string command;
	while(1)
	{
		cout << ">";
		getline(cin, command);
		try
		{
			commandOrQuery(command);
		}
		catch (char const* c)
		{
			cout << c << endl;
		}
		catch (...)
		{
			cout << "Unknown exception thrown" << endl;
		}
	}
}

void Parser::runOnSocket()
{
	cout << "Starting Server" << endl;
	int c;
	int socketFD;
	int clientSocketFD;
   	struct sockaddr_in server;
	struct sockaddr_in client;
	char buffer[BUFFSIZE];
	memset(buffer, '\0',BUFFSIZE);
	string returnString = "";
	
	socketFD = socket(AF_INET, SOCK_STREAM, 0); //establish a socket in domain, configure communication for default
	memset(&server, 0, sizeof(server));
	server.sin_family      = AF_INET;
	server.sin_port        = htons(PORT);
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	
	if(bind(socketFD, (struct sockaddr *)&server, sizeof(server)) < 0 ) { //bind master socket to port
		close(socketFD);
		throw "Error in bind, exiting program";
	}
	cout << "Server Running" << endl;
	int optval = 1;
	if (setsockopt(socketFD, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
	{
		close(socketFD);
		throw "Error in setsockopt, exiting program";
	}
		
	
	cout << "Server Running" << endl;
	int msgSize = 0;
	while(1)
	{
		listen(socketFD, 5);
		cout << "Listening on port: " << PORT << endl;
		
		c = sizeof(struct sockaddr_in);
		clientSocketFD = accept(socketFD, (struct sockaddr *)&client, (socklen_t*)&c);	
		
		cout << "Accepted a client" << endl;
		
		if(clientSocketFD < 0){							//chech that accept worked
			throw "Accept Failed!";
			close(clientSocketFD);
		}
		msgSize = recv(clientSocketFD, buffer, BUFFSIZE, 0);	//recieve command
		
		cout << "Message from client:" <<  buffer << " Size: " << msgSize << endl;
		
		cout << "Passing to Parser" << endl;
		try
		{
			string command(buffer);
			returnString = commandOrQuery(command);	//pass input to parser
		}
		catch (char const* c)
		{
			returnString = c;
		}
		catch (...)
		{
			returnString = "Failure";
		}
		cout << "Sending: " << returnString.c_str() << endl;
		send(clientSocketFD, returnString.c_str(), returnString.length(), 0);	
		
		close(clientSocketFD);
		cout << "Closed connection to client" << endl;
		
		memset(buffer, '\0', BUFFSIZE);
	}
}

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

vector<string> Parser::convertBoolExpression (string boolExpression)	//converts a boolean expression from infix to postfix expression
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

string Parser::commandOrQuery(string instruction)	//determines if the incoming instruction is a command or a query
{
	instruction.erase(remove(instruction.begin(), instruction.end(), '\r'), instruction.end());
	instruction.erase(remove(instruction.begin(), instruction.end(), '\n'), instruction.end());
	instruction.erase(remove(instruction.begin(), instruction.end(), '\t'), instruction.end());
	instruction.erase(remove(instruction.begin(), instruction.end(), ';'), instruction.end());
	instruction.erase(remove(instruction.begin(), instruction.end(), ' '), instruction.end());
	
	try{
		if(instruction.find("<-") != string::npos)// <- found 
		{
			return queryParse(instruction);
		}
		else
		{
			return commandParse(instruction);
		}
	}
	
	catch(char const* c)
	{
		return c;
	}
}	


string Parser::commandParse(string instruction)//parses a command
{
	if(instruction.find("OPEN") != string::npos)// <- found 
	{
		instruction.erase(0,4);
		commandOpen(instruction);//opens a table file and brings its table into scope
	}
	else if(instruction.find("CLOSE") != string::npos)
	{
		instruction.erase(0,5);
		commandClose(instruction);
	}
	else if(instruction.find("WRITE") != string::npos)
	{
		instruction.erase(0,5);
		commandWrite(instruction);//stores a table into permanent storage
	}
	else if(instruction.find("SHOW") != string::npos)
	{
		instruction.erase(0,4);
		return commandShow(instruction);//displays a requested table, its attributes, and all of its entries
	}
	else if(instruction.find("EXIT") != string::npos)
	{
		instruction.erase(0,4);
		commandExit();// exits the interactive system
	}
	else if(instruction.find("CREATETABLE") != string::npos)
	{
		instruction.erase(0,11);	//creates a table and initializes its attributes
		commandCreate(instruction);
	}
	else if(instruction.find("UPDATE") != string::npos)
	{
		instruction.erase(0,6);		//changes the value of an attribute of one table entry
		commandUpdate(instruction);
	}
	else if(instruction.find("INSERTINTO") != string::npos)//since whitespace will be eliminated
	{
		instruction.erase(0,10);//removes INSERT INTO
		commandInsert(instruction);
	}
	else if(instruction.find("DROPTABLE") != string::npos)
	{
		instruction.erase(0,9);
		commandDrop(instruction);//deletes an entire table
	}
	else if(instruction.find("DELETEFROM") != string::npos)
	{
		instruction.erase(0,10);
		commandDelete(instruction); //deletes a single entry from a table
	}
	else
	{
		return "Failure";
	}
	
	return "Success";
}

/////////////////////////////////////////////////////////////////////////////////////////////////
////////////////COMMAND FUNCTIONS////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
void Parser::commandOpen(string filename)
{
	filename = filename.substr(0, filename.size()-1);//eliminates semicolon at end of command
	
	db->readTableFromDisk(filename);//under the impression that the filename string has no ""
}

void Parser::commandClose(string tableName)
{	
	tableName = tableName.substr(0, tableName.size()-1);//eliminates semicolon at end of command
	
	bool table = db->containsTable(tableName);
	bool view = db->containsView(tableName);
	
	if(table)
		db->dropTable(tableName);
	else if(view)
		db->dropView(tableName);
	
	else
		throw "Not a table or view";//works the exact same way as commandDrop()
}

void  Parser::commandWrite(string tableName)
{	
	tableName = tableName.substr(0, tableName.size()-1);//eliminates semicolon at end of command
	
	db->writeTableToDisk(tableName);
}

void Parser::commandExit()
{	
	int success = db->exit();
	
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

string Parser::commandShow(string tableName)
{	
	try 
	{		
		bool table = db->containsTable(tableName);
		bool view = db->containsView(tableName);
		
		if(table)
		{
			cout << db->showTable(tableName) << endl;
			return db->showTable(tableName);
		}
		else if(view)
		{
			cout << db->showView(tableName) << endl;
			return db->showView(tableName);
		}
		else
			throw "Not a table or view";			
	}
	catch (char const* c)
	{
		cout << c << endl;
		return "Failure";
	}
	catch(...)
	{
		return "Failure";	
	}	
}

vector<string> Parser::commandPrimKeys(string instr)//example input (name,kind)
{
	instr.erase(0,1);//eliminates left parenthese
	instr = instr.substr(0,instr.size()-1);//eliminate right parenthese
	
	stringstream inss(instr);
	string key;
	vector<string> keyList;
	
	while(getline(inss, key, ','))
	{
		keyList.push_back(key);
	}
	
	return keyList;	
}

vector<pair<string, int>> Parser::commandAttributes(string instr)
{
	instr = instr.erase(0,1);//eliminates left parenthese
	instr = instr.substr(0,instr.size()-1);//eliminate right parenthese
	
	stringstream inss(instr);
	string pair;
	string eOne;//element 1 of pair
	int eTwo;//element 2 of pair
	vector<std::pair<string, int>> atList;
	
	while(getline(inss, pair, ','))
	{
		if(pair.find("INTEGER") != string::npos)
		{
			
			eOne = pair.substr(0,pair.find("INTEGER")); //get the name of attribute
			eTwo = -1;//signifies that the attribute is an integer
			std::pair<string, int> attr;
			
			attr.first = eOne;
			attr.second = eTwo;
			
			atList.push_back(attr);
			
		}
		else if(pair.find("VARCHAR") != string::npos)
		{
			eOne = pair.substr(0,pair.find("VARCHAR")); //get the name of attribute
			eTwo = stoi(pair.substr(pair.find("(")+1,pair.find(")")-1));
			std::pair<string, int> attr;
			
			attr.first = eOne;
			attr.second = eTwo;
			
			atList.push_back(attr);
			
		}
	}
	
	return atList;
}

void Parser::commandCreate(string instr)// We'll need 
{	
	string instruction = instr;
	string name;
	vector<std::pair<string, int>> attributes;
	vector<string> primKeys;
	
	name = instruction.substr(0, instruction.find("("));	//get name of table
	instruction.erase(0,instruction.find("("));				//erases section of the line containing the name
	//function here for getting attributes
	int pIndex = instruction.find("PRIMARYKEY");
	
	string aInstr = instruction.substr(0, pIndex);			//sub string of attribute list
	instruction.erase(0,pIndex + 10);						//erase everything up until PRIMARYKEY
	
	attributes = commandAttributes(aInstr);					//get attributes
	
	if (pIndex == string::npos)
	{
		throw "PRIMARYKEY not found";
	}
	
	primKeys = extractAttributes(instruction);
	db->createTable(name, attributes, primKeys);	
}

// split function from here: http://code.runnable.com/VHb0hWMZp-ws1gAr/splitting-a-string-into-a-vector-for-c%2B%2B
vector<string> split(string str, char delimiter)
{
  vector<string> internal;
  stringstream ss(str); // Turn the string into a stream.
  string tok;
  
  while(getline(ss, tok, delimiter)) {
    internal.push_back(tok);
  }
  
  return internal;
}

void Parser::commandUpdate(string instr)
{
	string name;	//name of relation
	
	name = instr.substr(0, instr.find("SET"));	//get name of table
	instr.erase(0, instr.find("SET")+3);		//erase previous part of string
	
	string sInstr = instr.substr(0, instr.find("WHERE"));	//get set commands
	instr.erase(0, instr.find("WHERE")+5);					//erase previous part of string
	
	sInstr.erase(remove(sInstr.begin(), sInstr.end(), '('), sInstr.end()); 
	sInstr.erase(remove(sInstr.begin(), sInstr.end(), ')'), sInstr.end());
	
	vector<string> sets = split(sInstr, ',');	//splits line into sets for parsing below
	vector<string> desired;						//desired attributes
	vector<string> values;						//values to be changed
	
	for(int i = 0; i < sets.size(); i++)
	{
		desired.push_back(sets[i].substr(0, sets[i].find("=")));					//gets our desired attributes
		values.push_back(sets[i].substr(sets[i].find("=")+1, sets[i].length()-1));	//gets our values
	}
	
	instr.erase(remove(instr.begin(), instr.end(), '('), instr.end()); 	//both lines eliminate parentheses 
	instr.erase(remove(instr.begin(), instr.end(), ')'), instr.end());
	instr.erase(remove(instr.begin(), instr.end(), '"'), instr.end());	//eliminates quotes
	
	vector<string> cond = convertBoolExpression(instr);	//condition
	
	db->updateTableRecord(name, desired, values, cond);
}

vector<string> Parser::extractAttributes (string attributeList) //returns a list of attributes
{
	
	attributeList.erase(remove(attributeList.begin(), attributeList.end(), '('), attributeList.end()); 
	attributeList.erase(remove(attributeList.begin(), attributeList.end(), ')'), attributeList.end());
	attributeList.erase(remove(attributeList.begin(), attributeList.end(), '\"'), attributeList.end());
	
	vector<string> attributes = split(attributeList, ',');
	
	return attributes;
}

void Parser::commandInsert(string instr)	//inserts a new entry to a table
{	
	instr.erase(remove(instr.begin(), instr.end(), ';'), instr.end());
	
	string name;
	vector<string> attributes;
	int attributesIndex = instr.find("VALUESFROMRELATION");
	
	if (attributesIndex != string::npos)
	{
		name = instr.substr(0, attributesIndex);
		attributesIndex += 18;
		
		string expression = instr.substr(attributesIndex);
		
		Table *result = queryParseHelper(expression, 0, 0);
		db->getTable(name)->insertRecord(result);
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
			name = instr.substr(0, attributesIndex);
			attributesIndex += 10;
			attributes = extractAttributes(instr.substr(attributesIndex));
			
			db->getTable(name)->insertRecord(attributes);
		}
	}
}

int getConditionEnd (string instr)
{
	int conditionEnd = 0;
	int parenCount = 0;
	
	do
	{
		if (instr[conditionEnd] == '(')
		{
			parenCount++;
		}
		else if (instr[conditionEnd] == ')')
		{
			parenCount--;
		}
		
		conditionEnd++;
		
	} while (parenCount > 0);
	
	return conditionEnd;
}

Parser::QueryType Parser::firstQuery (string instr) {
	int firstIndex = instr.length();
	QueryType q = Parser::ERROR;	
	
	if(db->containsTable(instr))
	{
		q = Parser::RELATION;
	}
	else if(db->containsView(instr))
	{
		q = Parser::RELATION;
	}
	if (instr.find("select") != string::npos)
	{
		int index = instr.find("select");
		
		if (index <= firstIndex)
		{
			firstIndex = index;
			q = Parser::SELECT;
		}
	}
	if (instr.find("project") != string::npos)
	{
		int index = instr.find("project");
		
		if (index <= firstIndex)
		{
			firstIndex = index;
			q = Parser::PROJECT;
		}
	}
	if (instr.find("rename") != string::npos)
	{
		int index = instr.find("rename");
		
		if (index <= firstIndex)
		{
			firstIndex = index;
			q = Parser::RENAME;
		}
	}
	if (instr.find("+") != string::npos)
	{
		int index = instr.find("+");
		
		if (index <= firstIndex)
		{
			firstIndex = index;
			q = Parser::UNION;
		}
	}
	if (instr.find("-") != string::npos)
	{
		int index = instr.find("-");
		
		if (index <= firstIndex)
		{
			firstIndex = index;
			q = Parser::DIFFERENCE;
		}
	}
	if (instr.find("*") != string::npos)
	{
		int index = instr.find("*");
		
		if (index <= firstIndex)
		{
			firstIndex = index;
			q = Parser::PRODUCT;
		}
	}
	if (instr.find("JOIN") != string::npos)
	{
		int index = instr.find("JOIN");
		
		if (index <= firstIndex)
		{
			firstIndex = index;
			q = Parser::JOIN;
		}
	}
	
	return q;
}

string Parser::queryParse(string instr)
{
	string name;
	string fileName = "tableFiles/";
	string returnString;	
	
	if (instr.find("<-") != string::npos)
	{
		name = instr.substr(0, instr.find("<-"));
	}
	else
	{
		name = "tmp";
	}
		
	instr.erase(0, instr.find("<-") + 2);
	QueryType q = firstQuery(instr);
		
	if (q == Parser::SELECT)
	{
		instr.erase(0, instr.find("select") + 6);
		
		int conditionEnd = getConditionEnd(instr);
		string conditionString = instr.substr(1, conditionEnd - 2);
		vector<string> conditions = convertBoolExpression(conditionString);
		
		instr.erase(0, conditionEnd);
		Table *tmp = queryParseHelper(instr, 0, 0);
		Table *result = tmp->select(name, conditions);
		
		db->createView(result);
		
		fileName += result->getTableName();
		fileName += ".table";
		ifstream ifs(fileName);
		string temp(static_cast<stringstream const&>(stringstream() << ifs.rdbuf()).str());
		
		returnString = temp;
	}
	else if (q == Parser::PROJECT)
	{
		instr.erase(0, instr.find("project") + 7);
		
		int openParen = instr.find("(");
		int closeParen = instr.find(")");
		
		string attributeListString = instr.substr(openParen + 1, closeParen - openParen - 1);
		vector<string> attributes = split(attributeListString, ',');
		
		instr.erase(0, closeParen + 1);
		
		if (instr[instr.length() - 1] == ')')
		{
			instr = instr.substr(0, instr.length() - 1);
		}
		
		Table *tmp = queryParseHelper(instr, 0, 0);
		Table *result = tmp->project(name, attributes);
		db->createView(result);
		
		fileName += result->getTableName();
		fileName += ".table";
		ifstream ifs(fileName);
		string temp( (std::istreambuf_iterator<char>(ifs) ), (std::istreambuf_iterator<char>()) );
		
		returnString = temp;
	}
	else if (q == Parser::RENAME)
	{
		instr.erase(0, instr.find("rename") + 6);
		
		int openParen = instr.find("(");
		int closeParen = instr.find(")");
		
		string newNameListString = instr.substr(openParen + 1, closeParen - openParen - 1);
		vector<string> newNames = split(newNameListString, ',');
		
		instr.erase(0, closeParen + 1);
		
		Table *tmp = queryParseHelper(instr, 0, 0);
		Table *result = tmp->rename(name, newNames);
		db->createView(result);
		
		fileName += result->getTableName();
		fileName += ".table";
		ifstream ifs(fileName);
		string temp( (std::istreambuf_iterator<char>(ifs) ), (std::istreambuf_iterator<char>()) );
		
		returnString = temp;
	}
	else if (q == Parser::UNION)
	{
		string expr1 = instr.substr(0, instr.find("+"));
		string expr2 = instr.substr(instr.find("+") + 1);
		
		Table *tmp1 = queryParseHelper(expr1, 0, 0);
		Table *tmp2 = queryParseHelper(expr2, 0, 1);
		Table *result = db->setUnion(tmp1, tmp2);
		
		result->setTableName(name);
		db->createView(result);
		
		fileName += result->getTableName();
		fileName += ".table";
		ifstream ifs(fileName);
		string temp( (std::istreambuf_iterator<char>(ifs) ), (std::istreambuf_iterator<char>()) );
		
		returnString = temp;
	}
	else if (q == Parser::DIFFERENCE)
	{
		string expr1 = instr.substr(0, instr.find("-"));
		string expr2 = instr.substr(instr.find("-") + 1);
		
		Table *tmp1 = queryParseHelper(expr1, 0, 0);
		Table *tmp2 = queryParseHelper(expr2, 0, 1);
		Table *result = db->setDifference(tmp1, tmp2);
		
		result->setTableName(name);
		db->createView(result);
		
		fileName += result->getTableName();
		fileName += ".table";
		ifstream ifs(fileName);
		string temp( (std::istreambuf_iterator<char>(ifs) ), (std::istreambuf_iterator<char>()) );
		
		returnString = temp;
	}
	else if (q == Parser::PRODUCT)
	{
		string expr1 = instr.substr(0, instr.find("*"));
		string expr2 = instr.substr(instr.find("*") + 1);
		
		Table *tmp1 = queryParseHelper(expr1, 0, 0);
		Table *tmp2 = queryParseHelper(expr2, 0, 1);
		Table *result = db->crossProduct(tmp1, tmp2);
		
		result->setTableName(name);
		db->createView(result);
		
		fileName += result->getTableName();
		fileName += ".table";
		ifstream ifs(fileName);
		string temp( (std::istreambuf_iterator<char>(ifs) ), (std::istreambuf_iterator<char>()) );
		
		returnString = temp;
	}
	else if (q == Parser::JOIN)
	{
		cout << "no natural join right now" << endl;
		// string expr1 = instr.substr(0, instr.find("JOIN"));
		// string expr2 = instr.substr(instr.find("JOIN") + 4);
		// Table *tmp1 = queryParseHelper(expr1, 0, 0);
		// Table *tmp2 = queryParseHelper(expr2, 0, 1);
		// Table *result = db->crossProduct(tmp1, tmp2);
		// result->setTableName(name);
		// db->createView(result);
		// delete tmp1;
		// delete tmp2;
	}
	else if (q == Parser::RELATION)
	{
		if(db->containsTable(instr))
		{
			Table *result = db->getTable(instr);
			result->setTableName(name);
			db->createView(result);
			
			fileName += result->getTableName();
			fileName += ".table";
			ifstream ifs(fileName);
			string temp( (std::istreambuf_iterator<char>(ifs) ), (std::istreambuf_iterator<char>()) );
			
			returnString = temp;
		}
		else if(db->containsView(instr))
		{
			Table *result = db->getView(instr);
			result->setTableName(name);
			db->createView(result);
			
			fileName += result->getTableName();
			fileName += ".table";
			ifstream ifs(fileName);
			string temp( (std::istreambuf_iterator<char>(ifs) ), (std::istreambuf_iterator<char>()) );
			
			returnString = temp;
		}
	}
	else
	{
		cout << "Query Parse failed" << endl;
		return "Failure";
	}
	
	return returnString;
}

Table* Parser::queryParseHelper(string instr, int depth, int pair)
{
	string name;
	
	if (instr.find("<-") != string::npos)
	{
		name = instr.substr(0, instr.find("<-"));
	}
	else
	{
		name = "tmp";
	}
		
	QueryType q = firstQuery(instr);
	
	if (instr[0] == '(')
	{
		instr = instr.substr(1);
	}
		
	if (q == Parser::SELECT)
	{
		instr.erase(0, instr.find("select") + 6);
		
		int conditionEnd = getConditionEnd(instr);
		string conditionString = instr.substr(1, conditionEnd - 2);
		
		vector<string> conditions = convertBoolExpression(conditionString);
		
		instr.erase(0, conditionEnd);
		
		if (instr[instr.length() - 1] == ')')
		{
			instr = instr.substr(0, instr.length() - 1);
		}
		
		Table *tmp = queryParseHelper(instr, depth + 1, pair);
		
		string tmpName = "tmp_";
		tmpName += depth;
		tmpName += "_";
		tmpName += pair;
		Table *result = tmp->select(tmpName, conditions);
		
		return result;
	}
	else if (q == Parser::PROJECT)
	{
		instr.erase(0, instr.find("project") + 7);
		
		int openParen = instr.find("(");
		int closeParen = instr.find(")");	
		string attributeListString = instr.substr(openParen + 1, closeParen - openParen - 1);
		
		vector<string> attributes = split(attributeListString, ',');
		
		instr.erase(0, closeParen + 1);
		
		if (instr[instr.length() - 1] == ')')
		{
			instr = instr.substr(0, instr.length() - 1);
		}
		
		Table *tmp = queryParseHelper(instr, depth + 1, pair);
		
		string tmpName = "tmp_";
		tmpName += depth;
		tmpName += "_";
		tmpName += pair;
		Table *result = tmp->project(tmpName, attributes);
		
		return result;
	}
	else if (q == Parser::RENAME)
	{
		instr.erase(0, instr.find("rename") + 6);
		
		int openParen = instr.find("(");
		int closeParen = instr.find(")");
		string newNameListString = instr.substr(openParen + 1, closeParen - openParen - 1);
		
		vector<string> newNames = split(newNameListString, ',');
		
		instr.erase(0, closeParen + 1);
		
		if (instr[instr.length() - 1] == ')')
		{
			instr = instr.substr(0, instr.length() - 1);
		}
		
		Table *tmp = queryParseHelper(instr, depth + 1, pair);
		
		string tmpName = "tmp_";
		tmpName += depth;
		tmpName += "_";
		tmpName += pair; 
		Table *result = tmp->rename(tmpName, newNames);
		
		return result;
	}
	else if (q == Parser::UNION)
	{
		string expr1 = instr.substr(0, instr.find("+"));
		string expr2 = instr.substr(instr.find("+") + 1);
		
		Table *tmp1 = queryParseHelper(expr1, depth + 1, pair);
		Table *tmp2 = queryParseHelper(expr2, depth + 1, pair);
		Table *result = db->setUnion(tmp1, tmp2);
		
		return result;
	}
	else if (q == Parser::DIFFERENCE)
	{
		string expr1 = instr.substr(0, instr.find("-"));
		string expr2 = instr.substr(instr.find("-") + 1);
		
		Table *tmp1 = queryParseHelper(expr1, depth + 1, pair);
		Table *tmp2 = queryParseHelper(expr2, depth + 1, pair);
		Table *result = db->setDifference(tmp1, tmp2);
		
		return result;
	}
	else if (q == Parser::PRODUCT)
	{
		string expr1 = instr.substr(0, instr.find("*"));
		string expr2 = instr.substr(instr.find("*") + 1);
		
		Table *tmp1 = queryParseHelper(expr1, 0, 0);
		Table *tmp2 = queryParseHelper(expr2, 0, 1);
		Table *result = db->crossProduct(tmp1, tmp2);
		
		return result;
	}
	else if (q == Parser::JOIN)
	{
		cout << "no natural join right now" << endl;
	}
	else if (q == Parser::RELATION)
	{
		if(db->containsTable(instr))
		{
			return db->getTable(instr);
		}
		else if(db->containsView(instr))
		{
			return db->getView(instr);
		}
	}
	else
	{
		throw "Query Parse failed";
	}
}

void Parser::commandDrop(string tableName)
{	
	bool table = db->containsTable(tableName);
	bool view = db->containsView(tableName);
	
	if(table)
		db->dropTable(tableName);
	else if(view)
		db->dropView(tableName);	
	else
		throw "Not a table or view";//works the exact same way as commandDrop()
}

void Parser::commandDelete(string instr)
{	
	string name;
	name = instr.substr(0, instr.find("WHERE"));//get name of table	
	instr.erase(0, instr.find("("));
	vector<string> delVect = convertBoolExpression(instr);
	db->deleteFromTable(name, delVect);
}