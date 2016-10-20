#pragma once

#include <vector>
#include <cstring>
#include <string>
#include <iostream>
#include <sstream>
#include <stack>
#include <utility>
#include <unordered_map>
#include <cctype>
#include <sys/types.h>
#include <arpa/inet.h>  
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <sstream>
#include "DataBase.h"
#include "Table.h"

using namespace std;

class Parser{
private:
	int PORT = 12002;
	int BUFFSIZE = 1024;
	
	DataBase *db;
	
	vector<string> stringToTokens (string boolExpression);
	vector<string> convertBoolExpression (string boolExpression);
	vector<string> extractAttributes (string attributeList);
	vector<pair<string, int>> commandAttributes(string instr);
	vector<string> commandPrimKeys(string instr);
	
	enum QueryType { ERROR, RELATION, SELECT, PROJECT, RENAME, UNION, DIFFERENCE, PRODUCT, JOIN };
	
public:

	// Initialize data base
	Parser() {db = new DataBase();};
	
	// Delete data base
	~Parser() {delete db;}

	// Runs the parser on an input loop on the command line
	void runOnCommandLine();
	
	// Runs the parser on an input loop on a server
	void runOnSocket();
	
	string commandOrQuery(string instruction);  
	//this is a relatively simple function. query instructions will always have an arrow "<-" somewhere in them (unless
	//the query is part of (or rather, a source of input for) a command instruction) commandOrQuery will check either 
	//for the existence of "<-", or the existence of "<" followed by "-", whichever is easier to implement.


	/* ---------- command functions ---------- */
	
	string commandParse(string instr); 
	//gets the name of the table on which we are performing the command on
	//parses through and looks for command instruction keywords: 
	//OPEN, CLOSE, WRITE, EXIT, SHOW, CREATE, UPDATE, INSERT, DELETE, and DROP
	//each of these keywords correspond to a command function. most of those functions take a table name, if any input at all.
	//the create, update, insert, and delete functions will take a bit more work, as they all require more parsing to determine

	// Wrapper function for database read from disk
	void commandOpen(string filename); 
	
	// Wrapper function for database drop table
	void commandClose(string tablename); 
	
	// Wrapper function for database write table to disk
	void commandWrite(string tableName);

	// Wrapper function for database show table
	string commandShow(string tablename);
	
	// Wrapper functoin for database exit
	void commandExit();
	
	// Wrapper function for database create table
	void commandCreate(string instr);
	
	// Wrapper function for database update table
	void commandUpdate(string instr); 
	//to implement this one, I'm thinking I just get the entry I'm trying to update, storing that in a
	//string vector, modifying that string vector, calling Delete on the old entry, then using insert
	//to put the new one in.
					  
	void commandInsert(string instr);
	//the following comment block concerns the insert function, and possibly the delete and update functions. but definitely the insert function.
	//
	//      the function will need to look for query syntax (if the command involves the result of the query, it will first need to find where
	//      in the instruction string that the query begins and ends. the  will then need to send that string containing that query to the
	//      queryParse, which will get a view table which will be used as an input for whichever command function called for the
	//      query (which is to say, whichever command function preceded the query call in the instruction string)
	//      the name of the query will be something like "cmd_query"  
	//

	// Wrapper function for database drop table
	void commandDrop(string tablename);
	
	// Wrapper function for database delete from table
	void commandDelete(string instr); 
	//this one pretty much does the opposite of Insert, with the fortunate exception (hopefully) of
	//not having to worry about getting information from a query
	
	

	


	/* --------------------------------------- */
	
	/* ---------- query functions ------------ */
	QueryType firstQuery (string instr);	// Checks the type of query
	string queryParse(string instr);		// Parse the query and call the corresponding function
	Table* queryParseHelper(string instr, int depth, int pair);	// Helps break apart a query
	
		
	/* --------------------------------------- */
};
